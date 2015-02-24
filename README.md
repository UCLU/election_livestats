Election Livestats
==================

Because this module uses Node.js, there are some additional installation and running steps.

Requirements
-------------------

* Node.js (or io.js)
* npm
* Redis
* [phpredis](https://github.com/phpredis/phpredis)

Installation
------------

* Install Drupal module and configure.
* Run `npm install` in `node/` folder
* Set your configuration in `node/config.json`. You can do this by renaming or copying `config.json.example`.


Configuration
-------------

This module requires metrics to be implemented via `hook_election_livestats_metrics_alter`. An example metric "Total votes" is included by way of example.

Documentation on the structure of metric classes is to come.


Running
-------
* Start Redis server.
* Start the `app.js` file in the `node/` subdirectory of the module.<br />*Note: If the certificates are in a read-protected directory, the app will need to be run in `sudo`*.
* For every election for which you want to have live statistics, you will need go to `election/%election_id/stats/actions` and `Cache eligibility and metadata.`. Depending on your electorate size, this may take some time.
* Once this is complete, you should see a "Total votes" header at `election/%election_id/stats/`. This will indicate how many votes have been cast as voting continues.

### Process Monitoring
Ideally the Node.js app should run as a service on the server, so start it as a daemon or service in the way you prefer. I would recommend using [PM2](https://github.com/Unitech/PM2/) to monitor the app, running `pm2 startup` to start it on boot.

Alternatively, the upstart script `node/election_livestats_node.conf` is included in this repository. To use it, change the directory in the `script` section to point to your `election_livestats` folder and then copy it to `/etc/init/`.
