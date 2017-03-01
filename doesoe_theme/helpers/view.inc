<?php
/**
 * @file
 * View helpers.
 */

/**
 * Check if view is a search page view.
 *
 * @param string $view
 *   A loaded view object.
 *
 * @return bool
 *   True if the view is a search page view.
 */
function _doesoe_theme_view_is_search_page($view) {
  $is_search = FALSE;
  $advanced_search = ($view->name === 'site_search' && $view->current_display == 'page');
  $region_search = ($view->name === 'site_search' && $view->current_display == 'region');
  $assum_search = ($view->name === 'assessment_summary_components' && $view->current_display == 'page');
  if ($advanced_search || $region_search || $assum_search) {
    $is_search = TRUE;
  }
  return $is_search;
}
