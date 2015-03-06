<?php

/**
 * @file
 * Interface for metric classes in the election_livestats module.
 */

/**
 * Interface for Livestats metrics.
 */
interface ElectionLiveStatsMetricInterface {
  /**
   * Classes should define a constructor.
   */
  public function __construct();

  /**
   * Classes should define a setter for the account property.
   */
  public function setAccount(stdClass $account);

  /**
   * Classes should define a getter for the name property.
   */
  public function getName();

  /**
   * Classes should define a getter for the label property.
   */
  public function getLabel();

  /**
   * Classes should define a getter for the type property.
   */
  public function getType();

  /**
   * Classes should define a method for incrementing a metric.
   */
  public function increment($data);

  /**
   * Classes should define a method for decrementing a metric.
   */
  public function decrement($data);

  /**
   * Classes should define a method for calculating total possible value.
   */
  public function total($election);

}
