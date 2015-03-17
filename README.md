Election Livestats
==================
Election Livestats is a module for Drupal 7 that extends the Election
module to provide live statistics.

Note that all version numbers provided for dependencies or requirements
reflect the development environment and can be substituted for other
versions with compatable APIs.

Dependencies
-------------
* Election 7.x-1.0-beta25  (http://drupal.org/project/election) with
[patch 2387261](https://www.drupal.org/node/2387261).
* Libraries API 7.x-2.2 (http://drupal.org/project/libraries)

Software Requirements
---------------------
* Node.js v0.10.33
* npm v1.4.28
* Redis v2.2.1
* [phpredis] v2.2.7 (https://github.com/phpredis/phpredis)
* [Highcharts v4.1.1](http://code.highcharts.com/zips/Highcharts-4.1.1.zip).

Installation
------------
* Run `npm install` in `node/` folder
* Set your configuration in `node/config.json`. You can do this by
renaming or copying `config.json.example`.
* Install the Highcharts library. The Libraries module has [a good FAQ](https://www.drupal.org/node/1440066)
on installing external libraries.

Configuration
-------------
This module requires metrics to be implemented via
`hook_election_livestats_metrics_alter`. An example metric "Total votes"
is included by way of example.
Documentation on the structure of metric classes is to come.

There is also a "view election live statistics" permission that you will
need to configure before the live statistics page is visible.

You may need to open port 3000 on your firewall (e.g. on Ubuntu,
`ufw allow 3000/tcp`).

Running
-------
* Start Redis server.
* Start the `app.js` file in the `node/` subdirectory of the module.
<br />*Note: If the certificates are in a read-protected directory, the
app will need to be run in `sudo`*.
* For every election for which you want to have live statistics, you
will need go to `election/%election_id/stats/actions` and
`Cache eligibility and metadata.`. Depending on your electorate size,
this may take some time.
* Once this is complete, you should see a "Total votes" header at
`election/%election_id/stats/`. This will indicate how many votes have
been cast as voting continues.

### Process Monitoring
Ideally the Node.js app should run as a service on the server, so start
it as a daemon or service in the way you prefer. I would recommend using
[PM2](https://github.com/Unitech/PM2/) to monitor the app, running
`pm2 startup` to start it on boot.

Alternatively, the upstart script `node/election_livestats_node.conf` is
included in this repository. To use it, change the directory in the
`script` section to point to your `election_livestats` folder and then
copy it to `/etc/init/`.

Security Considerations
-----------------------
Please note that currently there is no authentication on the websocket
server. As a result, even if your election is only visible to certain
users, anyone could connect to the websocket server and receive live
statistics updates.
