<?php
/**
 * @file
 * Rules integration for the Election Livestats module.
 */

/**
 * Implements hook_rules_action_info().
 */
function election_livestats_rules_action_info() {

  $actions = array();

  $defaults = array(
    'group' => t('Election livestats'),
  );

  $actions['election_livestats_cache_meta'] = $defaults + array(
    'label' => t('Cache election metadata'),
    'parameter' => array(
      'election' => array(
        'type' => 'election',
        'label' => t('The election'),
      ),
    ),
  );

  $actions['election_livestats_flush_meta'] = $defaults + array(
    'label' => t('Flush election metadata'),
    'parameter' => array(
      'election' => array(
        'type' => 'election',
        'label' => t('The election'),
      ),
    ),
  );

  $actions['election_livestats_cache_eligibility'] = $defaults + array(
    'label' => t('Cache voter eligibiltiy'),
    'parameter' => array(
      'election' => array(
        'type' => 'election',
        'label' => t('The election'),
      ),
    ),
  );

  return $actions;
}
