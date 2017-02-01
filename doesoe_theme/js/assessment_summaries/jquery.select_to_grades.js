
(function ($) {

  "use strict";

  /*
   * Turn a multiple select into a grade selector.
   *
   * This is used on the assessment summary search page for turning a
   * multi select element into a grade selector.
   */

  var SelectToGrade = function (dom, settings) {
    var self = this;

    self.defaults = {
      componentPrefix: 'select-to-grade',
      selectedClass: 'selected'
    };

    // Settings start with defaults and extended by options passed to the constructor.
    self.settings = $.extend(self.defaults, settings);

    // Store dom obj.
    self.$dom = $(dom);

    /*
     * Init and create the grade selector dom.
     */
    self.init = function () {
      var $opt;

      self.$el = $('<ul>')
        .addClass(self.compClass('wrapper'));

      self.$dom.find('option').each(function (i, opt) {
        // Create the options.
        $opt = $('<li>')
          .addClass(self.compClass('option'))
          .addClass(self.compClass('option-' + i))
          .data('value', $(opt).val())
          .html($(opt).html())
          .attr('title', $(opt).html())
          .click(function (e) {
            self.optionClick(this);
          })
          .appendTo(self.$el);

        // If option selected, add selected class to $opt.
        if ($(opt).is(':selected')) {
          $opt.addClass(self.settings.selectedClass);
        }

        // Use the tooltipster library if we have it available.
        if (typeof $.fn.tooltipster !== 'undefined') {
          $opt.tooltipster();
        }
      });

      // Add the grade selector after the select.
      self.$el.insertAfter(self.$dom);

      // Hide the select.
      self.$dom.hide();
    };

    /*
     * On option click.
     */
    self.optionClick = function (el) {
      var $opt = $(el),
        selected = [];

      // Toggle this option as selected.
      $opt.toggleClass(self.settings.selectedClass);

      // Get a new array of selected items.
      self.$el.find('li').each(function (i, d) {
        if ($(d).hasClass(self.settings.selectedClass)) {
          selected.push($(d).data('value'));
        }
      });

      // Update the select values and trigger change.
      self.$dom.val(selected).trigger('change');
    };

    /*
     * Create a component class.
     */
    self.compClass = function (name) {
      return self.settings.componentPrefix + '__' + name;
    };

    self.init();
  };


  /*
   * jQuery plugin/function.
   */
  $.fn.SelectToGrade = function (settings) {
    window.SelectToGrades = window.SelectToGrades || [];
    settings = settings || {};
    return this.each(function (i, dom) {
      window.SelectToGrades.push(
        new SelectToGrade(dom, settings)
      );
    });
  };

})(jQuery);
