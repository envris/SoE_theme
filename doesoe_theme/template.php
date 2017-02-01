<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728096
 */

$theme_dir = drupal_get_path('theme', 'doesoe_theme');

/**
 * Include helpers from the helpers folder.
 *
 * Naming convention for alter files should be something like this:
 * {helper}.inc.
 */
foreach (file_scan_directory($theme_dir . '/helpers', '/.+\.inc/i') as $file) {
  require_once $file->uri;
}

/**
 * Include preprocessors.
 *
 * For better organisation and to prevent this file from becoming monolithic
 * preprocessors should be placed in the preprocess folder. This code will then
 * include all those files as if the lived here.
 *
 * Naming convention for preprocess files should be something like this:
 * {theme_hook}.preprocess.inc
 */
foreach (file_scan_directory($theme_dir . '/preprocess', '/.+\.inc/i') as $file) {
  require_once $file->uri;
}

/**
 * Include alters from the alter folder.
 *
 * Naming convention for alter files should be something like this:
 * {theme_hook}.alter.inc.
 */
foreach (file_scan_directory($theme_dir . '/alter', '/.+\.inc/i') as $file) {
  require_once $file->uri;
}

/**
 * Implements hook_theme().
 *
 * All templates for theme keys defined here should live in templates/custom.
 */
function doesoe_theme_theme($existing, $type, $theme, $path) {
  $custom_template_folder = 'templates/custom/';
  return array(
    // Theme output for an assessment summary header.
    'assessment_summary_header' => array(
      'template' => $custom_template_folder . 'assessment-summary-header',
      'variables' => array(
        'grades' => NULL,
        'grade_scale' => 4,
      ),
    ),
    // Search box.
    'search_block_doesoe' => array(
      'template' => $custom_template_folder . 'search-block-doesoe',
      'variables' => array('search_form' => array()),
    ),
    // Search accordion toggle element.
    'search_accordion_toggle_element' => array(
      'template' => $custom_template_folder . 'search-accordion-toggle-element',
      'variables' => array('title' => NULL, 'icon' => NULL, 'alt_action' => NULL),
    ),
    // Icomoon icon element.
    'icon' => array(
      'template' => $custom_template_folder . 'icon',
      'variables' => array('icon' => NULL, 'title' => NULL, 'type'),
    ),
    // Citation.
    'doesoe_theme_citation' => array(
      'variables' => array(
        'theme' => NULL,
      ),
      'path' => $path . '/templates/custom',
      'template' => 'doesoe-theme-citation',
    ),
  );
}
