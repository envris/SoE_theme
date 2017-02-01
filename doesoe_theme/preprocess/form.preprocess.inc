<?php
/**
 * @file
 * Preprocessors for form.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function doesoe_theme_form_search_block_form_alter(&$form, &$form_state, $form_id) {
  $form['search_block_form']['#attributes']['placeholder'] = t('Search');
}
