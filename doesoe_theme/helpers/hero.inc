<?php
/**
 * @file
 * Hero helpers.
 */

define('DOESOE_THEME_HERO_DEFAULT_DELTA', 'default-hero');
define('DOESOE_THEME_HERO_DEFAULT_STYLE', 'hero');

/**
 * Returns the hero image for the current page.
 *
 * If first searches if the node has a hero_image_override field and returns the
 * image renderable array when it does. When the hero_image_override is not
 * present, it then checks if the node is tagged to a theme and returns the
 * theme's hero image. In the absense of the hero_image_override field and
 * theme field, this function returns the default global hero image.
 *
 * @return array
 *   A file entity for the matched hero image.
 */
function _doesoe_theme_get_hero_image() {
  $hero_image = &drupal_static(__FUNCTION__, array());

  // Return from cache if available.
  if (!empty($hero_image)) {
    return $hero_image;
  }

  // Get a default.
  $hero_entity = bean_load_delta(DOESOE_THEME_HERO_DEFAULT_DELTA);
  if (!empty($hero_entity->bid)) {
    $hero_image = _doesoe_theme_entity_get_value('bean', $hero_entity, 'field_hero_image');
  }

  // Override with theme.
  $theme = _doesoe_theme_get_current_theme_term();
  if (!empty($theme->tid)) {
    $term_hero_image = _doesoe_theme_entity_get_value('taxonomy_term', $theme, 'field_hero_image');
    $hero_image = empty($term_hero_image) ? $hero_image : $term_hero_image;
  }

  // Override with node.
  $node = menu_get_object();
  if (!empty($node->field_hero_image_override[LANGUAGE_NONE])) {
    $hero_image = _doesoe_theme_entity_get_value('node', $node, 'field_hero_image_override');
  }

  return (array) $hero_image;
}

/**
 * Returns the rendered hero image for the current page.
 *
 * @return string
 *   Markup for the hero image, or NULL if no image found.
 */
function _doesoe_theme_render_hero_image() {
  $hero_image = _doesoe_theme_get_hero_image();
  if (!empty($hero_image['uri'])) {
    return theme('image_style', array(
      'style_name' => DOESOE_THEME_HERO_DEFAULT_STYLE,
      'path' => $hero_image['uri'],
    ));
  }
  return NULL;
}

/**
 * Returns the hero subtitle for the current page.
 *
 * @return string
 *   The hero subtitle.
 */
function _doesoe_theme_get_hero_subtitle() {
  $hero_subtitle = '';

  if (!_doesoe_theme_is_theme_landing_page()) {
    $theme = _doesoe_theme_get_current_theme_term();
    if ($theme) {
      $hero_subtitle = $theme->name . ' ';
    }
    $years = _doesoe_theme_get_years_from_page();
    if (!empty($years['names'])) {
      $hero_subtitle .= '(' . implode(', ', $years['names']) . ')';
    }
  }

  return $hero_subtitle;
}

/**
 * Get the first slideshow image from the homepage slides.
 */
function _doesoe_theme_get_frontpage_hero_image() {
  $slideshow = views_get_view_result('slideshow', 'block');
  if (count($slideshow) > 0) {
    $row = reset($slideshow);
    return _doesoe_theme_entity_get_value('node', node_load($row->nid), 'field_slide_image');
  }
  return array();
}
