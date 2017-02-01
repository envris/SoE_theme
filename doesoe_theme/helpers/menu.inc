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
  return isset($link['#attributes']['class']) && in_array('active', $link['#attributes']['class']);
}

/**
 * A helper to check if a tree is active in the menu and has children.
 *
 * @param array $items
 *   The items in the current tree.
 * @param mixed $current_theme
 *   Either a term object for the current theme or FALSE if no theme applies.
 *
 * @return mixed
 *   If an active sub menu exists, return it. Otherwise return FALSE.
 */
function _doesoe_theme_menu_is_tree_active($items, $current_theme = FALSE) {
  if (empty($items)) {
    return FALSE;
  }

  foreach (element_children($items) as $mlid) {
    $link = $items[$mlid];
    $theme_match = $current_theme ? ($link['#href'] == 'taxonomy/term/' . $current_theme->tid) : FALSE;
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
  $types_with_sidebar = array('author', 'page');

  // Force sidebaron these paths.
  $paths_with_sidebar = array('authors');

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

  // Force sidebar on $types_with_sidebar.
  $node = menu_get_object();
  if (!empty($node->type) && in_array($node->type, $types_with_sidebar)) {
    return TRUE;
  }

  // Force sidebar on these paths.
  if (in_array(current_path(), $paths_with_sidebar)) {
    return TRUE;
  }

  // None of above matched.
  return FALSE;
}
