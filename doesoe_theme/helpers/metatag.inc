<?php
/**
 * @file
 * Helpers to deal with metatags.
 */

/**
 * This helper builds or updates a html head metatag.
 *
 * @param array $meta_tags
 *   Array of current tags passed by reference.
 * @param string $name
 *   Name of property eg 'og:image'.
 * @param string $value
 *   The value for the tag eg 'http://my.com/image.jpg'.
 * @param array $overrides
 *   Values to merge into the default tags, each key should start with #.
 */
function _doesoe_theme_build_metatag(&$meta_tags, $name, $value, $overrides = array()) {
  $id = 'metatag_' . $name . '_0';
  $default = array(
    '#theme' => 'metatag_property',
    '#tag' => 'meta',
    '#id' => $id,
    '#name' => $name,
    '#value' => $value,
    '#weight' => 30,
    '#type' => 'html_tag',
  );
  $meta_tags[$id] = array_merge($default, $overrides);
}
