<?php
/**
 * @file
 * Metric class for total number of votes.
 */

/**
 * Class definition for metric that tracks total number of ballots.
 */
class ElectionLiveStatsMetricTotalVotes extends ElectionLiveStatsMetricBase {

  protected $name = 'total_votes';
  protected $label = 'Votes cast';
  protected $type = 'count';

  /**
   * Increment total votes on every vote.
   */
  public function increment($data) {

    return $data + 1;

  }

  /**
   * Decrement total votes on every vote undo.
   */
  public function decrement($data) {

    return max(0, $data - 1);

  }

  /**
   * Totals do not apply for methods of type "count".
   */
  public function total($election) {

    return FALSE;

  }

}
