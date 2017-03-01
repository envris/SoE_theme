<?php
/**
 * @file
 * Preprocessor for ckan table wrapper.
 */

/**
 * Implements hook_preprocess_ckan_display_table_wrapper().
 */
function doesoe_theme_preprocess_ckan_display_table_wrapper(&$variables) {
  $palette = _doesoe_theme_get_palette('vis_palette');
  if (!empty($palette)) {
    foreach ($variables['content'] as $group => $table) {
      $variables['content'][$group]['table']['#attributes']['data-palette'] = implode(',', $palette);
    }
  }
  foreach ($variables['content'] as $group => $table) {
    $variables['content'][$group]['title']['#tag'] = 'h5';
    $variables['content'][$group]['title']['#attributes']['class'][] = 'graph__split-title';
    // Add a class if this table is using a visualisation so we can hide while
    // page loading.
    if (!empty($table['table']['#attributes']['data-type'])) {
      $variables['content'][$group]['table']['#attributes']['class'][] = 'doe-graph-table';
    }
  }
}
