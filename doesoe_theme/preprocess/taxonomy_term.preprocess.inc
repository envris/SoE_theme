<?php
/**
 * @file
 * Taxonomy term preprocessing.
 */

/**
 * Implements template_preprocess_taxonomy_term().
 */
function doesoe_theme_preprocess_taxonomy_term(&$variables, $hook) {
  $entity = $variables['term'];
  $entity_w = entity_metadata_wrapper('taxonomy_term', $entity);
  $variables['safe_description'] = strip_tags($variables['description']);

  // Themes preprocessing.
  if ($entity->vocabulary_machine_name == 'themes') {
    $variables['classes_array'][] = 'theme-' . $variables['view_mode'];

    // Add the palette key to the row.
    if (isset($entity_w->field_palette_key)) {
      $variables['classes_array'][] = 'row-palette__' . drupal_html_class($entity_w->field_palette_key->value());
    }

    // DS doesn't support truncating the description so we do it here.
    if ($variables['view_mode'] == 'teaser' && isset($variables['content']['description']['#markup'])) {
      $summary = _doesoe_theme_text_trim($variables['content']['description']['#markup'], 200);
      $variables['content']['description']['#markup'] = $summary;
    }

    if ($variables['view_mode'] == "framework_theme_item") {
      if (arg(2) && is_numeric(arg(2))) {
        $term = taxonomy_term_load(arg(2));
        if ($term && $term->vocabulary_machine_name == 'frameworks') {
          // Add framework to theme title.
          $variables['content']['title'][0]['#markup'] .= ' ' . strtolower($term->name);
          // Display at a glance content.
          $view = views_get_view('at_a_glance_by_theme_framework');
          $view->set_display('block');
          $view->set_arguments(array($variables['tid'], $term->tid));
          $view->pre_execute();
          $view->execute();
          $variables['glance'] = $view->render();

          // Display framework icon per theme.
          $result = field_view_field('taxonomy_term', $term, 'field_icon_code', 'list_teaser');
          $variables['icon'] = $result;

          // Display Assessment Summaries search link.
          $variables['assessment_summaries_link'] = l(
            t('> Assessment summaries for @title', array('@title' => $variables['content']['title'][0]['#markup'])),
            'theme/' . drupal_html_class($variables['name']) . '/assessment-summaries',
            array('query' => array('framework' => $term->tid))
          );

          // Display Topics search link.
          $variables['topics_link'] = l(
            t('> List all topics for @title', array('@title' => $variables['content']['title'][0]['#markup'])),
            'theme/' . drupal_html_class($variables['name']) . '/topics',
            array(
              'query' => array(
                'framework' => $term->tid,
              ),
            )
          );
        }
      }
    }
  }

  // Frameworks preprocessing.
  if ($entity->vocabulary_machine_name == 'frameworks') {
    $is_teaser = $variables['view_mode'] == 'teaser';
    $variables['classes_array'][] = 'frameworks-' . $variables['view_mode'];
    if ($is_teaser && isset($variables['content']['description']['#markup'])) {
      $summary = _doesoe_theme_text_trim($variables['content']['description']['#markup'], 200);
      $variables['content']['description']['#markup'] = $summary;
    }

    // We change the title link and the read more link to point to the framework
    // landing page (e.g. theme/%theme-name/framework/%framework-name).
    if ($is_teaser) {
      $theme = _doesoe_theme_get_current_theme_term();
      if ($theme) {
        $path = sprintf('theme/%s/framework/%s', $theme->name, $entity_w->label());
        $path = strtolower(str_replace(' ', '-', $path));
        $variables['content']['title'][0]['#markup'] = l($entity_w->label(), $path);
        $variables['content']['more_link'][0]['#markup'] = l(t('Find out more'), $path);
      }
    }
    if ($variables['view_mode'] == 'full_teaser') {
      $link_url = $variables['term_url'];
      $link_text = t('Compare summary information for @title across entire report', array('@title' => $variables['name']));
      $variables['content']['more_link'][0]['#markup'] = l($link_text, $link_url);
    }
  }

  // Rewrite region term_url to link to pre-populated search page.
  if ($entity->vocabulary_machine_name == 'regions') {
    $variables['term_url'] = url('search', array(
      'query' => array('region[]' => $variables['tid']),
    ));
  }
}
