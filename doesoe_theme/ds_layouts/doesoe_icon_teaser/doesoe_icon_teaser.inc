<?php
/**
 * @file
 * Layout for an icon teaser.
 */

/**
 * DS Implementation for icon teaser.
 */
function ds_doesoe_icon_teaser() {
  return array(
    'label' => t('DOESOE: Icon Teaser'),
    'category' => t('DOESOE Custom'),
    'theme' => 'doesoe_icon_teaser',
    'regions' => array(
      'side' => t('Side (icon)'),
      'main_content' => t('Content'),
      'below_content' => t('Below content'),
    ),
    'css' => FALSE,
    'image' => TRUE,
  );
}
