<?php
/**
 * @file
 * Entity helpers.
 */

/**
 * Do a reverse lookup based on a target id, returning matching references.
 *
 * @param string $lookup_type
 *   The entity type we expect in the results.
 * @param string $lookup_bundle
 *   The bundle we expect in the results.
 * @param string $lookup_field
 *   The field that references the the target id.
 * @param int $target_id
 *   The id of the entity that is being referenced via lookup_field.
 *
 * @return array
 *   An array of referenced entities.
 */
function _doesoe_theme_entity_ref_reverse_lookup($lookup_type, $lookup_bundle, $lookup_field, $target_id) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $lookup_type)
    ->entityCondition('bundle', $lookup_bundle)
    ->fieldCondition($lookup_field, 'target_id', array($target_id), 'IN');
  $result = $query->execute();

  if (!empty($result[$lookup_type])) {
    return $result[$lookup_type];
  }
  else {
    return array();
  }
}

/**
 * A helper to retrieve a value from an entity.
 *
 * @param string $entity_type
 *   The entity type.
 * @param object $entity
 *   The entity object.
 * @param string $field
 *   The field name.
 *
 * @return mixed
 *   The value from the field or NULL if no value.
 */
function _doesoe_theme_entity_get_value($entity_type, $entity, $field) {
  $entity_w = entity_metadata_wrapper($entity_type, $entity);
  if (isset($entity_w->{$field}) && !empty($entity_w->{$field}->value())) {
    $term = $entity_w->{$field}->value();
    return $term;
  }
  return NULL;
}
