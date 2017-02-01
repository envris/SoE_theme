<?php
/**
 * @file
 * Alters to html head tags.
 */

/**
 * Implements hook_html_head_alter().
 *
 * This is used for dynamic overrides of metatags that are not possible via the
 * metatag UI.
 */
function doesoe_theme_html_head_alter(&$head_elements) {

  // Add the most appropriate hero image, use share_image style.
  $hero_image = drupal_is_front_page() ? _doesoe_theme_get_frontpage_hero_image() : _doesoe_theme_get_hero_image();
  if (!empty($hero_image['uri'])) {
    $og_image = image_style_url('share_image', $hero_image['uri']);
    _doesoe_theme_build_metatag($head_elements, 'og:image', $og_image);
  }

  // Provide a fallback description for theme content if not exists.
  if (empty($head_elements['metatag_description_0'])) {
    $theme = _doesoe_theme_get_current_theme_term();
    if (!empty($theme->description)) {
      $desc = truncate_utf8(strip_tags($theme->description), 147, FALSE, TRUE);
      _doesoe_theme_build_metatag($head_elements, 'description', $desc, array('#weight' => 2));
      _doesoe_theme_build_metatag($head_elements, 'og:description', $desc, array('#weight' => 31));
    }
  }
  else {
    // If meta description is set, ensure og:description is the same.
    _doesoe_theme_build_metatag($head_elements, 'og:description', $head_elements['metatag_description_0']['#value'], array('#weight' => 31));
  }
}
