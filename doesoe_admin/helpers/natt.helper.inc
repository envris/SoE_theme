<?php
/**
 * @file
 * NATT (Not a theme theme) helpers.
 */

/**
 * Check if loaded term is a NATT.
 */
function _doesoe_admin_theme_is_natt($term) {
  $is_natt = FALSE;
  $natt_field = field_get_items('taxonomy_term', $term, 'field_natt');
  if (!empty($natt_field)) {
    $item = reset($natt_field);
    $value = $item['value'];
    if ($value == 1) {
      $is_natt = TRUE;
    }
  }
  return $is_natt;
}
