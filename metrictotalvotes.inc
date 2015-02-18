<?php
/**
 * @file
 * Metric class for total number of votes.
 */

class ElectionLiveStatsMetricTotalVotes extends ElectionLiveStatsMetricBase {

  protected $name = 'total_votes';
  protected $label = 'Total votes';
  protected $type = 'count';

  public function increment($data) {

    return $data + 1;

  }

  public function decrement($data) {

    return max(0, $data - 1);

  }

  public function total($election) {

    return FALSE;

  }

}
