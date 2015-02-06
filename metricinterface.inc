<?php

/**
 * @file
 * Interface for metric classes in the election_livestats module.
 */

interface ElectionLiveStatsMetricInterface {
  public function __construct();
  public function setAccount(stdClass $account);
  public function getName();
  public function getLabel();
  public function getType();
  public function increment($data);
  public function decrement($data);
  public function total($election);
}

