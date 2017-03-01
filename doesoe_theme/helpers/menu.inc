<?php
/**
 * @file
 * Menu helpers.
 */

/**
 * A helper to check if a link is active in the menu.
 *
 * @param array $link
 *   A menu link renderable array.
 *
 * @return bool
 *   TRUE if item is active, FALSE if not.
 */
function _doesoe_theme_menu_is_link_active($link) {
  return isset($link['#attributes']['class']) && (in_array('active', $link['#attributes']['class']) || in_array('active-trail', $link['#attributes']['class']));
}

/**
 * A helper to check if a tree is active in the menu and has children.
 *
 * @param array $items
 *   The items in the current tree.
 * @param mixed $current_term
 *   Either a term object for the current term or FALSE if no theme applies.
 *
 * @return mixed
 *   If an active sub menu exists, return it. Otherwise return FALSE.
 */
function _doesoe_theme_menu_is_tree_active($items, $current_term = FALSE) {
  if (empty($items)) {
    return FALSE;
  }

  foreach (element_children($items) as $mlid) {
    $link = $items[$mlid];
    $theme_match = $current_term ? ($link['#href'] == 'taxonomy/term/' . $current_term->tid) : FALSE;
    if (!empty($link['#below']) && (_doesoe_theme_menu_is_link_active($link) || $theme_match)) {
      return $link;
    }
  }
  return FALSE;
}

/**
 * A helper to determine if side nav should be visible on desktop.
 */
function _doesoe_theme_menu_show_nav_side() {
  $menu_name = 'main-menu';

  // Force sidebar on these content types.
  $types_with_sidebar = array('page');

  // Disable sidebar on these types.
  $types_without_sidebar = array('author');

  // Force sidebar on these paths.
  $paths_with_sidebar = array();

  // Force sidebar on $types_with_sidebar.
  $node = menu_get_object();
  if (!empty($node->type) && in_array($node->type, $types_with_sidebar)) {
    return TRUE;
  }

  // Disable sidebar on $types_without_sidebar.
  if (!empty($node->type) && in_array($node->type, $types_without_sidebar)) {
    return FALSE;
  }

  // All items tagged with a theme get side nav.
  $theme = _doesoe_theme_get_current_theme_term();
  if ($theme) {
    return TRUE;
  }

  // Get the current menu link.
  $current_menu_item = menu_link_get_preferred(current_path(), $menu_name);
  if (isset($current_menu_item['plid'])) {

    // If it has a parent, nav is visible.
    if ($current_menu_item['plid'] != 0) {
      return TRUE;
    }

    // Lastly, if it has children, should be visible, we need to build a tree to
    // discover this.
    $parameters = array(
      'active_trail' => array($current_menu_item['plid']),
      'only_active_trail' => FALSE,
      'min_depth' => $current_menu_item['depth'] + 1,
      'max_depth' => $current_menu_item['depth'] + 1,
      'conditions' => array('plid' => $current_menu_item['mlid']),
    );
    $children = menu_build_tree($menu_name, $parameters);
    return !empty($children);
  }

  // Force sidebar on these paths.
  if (in_array(current_path(), $paths_with_sidebar)) {
    return TRUE;
  }

  // None of above matched.
  return FALSE;
}

/**
 * Add icon to menu link.
 *
 * @param array $variables
 *   A variables array from theme_preprocess_menu_link().
 */
function _doesoe_theme_add_icon_to_menu_link(&$variables) {
  $el = $variables['element'];
  $tid = str_replace('taxonomy/term/', '', $el['#href']);
  if ($term = taxonomy_term_load($tid)) {
    $entity_w = entity_metadata_wrapper('taxonomy_term', $term);
    if (isset($entity_w->field_icon_code)) {
      $icon = theme('icon', array(
        'icon' => $entity_w->field_icon_code->value(),
      ));
      $variables['element']['#title'] = $icon . $variables['element']['#title'];
      $variables['element']['#localized_options']['html'] = TRUE;
      $variables['element']['#localized_options']['attributes']['class'][] = 'with-icon';
      // Add a class to pseudo-theme menu link so that we can distinguish
      // it from the others.
      if (!empty($entity_w->field_natt) && $entity_w->field_natt->value()) {
        $variables['element']['#localized_options']['attributes']['class'][] = 'pseudo';
      }
    }
  }
}
