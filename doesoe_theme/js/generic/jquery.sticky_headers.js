
(function ($) {

  "use strict";

  /*
   * Sticky headers
   * --------------
   *
   * Should be applied to the container that wraps the header, if the container is no longer
   * on screen it should no longer have a sticky header. There is many sticky header plugins
   * available but sadly none that really do just that.
   *
   * TODO: This class could be improved for performance, specifically changing it to a trigger
   * for sticky-in and sticky-out, but for now this meets MVP.
   *
   * Author: jeremy@doghouse.agency
   */

  var stickyHeaders = [];

  var stickyHeader = function(dom, settings) {
    var self = this;

    self.defaults = {
      headerSelector: '.nav-header',
      parentContainer: 'body',
      stickyClass: 'fixed',
      checkVisibility: true,
      inheritParentSize: true,
      inheritParentBottom: false,
      minWidth: 0
    };

    // Settings start with defaults and extended by options passed to the constructor.
    self.settings = $.extend(self.defaults, settings);

    /*
     * Initialize.
     */
    self.init = function () {

      // Define key variables.
      self.$dom = $(dom);
      self.$window = $(window);
      self.$doc = $(document);
      self.$header = $(self.settings.headerSelector, self.$dom);
      self.$parent = $(self.settings.parentContainer);


      // If header doesn't exist on the page then exit.
      if (self.$header.length === 0) {
        return;
      }

      // Cache original offests.
      self.originalLeft = self.$header.css('left');
      self.originalBottom = self.$header.css('bottom');

      // Define the placeholder.
      self.$placeholder = $('<div />')
        .addClass('sticky-placeholder')
        .height(self.$header.outerHeight())
        .hide()
        .insertAfter(self.$header);

      // Call scroll once to get started.
      self.scroll();

      // On scroll.
      self.$window.scroll(function() {
        self.scroll();
      });

      // On window resize.
      self.$window.resize(function() {
        self.scroll();
      });

      // Allow other scripts to trigger scroll/resize.
      self.$window.on('stickyHeader:scroll', function () {
        self.scroll();
      });

    };

    /*
     * Scroll callback.
     */
    self.scroll = function () {
      // Check visibility, if element hidden, don't apply stickyness.
      if (!self.checkPrerequisites()) {
        return;
      }

      // The offset for the wrapper.
      self.elTop = self.$dom.offset().top;
      self.elHeight = self.$dom.height();
      self.elBottom = self.elTop + self.elHeight;

      // Update last doc top pos.
      self.docTop = self.$doc.scrollTop();
      self.docHeight = self.$doc.height();

      // Toggle sticky if header should be visible.
      if (self.docTop > self.elTop && self.docTop < self.elBottom) {
        self.$header.addClass(self.settings.stickyClass);
        self.$placeholder.show();
        self.matchParentSize();
      }
      else {
        self.$header.removeClass(self.settings.stickyClass);
        self.$placeholder.hide();
        self.resetOffset();
      }
    };

    /*
     * Check if prerequisites are good to do sticky stuff.
     */
    self.checkPrerequisites = function() {
      if (
        self.settings.checkVisibility &&
        (!self.$header.is(':visible') || self.$window.width() <= self.settings.minWidth)
      ) {
        self.resetOffset();
        return false;
      }
      return true;
    };

    /*
     * Matches the header to its parent size and offset.
     */
    self.matchParentSize = function () {
      if (!self.settings.inheritParentSize) {
        return;
      }

      // Set the left/width based on parent.
      self.$header.width(self.$parent.width());
      self.$header.css({'left' : self.$parent.offset().left});

      // Dynamically set bottom property based on parent distance from bottom.
      if (self.settings.inheritParentBottom) {
        var newBottom = self.originalBottom,
          bottom = ((self.elTop + self.elHeight) - (self.$window.scrollTop() + self.$window.height()));
        if (bottom < 0) {
          newBottom = Math.abs(bottom);
        }
        self.$header.css({'bottom' : newBottom});
      }
    };

    /*
     * Reset the header back to original size.
     */
    self.resetOffset = function () {
      // Remove placeholder.
      self.$placeholder.hide();

      // If not inheriting, exit.
      if (!self.settings.inheritParentSize) {
        return;
      }

      // Reset left and width.
      self.$header.css({'left': self.originalLeft, 'width': '100%'});

      // Reset bottom.
      if (self.settings.inheritParentBottom) {
        self.$header.css({'bottom' : self.originalBottom});
      }
    };

    /*
     * Initialize the class.
     */
    self.init();

  };

  /**
   * Expose this class as a jQuery plugin.
   */
  $.fn.stickyHeaders = function (settings) {
    window.stickyHeaders = window.stickyHeaders || [];
    settings = settings || {};
    return this.each(function (i, dom) {
      window.stickyHeaders.push(
        new stickyHeader(dom, settings)
      );
    });
  };

})(jQuery);