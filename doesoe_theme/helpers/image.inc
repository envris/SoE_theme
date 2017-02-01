<?php
/**
 * @file
 * Image related helpers.
 */

/**
 * Parse image map settings string into attributes ready for theme_tag.
 *
 * @param string $string
 *   The user entered string, attributes on new line, key|val.
 *
 * @return array
 *   An array of attributes ready to be added to an <area> tag.
 */
function _doesoe_theme_image_parse_map($string) {
  $attrs = array();
  $attr_lines = explode(PHP_EOL, str_replace(array("\r\n", "\n"), PHP_EOL, $string));
  foreach ($attr_lines as $attr_line) {
    // No empty lines, no lines without pipes.
    if (empty($attr_line) || strpos($attr_line, '|') === FALSE) {
      continue;
    }
    list($attr, $val) = explode("|", $attr_line, 2);
    $attrs[$attr] = $val;
  }
  // Default to a poly.
  $attrs['shape'] = !empty($attrs['shape']) ? $attrs['shape'] : 'poly';
  return $attrs;
}
