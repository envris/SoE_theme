<?php
/**
 * @file
 * Image preprocessors.
 */

/**
 * Implements hook_preprocess_image_formatter().
 */
function doesoe_theme_preprocess_image_formatter(&$vars) {
  // Only purpose here is to add an image map, so we have one first.
  if (empty($vars['item']) || empty($vars['item']['field_image_map'])) {
    return;
  }
  // Add the 'usemap' attribute to the image.
  $vars['item']['attributes']['usemap'] = '#image-map-' . $vars['item']['fid'];
}
