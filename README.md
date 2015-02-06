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
* npm install in node/ folder
* Configure SSL details in app.js (will make this nicer in the future if I can work out the best way to pass variables from Drupal to JS)
* To use the same certificates as the website, the app needs to be run in sudo.
* You will need go to "Actions" and "Cache metadata" and "Cache eligibility".
* Enjoy!
