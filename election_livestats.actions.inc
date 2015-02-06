<?php
/**
 * @file
 * Form for carrying out Livestats actions on an Election.
 */

function election_livestats_actions_form($form, &$form_state, $election) {

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['cache_eligibility'] = array(
    '#type' => 'submit',
    '#value' => 'Cache eligibility data',
    '#submit' => array('election_livestats_form_cache_eligibility'),
  );

  $form['actions']['cache_meta'] = array(
    '#type' => 'submit',
    '#value' => 'Cache metadata',
    '#submit' => array('election_livestats_form_cache_meta'),
  );

  $form['actions']['flush_meta'] = array(
    '#type' => 'submit',
    '#value' => 'Flush cached metadata',
    '#submit' => array('election_livestats_form_flush_meta'),
  );

  return $form;

}
