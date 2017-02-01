<?php
/**
 * @file
 * Entity preprocessing.
 */

/**
 * Implements template_preprocess_entity().
 */
function doesoe_theme_preprocess_entity(&$variables, $hook) {

  // Paragraph item.
  if ($variables['entity_type'] == 'paragraphs_item') {
    $entity = $variables['elements']['#entity'];
    $entity_w = entity_metadata_wrapper($variables['entity_type'], $entity);

    // If the field_anchor exists, add it as the ID for this paragraph.
    if (isset($entity_w->field_anchor)) {
      $anchor = $entity_w->field_anchor->value();
      $variables['attributes_array']['id'] = filter_xss($anchor);
    }

    // Reference Attachment paragraphs.
    if ($variables['elements']['#bundle'] === 'reference_attachment') {
      $variables['attributes_array']['data-reference'] = 'true';
    }

    // Assessment summary classes (not using DS as that overrides year class).
    if ($variables['elements']['#bundle'] === 'assessment_summary') {
      $variables['classes_array'][] = 'paragraphs-item-assessment-summary';
      $variables['classes_array'][] = 'ass-sum__item';
    }

    // Add current year class if applicable.
    if (_doesoe_theme_is_current_year($entity, 'paragraphs_item')) {
      $variables['classes_array'][] = 'current-year';
    }

    // Iframe paragraphs.
    if ($variables['elements']['#bundle'] === 'iframe') {
      $source_url = $entity_w->field_source_url->value();
      $variables['iframe'] = $source_url;
      $variables['iframe_style'] = $entity_w->field_style->value();
      $variables['iframe_link'] = l($source_url, $source_url, array(
        'attributes' => array(
          'target' => '_blank',
        ),
      ));
    }

    // Add styles class to container paragraphs.
    if ($variables['elements']['#bundle'] === 'box_content') {
      if (isset($entity_w->field_additional_styles)) {
        foreach ($entity_w->field_additional_styles as $style) {
          $style_term = $style->value();
          $variables['classes_array'][] = drupal_html_class($style_term->name);
        }
      }
    }

    // Assessment summary reference placeholder class.
    if ($variables['elements']['#bundle'] === 'assessment_summary_reference') {
      if (empty($variables['content'])) {
        $variables['classes_array'][] = 'ass-sum-ref__placeholder';
      }
    }

    // Assessment summary supporting statement (popup).
    if ($variables['elements']['#bundle'] === 'assessment_summary_metadata') {
      // Load the host summary and then up to the host asc node for render.
      $host_entity = $entity->hostEntity();
      $asc_node = $host_entity->hostEntity();
      $asc_view = node_view($asc_node, 'compact');
      // Prepend the compact view of the asc to the body of the ascss.
      $variables['content']['field_body']['#prefix'] = render($asc_view);
    }

    // Known as Paragraph bundle.
    if ($variables['elements']['#bundle'] === 'known_as') {
      // If 'Display as link' is not set, hide link to AS/ASC.
      if (empty($entity_w->field_display_as_link->value())) {
        hide($variables['content']['field_known_as']);
      }
    }
  }

  // Bean.
  if ($variables['entity_type'] === 'bean') {
    $bean = $variables['elements']['#entity'];

    // Theme's topics link.
    if (isset($bean) && $bean->delta == DOESOE_THEME_THEME_TOPICS_DELTA) {
      $variables['text_link'] = _doesoe_theme_get_contextual_link('topics', 'See all @theme topics', 'button--arrow');
    }

    // Theme's assessment summaries link.
    if (isset($bean) && $bean->delta == DOESOE_THEME_THEME_ASS_SUMMARIES_DELTA) {
      $variables['text_link'] = _doesoe_theme_get_contextual_link('assessment-summaries', 'See all @theme assessment summaries', 'button--arrow');
    }

    // Theme's references link.
    if (isset($bean) && $bean->delta == DOESOE_THEME_THEME_REFERENCES_DELTA) {
      $variables['text_link'] = _doesoe_theme_get_contextual_link('references', 'See all @theme references', 'button--arrow');
    }

    // Theme's acknowledgements link.
    if (isset($bean) && $bean->delta == DOESOE_THEME_THEME_ACKNOWLEDGEMENTS_DELTA) {
      $theme = _doesoe_theme_get_current_theme_term();
      $variables['text_link'] = l(
        t('View acknowledgements for @theme', array('@theme' => $theme->name)),
        'acknowledgements',
        array(
          'fragment' => drupal_html_class($theme->name),
          'attributes' => array('class' => array('button--arrow')),
        )
      );
    }

  }

}
