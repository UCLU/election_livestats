Election Livestats
==================

Because this module uses Node.js, there are some additional installation and running steps.

Requirements
------------

* Node.js (or io.js)
* npm
* Redis

Installation
-------------

* Install module.
* Run `npm install` in `node/` folder
* Set your configuration in `node/config.json`. You can do this by renaming or copying `config.json.example`.

Running
-------
* To run simply start the `app.js` file in the `node/` subdirectory of the module.<br />*Note: If the certificates are in a read-protected directory, the app will need to be run in `sudo`*.
* For every election for which you want to have live statistics, you will need go to "Actions" and "Cache metadata" and "Cache eligibility". These can also be triggered by Rules.
