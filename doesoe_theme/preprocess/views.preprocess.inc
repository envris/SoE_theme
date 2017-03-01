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

  // Make the word "change" italics in widget labels.
  if ($vars['form']['#id'] === 'views-exposed-form-assessment-summary-components-page') {
    $widgets = array(
      'filter-field_grade_change_value',
      'filter-field_trend_change_value',
      'filter-field_confidence_in_grade_change_value',
      'filter-field_confidence_in_trend_change_value',
    );
    foreach ($widgets as $widget) {
      if (isset($vars['widgets'][$widget])) {
        $label = $vars['widgets'][$widget]->label;
        $new_label = str_replace('change', '<em>change</em>', $label);
        $vars['widgets'][$widget]->label = $new_label;
      }
    }
  }

  $search_pages = array(
    'views-exposed-form-site-search-region',
    'views-exposed-form-site-search-page',
    'views-exposed-form-assessment-summary-components-page',
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

  // Add a more link to the key findings view.
  if ($view->name == 'key_findings_by_theme_and_year') {
    $vars['footer'] = _doesoe_theme_get_contextual_link('key-findings', 'For more information see the key findings section', 'button--arrow');
  }

  // We change the footer link to point to theme/theme-name/by-framework.
  if ($view->name == 'frameworks_by_theme') {
    $term = _doesoe_theme_get_current_theme_term();
    if (!empty($term->tid)) {
      $url = url('taxonomy/term/' . $term->tid) . '/by-framework';
      // Check if are not on the actual page that we are trying to link to.
      $current_path = request_uri();
      if ($current_path !== $url) {
        // We build the link.
        $vars['footer'] = l(t('Visit by framework for more information'), $url, array('attributes' => array('class' => array('button--arrow'))));
      }
    }
  }

  // Add atmosphere into text.
  if ($view->name == 'report_topic' && $view->current_display == 'panel_pane_1') {
    $vars['exposed'] .= _doesoe_theme_atmosphere_sub_theme_intro_text('Topics', 'theme/!term/topics');
  }

  // Framework overview link.
  if ($view->name == 'framework_overview' && $view->current_display == 'panel_pane_1') {
    $framework = menu_get_object('taxonomy_term', 2);
    $theme = _doesoe_theme_get_natt_theme_by_name('overview');
    $url = _doesoe_theme_get_theme_framework_link($theme, $framework);
    $title = t('For more information read the overview section for @name', array('@name' => $framework->name));
    $vars['footer'] .= l($title, $url, array('attributes' => array('class' => array('button--arrow'))));
  }

  if (_doesoe_theme_view_is_search_page($view)) {
    // Add pager to top of page for search views.
    $vars['pager_top'] = TRUE;
    $vars['classes_array'][] = 'has-pager-top';
    // Bug in views, need to filter out empty filters.
    $exposed_input = array_filter($view->exposed_raw_input);
    $vars['pager'] = $view->query->render_pager($exposed_input);
  }

  // Append link for framework page to empty results text on topic view.
  if ($view->name == 'report_topic' && $view->current_display == 'panel_pane_1' && !empty($vars['empty'])) {
    if (is_numeric($view->exposed_raw_input['framework'])) {
      $fw = taxonomy_term_load($view->exposed_raw_input['framework']);
      $url = url('taxonomy/term/' . $view->args[0]) . 'framework/' . drupal_html_class($fw->name);
      $vars['empty'] .= '<p>' . l(t('View the framework introduction for @name', array('@name' => $fw->name)), $url) . '</p>';
    }
  }
}

/**
 * Implements hook_preprocess_views_view_fields().
 */
