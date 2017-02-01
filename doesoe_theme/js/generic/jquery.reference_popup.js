(function ($, Clipboard) {

  "use strict";

  /*
   * Reference popup
   * --------------
   *
   * This plugin shows the reference popup when the user clicks a links that has a
   * hash tag that points to a reference content.
   * The popup is only created once and it's content is replaced every time a user
   * clicks another reference link.
   *
   * Author: lemuel@doghouse.agency
   */

  var referencePopups = [];

  var referencePopup = function (dom, settings) {
    var self = this;

    self.defaults = {
      type: 'inline',
      title: '',
      src: null,
      content: null,
      selector: 'body',
      palette: null,
      wrapperClass: 'reference-popup',
      mainClass: 'reference-popup__main',
      headerClass: 'reference-popup__header',
      contentClass: 'reference-popup__content',
      footerClass: 'reference-popup__footer',
      titleClass: 'reference-popup__title',
      closeBtnClass: 'reference-popup__close-button',
      copyBtnClass: 'reference-popup__copy-button',
      activeClass: 'is-active'
    };

    // Settings start with defaults and extended by options passed to the
    // constructor.
    self.settings = $.extend(self.defaults, settings);

    /*
     * Initialize.
     */
    self.init = function () {

      // Define key variables.
      self.$dom = $(dom);
      self.$document = $(document);
      self.$body = $('body');

      self.createPopup();

      // Opens the popup when clicking on the reference.
      self.$dom.click(function (e) {
        switch(self.settings.type) {
          case 'inline':
            self.$content = $(self.settings.src);
            e.preventDefault();
            e.stopPropagation();
            self.openPopup();
            break;

          case 'ajax':
            if (self.settings.selector != '' && self.settings.selector != '#') {
              $.get(self.settings.src, function (data) {
                self.$content = $(data).find(self.settings.selector);
                self.openPopup();
              });
            }
            break;
        }
      });

      // Closes the popup when clicking on the close button.
      $('.' + self.settings.closeBtnClass).click(self.close);

      // Closes the popup when clicking outside of it.
      self.$document.click(function (e) {
        if (!self.$popup.is(e.target) && self.$popup.has(e.target).length === 0) {
          self.close();
        }
      });

      // Copies the reference when clicking on the copy button.
      self.clipboard = new Clipboard('.' + self.settings.copyBtnClass);
      self.clipboard.on('success', function(e) {
        e.clearSelection();
        e.trigger.textContent = 'Copied';
        window.setTimeout(function() {
          e.trigger.textContent = 'Copy';
        }, 2000);
      });
    };

    /*
     * Creates the popup container.
     */
    self.createPopup = function () {
      self.$popup = $('.' + self.settings.wrapperClass, self.$body);
      if (self.$popup.length === 0) {
        self.$popup = $('' +
          '<div class="' + self.settings.wrapperClass + '">' +
            '<div class="' + self.settings.mainClass + '">' +
              '<div class="' + self.settings.headerClass + '">' +
                '<h4 class="' + self.settings.titleClass + '"></h4>' +
                '<button class="' + self.settings.closeBtnClass + '" title="Close reference"></button>' +
              '</div>' +
              '<div class="' + self.settings.contentClass + '"></div>' +
              '<div class="' + self.settings.footerClass + '">' +
                '<button data-clipboard-target=".' + self.settings.contentClass + '" class="' + self.settings.copyBtnClass + '" title="Copy to clipboard">Copy</button>' +
              '</div>' +
            '</div>' +
          '</div>'
        );
        self.$body.append(self.$popup);
      }
    };

    /*
     * Opens the popup and insert reference content.
     */
    self.openPopup = function () {
      // If content is specified via settings it trumps content via the content selector.
      var content = self.settings.content !== null ? self.settings.content : self.$content.html();
      if (content.length > 0) {
        self.updatePaletteClass();
        $('.' + self.settings.titleClass, self.$popup).html(self.settings.title);
        $('.' + self.settings.contentClass, self.$popup).html(content);
        self.$popup.addClass(self.settings.activeClass);
      }
    };

    /*
     * Closes the popup and clears its content.
     */
    self.close = function () {
      if (self.$popup.hasClass(self.settings.activeClass)) {
        $('.' + self.settings.titleClass, self.$popup).empty();
        $('.' + self.settings.contentClass, self.$popup).empty();
        self.$popup.removeClass(self.settings.activeClass);
      }
    };

    /*
     * Removes existing palette classes and adds the current.
     */
    self.updatePaletteClass = function() {
      var palettePrefix = self.settings.wrapperClass + '__palette__';
      self.$popup.removeClass(function(index, css) {
          return (css.match(new RegExp('\\b(' + palettePrefix + '\\S*)\\b', 'g')) || []).join(' ');
        })
        .addClass(palettePrefix + self.settings.palette);
    };

    /*
     * Initialize the class.
     */
    self.init();

  };

  /**
   * Expose this class as a jQuery plugin.
   */
  $.fn.referencePopup = function (settings) {
    window.referencePopups = window.referencePopups || [];
    settings = settings || {};
    return this.each(function (i, dom) {
      window.referencePopups.push(
        new referencePopup(dom, settings)
      );
    });
  };

})(jQuery, Clipboard);
