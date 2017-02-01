<?php

/**
 * @file
 * Preprocessors for Citation.
 */

/**
 * Implements theme_preprocess_doesoe_theme_citation().
 */
function doesoe_theme_preprocess_doesoe_theme_citation(&$variables) {
  $theme = $variables['theme'];
  $theme_w = entity_metadata_wrapper('taxonomy_term', $theme);
  $current_year = _doesoe_theme_get_active_year(TRUE);

  // Authors.
  $authors = _doesoe_theme_get_authors($theme, $current_year);
  $authors_names = array();
  foreach ($authors as $author) {
    $author_w = entity_metadata_wrapper('node', $author);
    if (isset($author_w->field_short_title)) {
      $authors_names[] = check_plain($author_w->field_short_title->value());
    }
  }
  $variables['authors'] = implode(', ', $authors_names);

  // Theme.
  $variables['theme_name'] = check_plain($theme->name);
  $variables['doi'] = isset($theme_w->field_doi) ? $theme_w->field_doi->value() : '';

  // Generic.
  $variables['page_title'] = drupal_get_title();
  $variables['report_year'] = check_plain($current_year->name);
  $variables['date'] = format_date(time(), 'custom', 'j F Y');
  $variables['url'] = url(current_path(), array('absolute' => TRUE));

  // Report name.
  $default_report_name = t('Australia state of the environment 2016, Australian Government Department of the Environment and Energy, Canberra');
  $variables['report_name'] = variable_get('doesoe_cite_report_name', $default_report_name);

  // Attributes.
  $variables['classes_array'] = array('citation', 'element-invisible');
  $variables['classes'] = implode(' ', $variables['classes_array']);
  $variables['attributes_array']['id'] = 'citation';
  $variables['attributes'] = drupal_attributes($variables['attributes_array']);
}
