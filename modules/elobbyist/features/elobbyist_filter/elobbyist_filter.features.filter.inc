<?php
/**
 * @file
 * elobbyist.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function elobbyist_filter_filter_default_formats() {
  $formats = array();

  // Exported format: Email Lobbyist HTML Filter.
  $formats['elobbyist_html'] = array(
    'format' => 'elobbyist_html',
    'name' => 'Email Lobbyist HTML Filter',
    'cache' => '1',
    'status' => '1',
    'weight' => '1',
    'filters' => array(),
  );

  return $formats;
}
