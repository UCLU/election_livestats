<?php
/**
 * @file
 * Page to display Election Live Stats.
 */

/**
 * Page callback for showing election livestats.
 */
function election_livestats_page($election) {

  // If the metadata does not exist, alert the user.
  if (!election_livestats_metadata_exists($election)) {
    drupal_set_message(t('Metadata for live statistics have not been built for this election.'), 'warning');
    return array();
  }

  // Disable the cache on this page as it is based on live data.
  $GLOBALS['conf']['cache'] = FALSE;

  // Connect to Redis.
  $redis = election_livestats_get_redis_client();

  // Start render array that will contain the metrics.
  $metrics = array();

  // Start the array that will contain initial json for the frontend.
  $metrics_json = array();

  // Update Redis for each metric.
  foreach (election_livestats_metrics() as $class_name) {

    // Begin the array of the table and the rows for this metric.
    $table = array(
      '#theme' => 'table',
    );

    $rows = array();

    // Instantiate metric.
    $metric = new $class_name();

    // Get the metric label.
    $name = $metric->getName();
    $label  = $metric->getLabel();

    // Get the metric data.
    $key = election_livestats_get_redis_key($election->election_id, $name);
    $data = json_decode($redis->get($key), TRUE);

    // Get the total possible value for this metric.
    $metakey = election_livestats_get_redis_key($election->election_id, "meta-{$name}");
    $meta = json_decode($redis->get($metakey), TRUE);

    // Metrics are either arrays of multiple values (like in clubs and
    // societies) or single values (like number of students who have voted).
    // They need to be formatted differently.
    $type = $meta['type'];

    switch ($type) {

      case "grouped":

        $total = $meta['total'];

        $table['#header'] = array('Group', 'Value', 'Total');

        foreach ($total as $key => $value) {

          // The $total array should contain every possible metric, even though
          // the data may not. If the data doesn't exist for the total, we
          // assume that it is zero.
          if (!isset($data[$key])) {
            $data[$key] = 0;
          }

          if (!is_array($total[$key])) {
            $total[$key] = array(
              "value" => $total[$key],
              "label" => $key,
            );
          }

          $rows[] = array(
            $total[$key]['label'],
            $data[$key],
            $total[$key]['value'],
          );

        }

        break;

      case "single":

        $total = $meta['total'];

        $table['#header'] = array('Value', 'Total');

        $rows[] = array(
          (empty($data)) ? 0 : $data,
          $total,
        );

        break;

      case "count":

        $table['#header'] = array('Value');

        $rows[] = array(
          (empty($data)) ? 0 : $data,
        );

        break;

    }

    // Add this metric to the metrics array.
    $table['#rows'] = $rows;

    $metrics[] = array(
      '#markup' => "<h2>{$label}</h2>",
    );
    $metrics[] = $table;

    $json_entry = array(
      "election" => $election->election_id,
      "name" => $name,
      "label" => $label,
      "value" => $data,
      "type" => $type,
    );

    if (isset($total)) {
      $json_entry["total"] = $total;
    }

    $metrics_json[] = $json_entry;

  }

  $metrics_json = json_encode($metrics_json);

  $module_location = drupal_get_path('module', 'election_livestats');

  // Socket.io variables.
  $prot = variable_get("election_livestats_ssl", TRUE) ? "https" : "http";
  $host = variable_get("election_livestats_host", parse_url($GLOBALS['base_url'], PHP_URL_HOST));
  $port = variable_get("election_livestats_port", 3000);

  $socket_io_url = $prot . "://" . $host . ':' . $port;

  // Pass some variables to javascript.
  drupal_add_js("var election_id = '{$election->election_id}', socket_io_url = '{$socket_io_url}', initial_data = {$metrics_json};", array(
    'type' => 'inline',
  ));

  if (($library = libraries_load('highcharts')) && !empty($library['loaded'])) {
    // Load socket.io served by the node server.
    drupal_add_js($socket_io_url . '/socket.io/socket.io.js');

    // Load our JavaScript.
    drupal_add_js($module_location . '/js/livestats.js');
  } else {
    dpm($library);
    drupal_set_message('Highcharts library is not installed.','error');
  }

  return array(
    'noscript' => array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'noscript-statistics',
      ),
      'metrics' => $metrics,
    ),
  );

}
