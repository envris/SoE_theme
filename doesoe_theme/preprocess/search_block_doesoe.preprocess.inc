<?php
/**
 * @file
 * Custom search block preprocess.
 */

/**
 * Populate the search block with a form.
 */
function doesoe_theme_preprocess_search_block_doesoe(&$variables) {
  $variables['search_form'] = drupal_get_form('doesoe_theme_search_block_doesoe_form');
  $variables['classes_array'][] = 'search-block';
  $variables['classes'] = implode(' ', $variables['classes_array']);
}

/**
 * Form structure for the search form.
 *
 * This emulates the structure of the core drupal search block that we no
 * longer use.
 */
function doesoe_theme_search_block_doesoe_form($form, &$form_state) {
  $form = array();

  $form['container'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['container']['title'] = array(
    '#type' => 'markup',
    '#markup' => '<h2 class="element-invisible">' . t('Search form') . '</h2>',
  );

  $form['container']['text'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($_GET['text']) ? check_plain($_GET['text']) : '',
    '#title' => t('Enter your search keywords.'),
    '#title_display' => 'invisible',
  );

  $form['container']['search_type'] = array(
    '#type' => 'select',
    '#options' => array(
      'site' => t('Site search'),
      'map' => t('Region search'),
      'assessment' => t('Assessment summary search'),
    ),
    '#title' => t('Select the type of search.'),
    '#title_display' => 'invisible',
    '#default_value' => 'site',
  );

  $form['container']['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
  );

  $form['container']['actions']['op'] = array(
    '#type' => 'submit',
    '#value' => 'Search',
  );

  $form['#action'] = url(current_path());
  $form['#submit'][] = 'doesoe_theme_search_block_doesoe_form_submit';

  return $form;
}

/**
 * On search submit we redirect to the search page with the views page.
 *
 * The text query is populated from the value provided.
 */
function doesoe_theme_search_block_doesoe_form_submit($form, &$form_state) {
  $query = $form_state['values']['text'];

  // Change search dest based on type.
  switch ($form_state['values']['search_type']) {
    case 'site':
      drupal_goto('search', array('query' => array('text' => $query)));
      break;

    case 'map':
      drupal_goto('region-search', array('query' => array('text' => $query)));
      break;

    case 'assessment':
      drupal_goto('assessment-summary-components', array('query' => array('text' => $query)));
      break;
  }
}
