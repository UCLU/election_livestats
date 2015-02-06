<?php
/**
 * @file
 * Drush config for Election Live Stats.
 */

function election_livestats_drush_command() {

  $items = array();

  $items['election-live-statistics-simulate'] = array(
    'description' => 'Simulate a previously-held election on Redis.',
    'arguments' => array(
      'election-id' => 'The ID of the election (required).',
    ),
    'options' => array(
      'speed' => 'The time (in seconds) in which 1 day of the original election should elapse.',
      'steps' => 'The number of chunks of votes to be considered in one second.',
      'iterations' => 'The total number of chunks to simulate (a value of 0 will simulate all chunks).',
      'offset' => 'The number of chunks to miss before beginning the simulation.',
    ),
    'required-arguments' => 1,
    'aliases' => array('els-sim'),
  );

  return $items;

}

function drush_election_livestats_election_live_statistics_simulate($election_id) {

  $speed = drush_get_option('speed', 5);
  $steps = drush_get_option('steps', 1);
  $iterations = drush_get_option('iterations', 0);
  $offset = drush_get_option('offset', 0);

  $range = db_query("SELECT MIN(timestamp) as min, MAX(timestamp) as max FROM {election_ballot} WHERE {election_id} = :election_id", array(
    ':election_id' => $election_id,
  ))->fetchAssoc();

  $real_time_elapsed = $range['max'] - $range['min'];

  $days = $real_time_elapsed / 86400;

  $simulated_time = $days * $speed;

  $total_steps = $simulated_time * $steps;

  $real_time_chunk_size = $real_time_elapsed / $total_steps;

  for ($range_start = $range['min'] + ($real_time_chunk_size * $offset), $i = 0;
      $range_start < $range['max'], $i == $iterations;
      $range_start += $real_time_chunk_size, $i++) {

    $votes = db_query("SELECT * FROM {election_ballot} WHERE {election_id} = :election_id AND {timestamp} >= :range_start AND {timestamp} < :range_end", array(
      ':election_id' => $election_id,
      ':range_start' => $range_start,
      ':range_end' => $range_start + $real_time_chunk_size,
    ))->fetchAll();

    foreach ($votes as $vote) {

      $account = user_load($vote->uid);
      $post = election_post_load($vote->post_id);
      $election = election_load($vote->election_id);

      election_livestats_handle_vote("increment", $account, $post, $election, $vote->timestamp);

    }

  }

}
