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

/**
 * Strip tags from a text field.
 *
 * This ensures that the string has been completely decoded before stripping
 * the tags.
 *
 * @param string $string
 *   A string the strip tags from.
 * @param string $allowed_tags
 *   A tag list as expected by strip_tags.
 *
 * @return string
 *   A string value with tags removed.
 */
function _doesoe_theme_strip_tags($string, $allowed_tags = '<strong>') {
  $string = htmlspecialchars_decode($string);
  return strip_tags($string, $allowed_tags);
}

/**
 * Replace a href in a string.
 *
 * @param string $string
 *   String to find replacements in.
 * @param string $replacement
 *   Replacement pattern.
 * @param string $tag
 *   Tag to look for to replace.
 *
 * @return mixed
 *   A replaced string.
 */
function _doesoe_theme_replace_href($string, $replacement, $tag = 'a') {
  $regx = '/<' . $tag . '(.*?)href="(.*?)"(.*?)>/';
  $replacement = '<' . $tag . '$1href="' . $replacement . '"$3>';
  return preg_replace($regx, $replacement, $string);
}
