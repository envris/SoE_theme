
(function ($) {

  "use strict";

  /*
   * Sidebar toggle
   * --------------
   *
   * Toggle functionality for sidebar blocks.
   *
   * Author: jeremy@doghouse.agency
   */

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
      self.$header.addClass(self.settings.componentClass + '__header');
      self.$content.addClass(self.settings.componentClass + '__content');

      // If default state is open.
      if (self.settings.defaultOpen === true) {
        self.toggle();
      }

      // Toggle on header click.
      self.$header.click(function(e) {
        e.preventDefault();
        self.toggle();
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
    self.toggle = function () {
      self.$content.slideToggle();
      $(dom).toggleClass(self.settings.openClass);
      self.$header.toggleClass(self.settings.openClass);
      self.$window.trigger('sideBarToggle:toggle');
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
