/**
 * @file
 * Homepage Slideshow.
 *
 * Functionality for the homepage slideshow.
 * - Change img to bg image
 * - Add next teaser and pause button
 * - Add a scroll down to content button
 * - Start slick with provided options.
 *
 * Author: jeremy@doghouse.agency
 */

(function ($) {

  "use strict";

  var homeSlideshow = function(dom, settings) {
    var self = this,
      $self = $(dom);

    // Slick Settings.
    self.defaults = {
      autoplay: false,
      accessibility: true,
      dots: true,
      slidesToShow: 1,
      slidesToScroll: 1,
      slide: '.views-row',
      infinite: true,
      speed: 1000,
      autoplaySpeed: 8000,
      arrows: true,
      pauseOnHover: false,
      fade: true,
      cssEase: 'linear',
      prevArrow: '<button type="button" class="slick-prev" title="Previous slide"><i class="icon-arrow-left"></i></button>',
      nextArrow: '<button type="button" class="slick-next" title="Next slide"><i class="icon-chevron-right"></i></button>'
    };
    self.settings = $.extend(self.defaults, settings);

    // The slides.
    self.$slides = $(self.settings.slide, $self);

    /*
     * Initialize.
     */
    self.init = function () {
      self.convertImgToBg();
      self.addPauseButton();
      self.nextTeaser();
      self.scrollDownButton();
      // Init Slick.
      $self.slick(self.settings);
      // Start Slick.
      $self.slick('slickPlay');
    };

    /*
     * For dynamic scale and crop, change the image to a background image.
     */
    self.convertImgToBg = function() {
      self.$sliderImg = $('.slider__image', $self);
      self.$sliderImg.each(function(i, d){
        var $img = $(d).find('img');
        $(d).css('backgroundImage', 'url(' + $img.attr('src') + ')');
        $img.hide();
      });
    };

    /*
     * Add a pause button.
     */
    self.addPauseButton = function() {
      $('<button>')
        .addClass('slider__toggle-slideshow')
        .attr('title', 'Pause slideshow')
        .append('<span>')
        .click(function(e) {
          var $el = $(this), action, text;
          $el.toggleClass('paused');
          action = $el.hasClass('paused') ? 'slickPause' : 'slickPlay';
          text = $el.hasClass('paused') ? 'Resume' : 'Pause';
          $el.attr('title', text + ' slideshow');
          $self.slick(action);
        })
        .prependTo($self);
    };

    /*
     * Add 'Next item' Teaser.
     */
    self.nextTeaser = function() {
      var nextSlideIdx, $nextSlide, $nextTeaser, $nextTeaserImg,
        $nextTeaserTitle, nextTitle, component = 'slider__next-teaser';
      self.$slides.each(function(i, d) {
        // Get the next slide.
        nextSlideIdx = (i + 1) === self.$slides.length ? 0 : (i + 1);
        $nextSlide = self.$slides.eq(nextSlideIdx);
        nextTitle = $nextSlide.find('.slider__title').text();
        // Build the next slide teaser.
        $nextTeaserImg = $('<div>')
          .addClass(component + '__img')
          .css('backgroundImage', 'url(' + $nextSlide.find('img').attr('src') + ')');
        $nextTeaserTitle = $('<div>')
          .addClass(component + '__title')
          .html('<span>Next</span><h4>' + nextTitle + '</h4>');
        $nextTeaser = $('<button>')
          .addClass(component)
          .prepend($nextTeaserImg)
          .append($nextTeaserTitle)
          .attr('title', 'Next slide: ' + nextTitle)
          .click(function(e){
            $self.slick('slickNext');
          });
        $(d).append($nextTeaser);
      });
    };

    /*
     * Scroll down to content button.
     */
    self.scrollDownButton = function() {
      // NOTE: If this is used elsewhere, abstract it.
      $('<button>')
        .addClass('slider__scroll-next')
        .html('<span>Scroll down</span>')
        .attr('title', 'Scroll down')
        .click(function(e) {
          $('html, body').animate({
            scrollTop: $('.layout__content').offset().top - ($('.header__content').height() - 5)
          }, 500);
        })
        .appendTo($self);
    };

    /*
     * Initialize the class.
     */
    self.init();

  };

  /**
   * Expose this class as a jQuery plugin.
   */
  $.fn.homeSlideshow = function (settings) {
    window.homeSlideshows = window.homeSlideshows || [];
    settings = settings || {};
    return this.each(function (i, dom) {
      window.homeSlideshows.push(
        new homeSlideshow(dom, settings)
      );
    });
  };

})(jQuery);
