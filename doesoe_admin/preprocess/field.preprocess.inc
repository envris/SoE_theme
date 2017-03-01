<?php

/**
 * @file
 * Field process and preprocessors.
 */

/**
 * Implements template_preprocess_field().
 */
function doesoe_admin_preprocess_field(&$variables, $hook) {
  $element = $variables['element'];

  // Add a simplified template for the image description field and make it
  // visible only for screen readers.
  // NOTE: This is duplicated from the frontend theme due to some weird field
  // caching for file entity field.
  if ($element['#field_name'] == 'field_description' && $element['#bundle'] == 'image') {
    $variables['classes_array'][] = 'element-invisible';
    array_unshift($variables['theme_hook_suggestions'], "field__field_description__image");
  }
}
