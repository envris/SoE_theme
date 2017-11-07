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

    $delta_main_menu = $el['#bid']['delta'] == 'govcms_menu_block-main-menu';
    $delta_topics_a = $el['#bid']['delta'] == 4;
    $delta_topics_b = $el['#bid']['delta'] == 6;
    $delta_framework_menu = $el['#bid']['delta'] == 3;
    $depth_1 = $el['#original_link']['depth'] == 1;
    $depth_2 = $el['#original_link']['depth'] == 2;
    $has_children = !empty($el['#below']);
    $href_is_term = strpos($el['#href'], 'taxonomy/term/') === 0;

    // If main menu with depth 1 or 2 with children add icon.
    if ($delta_main_menu && ($depth_1 && $has_children || $depth_2 && $has_children)) {
      _doesoe_theme_add_icon_to_menu_link($variables);
    }

    // If topics page or topics all page with depth of 2 children.
    if ($delta_topics_a && $depth_2) {
      _doesoe_theme_add_icon_to_menu_link($variables);
    }

    // If framework menu and link is term.
    if ($delta_framework_menu && $href_is_term) {
      $tid = str_replace('taxonomy/term/', '', $el['#href']);
      $term = taxonomy_term_load($tid);
      if ($term->vocabulary_machine_name === 'frameworks') {
        _doesoe_theme_add_icon_to_menu_link($variables);
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
