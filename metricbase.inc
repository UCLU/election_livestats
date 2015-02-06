<?php
/**
 * @file
 * Base class for metrics in the election_livestats module.
 */

abstract class ElectionLiveStatsMetricBase implements ElectionLiveStatsMetricInterface {

  protected $account;
  protected $name;
  protected $label;

  // Can be one of: 'single', 'grouped', 'count'.
  protected $type;

  public function __construct() {

    // Throw an exception if the metric author has not defined a name.
    if (empty($this->name)) {
      throw new ElectionLiveStatsException('Name property not defined.');
    }
    elseif (strpos($this->name, "-")) {
      throw new ElectionLiveStatsException('Metric names cannot contain dashes (-).');
    }

  }

  public function setAccount(stdClass $account) {

    // Take account details of voter as constructor parameter.
    $this->account = $account;

  }

  public function getName() {

    return $this->name;

  }

  public function getLabel() {

    return (!isset($this->label)) ? $this->name : $this->label;

  }

  public function getType() {

    return $this->type;

  }

}
