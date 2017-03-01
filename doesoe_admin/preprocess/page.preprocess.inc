<?php
/**
 * @file
 * Preprocess html for admin pages.
 */

/**
 * Implements hook_preprocess_page().
 */
function doesoe_admin_preprocess_page(&$variables) {
  if (current_path() == 'admin/reports') {
    $links = array(
      l('References All', 'admin/reports/references-all'),
      l('References Assessment Summary Components', 'admin/reports/references-asc'),
      l('Taxonomy Terms', 'admin/reports/terms'),
      l('Content > References', 'admin/reports/references-topic'),
      l('Content > Maps', 'admin/reports/maps'),
      l('Content > Graphs', 'admin/reports/content-graph'),
    );

    // Update the link
    $markup = & $variables['page']['content']['system_main']['main']['#markup'];
    $markup = str_replace('</ul>', '<li class="leaf">' . implode('</li><li class="leaf">', $links) . '</li></ul>', $markup);
  }
}
