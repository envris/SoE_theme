/*
 * jTOC - jQuery/Jeremy's Table of contents.
 *
 * https://github.com/jez500/jtoc
 * v0.1
 *
 * Builds a table of contents based on headings on the page.
 *
 * Usage:
 * Call on a ul/ol and it will get populated with TOC links
 *
 * Example:
 * $('ul.toc').jtoc({content: '.content', headings: 'h2,h3'});
 *
 * Options:
 * - content: The selector to search for headings in. Default: 'body'
 * - headings: Headings to search for. Default: "h1,h2,h3"
 * - anchorPrefix: Optionally add a prefix to anchors.
 * - scrollEnabled: Should there be smooth scrolling to headings. Default true
 * - scrollSpeed: Speed of animation when scrolling to headings. Default 400
 * - scrollEase: Scroll animation. Default: 'swing'
 * - scrollEl: The element that scrolls. Default: 'body,html'
 * - scrollOffset: A pixel offset for scroll. Default 0
 *
 * Inspiration/Credits to:
 * - https://github.com/firstandthird/smooth-scroller
 * - https://github.com/ndabas/toc
 * - https://github.com/jeremy-doghouse/toc
 */
(function($) {

  "use strict";

  /**
   * jTOC class.
   */
  var jTOC = function(dom, options) {
    var self = this,
      defaultOpts;

    // Setup options.
    defaultOpts = {
      content: "body",
      headings: "h1,h2,h3",
      anchorPrefix: '',
      scrollEnabled: true,
      scrollSpeed: 400,
      scrollEase: 'swing',
      scrollEl: 'body,html',
      scrollOffset: 0
    };
    self.opts = $.extend(defaultOpts, options);

    /*
     * Initialise the class.
     */
    self.init = function () {
      // Init some vars we'll need.
      self.$dom = $(dom);
      self.stack = [self.$dom];
      self.listTag = dom.tagName;
      self.currentLevel = 0;
      self.headingSelectors = self.opts.headings.split(',');

      // Parse and return self for chaining.
      self.parseContent();

      // Add a trigger for other scripts to do their thing.
      self.$dom.trigger('jTocBuilt');

      return self;
    };

    /*
     * Parse the content into a nested list.
     */
    self.parseContent = function () {
      // Set up some automatic IDs if we do not already have them
      $(self.opts.content).find(self.opts.headings).each(function () {
        var $el = $(this), anchorName, $a, level, parentItem;

        // What level is the current heading?
        level = $.map(self.headingSelectors, function (selector, index) {
          return $el.is(selector) ? index : undefined;
        })[0];

        // Generate a valid ID: must start with a letter, and contain only letters and
        // numbers. All other characters are replaced with underscores.
        anchorName = self.opts.anchorPrefix + $el.text().replace(/^[^A-Za-z]*/, "").replace(/[^A-Za-z0-9]+/g, "_");

        // Add anchor.
        if ($el.attr('id') !== anchorName) {
          $('<span/>').attr('id', anchorName).insertBefore($el);
        }

        if (level > self.currentLevel) {
          // If the heading is at a deeper level than where we are, start a new nested
          // list, but only if we already have some list items in the parent. If we do
          // not, that means that we're skipping levels, so we can just add new list items
          // at the current level.
          // In the upside-down stack, unshift = push, and stack[0] = the top.
          parentItem = self.stack[0].children("li:last")[0];
          if (parentItem) {
            self.stack.unshift($("<" + self.listTag + "/>").appendTo(parentItem));
          }
        } else {
          // Truncate the stack to the current level by chopping off the 'top' of the
          // stack. We also need to preserve at least one element in the stack - that is
          // the containing element.
          self.stack.splice(0, Math.min(self.currentLevel - level, Math.max(self.stack.length - 1, 0)));
        }

        // Build TOC link.
        $a = $('<a/>')
          .text($el.text())
          .attr('href', '#' + anchorName)
          .addClass('toc-link toc-link--level-' + level)
          .bind('click', function (e) {
            // If scroll enabled.
            if (self.opts.scrollEnabled === true) {
              e.preventDefault();
              self.scrollTo('#' + anchorName);
            }
            $el.trigger('selected', $(this).attr('href'));
            $(this).blur();
          });

        // Add the list item
        $("<li/>").appendTo(self.stack[0]).append($a);

        self.currentLevel = level;
      });

      return self;
    };

    /*
     * ScrollTo element method.
     * @see https://github.com/firstandthird/smooth-scroller
     */
    self.scrollTo = function (selector) {
      var $el = $(selector);

      $(self.opts.scrollEl).animate({
        scrollTop: $el.offset().top - $(self.opts.scrollEl).offset().top - self.opts.scrollOffset
      }, self.opts.scrollSpeed, self.opts.scrollEase, function () {
        var hash = $el.attr('id');

        if (hash.length) {
          if (history.pushState) {
            history.pushState(null, null, '#' + hash);
          } else {
            document.location.hash = hash;
          }
        }

        $el.trigger('scrollComplete');
      });

      return self;
    };

    // Initialize.
    self.init();

    // Return self for chaining.
    return self;
  };


  // jQuery plugin.
  $.fn.jtoc = function(options) {
    this.each(function(i, d) {
      new jTOC(d, options);
    });
    return this;
  };

}(jQuery));
