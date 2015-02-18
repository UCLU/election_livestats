<?php
/**
 * @file
 * Form for carrying out Livestats actions on an Election.
 */

function election_livestats_actions_form($form, &$form_state, $election) {

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['cache_eligibility'] = array(
    '#type' => 'submit',
    '#value' => 'Cache eligibility and metadata.',
    '#submit' => array('election_livestats_form_cache_eligibility'),
  );

  return $form;

}
