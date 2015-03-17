#!/usr/bin/env node
// @codingStandardsIgnoreFile

// Require dependent libraries and config.
var config = require('./config.json');
var redis = require('redis');
var sio = require('socket.io');
var https = require('https');
var http = require('http');
var fs = require('fs');

// If we are using HTTPS, we need to configure the server to read the
// certificates, otherwise just create an HTTP server object.
var server;
if (config.useSSL) {
  var params;
  try {
    params = {
      key: fs.readFileSync(config.ssl.key),
      cert: fs.readFileSync(config.ssl.cert)
    };
  }
  catch (e) {
    if (e.errno === 3) {
      console.error("Your SSL key or certificate cannot be read. If they are in a read-protected directory, you will need to run the application with sudo (or equivalent).");
    }
    else {
      console.error(e);
    }
    process.exit(1);
  }
  server = https.createServer(params);
} else {
  server = http.createServer();
}

// We need to have two connections, one for subscribing and one for regular
// access.
try {
  var subscriber = redis.createClient(config.redis.port, config.redis.host);
  var regular = redis.createClient(config.redis.port, config.redis.host);
}
catch (e) {
  console.error("There was an error connecting to Redis.");
  process.exit(1);
}

// Start HTTPS and socket.io
var io = sio.listen(server);
server.listen(config.port, config.host, function(error) {
  if (error) {
    console.error("Could not start websocket server.");
    process.exit(1);
  }
  else {
    console.log("Websocket server started successfully!");
  }
});

// Initiate a job queue.
var q = new Queue();

// Subscribe clients to channels based on their election id.
io.on('connection', function(socket){
  socket.on('subscribe', function(election) {

    socket.join(election);

  });
});

// Subscribe to the redis pubsub to receive update notifications from PHP.
subscriber.subscribe("election_livestats");

// When we get a notification from PHP, send to clients in appropriate room.
subscriber.on("message", function(channel, message) {
  message = JSON.parse(message);

  var election = message.election;
  var metric = message.metric;

  var value_key = getRedisKey(election, metric);
  var meta_key = getRedisKey(election, "meta-" + metric);

  regular.get(value_key, function(err, value){
    if (value !== null) {
      value = JSON.parse(value);

      regular.get(meta_key, function(err, meta){
        if (meta !== null) {
          meta = JSON.parse(meta);

          var payload = {
            "election": election,
            "name": meta.name,
            "label": meta.label,
            "value": value,
            "type": meta.type
          };

          if (meta.total !== undefined) {
            payload.total = meta.total;
          }

          (function(payload) {
            q.add(payload.name, function() {
              io.to(payload.election).emit('update', payload);
            });
          })(payload);
        }
      });
    }
  });
});

// Introduce a delay to ensure that we don't lots of updates to the same data.
function Queue() {

  var queued = [];

  this.add = function(unique, callback) {
    var time;

    var getTimeLeft = function(timeout) {
      return timeout._idleStart + timeout._idleTimeout - Date.now();
    };

    if (queued[unique] !== undefined) {
      time = getTimeLeft(queued[unique]);

      clearTimeout(queued[unique]);
    }
    else {
      time = 2000;
    }

    queued[unique] = setTimeout(callback, time);
  };

}

// Analogous to election_livestats_get_redis_key($key).
function getRedisKey(election, metric){
  return config.prefix + "-" + election + "-" + metric;
}
