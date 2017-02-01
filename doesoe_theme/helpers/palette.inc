<?php
/**
 * @file
 * Palette helpers.
 */

/**
 * Get the palette key or vis palette for the current node.
 *
 * @param string $type
 *   The palette_key or the vis_palette.
 *
 * @return mixed
 *   If requesting a palette key, returns a string. If requesting a vis_palette
 *   return an array of hex colors added to the theme.
 */
function _doesoe_theme_get_palette($type = 'palette_key') {

  // Get current theme.
  $theme_term = _doesoe_theme_get_current_theme_term();

  // If we have a theme from this context, return palette.
  if ($theme_term !== FALSE) {
    return _doesoe_theme_get_palette_from_term($theme_term, $type);
  }

  // No match.
  return ($type == 'palette_key' ? NULL : array());
}

/**
 * Get the palette from a term.
 *
 * @param object $term
 *   A theme term.
 * @param string $type
 *   The palette_key or the vis_palette.
 *
 * @return mixed
 *   If requesting a palette key, returns a string. If requesting a vis_palette
 *   return an array of hex colors added to the theme.
 */
function _doesoe_theme_get_palette_from_term($term, $type = 'palette_key') {
  $entity_w = entity_metadata_wrapper('taxonomy_term', $term);
  // Return palette key.
  if ($type == 'palette_key' && isset($entity_w->field_palette_key)) {
    return $entity_w->field_palette_key->value();
  }
  // Return a vis palette.
  if ($type == 'vis_palette' && isset($entity_w->field_colors)) {
    return $entity_w->field_colors->value();
  }
  return ($type == 'palette_key' ? NULL : array());
}
