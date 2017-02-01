<?php
/**
 * @file
 * Views preprocessor.
 */

/**
 * Implements hook_preprocess_views_view_grouping().
 */
function doesoe_theme_preprocess_views_view_grouping(&$vars) {
  $view = $vars['view'];
  $rows = $vars['rows'];

  // Our assessment summary component formatting magic.
  if ($view->name == 'assessment_summary_components') {

    // We manipulate the labels group to create a header and add attributes
    // required by the JS.
    if ($vars['grouping']['field'] == 'field_theme') {

      // Arrrggh...
      // So this gets a bit messy and will break if additional groups are added
      // but to do this complex manipulation it is a necessary evil as the
      // 'grouping field' is not actually correct.
      $first_group = array_shift($rows);
      $second_group = array_shift($first_group['rows']);

      // If the second group is an object we are on the correct level.
      if (is_object($second_group)) {

        // Grade scale and labels.
        $scale = 4;
        $labels = 'Very poor,Poor,Good,Very Good';
        if (isset($second_group->field_field_assessment_summary_scale[0])) {
          $scale = $second_group->field_field_assessment_summary_scale[0]['raw']['value'];
          $labels = $second_group->field_field_assessment_summary_labels[0]['raw']['value'];
        }

        // Re-write the title (group) to be a header.
        $vars['title'] = theme('assessment_summary_header', array(
          'grades' => $labels,
          'grade_scale' => $scale,
        ));

        // The JS looks at this class to find information about the grade.
        $vars['classes_array'][] = 'ass-sum__component-data';
        $vars['classes_array'][] = 'ass-sum__component__scale-' . $scale;
        // The grade scale and labels.
        $vars['attributes_array'] = array(
          'data-scale' => $scale,
          'data-labels' => $labels,
        );

      }
      else {
        // This is a non-header group.
        $vars['classes_array'][] = 'ass-sum__component__group';
      }
    }
  }

}

/**
 * Implements hook_preprocess_views_exposed_form().
 */
function doesoe_theme_preprocess_views_exposed_form(&$vars) {

  // We should always have an ID, but just in case, some guard code.
  if (!isset($vars['form']['#id'])) {
    return;
  }

  if($vars['form']['#id'] === 'views-exposed-form-site-search-region') {
    if (!empty($vars['form']['extent'])) {
      $widget = new stdClass;
      $widget->id = $vars['form']['extent']['#id'];
      $widget->label = 'Map extent';
      $widget->widget = render($vars['form']['extent']);
      $widget->operator = NULL;
      $widget->description = NULL;
      $offset = array_search('filter-field_theme', array_keys($vars['widgets'])) + 1;
      $vars['widgets'] = array_slice($vars['widgets'], 0, $offset, true) +
      array("extent" => $widget) +
      array_slice($vars['widgets'], $offset, count($vars['widgets']) - 1, true);
      unset($vars['form']['extent']);
      $vars['button'] = render($vars['form']['submit']);
    }
  }

  $search_pages = array(
    'views-exposed-form-site-search-region',
    'views-exposed-form-site-search-page',
    'views-exposed-form-assessment-summary-components-page'
  );

  if (in_array($vars['form']['#id'], $search_pages)) {

    $items = array();

    $tabs = array(
      array(
        'title' => 'Advanced Site Search',
        'icon' => 'search2',
        'url' => 'search',
      ),
      array(
        'title' => 'Search by Region',
        'icon' => 'map-marker',
        'url' => 'region-search',
      ),
      array(
        'title' => 'Search Assessment Summaries',
        'icon' => 'ct-assessment-summary',
        'url' => 'assessment-summary-components',
      ),
    );

    foreach ($tabs as $tab) {
      $items[] = array(
        'data' => l($tab['title'], $tab['url']),
        'class' => array($tab['icon'] . ' search-tabs__tab'),
      );
    }

    $tabs = theme('item_list', array(
      'items' => $items,
      'attributes' => array(
        'class' => array('search-tabs__list'),
      ),
    ));

    $vars['form_prefix'] = theme('html_tag', array(
      'element' => array(
        '#tag' => 'div',
        '#attributes' => array(
          'class' => array('search-tabs'),
        ),
        '#value' => $tabs,
      ),
    ));

  }
  if ($vars['form']['#id'] === 'views-exposed-form-site-search-region') {
    $map_container = theme('html_tag', array(
      'element' => array(
        '#tag' => 'div',
        '#attributes' => array(
          'id' => 'search-map',
        ),
        '#value' => '',
      ),
    ));
    $suffix = isset($vars['form_suffix']) ? $vars['form_suffix'] : '';
    $vars['form_suffix'] = $map_container . $suffix;
  }

}

/**
 * Implements hook_preprocess_views_view().
 */
function doesoe_theme_preprocess_views_view(&$vars) {
  $view = $vars['view'];

  // Assessment summary component view (page).
  if (isset($vars['empty']) && $view->name == 'assessment_summary_components' && $view->current_display == 'page') {

    // Empty gets reused by input req and no result, so check we have no input.
    if (empty($view->exposed_input)) {

      // Get default assessment summary key and append to empty text content.
      // This is to avoid referring to a NID.
      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'assessment_summary_key')
        ->fieldOrderBy('field_default', 'value', 'DESC')
        ->range(0, 1)
        ->execute();

      // Append the rendered key node to the empty text.
      if (isset($result['node'])) {
        $nids = array_keys($result['node']);
        $nid = reset($nids);
        $node_view = node_view(node_load($nid), 'full');
        $vars['empty'] .= render($node_view);
      }

    }
  }

  // We change the footer link to point to theme/theme-name/by-framework.
  if ($view->name == 'frameworks_by_theme') {
    // We build the link.
    $vars['footer'] = _doesoe_theme_get_contextual_link('by-framework', 'Visit by framework for more information', 'button--arrow');
  }

  // Add atmosphere into text.
  if ($view->name == 'report_topic' && $view->current_display == 'panel_pane_1') {
    $vars['exposed'] .= _doesoe_theme_atmosphere_sub_theme_intro_text('Topics', 'theme/!term/topics');
  }
}

