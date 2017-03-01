<?php
/**
 * @file
 * Url helpers.
 */

/**
 * Return an ID for an anchor on nodes via the title.
 *
 * @param object $node
 *   Node object.
 *
 * @return string
 *   Anchor ID.
 */
function _doesoe_theme_url_node_anchor_id($node) {
  return drupal_html_class(strip_tags($node->title)) . '--' . $node->nid;
}

/**
 * Helper to replace the urls in search results that link to a topic + anchor.
 *
 * @see views.preprocess.inc
 */
function _doesoe_theme_url_search_result_link_via_topic_and_anchor(&$vars, $entity, $topic_field = 'field_topic') {
  $topics = field_get_items('node', $entity, $topic_field);

  // If we don't have any topics we can't update the link.
  if (empty($topics)) {
    return;
  }

  $topic = reset($topics);
  $link = url('node/' . $topic['target_id'], array('fragment' => _doesoe_theme_url_node_anchor_id($entity)));

  $link_fields = array('title', 'view_node');
  foreach ($link_fields as $field) {
    $vars['fields'][$field]->content = _doesoe_theme_replace_href($vars['fields'][$field]->content, $link);
  }
}