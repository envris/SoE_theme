<?php
/**
 * @file
 * Text helpers.
 */

/**
 * Helper function for nice text trimming.
 *
 * Uses views_trim_text() with sensible defaults if available and falls back
 * to text_summary().
 */
function _doesoe_theme_text_trim($text = NULL, $length = 300) {
  if (function_exists('views_trim_text')) {
    $options = array(
      'max_length' => $length,
      'ellipsis' => TRUE,
      'word_boundary' => TRUE,
      'html' => TRUE,
    );
    return views_trim_text($options, $text);
  }
  return text_summary($text, 'rich_text', $length);
}