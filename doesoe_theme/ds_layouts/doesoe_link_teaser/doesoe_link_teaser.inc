<?php
/**
 * @file
 * Link teaser wraps everything in a link to the content.
 */


function ds_doesoe_link_teaser() {
  return array(
    'label' => t('DOESOE: Link Teaser'),
    'category' => t('DOESOE Custom'),
    'theme' => 'doesoe_link_teaser',
    'regions' => array(
      'side' => t('Side (icon)'),
      'main_content' => t('Content'),
    ),
    'css' => FALSE,
    'image' => TRUE,
  );
}
