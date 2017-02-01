<?php
/**
 * @file
 * Table of contents helpers.
 */

/**
 * This adds the settings for what headings the TOC (On this page) should use.
 */
function _doesoe_theme_toc_headings() {
  // Deafult h2s.
  $settings = array('headings' => 'h2', 'content' => '.layout__content');

  // Theme landing has all the headings as h4s.
  $term = menu_get_object('taxonomy_term', 2);
  if (isset($term->vocabulary_machine_name) && $term->vocabulary_machine_name == 'themes') {
    $settings = array('headings' => 'h4', 'content' => '.layout__content');
  }

  // Node pages use h3,h4 levels.
  $node = menu_get_object('node');
  if (!empty($node->nid)) {
    $settings = array('headings' => 'h3,h4', 'content' => '.layout__content .node.view-mode-full');
  }

  return $settings;
}
