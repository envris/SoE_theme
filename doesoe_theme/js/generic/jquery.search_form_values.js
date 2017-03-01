/**
 * @file
 * Search Form Values.
 *
 * This plugin returns markup for displaying search form values when printing
 * the results of the search page.
 */

(function ($) {

  "use strict";

  var searchFormValues = function (el, settings) {
    var self = this;
    // Search form widgets to find.
    this.widgets = $(settings.widgets.join(', '), el);

    /*
     * Constructor
     */
    this.init = function () {
      this.addFilterMarkup();
    }

    /*
     * Add the filter markup to the dom.
     */
    this.addFilterMarkup = function () {
      var list = this.filterValueListMarkup();
      $(el).find('> div').addClass(settings.containerClass + '__filters');
      if (list !== '') {
        var container = $('<div />')
          .addClass(settings.containerClass)
          .attr('aria-hidden', true)
          .html(list)
          .prepend('<h4>Filter Criteria</h4>')
          .prependTo(el);
      }
    }

    /*
     * Return the filter markup.
     */
    this.filterValueListMarkup = function () {
      var list = this.getFilterValueList();
      var markup = '';
      for (var key in list) {
        var item = '<div><span>' + key + ': </span><span>' + list[key] + '</span></div>';
        markup = markup + item;
      }
      return markup;
    };

    /*
     * Get an array of filters and their values.
     */
    this.getFilterValueList = function () {
      var list = [];
      this.widgets.each(function(i,v) {
        var label = self.getLabel($(this));
        var value = self.getHumanValue($(this));
        if (label && value) {
          list[label] = value;
        }
      });
      return list;
    };

    /*
     * Get the filters label.
     */
    this.getLabel = function (widget) {
      return widget.find('label:first').text().replace(/^\s+|\s+$/g, "");
    },

    /*
     * Get the filter input element.
     */
    this.getInput = function (widget) {
      var types = ['select', 'input'];
      var output;
      $.each(types, function (i, v) {
        if (widget.find(v).length > 0) {
          output = widget.find(v);
          return false;
        }
      });
      return output;
    },

    /*
     * Get the filter input Human value.
     */
    this.getHumanValue = function (widget) {
      var $input = this.getInput(widget);
      var InputTag = $input.get(0).tagName;
      var humanValue = false;
      switch (InputTag) {
        case 'SELECT':
          var filterValue = $input.val();
          if (filterValue) {
            var humanValue = this.selectListHumanValues(filterValue, $input);
          }
          break;

        case 'INPUT':
          var inputType = $input.attr('type');
          switch (inputType) {
            case 'radio':
              humanValue = this.radioHumanValue($input);
              break;

            case 'text':
              humanValue = $input.val();
              break;

          }
          break;

      }
      return humanValue;
    };

    /*
     * Get radio input value.
     */
    this.radioHumanValue = function ($input) {
      var value;
      $input.each(function (i, v) {
        if ($(this).prop("checked")) {
          value = $(this).next('label').text();
          return false;
        }
      });
      return value;
    };

    /*
     * Get select list human values.
     */
    this.selectListHumanValues = function (selections, element) {
      var self = this;
      var humanVals = [];
      if (selections instanceof Array) {
        $.each(selections, function (i, v) {
          humanVals.push(self.selectListHumanValue(v, element));
        });
      }
      else if (parseInt(selections)) {
        humanVals.push(self.selectListHumanValue(selections, element));
      }
      return (humanVals.length > 0) ? humanVals.join(', ') : false;
    };

    /*
     * Get a single human value from a select list.
     */
    this.selectListHumanValue = function (selection, element) {
      var value = false;
      if (/^\d+$/.test(selection)) {
        value = $('[value="' + selection + '"]', element).text();
      }
      return value;
    };
    // Initiate the plugin.
    this.init();
  }

  /*
   * Extend jQuery with our search form falues function.
   */
  $.fn.searchFormValues = function (settings) {
    var defaults = {
      widgets: ['.views-exposed-widget'],
      containerClass: 'search-form-values-print',
    };
    settings = $.extend(defaults, settings);
    return this.each(function (i, el) {
      new searchFormValues(el, settings);
    });
  };

})(jQuery);
