<?php
/**
 * @file
 * Preprocessors for Page.
 */

/**
 * Implements theme_preprocess_page().
 */
function doesoe_theme_preprocess_page(&$variables) {
  $variables['logo'] = base_path() . path_to_theme() . '/images/logo-white.png';

  $variables['theme_path'] = base_path(). path_to_theme();

  $variables['header_class'] = '';
  if ($variables['is_front']) {
    $variables['header_class'] = 'is-transparent';
  }
  else {
    // Hero image.
    $variables['hero'] = _doesoe_theme_render_hero_image();
    $variables['subtitle'] = _doesoe_theme_get_hero_subtitle();
  }

  // Add the search form to be used as a variable.
  $variables['search_block'] = theme('search_block_doesoe', array(
    'classes_array' => array('search-header-block'),
  ));

  // Set active menu item for content types.
  if (isset($variables['node'])) {
    switch ($variables['node']->type) {
      case "report_topic_content":
        $theme = _doesoe_theme_get_theme_from_node($variables['node']);
        if ($theme) {
          menu_set_active_item('theme/' . drupal_html_class($theme->name) . '/topics');
        }
        break;

      case "assessment_summary":
        $theme = _doesoe_theme_get_theme_from_node($variables['node']);
        if ($theme) {
          menu_set_active_item('theme/' . drupal_html_class($theme->name) . '/assessment-summaries');
        }
        break;
    }
  }

  // Set active menu item for framework panel page.
  if (arg(0) == 'theme' && arg(2) == 'framework' && arg(3) !== 'introduction') {
    menu_set_active_item('theme/' . arg(1) . '/by-framework');
  }

  // Add citation to Topic and Assessment Summary pages.
  $theme = _doesoe_theme_get_current_theme_term();
  if (!empty($theme->tid)) {
    if ($theme) {
      $variables['citation'] = theme('doesoe_theme_citation', array(
        'theme' => $theme,
      ));
    }
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function doesoe_theme_preprocess_maintenance_page(&$variables) {
  // Add the theme_path variable so we can use the path in the template.
  $variables['theme_path'] = base_path(). path_to_theme();
}
