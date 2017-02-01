<?php
/**
 * @file
 * Preprocessors for menu links.
 */

/**
 * Implements theme_preprocess_menu_link().
 */
function doesoe_theme_preprocess_menu_link(&$variables) {
  $el = $variables['element'];

  // Adds icons to links in the sidebar.
  if (isset($el['#bid']['delta'])) {
    $available_depths = array(2, 3);
    if (($el['#bid']['delta'] == 'govcms_menu_block-main-menu' && in_array($el['#original_link']['depth'], $available_depths)) || ($el['#bid']['delta'] == 3)
      || ($el['#bid']['delta'] == 4) || ($el['#bid']['delta'] == 5)) {
      if (0 === strpos($el['#href'], 'taxonomy/term/')) {
        $tid = str_replace('taxonomy/term/', '', $el['#href']);
        $term = taxonomy_term_load($tid);
        $entity_w = entity_metadata_wrapper('taxonomy_term', $term);
        if (isset($entity_w->field_icon_code)) {
          $icon = theme('icon', array(
            'icon' => $entity_w->field_icon_code->value(),
          ));
          $variables['element']['#title'] = $icon . $variables['element']['#title'];
          $variables['element']['#localized_options']['html'] = TRUE;
          $variables['element']['#localized_options']['attributes']['class'][] = 'with-icon';
        }
      }
    }
    // Add header class to first level items in frameworks menu.
    if ($el['#bid']['delta'] == 3) {
      if ($el['#original_link']['depth'] == '1') {
        $variables['element']['#localized_options']['attributes']['class'][] = 'link-header';
      }
    }
  }
}
