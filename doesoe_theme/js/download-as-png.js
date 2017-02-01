/**
 * @file
 * A JavaScript file for the theme.
 */
(function ($, Drupal, window, document, undefined) {

  /*
   * Download image rather than opening in a new tab.
   */
  Drupal.behaviors.downloadAsPng = {

    // Download button.
    target: '.download-as-png',

    attach: function(context, settings) {
      var self = this;
      $(this.target, context).click(function(e) {
        e.preventDefault();
        self.savePNG($(this).attr('href'));
      });
    },

    /*
     * Save as PNG.
     */
    savePNG: function (imgSrc) {
      // Create a new image and add the src.
      var image = new Image();
      var self = this;
      image.src = imgSrc;

      image.onload = function () {
        var canvas = document.createElement('canvas'), context, dimensions,
          filename = imgSrc.replace(/^.*[\\\/]/, '');
        dimensions = self.getDimensions(image);
        canvas.width = dimensions.width;
        canvas.height = dimensions.height;
        context = canvas.getContext('2d');
        context.drawImage(image, 0, 0, dimensions.width, dimensions.height);
        canvas.toBlob(function (blob) {
          //filesaver.js
          saveAs(blob, filename);
        });
      };
    },

    /*
     * Return the dimensions object.
     */
    getDimensions: function (image) {
      return {width: image.width, height: image.height}
    }
  };

})(jQuery, Drupal, this, this.document);
