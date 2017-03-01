/**
 * @file
 * Search Map.
 *
 * This plugin handles the rendering and control over the leaflet map on the search page.
 * The map is used as a filter to filter content by region.
 *
 * Dependencies:
 * http://leafletjs.com
 * https://github.com/heigeo/leaflet.wms
 *
 * Author: carl@doghouse.agency
 */

(function ($, window) {

  /*
   * Search Map
   * --------------
   *
   * This plugin handles the rendering and control over the leaflet map on the search page.
   * The map is used as a filter to filter content by region.
   *
   * Dependencies:
   * http://leafletjs.com
   * https://github.com/heigeo/leaflet.wms
   *
   * Author: carl@doghouse.agency
   */

  "use strict";

  var searchMap = function (el, settings) {
    var self = this;

    self.selectedRegions = [];

    self.autoSubmitTimeout = {};

    // Get settings.
    self.defaults = {
      mapSelector: 'search-map',
      mapCenter: [-25, 133],
      mapZoom: 3,
      regionFeatures: [],
      scrollWheelZoom: false,
      mapTileLayer: 'http://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}.png',
      mapTileLayerAttr: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, &copy; <a href="http://cartodb.com/attributions">CartoDB</a>'
    };
    self.settings = $.extend(self.defaults, settings);

    // Build the map based on the settings provided.
    self.init = function () {
      self.getFilterRegions();
      self.setMap();
      self.setRegions();
      self.filterChange();
      self.extentChange();
      self.setExtentOnInit();
    };

    // Sync region selection on filter change.
    self.filterChange = function () {
      var $filter = $(el);
      $filter.change(function () {
        self.selectedRegions = $filter.val() || [];
        self.removeAllLayers();
        self.setRegions();
      });
    };

    // Remove all pollygon layers from the map.
    self.removeAllLayers = function () {
      $.each(self.map.allLayers, function (i,v) {
        self.map.removeLayer(v);
      });
    };

    // Get selected filter regions.
    self.getFilterRegions = function () {
      var $filter = $(el);
      var selected = $filter.val();
      self.selectedRegions = selected || [];
      return $filter.val();
    };

    // Set filter regions.
    self.setFilterRegions = function (selected) {
      var $filter = $(el);
      self.selectedRegions = selected || [];
      $filter.val(selected);
      $filter[0].sumo.reload();
    };

    // Set the map positioning and zoom.
    self.setMap = function () {
      self.map = L.map(self.settings.mapSelector, {
        center: self.settings.mapCenter,
        zoom: self.settings.mapZoom,
        scrollWheelZoom: self.settings.scrollWheelZoom
      });
      var basemap = L.tileLayer(self.settings.mapTileLayer, {
        attribution: self.settings.mapTileLayerAttr
      });
      self.map.allLayers = [];
      self.map.activeRegions = [];
      self.map.addLayer(basemap);
      self.map.zoomControl.setPosition('bottomleft');
    };

    // Set Regions.
    self.setRegions = function () {
      // Add Regions.
      if (self.settings.regionFeatures !== undefined) {
        L.geoJson(self.settings.regionFeatures, {
          style: self.settings.regionStyles,
          onEachFeature: self.onEachFeature
        }).addTo(self.map);
      }
    };

    // Build polygon style based on state.
    self.buildRegionStyle = function (properties, hover) {
      var isActive = self.isRegionActive(properties.tid) > -1;
      var style = {
            color:  "#9a953a",
            fillColor:  "#9a953a",
            fillOpacity:0.4,
            opacity: 0.6,
            weight: 1
          };
      if (isActive || hover) {
        style.fillOpacity = 0.7;
      }
      if (properties.color !== null) {
        style.fillColor = properties.color;
        style.color = properties.color;
      }
      return style;
    },

    // Region actions.
    self.onEachFeature = function(feature, layer) {
      self.map.allLayers.push(layer);

      (function (layer, properties) {
        layer.bindTooltip(properties.name).addTo(self.map);

        layer.setStyle(self.buildRegionStyle(properties));

        // Hover on a region.
        layer.on("mouseover", function (e) {
          layer.setStyle(self.buildRegionStyle(properties, true));
        });
        layer.on("mouseout", function (e) {
          layer.setStyle(self.buildRegionStyle(properties));
        });

        // Click a region.
        layer.on("click", function(e) {
          // Toggle active regions.
          var idx = self.isRegionActive(properties.tid);
          if (idx === -1) {
            self.selectedRegions.push(properties.tid);
            layer.setStyle(self.buildRegionStyle(properties));
          }
          else {
            self.selectedRegions.splice(idx, 1);
            layer.setStyle(self.buildRegionStyle(properties));
          }
          // Update filter with active regions.
          var $filter = $(el);
          $filter.val(self.selectedRegions);
          $filter[0].sumo.reload();
          self.submitSearch($filter);
        });

      })(layer, feature.properties);

    };

    // Trigger form submit on slection (with timeout).
    self.submitSearch = function ($filter) {
      clearTimeout(self.autoSubmitTimeout);
      self.autoSubmitTimeout = setTimeout(function(){
        $('body').append('<div class="ajax-progress"><div class="throbber"></div></div>');
        $filter.closest('form').trigger('submit');
      }, 1500);
    };

    // Wrapper for determining if a region tid is active.
    self.isRegionActive = function(tid) {
      return self.selectedRegions.indexOf(tid);
    };

    // Get an extent from via tid.
    self.extentViaTid = function(tid) {
      if (self.settings.regionExtents && self.settings.regionExtents[tid]) {
        return self.settings.regionExtents[parseInt(tid)];
      }
      return false;
    };

    // Go to coords on map extent filter change.
    self.extentChange = function () {
      var $filter = self.settings.extentFilter;
      $filter.change(function () {
        var extent = self.extentViaTid($(this).val());
        if (extent) {
          self.extentGoTo(extent);
        }
      });
    };

    // Get map extent from url query.
    self.getExtentFromUrl = function () {
      var search = new URLSearchParams(window.location.search);
      return self.extentViaTid(search.get('extent'));
    };

    // Check if extent is an array of coords.
    self.extentIsArray = function (extent) {
      try {
        JSON.parse(extent.coords);
      }
      catch (e) {
        return false;
      }
      return true
    };

    // Set map extent on map init.
    self.setExtentOnInit = function () {
      var extent = self.getExtentFromUrl();
      if (extent) {
        self.extentGoTo(extent);
      }
    };

    // Go to coords on map.
    self.extentGoTo = function (extent) {
      var coords, zoom;
      if (!extent || !self.extentIsArray(extent)) {
        coords = self.settings.mapCenter;
        coords.reverse();
        zoom = self.settings.mapZoom;
      }
      else {
        coords = JSON.parse(extent.coords);
        zoom = parseInt(extent.zoom);
      }
      self.map.setView([coords[1], coords[0]], zoom);
    };

    self.init();
  };

  window.searchMap = window.searchMap || searchMap;

    /**
   * Expose this class as a jQuery plugin.
   */
  $.fn.searchMap = function (settings) {
    settings = settings || {};
    return this.each(function (i, el) {
      new searchMap(el, settings);
    });
  };

})(jQuery, this);
