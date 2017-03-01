<?php
/**
 * @file
 * Theme helpers.
 */

define('DOESOE_THEME_THEME_TOPICS_DELTA', 'theme-topics');
define('DOESOE_THEME_THEME_ASS_SUMMARIES_DELTA', 'theme-assessment-summaries');
define('DOESOE_THEME_THEME_REFERENCES_DELTA', 'references-intro');
define('DOESOE_THEME_THEME_ACKNOWLEDGEMENTS_DELTA', 'acknowledgements-intro');
define('DOESOE_THEME_THEME_HEADLINES_DELTA', 'overview-headline-intro');

/**
 * Returns a contextual link for a theme, eg. See all @theme content.
 *
 * @return string
 *   The assessment summary link.
 */
function _doesoe_theme_get_contextual_link($path, $text, $class = NULL, $options = array()) {
  $link = '';
  if (!empty($class)) {
    $options['attributes']['class'][] = $class;
  }

  $term = _doesoe_theme_get_current_theme_term();
  if (!empty($term->tid)) {
    $text = t($text, array('@theme' => $term->name));
    $url = url('taxonomy/term/' . $term->tid) . '/' . $path;
    $link = l($text, $url, $options);
  }

  return $link;
}


/**
 * Returns a framework link for a theme.
 *
 * @param object $theme
 *   A theme taxonomy term object.
 * @param object $framework
 *   A framework taxonomy term object.
 *
 * @return string
 *   The topics link.
 */
function _doesoe_theme_get_theme_framework_link($theme, $framework) {
  return url('taxonomy/term/' . $theme->tid) . '/framework/' . drupal_html_class($framework->name);
}

/**
 * Based on the current page, returns the applicable theme term.
 *
 * @return object|bool
 *   A Theme term if found, FALSE if no theme applicable.
 */
function _doesoe_theme_get_current_theme_term() {

  // Potentially gets called multiple times per page so static cache.
  $term = &drupal_static(__FUNCTION__, FALSE);

  // If cached term found, return it.
  if ($term !== FALSE) {
    return $term;
  }

  // Theme is based on the context of the current node/term.
  $node = menu_get_object();
  if (isset($node->type)) {
    $node_theme = _doesoe_theme_get_theme_from_node($node);
    if (isset($node_theme->tid)) {
      $term = $node_theme;
      return $term;
    }
  }

  // Still here? not a node then, maybe a term?
  $current_term = menu_get_object('taxonomy_term', 2);
  if (isset($current_term->vocabulary_machine_name) && $current_term->vocabulary_machine_name == 'themes') {
    $term = $current_term;
    return $term;
  }

  // Still here? do one final path match (for views).
  if (arg(0) == 'theme') {
    $term_name = str_replace('-', ' ', arg(1));
    $term_matches = taxonomy_get_term_by_name($term_name, 'themes');
    if (!empty($term_matches)) {
      $term = reset($term_matches);
      return $term;
    }
  }

  // No match.
  return FALSE;
}

/**
 * Return the most appropriate theme from a node.
 *
 * @return mixed
 *   If node has natt then it will be returned, fallback to normal theme. If no
 *   term returns NULL.
 */
function _doesoe_theme_get_theme_from_node($node) {
  $natt_theme = _doesoe_theme_entity_get_value('node', $node, 'field_natt_reference');
  $node_theme = !empty($natt_theme[0]) ? $natt_theme[0] : NULL;
  if (!isset($node_theme->tid)) {
    $node_theme = _doesoe_theme_entity_get_value('node', $node, 'field_theme');
  }
  return $node_theme;
}

/**
 * Checks and returns TRUE if we're viewing the main theme landing page.
 *
 * @return bool
 *   TRUE if the we're viewing a theme landing page, FALSE otherwise.
 */
function _doesoe_theme_is_theme_landing_page() {
  $term = menu_get_object('taxonomy_term', 2);
  return (isset($term->vocabulary_machine_name) && $term->vocabulary_machine_name == 'themes');
}

/**
 * Returns a list of author nodes for a given theme.
 *
 * @param object $theme
 *   The theme term object.
 *
 * @return array
 *   A list of author nodes.
 */
function _doesoe_theme_get_authors($theme, $year, $natt_theme = NULL) {
  $authors = array();

  // Base query.
  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'author')
    ->propertyCondition('status', NODE_PUBLISHED);

  // Refine list to theme, sort by weight.
  if (!empty($theme->tid)) {
    $query->fieldCondition('field_themes', 'tid', array($theme->tid), 'IN');
    $query->fieldOrderBy('field_weight', 'value');
  }
  else {
    // Get all authors if no theme provided (overview) sort by cite name.
    $query->fieldOrderBy('field_chief_author', 'value', 'DESC');
    $query->fieldOrderBy('field_short_title', 'value');
  }

  // Get all years if no year provided.
  if (!empty($year->tid)) {
    $query->fieldCondition('field_years', 'tid', array($year->tid), 'IN');
  }

  // Filter to natt theme if provided.
  if (!empty($natt_theme->tid)) {
    $query->fieldCondition('field_natt_reference', 'target_id', array($natt_theme->tid), 'IN');
  }

  $result = $query->execute();

  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    $authors = entity_load('node', $nids);
  }

  return $authors;
}

/**
 * Get all citations for all themes.
 *
 * @return array
 *   A list of citations keyed by theme tid.
 */
function _doesoe_theme_get_all_theme_citations() {
  $vocab = taxonomy_vocabulary_machine_name_load('themes');
  $themes = taxonomy_get_tree($vocab->vid, 0, 1, TRUE);
  $citations = array();
  foreach ($themes as $theme) {
    $citations[$theme->tid] = trim(theme('doesoe_theme_citation', array(
      'theme' => $theme,
    )));
  }
  return $citations;
}

/**
 * Get framework at a glance landing node.
 */
function _doesoe_theme_get_framework_landing_node($theme_tid, $framework_tid) {
  $node = &drupal_static(__FUNCTION__ . $theme_tid . '-' . $framework_tid, NULL);
  if (!empty($node)) {
    return $node;
  }
  $year_tid = _doesoe_theme_get_current_year_tid();
  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'framework_at_a_glance')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_theme', 'tid', array($theme_tid), 'IN')
    ->fieldCondition('field_framework', 'tid', array($framework_tid), 'IN')
    ->fieldCondition('field_years', 'tid', array($year_tid), 'IN')
    ->range(0, 1)
    ->execute();
  if (isset($result['node'])) {
    $nodes = node_load_multiple(array_keys($result['node']));
    $node = reset($nodes);
  }
  return $node;
}

/**
 * Get a nat theme by name.
 */
function _doesoe_theme_get_natt_theme_by_name($name) {
  $terms = taxonomy_get_term_by_name($name, 'themes');
  return reset($terms);
}
