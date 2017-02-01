<?php
/**
 * @file
 * Year helpers.
 */

/**
 * Returns enitity types and bundles that require year to be multi select.
 *
 * @return array
 *   Array keyed by entity types, values are entity bundles.
 */
function _doesoe_admin_form_multi_select_year_entities() {
  return array(
    'node' => array(
      'assessment_summary',
      'assessment_summary_component',
      'assessment_summary_key',
      'author',
    ),
    'taxonomy_term' => array(
      'themes',
    ),
  );
}
