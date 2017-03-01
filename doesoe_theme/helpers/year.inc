<?php
/**
 * @file
 * Year helpers (taxonomy).
 */

/**
 * Pass this an entity and will return TRUE if tagged with current year.
 *
 * @param object $entity
 *   The entity that is tagged with a year.
 * @param string $entity_type
 *   The type of entity we are checking.
 *
 * @return bool
 *   TRUE if tagged with current year, FALSE if not.
 */
function _doesoe_theme_is_current_year($entity, $entity_type = 'node') {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $entity_w = entity_metadata_wrapper($entity_type, $entity);
  $current = FALSE;

  // If entity has field_years.
  if (field_info_instance($entity_type, 'field_years', $bundle)) {
    if (isset($entity_w->field_years)) {
      // For each year check if one of them is current.
      foreach ($entity_w->field_years as $year) {
        if (isset($year->field_is_current_year) && $current !== TRUE) {
          $current = (bool) $year->field_is_current_year->value();
        }
      }
    }
  }

  return $current;
}

/**
 * Get an array of years that apply to the current node/page.
 *
 * @note
 *   Uses only field_years as field_year is depricated.
 *
 * @return array
 *   An array of year 'names' for the current page.
 */
function _doesoe_theme_get_years_from_page() {
  $years = &drupal_static(__FUNCTION__, array());
  if (empty($years)) {
    $years = array('names' => array(), 'terms' => array(), 'tids' => array());
    $node = menu_get_object();
    if (isset($node->type) && field_info_instance('node', 'field_years', $node->type)) {
      $node_w = entity_metadata_wrapper('node', $node);
      if (isset($node_w->field_years)) {
        foreach ($node_w->field_years as $year) {
          $term = $year->value();
          $years['terms'][] = $term;
          $years['names'][] = $term->name;
          $years['tids'][] = $term->tid;
        }
      }
    }
  }
  return $years;
}

/**
 * Get the TID for the current year.
 */
function _doesoe_theme_get_current_year_tid() {
  $tid = &drupal_static(__FUNCTION__, NULL);
  if (is_null($tid)) {
    $tid = FALSE;
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'taxonomy_term')
      ->entityCondition('bundle', 'years')
      ->fieldCondition('field_is_current_year', 'value', 1, '=')
      ->execute();
    if (isset($result['taxonomy_term'])) {
      $tids = array_keys($result['taxonomy_term']);
      $tid = reset($tids);
    }
  }
  return $tid;
}

/**
 * Is the current viewed node historic (ie not tagged against current year).
 *
 * @return bool
 *   Returns TRUE if this content is historic.
 */
function _doesoe_theme_is_current_node_historic() {
  $years = _doesoe_theme_get_years_from_page();
  $current_year = _doesoe_theme_get_current_year_tid();
  return (!empty($years['tids']) && !in_array($current_year, $years['tids']));
}

/**
 * Check if a tid is a valid year tid.
 *
 * @param int $tid
 *   Tid to check.
 *
 * @return bool
 *   Is valid.
 */
function _doesoe_theme_is_valid_year_tid($tid) {
  $query = new EntityFieldQuery();
  $res = $query->entityCondition('entity_type', 'taxonomy_term')
    ->entityCondition('bundle', 'years')
    ->propertyCondition('tid', $tid)
    ->execute();
  return isset($res['taxonomy_term']);
}

/**
 * Get active year from a page.
 */
function _doesoe_theme_get_active_year($load = FALSE) {
  $years = _doesoe_theme_get_years_from_page();
  $year_tid = !empty($years['tids']) ? reset($years['tids']) : _doesoe_theme_get_current_year_tid();
  $year_tid = (!empty($_GET['year']) && _doesoe_theme_is_valid_year_tid($_GET['year'])) ? $_GET['year'] : $year_tid;
  return $load === TRUE ? taxonomy_term_load($year_tid) : $year_tid;
}
