<?php
/**
 * @file
 * Element process and preprocessors.
 */

/**
 * Process callback for the media form element.
 *
 * Replacing media class in order to override ctools modal window sizing.
 * This gets added in element.alter.inc.
 */
function doesoe_admin_media_element_process($element, &$form_state, $form) {
  if (isset($element['edit'])) {
    $key = array_search('ctools-modal-media-file-edit', $element['edit']['#attributes']['class']);
    $element['edit']['#attributes']['class'][$key] = 'ctools-modal-doesoe-admin-media-edit';

    drupal_add_js(array(
      'doesoe-admin-media-edit' => array(
        'modalSize' => array(
          'type' => 'scale',
          'width' => .98,
          'height' => .98,
        ),
      ),
    ), 'setting');
  }

  return $element;
}

/**
 * Implements hook_preprocess_HOOK().
 */
function doesoe_admin_preprocess_container(&$variables) {
  // Add label above single on/off checkbox for pseudo theme field.
  if (!empty($variables['element']['#language'])) {
    $lang = $variables['element']['#language'];
    if (!empty($variables['element'][$lang]['#field_name']) && $variables['element'][$lang]['#field_name'] === 'field_natt') {
      $label = theme('html_tag', array(
        'element' => array(
          '#tag' => 'label',
          '#value' => t('Pseudo Theme'),
        ),
      ));
      $variables['element']['#children'] = $label . $variables['element']['#children'];
    }
  }
}
