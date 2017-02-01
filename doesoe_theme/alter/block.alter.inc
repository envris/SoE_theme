<?php
/**
 * @file
 * Block alters.
 */

/**
 * Implements hook_block_view_alter().
 */
function doesoe_theme_block_view_alter(&$data, $block) {

  // We turn the sidebar authors block into a view using the context of
  // the theme it is viewed in.
  if ($block->module == 'bean' && $block->delta == 'sidebar-authors-explore') {
    // Get the current theme.
    $current_theme = _doesoe_theme_get_current_theme_term();
    // Get the current year.
    $year_tid = _doesoe_theme_get_active_year();
    // If we have a theme, replace block content.
    if ($current_theme && $current_theme->tid) {
      $data['content'] = views_embed_view('authors', 'sidebar_block', $current_theme->tid, $current_theme->tid, $year_tid);
    }
    // Else will fallback to a link to all authors.
  }

}
