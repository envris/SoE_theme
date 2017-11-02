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
    if ($delta_topics_a || $delta_topics_b && $depth_2) {
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

/**
 * Implements theme_menu_link().
 *
 * @param array $variables
 * @return string
 */
function doesoe_theme_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';
  $name_id = strtolower(strip_tags($element['#title']));
  $pattern = '/[^a-z]+/ ';

  // Rebuilding of link.
  $name_id = preg_replace($pattern, '', $name_id);
  $element['#attributes']['class'][] = $name_id;

  // Disable Atmosphere link on topics page.
  if ($element['#bid']['delta'] == 6 && $name_id == 'atmosphere') {
    // Output title without link
    $output = $element['#title'];
  }
  else {
    // Output title with link
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  }

  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}
