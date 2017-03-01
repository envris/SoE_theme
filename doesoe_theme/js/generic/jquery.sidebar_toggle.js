/**
 * @file
 * Sidebar toggle.
 *
 * Toggle functionality for sidebar blocks.
 *
 * Author: jeremy@doghouse.agency
 */

(function ($) {

  "use strict";

  var sideBarToggles = [];

  var sideBarToggle = function(dom, settings) {
    var self = this;

    self.defaults = {
      slideSpeed: 200,
      openClass: 'open',
      defaultOpen: false,
      headerSelector: '> .sidebar-toggle__header',
      contentSelector: '> .sidebar-toggle__content',
      componentClass: 'sidebar-toggle',
      subSelector: null,
      subHeaderSelector: '> .sidebar-toggle__sub-header',
      subContentSelector: '> .sidebar-toggle__sub-content'
    };

    // Settings start with defaults and extended by options passed to the constructor.
    self.settings = $.extend(self.defaults, settings);

    /*
     * Initialize.
     */
    self.init = function () {
      self.$window = $(window);
      self.$header = $(self.settings.headerSelector, dom);
      self.$content = $(self.settings.contentSelector, dom);

      // Add some identifying classes for styling.
      $(dom).addClass(self.settings.componentClass);
      self.$content.addClass(self.settings.componentClass + '__content');

      self.$header.each(function() {
        var $el = $(this);
        // If default state is open.
        if (self.settings.defaultOpen === true) {
          self.toggle($el);
        }

        // Add the class for the component.
        $el.addClass(self.settings.componentClass + '__header');

        // Check if the header has any children and add a class accordingly.
        var $content = $el.find(self.settings.contentSelector);
        if ($content.length === 0) {
          $content = $el.parent().find(self.settings.contentSelector);
        }

        $el.addClass(self.settings.componentClass + ($content.length === 0 ? '__no-content' : '__has-content'));
      });

      // Toggle on header click.
      self.$header.click(function(e) {
        self.toggle($(this));
        return false;
      });

      // Apply again to sub selector.
      if (self.settings.subSelector !== null) {
        $(self.settings.subSelector, dom)
          .once('sub-sidebar-toggle')
          .sideBarToggle({
            headerSelector: self.settings.subHeaderSelector,
            contentSelector: self.settings.subContentSelector,
            componentClass: self.settings.componentClass + '-sub'
          });
      }
    };

    /*
     * Toggle opening and closing.
     */
    self.toggle = function ($element) {
      var $content = $element.find(self.settings.contentSelector);

      if ($content.length == 0) {
        // This is to preserve backwards compatibility with old settings as they're typically expected to be found in
        // context of the parent of the current element.
        $content = $element.parent().find(self.settings.contentSelector);
      }

      $content.slideToggle();
      $element.toggleClass(self.settings.openClass);
      $(dom).toggleClass(self.settings.openClass);
      self.$window.trigger('sideBarToggle:toggle', [$element]);
    };

    /*
     * Initialize the class.
     */
    self.init();

  };

  /**
   * Expose this class as a jQuery plugin.
   */
  $.fn.sideBarToggle = function (settings) {
    window.sideBarToggles = window.sideBarToggles || [];
    settings = settings || {};
    return this.each(function (i, dom) {
      window.sideBarToggles.push(
        new sideBarToggle(dom, settings)
      );
    });
  };

})(jQuery);
