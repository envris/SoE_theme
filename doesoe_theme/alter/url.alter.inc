<?php
/**
 * @file
 * Url alters.
 */

/**
 * Implements hook_url_outbound_alter().
 */
function doesoe_theme_url_outbound_alter(&$path, &$options, $original_path) {
  // If the outbound path is a ckan dataset url.
  $ckan_base_url = variable_get('govcms_ckan_endpoint_url', '');
  if (!empty($ckan_base_url) && 0 === strpos($path, $ckan_base_url . '/dataset/')) {
    // We remove the resource from the path so the final url is to the package.
    $url_parts = explode('/resource/', $path);
    $path = $url_parts[0];
  }
}
