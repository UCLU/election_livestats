<?php
/**
 * @file
 * Base class for metrics in the election_livestats module.
 */

/**
 * Abstract class for Livestats Metrics to extend.
 */
abstract class ElectionLiveStatsMetricBase implements ElectionLiveStatsMetricInterface {

  protected $account;
  protected $name;
  protected $label;

  // Can be one of: 'single', 'grouped', 'count'.
  protected $type;

  /**
   * Construct a metric class.
   *
   * Throws an error if a name has not been defined or has been defined
   * incorrectly.
   */
  public function __construct() {

    // Throw an exception if the metric author has not defined a name.
    if (empty($this->name)) {
      throw new ElectionLiveStatsException('Name property not defined.');
    }
    elseif (strpos($this->name, "-")) {
      throw new ElectionLiveStatsException('Metric names cannot contain dashes (-).');
    }

  }

  /**
   * Setter for account property.
   */
  public function setAccount(stdClass $account) {

    // Take account details of voter as constructor parameter.
    $this->account = $account;

  }

  /**
   * Getter for name property.
   */
  public function getName() {

    return $this->name;

  }

  /**
   * Getter for label property.
   */
  public function getLabel() {

    return (!isset($this->label)) ? $this->name : $this->label;

  }

  /**
   * Getter for type property.
   */
  public function getType() {

    return $this->type;

  }

}