function doesoe_theme_preprocess_views_view_fields(&$vars) {
  $view = $vars['view'];

  // Preproces views_view_fields_view_name.
  $hook = __FUNCTION__ . "__{$view->name}";
  if (is_callable($hook)) {
    $hook($vars);
  }

  // Site search row tweaks.
  if ($view->name == 'site_search') {
    $entity = is_object($vars['row']->entity)
      ? $vars['row']->entity
      : isset($vars['row']->_entity_properties['entity object']) ? $vars['row']->_entity_properties['entity object'] : FALSE;

    // Only dealing with nodes.
    if (isset($entity->type)) {
      // Entity type search preprocess fields.
      $hook .= "__{$entity->type}";
      if (is_callable($hook)) {
        $hook($vars, $entity);
      }
    }

    // This will ensure that the content returned from the search server does
    // not render with any markup in the body.
    if (!empty($vars['fields']['search_api_excerpt'])) {
      $vars['fields']['search_api_excerpt']->content = _doesoe_theme_strip_tags($vars['fields']['search_api_excerpt']->content, '<strong><div>');
    }

    // Replace content type with more human friendly versions.
    // @see helpers/bundle.inc for more info.
    if (isset($vars['fields']['type_1']->content)) {
      _doesoe_theme_bundle_name_replace($vars['fields']['type_1']->content);
    }
  }

  // Add an a tag with a title so that the "At a glance" label is accessible.
  if ($view->name == "at_a_glance_by_theme_framework") {
    // Unfortunately we can't render as a different view mode so do it manually.
    if (!empty($vars['row']->field_field_theme) && !empty($vars['row']->field_field_framework)) {
      $theme_term = taxonomy_term_view($vars['row']->field_field_theme[0]['raw']['taxonomy_term'], 'icon');
      $framework_term = taxonomy_term_view($vars['row']->field_field_framework[0]['raw']['taxonomy_term'], 'icon');
      $vars['fields']['field_framework']->content = render($framework_term);
      $vars['fields']['field_theme']->content = render($theme_term);
    }
  }

  // Add framework by theme and year link to search results.
  if ($view->name == 'site_search' && !empty($view->result)) {
    if (!empty($vars['row']->_entity_properties['field_years'])) {
      $theme = FALSE;
      $framework = FALSE;

      // Get the theme & framework to be used to get the url.
      if (!empty($vars['row']->_entity_properties['field_theme'][0])) {
        $theme = taxonomy_term_load($vars['row']->_entity_properties['field_theme'][0]);
      }

      if (!empty($vars['row']->_entity_properties['field_framework'][0])) {
        $framework = taxonomy_term_load($vars['row']->_entity_properties['field_framework'][0]);
      }

      if ($theme && $framework) {
        // Get the year. If there is more than 1, get the current year.
        $years = $vars['row']->_entity_properties['field_years'];
        $year = count($years) > 1 ? _doesoe_theme_get_current_year_tid() : $years[0];

        // Get the url with year appended.
        $url = _doesoe_theme_get_theme_framework_link($theme, $framework) . '?year=' . $year;

        // Replace the url with the new one.
        $regx = '/<a(.*?)href="(.*?)"(.*?)>/';
        $replacement = '<a$1href="' . $url . '"$3>';
        $new_content = preg_replace($regx, $replacement, $vars['fields']['field_framework']->content);
        $vars['fields']['field_framework']->content = $new_content;
      }
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

/**
 * Implements hook_preprocess_views_view_fields().
 *
 * Create excerpts for search results if they aren't present.
 */
function doesoe_theme_preprocess_views_view_fields__site_search(&$vars) {
  if (!empty($vars['fields']['search_api_excerpt'])) {
    // If Search API was able to build an excerpt skip this and just display
    // what was return by search api.
    return;
  }

  // Try and find the entity.
  $entity = FALSE;
  if (isset($vars['row']->entity) && is_object($vars['row']->entity)) {
    $entity = $vars['row']->entity;
  }
  if (!$entity && isset($vars['row']->_entity_properties['entity object'])) {
    $entity = $vars['row']->_entity_properties['entity object'];
  }

  if (!$entity) {
    // If the entity couldn't be found... we're out of luck.
    return;
  }

  // Render the default view mode.
  $entity_view = entity_view('node', array($entity));
  if (empty($entity->nid) || empty($entity_view['node'][$entity->nid])) {
    return;
  }

  // Clean up the view mode.
  $entity_view = $entity_view['node'][$entity->nid];

  // Remove the unecessary items from the render array.
  unset($entity_view['links']);
  unset($entity_view['title']);
  unset($entity_view['#contextual_links']);

  $entity_view = drupal_render($entity_view);
  $entity_view = _doesoe_theme_strip_tags($entity_view);
  $entity_view = _doesoe_theme_text_trim($entity_view);

  $render_array = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('field-content', 'search-result__body')
    ),
    'content' => array(
      '#theme' => 'html_tag',
      '#tag' => 'p',
      '#value' => $entity_view,
    ),
  );

  $vars['fields']['view_node']->content = drupal_render($render_array) . $vars['fields']['view_node']->content;
}
/**
 * Implements hook_preprocess_views_view_fields().
 *
 * Update the case study url to anchor to correct place on topic page.
 */
function doesoe_theme_preprocess_views_view_fields__site_search__case_study(&$vars, $entity) {
  _doesoe_theme_url_search_result_link_via_topic_and_anchor($vars, $entity, 'field_topics');
}

/**
 * Implements hook_preprocess_views_view_fields().
 *
 * Update the graph url to anchor to correct place on topic page.
 */
function doesoe_theme_preprocess_views_view_fields__site_search__ckan_graph(&$vars, $entity) {
  _doesoe_theme_url_search_result_link_via_topic_and_anchor($vars, $entity, 'field_topic');
}

/**
 * Implements hook_preprocess_views_view_fields().
 *
 * Update the map url to anchor to correct place on topic page.
 */
function doesoe_theme_preprocess_views_view_fields__site_search__map(&$vars, $entity) {
  _doesoe_theme_url_search_result_link_via_topic_and_anchor($vars, $entity, 'field_topic');
}

/**
 * Implements hook_preprocess_views_view_fields().
 *
 * Update the framework at a glance url to take you to the landing page.
 */
function doesoe_theme_preprocess_views_view_fields__site_search__framework_at_a_glance(&$vars, $entity) {
  $theme = _doesoe_theme_entity_get_value('node', $entity, 'field_theme');
  $framework = _doesoe_theme_entity_get_value('node', $entity, 'field_framework');

  if (empty($theme->tid) || empty($framework->tid)) {
    return;
  }

  // Build a url for framework landing page.
  $url = url('taxonomy/term/' . $theme->tid) . '/framework/' . drupal_html_class($framework->name);

  $link_fields = array('title', 'view_node');
  foreach ($link_fields as $field) {
    $vars['fields'][$field]->content = _doesoe_theme_replace_href($vars['fields'][$field]->content, $url);
  }
}


/**
 * Implements hook_preprocess_views_view_fields().
 *
 * Update the exec summary url to take you to the theme landing page.
 */
function doesoe_theme_preprocess_views_view_fields__site_search__executive_summary(&$vars, $entity) {
  $theme = _doesoe_theme_entity_get_value('node', $entity, 'field_theme');

  if (empty($theme->tid)) {
    return;
  }

  $link_fields = array('title', 'view_node');
  foreach ($link_fields as $field) {
    $vars['fields'][$field]->content = _doesoe_theme_replace_href($vars['fields'][$field]->content, url('taxonomy/term/' . $theme->tid));
  }
}


/**
 * Implements hook_preprocess_views_view_fields().
 *
 * Update the Key finding url to go to KF page with year and anchor.
 */
function doesoe_theme_preprocess_views_view_fields__site_search__key_finding(&$vars, $entity) {
  $theme = _doesoe_theme_entity_get_value('node', $entity, 'field_theme');
  $years = _doesoe_theme_entity_get_value('node', $entity, 'field_years');

  if (empty($theme->tid)) {
    return;
  }

  // Build URL with optional year and anchor.
  $url = url('taxonomy/term/' . $theme->tid) . '/key-findings';
  if (!empty($years)) {
    $year = reset($years);
    $url .= '?year=' . $year->tid;
  }
  $url .= '#key-finding-' . $entity->nid;

  $link_fields = array('title', 'view_node');
  foreach ($link_fields as $field) {
    $vars['fields'][$field]->content = _doesoe_theme_replace_href($vars['fields'][$field]->content, $url);
  }
}
