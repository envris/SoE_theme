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
}
