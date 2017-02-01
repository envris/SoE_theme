<?php
/**
 * @file
 * Element alters.
 */


/**
 * Implements hook_element_info_alter().
 */
function doesoe_admin_element_info_alter(&$type) {
  if (isset($type['media'])) {
    // @see element.preprocess.inc.
    $type['media']['#process'][] = 'doesoe_admin_media_element_process';
  }
}