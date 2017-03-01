<?php
/**
 * @file
 * Atmosphere theme helpers.
 */

/**
 * This helper returns the intro text to toggle to the alternate sub theme.
 *
 * Example output:
 * These $title are for the $current_theme section of the Atmosphere theme
 * > $title for $other_theme (link).
 */
function _doesoe_theme_atmosphere_sub_theme_intro_text($section_title, $section_path = 'theme/!term/by-framework') {
  // Get current theme.
  $current_theme = _doesoe_theme_get_current_theme_term();
  if (!$current_theme) {
    return NULL;
  }

  // Get all sub themes.
  $vocab = taxonomy_vocabulary_machine_name_load('themes');
  $atmo_themes = taxonomy_get_term_by_name('Atmosphere', 'themes');
  $atmo_theme = reset($atmo_themes);
  $tree = taxonomy_get_tree($vocab->vid, $atmo_theme->tid);
  if (empty($tree)) {
    return NULL;
  }

  // Get 'other' sub theme (filter out current).
  $other_theme = FALSE;
  $subtheme_list = array();
  foreach ($tree as $term) {
    $subtheme_list[] = $term->tid;
    if ($term->tid != $current_theme->tid) {
      $other_theme = $term;
    }
  }

  // Check is a vailid subtheme, if so output text.
  if (in_array($current_theme->tid, $subtheme_list) && $other_theme) {
    $text = t('These @section_title are for the @current_theme section of the Atmosphere theme.', array(
      '@current_theme' => $current_theme->name,
      '@section_title' => $section_title,
    ));
    $link_url = str_replace('!term', drupal_html_class($other_theme->name), $section_path);
    $link = l(t('@section for @term', array('@section' => $section_title, '@term' => $other_theme->name)), $link_url, array(
      'attributes' => array('class' => array('button--arrow')),
    ));
    // Wrap in paragraphs and return.
    return '<div class="theme-intro__sub-theme theme-palette-inherit-links"><p>' . $text . '</p><p>' . $link . '</p></div>';
  }
  return NULL;
}
