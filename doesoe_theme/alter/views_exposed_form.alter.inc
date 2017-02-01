<?php
/**
 * @file
 * Alters for views exposed forms.
 */

/**
 * Implements hook_form_views_exposed_form_alter().
 */
function doesoe_theme_form_views_exposed_form_alter(&$form, &$form_state, $form_id) {
  $view = $form_state['view'];

  // For some reason, the default values of the Search API exposed filters never
  // get applied. Because the 'Year' filter must have a default value, we have
  // to implement the below workaround.
  if (empty($form_state['input'])) {
    $broken_filters = array('year');
    foreach ($broken_filters as $broken_filter) {
      if (isset($form_state['input'][$broken_filter])) {
        $form_state['input'][$broken_filter] = $form[$broken_filter]['#default_value'];
      }
    }
  }

  // Create map extent filter.
  if ($view->name == 'site_search' && $view->current_display == 'region') {
    $vocab = taxonomy_vocabulary_machine_name_load('territories');
    $vid = $vocab->vid;
    $tree = taxonomy_get_tree($vid, 0, NULL, TRUE);
    $options = array();
    foreach ($tree as $term) {
      if ($coords_field = field_get_items('taxonomy_term', $term, 'field_geometry_collection')) {
        $coords = reset($coords_field);
        $options[$coords['value']] = $term->name;
      }
    }
    $form['extent'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#empty_option' => 'Australia',
      '#empty_value' => 'All',
    );
  }

  // Modify site ass sum exposed form.
  if ($view->name == 'assessment_summary_components' && $view->current_display == 'page') {
    // Modify grad labels to indicate what is good vs bad.
    $form['grade']['#options'][0] = '0 - Worst';
    $form['grade']['#options'][5] = '5 - Best';
  }

  $ass_summ_search = ($view->name == 'assessment_summary_components' && $view->current_display == 'page');
  $advanced_search = ($view->name == 'site_search' && $view->current_display == 'page');

  if ($advanced_search || $ass_summ_search) {

    $icon = theme('html_tag', array(
      'element' => array(
        '#tag' => 'i',
        '#attributes' => array(
          'class' => array('icon-map-marker'),
        ),
        '#value' => '',
      ),
    ));

    $form['region']['#prefix'] = l(t('!icon Show on map', array('!icon' => $icon)), 'region-search', array(
      'html' => TRUE,
      'attributes' => array(
        'class' => array('show-on-map'),
      ),
    ));
  }
}
