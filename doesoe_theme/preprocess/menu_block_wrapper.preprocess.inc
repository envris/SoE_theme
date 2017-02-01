<?php
/**
 * @file
 * Preprocessors for menu block wrapper.
 */

define('PSEUDO_THEME_MENU_BLOCK_DELTA', '5');
define('TOPICS_MENU_BLOCK_DELTA', 4);
define('FRAMEWORKS_MENU_BLOCK_DETLA', 3);

/**
 * Implements theme_preprocess_menu_block_wrapper().
 */
function doesoe_theme_preprocess_menu_block_wrapper(&$variables) {
  // Suffix has been added to the template file to prepend content.
  $variables['suffix'] = array();

  $menus = array(
    'govcms_menu_block-main-menu',
    PSEUDO_THEME_MENU_BLOCK_DELTA,
    TOPICS_MENU_BLOCK_DELTA,
    FRAMEWORKS_MENU_BLOCK_DETLA,
  );

  /*
   * Mobile menu.
   *
   * The mobile menu has some tricky business logic, it needs to selectively
   * show menu trees based on the context of the page, the following business
   * rules are applied here:
   * - If not on any active trail it should show level one of menu tree.
   * - If active item is in level one but has no children it should show level
   *   one of menu tree.
   * - If level one item active and it has children, then it should show level
   *   two of tree.
   * - If level two is active and has children should it expand children.
   * - If level three is active, show level two tree with active item class.
   * - If a page is not in the tree, yet has a theme, show the tree for that
   *   theme (as if it was in that tree).
   * - This expects there will NEVER be a level four.
   *
   * The 'govcms_menu_block-main-menu' delta is a fully expanded main menu tree.
   */

  if (in_array($variables['delta'], $menus)) {

    // Get the current theme (term) context if applicable.
    $current_theme = _doesoe_theme_get_current_theme_term();

    // Placeholders if we do any re-writing.
    $back_link = array();
    $tree_override = array();
    $depth = 1;

    // Check level one for active state with children.
    if ($sub_tree = _doesoe_theme_menu_is_tree_active($variables['content'])) {
      // If matched add overrides.
      $back_link = _doesoe_theme_menu_get_back_link('Home', '<front>');
      $tree_override = $sub_tree;
    }

    // Check level two for active state with children.
    foreach (element_children($variables['content']) as $mlid) {
      $link = $variables['content'][$mlid];

      // If this is a NATT theme it should be in the top level.
      if ($sub_tree = _doesoe_theme_menu_is_tree_active($variables['content'], $current_theme)) {
        // If matched add overrides.
        $back_link = _doesoe_theme_menu_get_back_link('Home', '<front>');
        $tree_override = $sub_tree;
      }

      // We also pass the current theme if we need to dynamically activate.
      if ($sub_tree = _doesoe_theme_menu_is_tree_active($link['#below'], $current_theme)) {
        // If matched add overrides.
        $back_link = _doesoe_theme_menu_get_back_link($link['#title'], $link['#href']);
        $tree_override = $sub_tree;
        $depth = 2;
      }

      // Check level three for active state with children.
      if (!empty($link['#below'])) {
        foreach (element_children($link['#below']) as $st_mlid) {
          $st_link = $link['#below'][$st_mlid];
          // We also pass the current theme if we need to dynamically activate.
          if ($st_sub_tree = _doesoe_theme_menu_is_tree_active($st_link['#below'], $current_theme)) {
            // If matched add overrides.
            $back_link = _doesoe_theme_menu_get_back_link($st_link['#title'], $st_link['#href']);
            $tree_override = $st_sub_tree;
            $tree_override['#title'] = $st_link['#title'] . ' - ' . $tree_override['#title'];
            $depth = 3;
          }
        }
      }
    }

    // If we have an override.
    if (!empty($tree_override)) {
      // First item in an override list forms a header.
      $tree_override['#localized_options']['attributes']['class'][] = 'link-header';
      // Add a unique wrapper class based on the override list title.
      $tree_override['#attributes']['class'][] = 'tree__' . drupal_html_class($tree_override['#title']);
      // Replace the menu tree.
      $variables['content'] = array(
        'active_tree' => $tree_override,
        '#sorted' => TRUE,
        '#theme_wrappers' => $variables['content']['#theme_wrappers'],
      );
      // Add a back link.
      if (!empty($back_link)) {
        $variables['suffix'] = $back_link;
      }
      // Add depth.
      $variables['classes_array'][] = 'depth-' . $depth;
    }
    else {
      // If we have no overrides should be on top level, so remove all children.
      foreach (element_children($variables['content']) as $mlid) {
        $variables['content'][$mlid]['#below'] = array();
      }
      // Hide top level sidebar menu from screen readers to avoid the
      // duplication of the header menu.
      if ($variables['delta'] == 'govcms_menu_block-main-menu') {
        $variables['attributes_array']['aria-hidden'] = 'true';
      }
    }

    // If we haven't found a back link default to home for these.
    if (empty($variables['suffix'])) {
      $back_link = _doesoe_theme_menu_get_back_link('Home', '<front>');
      $variables['suffix'] = $back_link;
    }
  }
}

/**
 * A helper to return a back link as a renderable array.
 *
 * @param string $text
 *   The link text. "Back to" will be prepended.
 * @param string $path
 *   The link url.
 *
 * @return array
 *   A back link array.
 */
function _doesoe_theme_menu_get_back_link($text, $path) {
  return array(
    '#theme' => 'link',
    '#text' => '<span></span>' . t('Back to @text', array('@text' => $text)),
    '#path' => $path,
    '#options' => array(
      'html' => TRUE,
      'attributes' => array('class' => array('back-link')),
    ),
  );
}
