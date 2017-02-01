<?php
/**
 * @file
 * Preprocessors for Block.
 */

/**
 * Implements theme_preprocess_block().
 */
function doesoe_theme_preprocess_block(&$variables) {
  $block = $variables['block'];
  if (!$block->bid == 'search-form' || !$block->bid == 'menu_block-1') {
    return;
  }

  // Automatically add some view classes so we aren't dealing with hashes or
  // ids to identify views blocks.
  if ($block->module == 'views') {
    $hash_dictionary = variable_get('views_block_hashes', array());
    $class = (isset($hash_dictionary[$block->delta]) ? $hash_dictionary[$block->delta] : $block->delta);
    $variables['classes_array'][] = 'views--' . drupal_html_class($class);
  }
  else {
    // If not a view block, add a module/delta class that Zen fails to do.
    $variables['classes_array'][] = $block->module . '--' . $block->delta;
  }

  // Block specific classes.
  switch ($block->bid) {
    case 'search-form':
      unset($variables['classes_array'][array_search('block-search', $variables['classes_array'])]);
      $variables['classes_array'][] = 'search-header-block';
      break;

    case 'menu_block-1':
      $variables['classes_array'][] = 'main-menu-header-block';
      break;

    case 'menu-menu-footer-secondary':
      $variables['classes_array'][] = 'menu-footer-secondary';
      break;

    case 'menu-menu-footer-primary':
      $variables['classes_array'][] = 'menu-footer-primary';
      break;

    case 'bean-footer-freetext':
      $variables['classes_array'][] = 'footer-freetext';
      break;

    case 'bean-social-sharing-block':
      $variables['classes_array'][] = 'social-sharing-block';
      break;
  }
}
