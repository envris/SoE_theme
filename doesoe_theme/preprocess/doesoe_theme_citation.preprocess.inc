<?php

/**
 * @file
 * Preprocessors for Citation.
 */

/**
 * Implements theme_preprocess_doesoe_theme_citation().
 */
function doesoe_theme_preprocess_doesoe_theme_citation(&$variables) {
  global $base_root;
  $theme = $variables['theme'];
  $theme_w = entity_metadata_wrapper('taxonomy_term', $theme);
  $current_year = _doesoe_theme_get_active_year(TRUE);

  // If NATT we don't filter by an normal theme.
  $is_natt = _doesoe_theme_entity_get_value('taxonomy_term', $theme, 'field_natt');
  $authors_theme = !empty($is_natt) ? NULL : $theme;

  // If Drivers we filter by NATT.
  $natt_theme = (strtolower($theme->name) === 'drivers') ? $theme : NULL;

  // Get and build a list of author citation names.
  $authors = _doesoe_theme_get_authors($authors_theme, $current_year, $natt_theme);
  $authors_names = array();
  foreach ($authors as $author) {
    $short_title = _doesoe_theme_entity_get_value('node', $author, 'field_short_title');
    if (!empty($short_title)) {
      $authors_names[] = check_plain($short_title);
    }
  }
  $variables['authors'] = implode(', ', $authors_names);

  // Theme.
  $variables['theme_name'] = check_plain($theme->name);
  $variables['doi'] = isset($theme_w->field_doi) ? $theme_w->field_doi->value() : '';

  // Generic.
  $variables['page_title'] = drupal_get_title();
  $variables['report_year'] = isset($current_year->name) ? check_plain($current_year->name) : '';
  $variables['date'] = format_date(time(), 'custom', 'j F Y');
  $variables['url'] = $base_root . request_uri();

  // Report name.
  $default_report_name = t('Australia state of the environment !year, Australian Government Department of the Environment and Energy, Canberra', array(
    '!year' => $variables['report_year'],
  ));
  $variables['report_name'] = variable_get('doesoe_cite_report_name', $default_report_name);

  // Attributes.
  $variables['classes_array'] = array('citation', 'element-invisible');
  $variables['classes'] = implode(' ', $variables['classes_array']);
  $variables['attributes_array']['id'] = 'citation';
  $variables['attributes'] = drupal_attributes($variables['attributes_array']);
}
