<?php
/**
 * @file
 * Form alters and after builds.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function doesoe_admin_form_node_type_form_alter(&$form, &$form_state, $form_id) {
  $type = $form['#node_type'];

  // Nodes.
  $form['doesoe_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Doesoe Admin - Hide fields'),
    '#description' => t("These fields will be hidden from <em>@type</em> node forms.", array('@type' => $type->name)),
    '#access' => user_access('administer content types'),
    '#group' => 'additional_settings',
  );
  $form['doesoe_admin']['doesoe_admin_nodes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Hide'),
    '#options' => doesoe_admin_get_fields('nodes'),
    '#default_value' => variable_get('doesoe_admin_nodes_' . $type->type, array()),
  );

}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function doesoe_admin_form_node_form_alter(&$form, &$form_state, $form_id) {
  $node = $form['#node'];

  // Get array of fields to hide.
  $fields = variable_get('doesoe_admin_nodes_' . $node->type, array());

  // Hide fields.
  doesoe_admin_hide_fields($fields, $form);

  if (!empty($form['#node_edit_form'])) {
    $theme_fields = array('field_theme', 'field_themes');
    foreach ($theme_fields as $field) {
      if (!empty($form[$field])) {
        $language = $form[$field]['#language'];
        if ($form[$field][$language]['#type'] === 'select') {
          $form[$field][$language]['#after_build'][] = 'doesoe_admin_field_theme_after_build';
        }
      }
    }

    // Turn the weight textfield into a select.
    if (!empty($form['field_weight'])) {
      $language = $form['field_weight']['#language'];
      $element = $form['field_weight'][$language][0]['value'];
      $opts = array();
      for ($i = -100; $i <= 100; $i++) {
        $opts[$i] = $i;
      }
      $select = array('#type' => 'select', '#options' => $opts, '#size' => 1);
      $element['#default_value'] = $element['#default_value'] == '' ? 0 : $element['#default_value'];
      $form['field_weight'][$language][0]['value'] = array_merge($element, $select);
    }
  }
}

/**
 * After build function for field_theme.
 *
 * Loop through select list options and remove option if it is a NATT.
 */
function doesoe_admin_field_theme_after_build($form_element, &$form_state) {
  $options = &$form_element['#options'];
  $tids = array_filter(array_keys($options), 'is_int');
  foreach ($tids as $tid) {
    $term = taxonomy_term_load($tid);
    if ($term && _doesoe_admin_theme_is_natt($term)) {
      unset($options[$tid]);
    }
  }
  return $form_element;
}

/**
 * Implements hook_field_widget_form_alter().
 */
function doesoe_admin_field_widget_form_alter(&$element, &$form_state, $context) {
  // Turn year fields that should only accept a single year into radios.
  if (isset($element['#field_name']) && $element['#field_name'] == 'field_years') {
    $multi_field_entities = _doesoe_admin_form_multi_select_year_entities();
    // If this type/bundle is not multiple, change to radios.
    if (
      !isset($multi_field_entities[$element['#entity_type']]) ||
      !in_array($element['#bundle'], $multi_field_entities[$element['#entity_type']])
    ) {
      $default_value = reset($element['#default_value']);
      $element['#type'] = 'radios';
      $element['#default_value'] = $default_value !== FALSE ? $default_value : NULL;
    }
  }

  // If alt text is empty, set the default to the file name.
  if (isset($element['#field_name']) && $element['#field_name'] == 'field_file_image_alt_text') {
    if (empty($element['value']['#default_value'])) {
      $element['value']['#default_value'] = truncate_utf8($element['value']['#entity']->filename, 100);
    }
  }
}
