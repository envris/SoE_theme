<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728096
 */

$theme_dir = drupal_get_path('theme', 'doesoe_admin');

/**
 * Include helpers from the helpers folder.
 *
 * Naming convention for helper files should be something like this:
 * {theme_hook}.helper.inc.
 */
foreach (file_scan_directory($theme_dir . '/helpers', '/.+\.inc/i') as $file) {
  require_once $file->uri;
}

/**
 * Include alters and after builds from the alter folder.
 *
 * Naming convention for alter files should be something like this:
 * {theme_hook}.alter.inc.
 */
foreach (file_scan_directory($theme_dir . '/alter', '/.+\.inc/i') as $file) {
  require_once $file->uri;
}

/**
 * Include preprocessors and processors from the preprocess folder.
 *
 * Naming convention for alter files should be something like this:
 * {theme_hook}.preprocess.inc.
 */
foreach (file_scan_directory($theme_dir . '/preprocess', '/.+\.inc/i') as $file) {
  require_once $file->uri;
}
