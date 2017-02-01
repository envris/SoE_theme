<?php
/**
 * @file
 * This replicates the functionality provided by the 'simplify' module.
 */

/**
 * Get an array of fields (by type) that can be hidden.
 */
function doesoe_admin_get_fields($type) {
  $fields = array();

  switch ($type) {
    // Nodes.
    case 'nodes':
      // Drupal core.
      $fields['author'] = t('Authoring information');
      $fields['options'] = t('Publishing options');
      $fields['revision'] = t('Revision information');
      if (module_exists('book')) {
        $fields['book'] = t('Book outline');
      }
      if (module_exists('comment')) {
        $fields['comment'] = t('Comment settings');
      }
      if (module_exists('menu')) {
        $fields['menu'] = t('Menu settings');
      }
      if (module_exists('path')) {
        $fields['path'] = t('URL path settings');
      }
      // Third-party modules.
      if (module_exists('metatag')) {
        $fields['metatag'] = t('Meta tags');
      }
      if (module_exists('xmlsitemap_node')) {
        $fields['xmlsitemap'] = t('XML sitemap');
      }
      if (module_exists('defaultcontent')) {
        $fields['machine_name_fs'] = t('Machine Name');
      }
      if (module_exists('redirect')) {
        $fields['redirect'] = t('URL redirects');
      }
      break;

  }

  return $fields;
}

/**
 * Hide fields from all users.
 */
function doesoe_admin_hide_fields($fields, &$form) {
  if (!empty($fields)) {
    foreach ($fields as $field) {
      doesoe_admin_hide_field($field, $form);
    }
  }
}

/**
 * Hide a given field.
 */
function doesoe_admin_hide_field($field, &$form) {
  switch ($field) {

    // Authoring information.
    case 'author':
      $form['author']['#access'] = FALSE;
      break;

    // Book outline.
    case 'book':
      $form['book']['#access'] = FALSE;
      break;

    // Comment settings.
    case 'comment':
      $form['comment_settings']['#access'] = FALSE;
      break;

    // Contact settings.
    case 'contact':
      $form['contact']['#access'] = FALSE;
      break;

    // Menu settings.
    case 'menu':
      $form['menu']['#access'] = FALSE;
      break;

    // Meta tags.
    case 'metatag':
      $form['metatags']['#access'] = FALSE;
      break;

    // Publishing options.
    case 'options':
      $form['options']['#access'] = FALSE;
      break;

    // Administrative overlay.
    case 'overlay':
      $form['overlay_control']['#access'] = FALSE;
      break;

    // URL path settings.
    case 'path':
      $form['path']['#access'] = FALSE;
      break;

    // Revision information.
    case 'revision':
      $form['revision_information']['#access'] = FALSE;
      break;

    // Status.
    case 'status':
      $form['account']['status']['#access'] = FALSE;
      break;

    // XML sitemap.
    case 'xmlsitemap':
      $form['xmlsitemap']['#access'] = FALSE;
      break;

    // Machine Name.
    case 'machine_name_fs':
      $form['machine_name_fs']['#access'] = FALSE;
      break;

    // URL redirects.
    case 'redirect':
      $form['redirect']['#access'] = FALSE;
      break;

  }
}
