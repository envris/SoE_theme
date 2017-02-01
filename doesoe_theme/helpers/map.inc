<?php
/**
 * @file
 * Map related functionality.
 */

/**
 * Add the regions geoJSON to drupal JS settings so it can be used by Leaflet.
 */
function _doesoe_theme_map_add_region_geojson() {

  // Define pages that have a search map (and require regions).
  $allowed_paths = array('region-search');

  // Exit if not allowed path.
  if (!in_array(arg(0), $allowed_paths)) {
    return;
  }

  // This should rarely change so we'll cache it for a year to prevent
  // unnecessary db calls.
  $cache_key = 'doesoe_theme_region_geojson';
  $cache_expiry = (60 * 60 * 24 * 365);

  $cache = cache_get($cache_key);
  if (!empty($cache->data) && $cache->expire > time()) {
    $features = $cache->data;
  }
  else {
    // Load the library that converts WKT to geoJSON.
    $lib_path = drupal_get_path('theme', 'doesoe_theme') . '/libraries/gisconverter/gisconverter.php';
    require_once $lib_path;
    $decoder = new gisconverter\WKT();

    // Get all the regions.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'taxonomy_term')
      ->entityCondition('bundle', 'regions');
    $result = $query->execute();

    // Build features.
    $features = array();
    if (isset($result['taxonomy_term'])) {
      $tids = array_keys($result['taxonomy_term']);
      $regions = taxonomy_term_load_multiple($tids);
      foreach ($regions as $region) {
        // Skip of no WKT data.
        if (empty($region->field_geometry_collection)) {
          continue;
        }
        // Get the WKT from the region term, multiple items allowed and are
        // split by new lines.
        $region_w = entity_metadata_wrapper('taxonomy_term', $region);
        $region_wkt = $region_w->field_geometry_collection->value();
        $region_wkts = explode(PHP_EOL, str_replace("\r\n", PHP_EOL, $region_wkt));
        $color = $region_w->field_color_scheme->value();
        // For each WKT decode to json and add term info.
        foreach ($region_wkts as $wkt) {
          $geojson = $decoder->geomFromText($wkt)->toGeoJSON();
          $feature = json_decode($geojson);
          $feature->properties = array(
            'name' => $region->name,
            'tid' => $region->tid,
            'color' => $color,
          );
          $features[] = $feature;
        }
      }
    }

    // Save to cache.
    cache_set($cache_key, $features, 'cache', time() + $cache_expiry);
  }

  // Add JS to the page.
  drupal_add_js(array('doesoe_theme' => array('mapRegionFeatures' => $features)), 'setting');
}