/**
 * Implements hook_preprocess_views_view_fields().
 */
function doesoe_theme_preprocess_views_view_fields(&$vars) {
  $view = $vars['view'];

  // Site search row tweaks.
  if ($view->name == 'site_search') {
    $entity = $vars['row']->entity;

    // Only dealing with nodes.
    if (isset($entity->type)) {

      // Graphs should take you to the parent topic not the graph itself.
      if ($entity->type == 'ckan_graph' && !empty($entity->field_topic)) {
        // Not using metadata wrapper here as it will load the topic and we
        // don't want that extra overhead when all we need is the nid.
        $topic_nid = $entity->field_topic[LANGUAGE_NONE][0]['target_id'];

        // Replacement patterns for links.
        $regx = '/<a(.*?)href="(.*?)"(.*?)>/';
        $subst = '<a$1href="' . url('node/' . $topic_nid) . '"$3>';

        // Replace the href in the title and view_node with the topic url.
        $link_fields = array('title', 'view_node');
        foreach ($link_fields as $field) {
          $vars['fields'][$field]->content = preg_replace($regx, $subst, $vars['fields'][$field]->content);
        }
      }
    }

    // Replace content type with more human friendly versions.
    // @see helpers/bundle.inc for more info.
    if (isset($vars['fields']['type_1']->content)) {
      _doesoe_theme_bundle_name_replace($vars['fields']['type_1']->content);
    }
  }

  // Add an a tag with a title so that the "At a glance" label is accessible.
  if ($view->name == "at_a_glance_by_theme_framework") {

    // Apply logic only if a label is provided.
    if (!empty($vars['fields']['field_body']->label)) {
      $raw_label = $vars['fields']['field_body']->label;
      $label_with_link = '<a href="#" title="View theme framework summary">' . $raw_label . '</a>';
      $new_label_html = str_replace($raw_label, $label_with_link, $vars['fields']['field_body']->label_html);
      $vars['fields']['field_body']->label_html = $new_label_html;
    }
  }
}

/**
 * Implements hook_views_pre_render().
 */
function doesoe_theme_views_pre_render(&$view) {
  // If on an assessment sum component view and we have results.
  if ($view->name == 'assessment_summary_components' && !empty($view->result)) {

    // What is happening here...?
    // A framework url is a combination of the theme/framework as frameworks are
    // always viewed in the context of its related theme. So... we have to get
    // a bit messy in this view to find both those entities, and use theme to
    // generate a custom url: /theme/THEME-NAME/framework/FRAMEWORK-NAME
    // We cant do this with tokens because some names have spaces.
    foreach ($view->result as $i => $row) {
      if (!empty($row->field_field_framework[0]['rendered'])) {
        // Get the theme and framework term entities.
        $theme = isset($row->field_field_theme[0]['raw']['taxonomy_term']) ? $row->field_field_theme[0]['raw']['taxonomy_term'] : FALSE;
        $framework = isset($row->field_field_framework[0]['raw']['taxonomy_term']) ? $row->field_field_framework[0]['raw']['taxonomy_term'] : FALSE;
        // If we have both a framework and a theme we can re-write the link.
        if (isset($theme->tid) && isset($framework->tid)) {
          $view->result[$i]->field_field_framework[0]['rendered']['#href'] = _doesoe_theme_get_theme_framework_link($theme, $framework);
        }
      }
    }
  }
}

/**
 * Implements hook_preprocess_views_view_list().
 */
function doesoe_theme_preprocess_views_view_list(&$vars) {
  // Create a group class based on the title (group) value.
  $vars['group_class'] = 'views-group';
  if (!empty($vars['title'])) {
    $vars['group_class'] .= '--' . drupal_html_class(strip_tags($vars['title']));
  }
}

/**
 * Implements hook_preprocess_HOOK().
 */
function views_preprocess_views_view_unformatted(&$variables) {

  $view = $variables['view'];
  $rows = $variables['rows'];

  if ($view->name === 'key_findings') {
    $index = $variables['view']->row_index;
    $variables['row_palette'] = '';
    if (isset($view->style_plugin->rendered_fields[$index]['field_palette_key'])) {
      $variables['row_palette'] = $view->style_plugin->rendered_fields[$index]['field_palette_key'];
    }
    $variables['cite_text'] = theme('doesoe_theme_citation', array(
      'theme' => taxonomy_term_load($view->style_plugin->rendered_fields[$index]['tid']),
    ));
  }

  if ($variables['view']->name === 'assessment_summary_components') {
    $fields = array('field_prev_known_as', 'field_now_known_as');
    foreach ($rows as $index => $row) {
      $known_as = '';
      foreach ($fields as $field_name) {
        if (!empty($view->style_plugin->rendered_fields[$index][$field_name])) {
          $known_as .= theme('html_tag', array(
            'element' => array(
              '#tag' => 'div',
              '#value' => $view->render_field($field_name, $index),
              '#attributes' => array(
                'class' => str_replace('_', '-', $field_name),
              ),
            ),
          ));
        }
      }
    }
    $variables['known_as'] = $known_as;
  }
}
