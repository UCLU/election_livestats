<?php
/**
 * @file
 * Form for carrying out Livestats actions on an Election.
 */

/**
 * Implemenets hook_form().
 */
function election_livestats_actions_form($form, &$form_state, $election) {
  // Ensure that this file is loaded when the form is processed.
  form_load_include($form_state, 'inc', 'election_livestats', 'election_livestats.actions');

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['cache_eligibility'] = array(
    '#type' => 'submit',
    '#value' => 'Cache eligibility and metadata.',
    '#submit' => array('election_livestats_form_cache_eligibility'),
  );

  return $form;
}

/**
 * Caches user eligibility to vote from a form submission.
 */
function election_livestats_form_cache_eligibility($form, &$form_state) {
  $election = $form_state['build_info']['args'][0];

  return election_livestats_cache_eligibility($election);
}
