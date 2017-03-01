/**
 * @file
 * A JavaScript file for the admin theme.
 */
(function ($, Drupal, window, document, undefined) {

  /*
   * De-clutter text areas by hiding guidelines behind a button.
   */
  Drupal.behaviors.inputFilterDescToggle = {
    attach: function (context, settings) {

      // For each guidelines, hide tips and replace with a link to show again.
      $('.filter-guidelines-item', context).once('guidelines-toggle').each(function (i, d) {
        var $container = $(d);
        $container.find('.tips').hide();
        $('<a>')
          .html('Show ' + $container.find('h3').html() + ' guidelines')
          .click(function (e) {
            $(this).closest('.filter-guidelines-item').find('.tips').show();
            $(this).hide();
          })
          .appendTo($container);
      });

      $('.views-csv-export')
        .find('.view-content .views-table:first').attr('id', 'views-table-first').end()
        .prepend('<a href="#" download="' + $(document).find('title').text() + '.csv" onclick="return ExcellentExport.csv(this, \'views-table-first\')">Download CSV</a>')
    }
  };

})(jQuery, Drupal, this, this.document);
