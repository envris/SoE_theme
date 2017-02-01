<?php
/**
 * @file
 * Node preprocessing.
 */

/**
 * Implements template_preprocess_entity().
 */
function doesoe_theme_preprocess_node(&$variables, $hook) {
  $entity = $variables['node'];
  $entity_w = entity_metadata_wrapper('node', $entity);

  // Add a 'current year' class if applicable.
  if (_doesoe_theme_is_current_year($entity)) {
    $variables['classes_array'][] = 'current-year';
  }

  // Assessment summary component.
  // Adds the grade scale and labels to the wrapper and used later is JS.
  if ($variables['type'] == 'assessment_summary_component') {

    // Technically this should always be replaced by the value on a component
    // but in the rare case of dirty data, this applies default scale/labels.
    $scale = 4;
    $labels = 'Very poor,Poor,Good,Very good';

    // Get the scale.
    if ($entity_w->field_assessment_summary_scale) {
      $scale = $entity_w->field_assessment_summary_scale->value();
    }

    // Get the labels, trimming any whitespace in the process.
    if ($entity_w->field_assessment_summary_labels) {
      $label_array = explode(',', $entity_w->field_assessment_summary_labels->value());
      foreach ($label_array as &$label) {
        $label = trim($label);
      }
      $labels = implode(',', $label_array);
    }

    // Add the scale and labels as data attribute.
    $variables['attributes_array']['data-scale'] = $scale;
    $variables['attributes_array']['data-labels'] = filter_xss($labels);
    $variables['classes_array'][] = 'ass-sum__component-data';
  }

  // Add a grade scale class to change the key colours if 5 point scale.
  if ($variables['type'] == 'assessment_summary_key') {
    $variables['classes_array'][] = 'ass-sum-key';
    if (isset($variables['field_assessment_summary_key_gra'])) {
      $variables['classes_array'][] = 'ass-sum-key__grade__scale-' . count($variables['field_assessment_summary_key_gra']);
    }
  }

  // Key findings.
  if ($variables['type'] == 'key_finding') {

    if ($variables['view_mode'] === 'headline') {
      $variables['icon'] = array(
        '#theme' => 'icon',
        '#icon' => 'ct-page',
      );
    }
    else {
      $variables['icon'] = array(
        '#theme' => 'icon',
        '#icon' => 'ct-key-finding',
      );
    }

    // If a long title exists then use this instead of the normal title.
    if (isset($entity_w->field_long_title) && $entity_w->field_long_title->value() != '') {
      $variables['title'] = $entity_w->field_long_title->value();
    }

    if ($variables['view_mode'] == 'full_teaser') {
      // Add id so we can target it later in an anchor tag.
      $variables['attributes_array']['id'] = 'key-finding-' . $variables['nid'];
      // Recreate title markup in the case that we used the long_title.
      $variables['content']['title'][0] = array(
        '#theme' => 'html_tag',
        '#tag' => 'h4',
        '#value' => $variables['title'],
      );
    }

    // Get the theme term.
    if (isset($entity_w->field_theme)) {
      $theme = $entity_w->field_theme->value();
    }
    // We change the title link to point to the key finding listing page.
    // We also pass the year value to the key findings listing page.
    // link eg: /theme/theme-name/key-findings?year=year-value#key-finding-1234.
    $is_teaser = $variables['view_mode'] == 'teaser';
    if ($is_teaser && isset($entity_w->field_years) && isset($entity_w->field_theme) && isset($theme)) {
      $year = $entity_w->field_years->value();
      $year = is_array($year) ? reset($year) : $year;

      // We build the link.
      $link_path = strtolower(str_replace(' ', '-', $theme->name)) . '/key-findings';
      $link_options = array(
        'query' => array('year' => $year->tid),
        'fragment' => 'key-finding-' . $variables['nid'],
        'external' => TRUE,
      );
      $link = l($variables['title'], $link_path, $link_options);
      $variables['content']['title'][0]['#markup'] = $link;
      $readmore_title = $variables['content']['node_link']['#title'];
      $readmore_link = l($readmore_title, $link_path, $link_options);
      $variables['content']['node_link'][0]['#markup'] = $readmore_link;
    }

    if ($variables['view_mode'] === 'headlines') {
      $variables['classes_array'][] = str_replace('_', '-', $variables['type']) . '__' . $variables['view_mode'];
    }

  }

  // Homepage feature.
  if ($variables['type'] == 'homepage_feature') {
    $variables['classes_array'][] = 'homepage-feature';
    $variables['classes_array'][] = 'homepage-feature-' . drupal_html_class($variables['view_mode']);
    // If the feature has a link (it should as required field).
    if (isset($entity_w->field_url) && $entity_w->field_url->value()) {
      $link = $entity_w->field_url->value();
      // Make the image clickable with the correct link.
      $variables['content']['field_feature_image'][0]['#path']['path'] = $link['url'];
      // On a teaser we re-write the title.
      if ($variables['view_mode'] == 'teaser') {
        $variables['content']['title'][0]['#markup'] = l(
          t('<span class="title">@title</span><span class="icon"></span>', array('@title' => $variables['title'])),
          $link['url'],
          array('html' => TRUE)
        );
      }
    }
  }

  // Standard Report and Report topic content.
  if (in_array($variables['type'], array('report_topic_content', 'standard_report_content'))) {
    // Update the links to include a link to assessment summaries.
    if ($variables['view_mode'] == 'teaser' && isset($variables['content']['links'])) {
      $ass_sum_link = FALSE;
      $ass_sum_link_query = array();

      // Get any referenced Ass Sums.
      $referenced_ass_sums = _doesoe_theme_entity_ref_reverse_lookup('node', 'assessment_summary', 'field_topic', $entity->nid);

      // If we have a single ass sum, link to that.
      if (count($referenced_ass_sums) == 1) {
        $ass_sum_row = reset($referenced_ass_sums);
        $ass_sum_link = 'node/' . $ass_sum_row->nid;
      }
      elseif (count($referenced_ass_sums) > 1) {
        // Have more than one summary, link to all ass sums for the theme.
        $theme = _doesoe_theme_entity_get_value('node', $entity, 'field_theme');
        if ($theme) {
          $ass_sum_link = url('taxonomy/term/' . $theme->tid) . '/assessment-summaries';
        }
        // If framework and year, populate the query to get more specific.
        $framework = _doesoe_theme_entity_get_value('node', $entity, 'field_framework');
        if ($framework) {
          $ass_sum_link_query['framework'] = $framework->tid;
        }
        $year = _doesoe_theme_entity_get_value('node', $entity, 'field_year');
        if ($year) {
          $ass_sum_link_query['year'] = $year->tid;
        }
      }

      // If link exists we have summaries for this theme.
      if ($ass_sum_link !== FALSE) {
        // DS weight seems to get ignored?
        $variables['content']['links']['#weight'] = 50;
        // Update the links to include AS link.
        $variables['content']['links']['node']['#links']['ass_sum'] = array(
          'title' => t('See Assessment Summaries'),
          'href' => $ass_sum_link,
          'html' => FALSE,
          'query' => $ass_sum_link_query,
        );
      }

      // Just in case comments got in there somehow.
      unset($variables['content']['links']['comment']);
    }
  }

  // Map.
  if ($variables['type'] == 'map') {
    if (isset($entity_w->field_map_url) && $entity_w->field_map_url->value()) {
      $link = $entity_w->field_map_url->value();
      $variables['content']['field_image'][0]['#path']['path'] = $link['url'];
      $variables['content']['field_image'][0]['#path']['options'] = $link;
      $variables['content']['field_image'][0]['#item']['title'] = $link['attributes']['title'];
    }

    // Requires "filesaver" library used in govcms_ckan_display to download
    // image rather than opening in a new tab.
    drupal_add_library('govcms_ckan_display', 'filesaver');
    drupal_add_js(drupal_get_path('theme', 'doesoe_theme') . '/js/download-as-png.js');

  }

  // Graph.
  if ($variables['type'] == 'ckan_graph') {
    if (!empty($variables['content']['field_ckan_dataset']) && !empty($variables['content']['field_fallback_image'])) {
      // We have a ckan graph and a fallback image, let's hide the fallback.
      $variables['content']['field_fallback_image']['#access'] = FALSE;
    }
  }
}
