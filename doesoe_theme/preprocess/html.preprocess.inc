<?php
/**
 * @file
 * Preprocessors for html.
 */

/**
 * Implements hook_preprocess_html().
 */
function doesoe_theme_preprocess_html(&$variables) {

  // Theme font.
  drupal_add_css('//fonts.googleapis.com/css?family=Ubuntu:400,500,300,700', array(
    'type'  => 'external',
    'group' => CSS_THEME,
  ));

  // D3 JS.
  // GovCMS CKAN includes this when required but assessment summaries also req
  // it for visualisations. By including this as JS_LIBRARY it should ensure
  // it is not loaded twice. If GovCMS CKAN changes D3 version or includes it
  // locally then this should be re-addressed.
  drupal_add_js('https://cdnjs.cloudflare.com/ajax/libs/d3/3.5.13/d3.js', array(
    'type'  => 'external',
    'group' => JS_LIBRARY,
  ));

  // Apply palette class to body.
  $palette_key = _doesoe_theme_get_palette('palette_key');
  if (!empty($palette_key)) {
    $variables['classes_array'][] = 'palette__' . drupal_html_class($palette_key);
  }

  // If the current page is not in the main menu tree, or it is top level then
  // we add a class to show the sidebar (desktop only).
  if (_doesoe_theme_menu_show_nav_side()) {
    $variables['classes_array'][] = 'with-nav-side';
  }

  // Addthis sharing widget JS. This not using drupal_add_js as we want to
  // include the async attribute. More: https://www.drupal.org/node/2299773.
  // Configure widget here: http://addthis.com/dashboard
  // @see bean delta social-sharing-block for embeding.
  $variables['page']['page_bottom']['addthis'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'script',
    '#value' => ' ',
    '#attributes' => array(
      'src' => '//s7.addthis.com/js/300/addthis_widget.js#pubid=ra-5758afeff41f0404',
      'type' => 'text/javascript',
      'async' => 'async',
    ),
  );

  // Add in a custom stylesheet to be embedded in the ckan graph exports so we
  // can do some soe specific style changes.
  drupal_add_js(array(
    'govcmsCkanDisplay' => array(
      'exportStylesheets' => array(url($variables['directory'] . '/css/govcms_ckan.css')),
    ),
  ), 'setting');

  // Some JS scripts need to know the tid of the current year, all citations or
  // TOC settings so we add them all to in-page js.
  $current_year = _doesoe_theme_get_current_year_tid();
  drupal_add_js(array(
    'doeSoe' => array(
      'currentYearTid' => $current_year,
      'allCitations' => _doesoe_theme_get_all_theme_citations(),
      'tocSettings' => _doesoe_theme_toc_headings(),
    ),
  ), 'setting');

  // Add Regions geoJSON for search maps.
  _doesoe_theme_map_add_region_geojson();

  // For searches, we add historic content class if not current year.
  if (!empty($_REQUEST['year']) && is_array($_REQUEST['year']) && !in_array($current_year, $_REQUEST['year'])) {
    $variables['classes_array'][] = 'historic-content';
    $variables['classes_array'][] = 'search-historic';
  }

  // For node pages add historic if required.
  if (_doesoe_theme_is_current_node_historic()) {
    $variables['classes_array'][] = 'historic-content';
    $variables['classes_array'][] = 'force-historic';
  }
}
