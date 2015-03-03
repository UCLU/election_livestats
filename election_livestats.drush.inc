<?php
/**
 * @file
 * Drush config for Election Live Stats.
 */

/**
 * Implements hook_drush_command().
 */
function election_livestats_drush_command() {

  $items = array();

  $items['election-live-statistics-simulate'] = array(
    'description' => 'Run a set of votes already cast through livestats hooks.',
    'arguments' => array(
      'election-id' => 'The ID of the election (required).',
    ),
    'options' => array(
      'timestamp' => 'The timestamp before which votes should be simulated.',
    ),
    'required-arguments' => 1,
    'aliases' => array('els-sim'),
  );

  return $items;

}

/**
 * Run live stats hooks against votes already cast.
 *
 * @param int $election_id
 *   The ID of the election.
 */
function drush_election_livestats_election_live_statistics_simulate($election_id) {

  $timestamp = drush_get_option('timestamp', REQUEST_TIME);

  $votes = db_query('SELECT * FROM {election_ballot} WHERE {election_id} = :election_id AND `value` >= 1 AND {timestamp} < :timestamp', array(
    ':election_id' => $election_id,
    ':timestamp' => $timestamp,
  ))->fetchAll();

  foreach ($votes as $vote) {

    $account = user_load($vote->uid);
    $post = election_post_load($vote->post_id);
    $election = election_load($vote->election_id);

    election_livestats_handle_vote("increment", $account, $post, $election, $vote->timestamp);

  }

}
