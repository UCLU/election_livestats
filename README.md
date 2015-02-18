Election Livestats
==================

Because this module uses Node.js, there are some additional installation and running steps.

Requirements
-------------------

* Node.js (or io.js)
* npm
* Redis

Metrics
---------

This module requires metrics to be implemented via custom submodules and added using `hook_election_livestats_metrics_alter`. An example metric "Total votes" is included by way of example.

Documentation on the structure of metric classes is to come.

Installation
---------------

* Install Drupal module and configure.
* Run `npm install` in `node/` folder
* Set your configuration in `node/config.json`. You can do this by renaming or copying `config.json.example`.

Running
-----------
* Start Redis server.
* Start the `app.js` file in the `node/` subdirectory of the module.<br />*Note: If the certificates are in a read-protected directory, the app will need to be run in `sudo`*.
* For every election for which you want to have live statistics, you will need go to `election/%election_id/stats/actions` and `Cache eligibility and metadata.`. Depending on your electorate size, this may take some time.
* Once this is complete, you should see a "Total votes" header at `election/%election_id/stats/`. This will indicate how many votes have been cast as voting continues.

