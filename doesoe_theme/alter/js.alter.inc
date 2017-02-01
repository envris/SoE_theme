<?php
/**
 * @file
 * Javascript alter hooks.
 */

/**
 * Implements hook_js_alter().
 */
function doesoe_theme_js_alter(&$javascript) {
  $jquery_version = '1.8.3';
  $jquery_url = '//ajax.googleapis.com/ajax/libs/jquery/' . $jquery_version . '/jquery.min.js';

  // Swap out jQuery to use a version compatible with the DataTables plugin.
  // @see https://datatables.net/manual/installation#Dependencies
  $javascript['misc/jquery.js']['data'] = url($jquery_url);
  $javascript['misc/jquery.js']['type'] = 'external';
  $javascript['misc/jquery.js']['version'] = $jquery_version;

  if (isset($javascript['misc/ui/jquery.ui.core.min.js'])) {
    $jquery_ui_version = '1.10.2';
    $javascript['misc/ui/jquery.ui.core.min.js']['version'] = $jquery_ui_version;
    $javascript['misc/ui/jquery.ui.core.min.js']['type'] = 'external';
    $javascript['misc/ui/jquery.ui.core.min.js']['data'] = url('//ajax.googleapis.com/ajax/libs/jqueryui/' . $jquery_ui_version . '/jquery-ui.min.js');
  }
}
