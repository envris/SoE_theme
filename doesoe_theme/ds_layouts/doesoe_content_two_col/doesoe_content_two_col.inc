<?php
/**
 * @file
 * Layout for an content two col.
 *
 * This provides a two column layout intended for responsive content.
 * Eg. Author page.
 */

function ds_doesoe_content_two_col() {
  return array(
    'label' => t('DOESOE: Content Two Col'),
    'category' => t('DOESOE Custom'),
    'theme' => 'doesoe_content_two_col',
    'regions' => array(
      'col_1' => t('Col 1'),
      'col_2' => t('Col 2'),
    ),
    'css' => FALSE,
    'image' => TRUE,
  );
}
