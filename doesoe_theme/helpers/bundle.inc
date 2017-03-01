<?php
/**
 * @file
 * Entity bundle helpers.
 */

/**
 * Provides a map of front-end friendly name replacements for bundles.
 *
 * The main reason for needing this is due to having two content types that do
 * pretty much exactly the same thing. AKA 'standard report content' and
 * 'report topic content'. But could also prove useful in the future for when
 * human content type names (or vocabs) need to be different for admins vs
 * frontend or when many bundle types all need the same human name.
 *
 * @return array
 *   An array of bundle name replacements, keyed by original name and value
 *   being the new 'front-end-friendly' name.
 */
function _doesoe_theme_bundle_name_replacements_map() {
  return array(
    'Standard Report Content' => t('Report Content'),
    'Report Topic Content' => t('Report Content'),
    'Framework landing page content' => t('At a glance'),
  );
}

/**
 * Rewrite bundle names in a provided string. Commonly used in views preprocess.
 *
 * @param string $string_containing_bundle_name
 *   A find and replace will be performed on this string to rewrite the bundle
 *   name. Passed by reference.
 *
 * @see _doesoe_theme_bundle_name_replacements_map()
 */
function _doesoe_theme_bundle_name_replace(&$string_containing_bundle_name) {
  foreach (_doesoe_theme_bundle_name_replacements_map() as $original => $new) {
    if (strpos($string_containing_bundle_name, $original) !== FALSE) {
      $string_containing_bundle_name = str_replace($original, $new, $string_containing_bundle_name);
    }
  }
}
