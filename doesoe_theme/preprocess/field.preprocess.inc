<?php
/**
 * @file
 * Field preprocessing.
 */

/**
 * Implements template_preprocess_field().
 */
function doesoe_theme_preprocess_field(&$variables, $hook) {
  $element = $variables['element'];
  $theme_path = drupal_get_path('theme', 'doesoe_theme');

  // Add a simplified template for the image description field and make it
  // visible only for screen readers.
  if ($element['#field_name'] == 'field_description' && $element['#bundle'] == 'image') {
    $variables['classes_array'][] = 'element-invisible';
    array_unshift($variables['theme_hook_suggestions'], "field__field_description__image");
  }

  // Assessment Summaries confidence and comparability icons rewrites.
  $ass_sum_icon_fields = array(
    'field_confidence_trend',
    'field_confidence_grade',
    'field_assessment_comparability',
  );
  // If bundle/field is applicable.
  if ($element['#bundle'] == 'assessment_summary' && in_array($element['#field_name'], $ass_sum_icon_fields)) {
    $type = $element['#field_name'] == 'field_assessment_comparability' ? 'compare' : 'confidence';
    // For each delta, re-write the markup to be an icon.
    foreach ($element['#items'] as $k => $value) {
      // Get the title/value.
      $title = strip_tags($element[$k]['#markup']);
      // Add the value as a class to the wrapper.
      $variables['ds-config']['ow-cl'] .= ' ' . 'ass-sum__item__' . $type . '__' . drupal_html_class($title);
      // Re-write markup as SVG in a wrapper.
      $variables['items'][$k]['#markup'] = theme('html_tag', array(
        'element' => array(
          '#tag' => 'div',
          '#value' => theme('icon', array(
            'icon' => "{$type}-{$value['value']}",
            'title' => $title,
          )),
          '#attributes' => array(
            'class' => array('ass-sum-vis__' . $type),
            'title' => $title,
          ),
        ),
      ));
    }
  }

  // If parent entity is a node.
  if ($element['#entity_type'] == 'node') {

    // Re-write a list of assessment summaries.
    // If we are dealing with a field that outputs a list of assessment
    // summaries, we replace that field with a view, passing a contextual filter
    // of the NID. This emulates the EVA module and standardises the output for
    // all summaries down to one single view.
    $ass_sum_list_fields = array('field_assessment_summary_compone', 'field_assessment_summary');
    if (in_array($element['#field_name'], $ass_sum_list_fields)) {
      $display = $element['#bundle'] . '_eva';
      $variables['items'] = array(
        array(
          '#markup' => views_embed_view('assessment_summary_components', $display, $element['#object']->nid),
        ),
      );
    }
  }

  if ($element['#bundle'] == 'reference') {
    if (!isset($element['#object']->nid)) {
      return;
    }

    $nid = $element['#object']->nid;

    // We wrap short reference field in a link.
    if ($element['#field_name'] == 'field_short_reference') {
      foreach ($variables['items'] as $key => $item) {
        $variables['items'][$key]['#markup'] = l($item['#markup'], '', array(
          'fragment' => 'reference-' . $nid,
          'external' => TRUE,
          'html' => TRUE,
        ));
      }
    }

    // We wrap full reference field in a div.
    if ($element['#field_name'] == 'field_full_reference') {
      foreach ($variables['items'] as $key => $item) {
        $variables['items'][$key]['#markup'] = theme('html_tag', array(
          'element' => array(
            '#tag' => 'div',
            '#value' => $item['#markup'],
            '#attributes' => array(
              'data-reference' => 'true',
              'id' => 'reference-' . $nid,
              'class' => array(
                'reference',
              ),
            ),
          ),
        ));
      }
    }
  }

  // Transform an icon code into icon element.
  if ($element['#field_name'] == 'field_icon_code') {
    foreach ($variables['items'] as $key => $item) {
      $variables['items'][$key]['#markup'] = theme('icon', array(
        'icon' => $item['#markup'],
      ));
    }
  }

  // Turn field_image_map into <area> tags with attributes applied.
  // The <map> tag is added in field--field-image-map.tpl.php and the usemap
  // attribute is added in hook_preprocess_image_formatter().
  if ($element['#field_name'] == 'field_image_map') {
    // NOTE: this may break if more than one of these on a page (unlikely).
    $map_key = 'image-map-' . $element['#object']->fid;
    // Name attribute is depricated but still required.
    $variables['attributes_array']['id'] = $map_key;
    $variables['attributes_array']['name'] = $map_key;
    $variables['classes_array'] = array('image-map-area');
    // Rewrite each field as a area tag.
    foreach ($variables['items'] as $key => $item) {
      // Only output area if markup.
      if (empty($item['#markup'])) {
        $markup = '';
      }
      else {
        $markup = theme('html_tag', array(
          'element' => array(
            '#tag' => 'area',
            '#attributes' => _doesoe_theme_image_parse_map($item['#markup']),
          ),
        ));
      }
      $variables['items'][$key]['#markup'] = $markup;
    }
  }

  // For all image fields, if the alt value is empty, set it to the filename.
  if ($element['#field_type'] == "image") {
    foreach ($variables['items'] as &$item) {
      if (!empty($item['#item']) && empty($item['#item']['alt'])) {
        // Limit to 100 chars to comply with Accessibility guidelines.
        $item['#item']['alt'] = truncate_utf8($item['#item']['filename'], 100);
      }
      // Suffix the image description when using picture formatter as it does
      // not include the file fields when rendering.
      if ($item['#theme'] == 'picture_formatter' && !empty($item['#item']) && !empty($item['#item']['field_description'])) {
        $desc = $item['#item']['field_description'][LANGUAGE_NONE][0]['value'];
        $item['#suffix'] = '<div class="image__description element-invisible"><p>' . filter_xss($desc) . '</p></div>';
      }
    }
  }

  // Add the title attribute for images which have a link.
  if (!empty($variables['items'][0]['#path']['options'])) {
    $is_image_formatter = !empty($element['#formatter']) && $element['#formatter'] == "image";
    $has_path = !empty($variables['items'][0]['#path']['path']);
    $options =& $variables['items'][0]['#path']['options'];
    $no_title = $has_path && empty($options['attributes']['title']);
    if (!empty($options['entity']) && $is_image_formatter && $no_title) {
      // Get the title from either node title or term name.
      if ($options['entity_type'] == "node") {
        $title = $options['entity']->title;
      }
      elseif ($options['entity_type'] == "taxonomy_term") {
        $title = $options['entity']->name;
      }
      if (isset($title)) {
        $options['attributes']['title'] = check_plain($title);
      }
    }
  }
}
