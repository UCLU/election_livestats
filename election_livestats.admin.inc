<?php
/**
 * @file
 * Forms for configuring Election Live Stats.
 */

function election_livestats_config_form($form, &$form_state) {

  drupal_set_title(t("Configure Live Statistics"), PASS_THROUGH);

  $form['election_livestats_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#description' => t('The host of the statistics websocket server.'),
    '#default_value' => variable_get('election_livestats_host', parse_url($GLOBALS['base_url'], PHP_URL_HOST)),
  );

  $form['election_livestats_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#description' => t('The port of the statistics websocket server.'),
    '#default_value' => variable_get('election_livestats_port', 3000),
  );

  return system_settings_form($form);

};
