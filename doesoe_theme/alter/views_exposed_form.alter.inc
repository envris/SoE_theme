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
      $form_state['input'][$broken_filter] = $form[$broken_filter]['#default_value'];
    }
  }

  // Create map extent filter.
  if ($view->name == 'site_search' && $view->current_display == 'region') {
    // Anchor to map on submit.
    $form['#action'] = $form['#action'] . '#search-map';
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

  // For the authors page, We use a combine filter to search both natt and theme
  // but need to manually turn it into a select.
  if ($view->name == 'authors') {
    $vocab = taxonomy_vocabulary_machine_name_load('themes');
    $tree = taxonomy_get_tree($vocab->vid, 0, NULL, TRUE);
    $form['theme'] = array_merge($form['theme'], array(
      '#type' => 'select',
      '#size' => NULL,
      '#empty_option' => '- Any -',
      '#empty_value' => '',
      '#options' => array(),
    ));
    foreach ($tree as $term) {
      $form['theme']['#options'][$term->tid] = check_plain($term->name);
    }
  }
}
