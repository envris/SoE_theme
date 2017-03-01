<?php
/**
 * @file
 * Preprocess links when they are rendered using theme_link.
 */

/**
 * Implements hook_preprocess_link().
 */
function doesoe_theme_preprocess_link(&$variables) {
  if (isset($variables['options']['entity_type'])) {
    $hook = __FUNCTION__ . "_{$variables['options']['entity_type']}";
    if (function_exists($hook)) {
      $hook($variables);
    }
  }
}

/**
 * Implements hook_preprocess_link().
 */
function doesoe_theme_preprocess_link_taxonomy_term(&$variables) {
  $term = $variables['options']['entity'];

  if ($term->vocabulary_machine_name == 'regions') {
    // When these terms are rendered via DS they don't get parsed through the
    // taxonomy term preprocess which rewrites the URL for the term. Instead
    // we rewrite the URL when the link is going to be rendered.
    $variables['options']['query']['region[]'] = $term->tid;
    $variables['path'] = 'region-search';
  }
}
