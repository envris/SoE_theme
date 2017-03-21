/**
 * @file
 * A JavaScript file for the theme.
 */

(function ($, Drupal, window, document, undefined) {

  /*
   * Old browser check.
   *
   * See: https://github.com/burocratik/Outdated-Browser/tree/master
   */
  Drupal.behaviors.outdatedBrowser = {
    attach: function(context, settings) {
      outdatedBrowser({
        bgColor: '#f25648',
        color: '#ffffff',
        // Less than IE9, change to 'transform' for less than IE10.
        lowerThan: 'boxShadow',
        languagePath: '/sites/default/themes/doesoe_theme/libraries/outdatedbrowser/outdatedbrowser.html'
      })
    }
  };

  /*
   * Attach datatables.js to ckan tables.
   */
  Drupal.behaviors.ckanDataTables = {
    attach: function (context, settings) {

      // Responsive DataTables.
      $('table.govcms-ckan-table', context)
        .once('ckan-datatables')
        .attr('width', '100%')
        .DataTable({
          responsive: true,
          order: [],
          language: {
            "search": "Table search:"
          },
          // Only enable tab index on thead th and pagination.
          tabIndex: -1,
          headerCallback: function(thead, data, start, end, display) {
            $('th', thead).attr('tabindex', 0);
          },
          drawCallback: function(settings) {
            $('.paginate_button', settings.nTableWrapper).attr('tabindex', 0);
          }
        });
    }
  };

  /*
   * Makes the header region transparent when scrollbar is at the very top.
   * It also makes the header opaque when user hovers to the header region.
   */
  Drupal.behaviors.headerTransparent = {
    attach: function (context, settings) {

      var $header = $('header'),
        $searchBlock = $('.search-header-block', $header),
        $txtSearch = $('.form-text', $searchBlock);

      // Determines if we are in the home page or not.
      function isFront() {
        return $('body').hasClass('front') ? true : false;
      }

      // Determines if the header is transparent.
      function isHeaderTransparent() {
        return $('header').hasClass('is-transparent') ? true : false;
      }

      // Determines if the scrollbar is at the very top.
      function isPageOnTop() {
        return ($(document).scrollTop() < 1) ? true : false;
      }

      // Determines if the search box has been activated.
      function isSearchActive() {
        return $('.search-header-block').hasClass('is-active') ? true : false;
      }

      // Sets the header transparent and resets the search box to inactive state.
      function setHeaderTransparent() {
        $header.addClass('is-transparent');
        $searchBlock.removeClass('is-active');
        $txtSearch.blur();
      }

      // We add a is-transparent class when the scrollbar is at the very top.
      $(window).scroll(function () {
        if (!isFront()) {
          return;
        }

        if (isPageOnTop()) {
          setHeaderTransparent();
        }
        else {
          $header.removeClass('is-transparent');
        }
      });

      // Makes the header opaque on mouse hover.
      $header.hover(function () {
        if (isHeaderTransparent() && isFront()) {
          $header.removeClass('is-transparent');
        }
      }, function () {
        if (isPageOnTop() && !isHeaderTransparent() && isFront() && !isSearchActive()) {
          setHeaderTransparent();
        }
      });

    }
  };

  /*
   * Menu mobile button.
   */
  Drupal.behaviors.menuMobileButton = {
    attach: function (context, settings) {
      $('.menu-mobile-button').click(function () {
        $('body').toggleClass('menu-open');
      });
    }
  };

  /*
   * Process assessment summaries into visualisations.
   */
  Drupal.behaviors.assessmentSummaryVisualisations = {
    attach: function (context, settings) {

      $('.paragraphs-item-assessment-summary', context)
        .once('assessment-summary-vis')
        .AssessmentSummaryVisualisation();
    }
  };

  /*
   * Make assessment summary headers sticky.
   */
  Drupal.behaviors.assessmentSummaryStickyHeader = {
    attach: function (context, settings) {

      $('.ass-sum--view .ass-sum__component-data', context)
        .once('ass-sum-sticky-header')
        .stickyHeaders({
          headerSelector: '.ass-sum__header',
          parentContainer: '.ass-sum--view'
        });

    }
  };

  /*
   * Make the Historic banner sticky.
   */
  Drupal.behaviors.historicBannerSticky = {
    attach: function (context, settings) {

      $('.layout__content-wrapper', context)
        .once('historic-banner-sticky')
        .stickyHeaders({
          headerSelector: '#block-bean-historic-banner',
          parentContainer: '.main__content'
        });

    }
  };

  /*
   * Make sidebar second sticky.
   */
  Drupal.behaviors.sidebarSecondSticky = {
    attach: function (context, settings) {

      $('.layout__content-wrapper', context)
        .once('sbs-sticky')
        .stickyHeaders({
          headerSelector: '.sidebar__second__inner',
          parentContainer: '.sidebar__second',
          inheritParentBottom: true,
          minWidth: 992
        });

    }
  };

  /*
   * Hero image CSS cropping.
   */
  Drupal.behaviors.heroCssCrop = {
    attach: function (context, settings) {

      // Turn hero image into background image then hide image tag.
      $('.hero', context)
        .once('hero-crop', function(){
          var $img = $(this).find('img');
          $(this).css('background-image', 'url("' + $img.attr('src') + '")');
          $img.hide();
        });
    }
  };

  /*
   * Watch for changes that indicate historical content is being viewed.
   * This is used on ajax views to toggle the historical content banner.
   *
   * If this is to work with any other ajax views, ensure the year filter is called "year".
   */
  Drupal.behaviors.historicContentToggle = {
    attach: function (context, settings) {
      var self = this,
        $body = $('body'),
        currentYear = String(settings.doeSoe.currentYearTid),
        $yearSelect = $('.views-exposed-form #edit-year', context);

      // If we have a year filter and not a search page (which handles this in html.preprocess).
      if ($yearSelect.length > 0 && !$body.hasClass('search-historic')) {
        // If year hasn't been forced (in php) then auto set on page load.
        if (!$body.hasClass('force-historic')) {
          self.historicContent($yearSelect, currentYear);
        }
        // Bind to year change in select box.
        $yearSelect.change(function(e) {
          self.historicContent($(this), currentYear);
        });
      }
    },

    // Toggle historic content class on body.
    historicContent: function($yearSelects, currentYear) {
      var $body = $('body'),
          val = $yearSelects.val();

      // If multi select and current year isn't in array,
      val = $.isArray(val) && $.inArray(currentYear, val) === -1 ? val[0] : val;
      // Check if historic should be applied.
      if (!$.isArray(val) && val !== null && val !== 'All' && val !== currentYear) {
        $body.addClass('historic-content');
      } else {
        $body.removeClass('historic-content');
      }

    }
  };

  /*
   * Assessment summary supporting data link.
   */
  Drupal.behaviors.assessmentSummarySupportingData = {
    attach: function(context, settings) {

      $('.ass-sum__item__metadata', context)
        .once('ass-sum-metadata', function (){

          // Define els we will use.
          var $parent = $(this).closest('.ass-sum__item'),
            $summary = $parent.find('.ass-sum__item__summary'),
            $link = $('<a />').attr('href', '#');

          // Link click callback.
          $link
            .addClass('ass-sum__item__metadata-link')
            .html('Supporting information')
            .magnificPopup({
              items: {
                src: $(this).find('.entity').html(),
                type: 'inline'
              },
              callbacks: {
                open: function () {
                  // Call behaviours on added content.
                  Drupal.attachBehaviors($('.mfp-content'));
                }
              }
            });

          // Add link to summary.
          $summary.append($link);
        });
    }
  };

  /*
   * Assessment summary topics link.
   */
  Drupal.behaviors.assessmentSummaryTopics = {
    attach: function(context, settings) {

      $('.ass-sum__topics', context)
        .once('ass-sum-topics', function () {

          // Define els we will use.
          var $parent = $(this).closest('.views-row'),
              $summary = $parent.find('.ass-sum__item__summary'),
              $link = $('<a />').attr('href', '#'),
              classes = ['ass-sum__topics-popup', 'theme-palette-inherit-headings__dark', 'theme-palette-inherit-links'];

          // Link click callback.
          $link
            .addClass('ass-sum__item__topics-link')
            .html('Topics')
            .magnificPopup({
              items: {
                src: '<div class="' + classes.join(' ') + '">' + $(this).html() + '</div>',
                type: 'inline'
              },
              callbacks: {
                open: function () {
                  // Call behaviours on added content.
                  Drupal.attachBehaviors($('.mfp-content'));
                }
              }
            });

          // Add link to summary.
          $summary.append($link);
        });
    }
  };

  /*
   * Enable jquery ui tooltips on items with a title within these containers.
   */
  Drupal.behaviors.toolTips = {
    attach: function(context, settings) {

      // Containers that have elements requiring tooltip enabled.
      var tooltipSelectors = [
        '.tooltip',
        '.ass-sum__item__grade',
        '.ass-sum-vis__confidence',
        '.ass-sum-vis__compare',
        'abbr[title]',
        '.category-with-icon a',
        '.menu-footer-primary .menu__link',
        '.menu-name-main-menu .menu__link',
        '.menu-footer-secondary .menu__link'
        ];

      $(tooltipSelectors.join(','), context).once('tooltips', function () {
        var $this = $(this);
        if ($this.attr('title') != '') {
          // Create an element to retrieve the text. This will HTML entities decode so we can use special UTF-8 chars
          // in tooltip text.
          var title = $('<div/>').html($this.attr('title')).text();
          if (title == '') {
            // If this title is empty we should skip.
            return;
          }
          $this.attr('title', title);
          $this.tooltipster();
        }
      });

    }
  };

  /*
   * Assessment Summary Key functionality.
   * If key exists, prepend a block to sidebar that links to key.
   */
  Drupal.behaviors.assSumKey = {
    attach: function(context, settings) {
      $('.ass-sum-key', context)
        .once('ass-sum-key', function () {
          var self = this,
            $sbs = $('.region-sidebar-second', $(this).closest('.main')),
            $moreBlock = $('<div class="block block__as-key-link"></div>'),
            $link = $('<div class="sbs-content-row">View Assessment Summary Key</div>');
          // The link that scrolls to key.
          $link
            // Bind scroll to key event.
            .click(function (e){
              e.preventDefault();
              $('html, body').animate({
                scrollTop: $(self).offset().top
              }, 1000);
            });
          // Build the more block and prepend to sidebar.
          $moreBlock
            .prepend('<h2 class="block__title">On this page</h2>')
            .append($link)
            .prependTo($sbs);
        });
    }
  };

  /*
   * Truncate text and add a read more link.
   */
  Drupal.behaviors.readMoreTruncate = {
    attach: function (context, settings) {

      // Default options overridable with data attributes.
      var self = this, key, dataKey, options = {
        collapsedHeight: 200,
        speed: 100,
        moreLink: self.btnHtml('more', 'more'),
        lessLink: self.btnHtml('less', 'less'),
        beforeToggle: function(trigger, element, expanded) {
          if (expanded) {
            element.addClass('collapsed');
            element.attr('aria-expanded', 'false');
          }
          else {
            element.removeClass('collapsed');
            element.attr('aria-expanded', 'true');
          }

        },
        blockProcessed: function(element, collapsable) {
          if (collapsable) {
            $(element).addClass('collapsed');
          }
        }
      };

      // Generic selector for adding readmore, allowing options to be overridden.
      $('.readmore-truncate', context)
        .each(function (i, d) {
          for (key in options) {
            dataKey = key.toLowerCase();
            if ($(d).data(dataKey) !== undefined) {
              options[key] = $(d).data(dataKey);
            }
          }
          if ($(d).data('moretitle') !== undefined) {
            options.moreLink = self.btnHtml('more', $(d).data('moretitle'));
          }
          $(d).once('readmore-truncate').readmore(options);
        });
    },

    // Make the more/less button html.
    btnHtml: function(op, text) {
      return '<a href="#" class="readmore-toggle readmore-toggle--' + op + '">' + text + '</a>';
    }
  };

  /*
   * Search button.
   */
  Drupal.behaviors.searchButton = {
    attach: function (context, settings) {

      var $btn = $('.search-mobile-button'),
        $txtSearch = $('.search-header-block .form-text'),
        $header = $('.search-header-block'),
        // The max-width of $header when hidden.
        headerInactiveWidth = 60;

      // We add/remove the is-active class when the button is clicked.
      $btn.once('mobile-search').click(function () {
        if (!$header.hasClass('is-active')) {
          $header.addClass('is-active');
          // Wait half a second before focus as it confuses iOS otherwise.
          setTimeout(function(e){
            $txtSearch.focus();
          }, 500);
        }
        else {
          // If there is no search text, close search, if there is submit search.
          if ($txtSearch.val() == '') {
            $header.removeClass('is-active');
          }
          else {
            $btn.parent().find('form').submit();
          }
        }
      });

      $('.main').click(function () {
        if ($header.hasClass('is-active')) {
          $txtSearch.val('');
          $btn.removeClass('is-hidden');
          $header.removeClass('is-active');
        }
      });

      $txtSearch.focus(function () {
        $header.addClass('is-active');
      });

      // Prevent submit (and open search box) when search box hidden.
      $header.find('.form-submit').click(function (e) {
        if ($header.width() <= headerInactiveWidth) {
          e.preventDefault();
          $header.addClass('is-active');
        }
      });

      // We add a class to the search button so we can target it later in css.
      $txtSearch.keyup(function () {
        if (!$(this).val().length > 0) {
          $btn.removeClass('is-hidden');
        }
        else {
          $btn.addClass('is-hidden');
        }
      });
    }
  };

  /*
   * Create the homepage slide show.
   */
  Drupal.behaviors.doeSliderView = {
    attach: function (context, settings) {
      $('.slider-view > .view-content', context).once('slider-view', function () {
        // See jquery.home_slideshow.js for what this does.
        $(this).homeSlideshow();
      });
    }
  };

  /*
   * Attaches citation popup to 'Cite this' links.
   */
  Drupal.behaviors.doeCitationPopup = {
    attach: function (context, settings) {
      var dataKey,
          dataKeyPrefix = 'citation-',
          optionKey,
          options = {
            type: 'inline',
            title: Drupal.t('Cite this'),
            src: null,
            content: null,
            palette: ''
          };

      $('a[data-citation-src]', context).each(function (i, elem) {
        var $elem = $(elem);
        // Allow other options added via data attrs.
        for (optionKey in options) {
          dataKey = dataKeyPrefix + optionKey.toLowerCase();
          if ($elem.data(dataKey) !== undefined) {
            options[optionKey] = $elem.data(dataKey);
          }
        }
        $elem.once('citation-popup').referencePopup(options);
      });
    }
  };

  /*
   * Attaches reference popup to reference links.
   */
  Drupal.behaviors.doeReferencePopup = {
    attach: function (context, settings) {
      $('a[href^="#"]', context)
        .each(function (i, elem) {
          var $elem = $(elem),
              title = $elem.hasClass('footnote') ? Drupal.t('Footnote') : Drupal.t('Reference'),
              src = $elem.attr('href'),
              $src = $(src);
          if ($src.length > 0 && $src.attr('data-reference') !== undefined) {
            $elem.once('reference-popup').referencePopup({
              type: 'inline',
              title: title,
              src: src
            });
          }
          else if (src !== '#') {
            $elem.once('reference-popup').referencePopup({
              type: 'ajax',
              title: title,
              src: '/reference-attachment/' + src.substring(1),
              selector: src + '[data-reference]'
            });
          }
        });
    }
  };

  /*
   * Toggle visibility of advanced exposed filters.
   */
  Drupal.behaviors.doeToggleAdvancedFilters = {
    attach: function (context, settings) {
      $('.views-exposed-widget-top__toggle', context)
        .once('toggle-advanced-filters')
        .click(function () {
          $(this).closest('.views-exposed-form__wrapper').toggleClass('advanced-filters');
        });
    }
  };

  /*
   * Change the action on a form and submit.
   */
  Drupal.behaviors.doeAltAction = {
    attach: function (context, settings) {
      $('.alt-action', context)
        .once('alt-action')
        .click(function () {
          $(this)
            .closest('form')
            .attr('action', $(this).data('action'))
            .attr('method', 'get')
            .submit();
        });
    }
  };

  /*
   * Add loader to dom for specific element click. This does not remove the loader too so it is
   * assumed you are taken to a new page to clear the loader from the dom.
   */
  Drupal.behaviors.doeTriggerLoader = {
    attach: function (context, settings) {
      var selectors = [
        '.alt-action',
        '#views-exposed-form-assessment-summary-components-page .form-submit',
        '#views-exposed-form-site-search-page .form-submit',
      ];

      $(selectors.join(','), context)
        .once('trigger-loader')
        .click(function (event) {
          // This detects if the link was opened in a new window/tab and omits the loader.
          if (event.ctrlKey || event.shiftKey || event.metaKey || event.which === 2) {
            return;
          }
          // This detects if the form had errors and if so, omits the loader.
          if ($(this).closest('form').find('.required.error').length > 0) {
            return;
          }
          $('body').append('<div class="ajax-progress"><div class="throbber"></div></div>');
        });
    }
  };

  /*
   * Dropdown toggle.
   */
  Drupal.behaviors.doeDropdownToggle = {
    attach: function (context, settings) {
      $('.dropdown-wrapper', context)
        .once('dropdown-wrapper')
        .click(function () {
          $(this).toggleClass('open');
        });
    }
  };

  /*
   * Grade selector (search).
   */
  Drupal.behaviors.gradeSelector = {
    attach: function (context, settings) {
      // Target grade element in ass sum exposed form.
      $('#views-exposed-form-assessment-summary-components-page #edit-assessment-grade', context)
        .once('select-to-grade')
        .SelectToGrade()
        .closest('.form-type-select')
        .addClass('select-processed');
    }
  };

  /*
   * Event listener for when a chart has fished being initialised.
   *
   * This adds a 'download data' button using href from the file link. Due to
   * order of execution this cannot be inside a behaviour.
   */
  $(window).on('tableCharts:init:dom', function(e) {
    var $parent = e.el.$actions.closest('.file-ckan'),
      $fileLink = $parent.find('span.file a'),
      $btn = $('<button>');

    // Build the button and add to DOM.
    $btn
      .html('Download data')
      .addClass('download-data')
      .click(function(e) {
        window.open($fileLink.attr('href'));
      })
      .appendTo(e.el.$actions);
  });

  /*
   * Makes the homepage feature teaser's body the same height.
   */
  Drupal.behaviors.doeHomepageFeature = {
    attach: function(context, settings) {
      $('.homepage-feature-full-teaser__body', context)
        .once('homepage-feature-match-height')
        .matchHeight();
    }
  };

  /*
   * Apply table of contents to a topic page.
   */
  Drupal.behaviors.doeTableOfContents = {
    attach: function(context, settings) {
      $('#topic-toc', context)
        .once('toc')

        // Once TOC has been built event.
        .bind('jTocBuilt', function (e) {
          var $self = $(this);
          // If items exist.
          if ($self.find('li').length > 0) {
            // Show block (hidden by default.
            $self.closest('.block').show();
            // Make items toggle.
            $self
              .addClass('sbs-list')
              .sideBarToggle({
                headerSelector: '> li > a',
                contentSelector: '> ul'
              });
          }
          else {
            // No items exist.
            $self.closest('.block').remove();
          }
        })

        // Build the TOC, h3 = sub topic, h4 = component.
        .jtoc({
          content: '.layout__content .node.view-mode-full',
          headings: 'h3,h4',
          scrollOffset: 40
        });
    }
  };

  /*
   * Apply table of contents to a landing page.
   */
  Drupal.behaviors.doeThemeTableOfContents = {
    attach: function(context, settings) {
      // Get TOC settings and add overrides/defaults.
      var tocSettings = $.extend(settings.doeSoe.tocSettings, {scrollOffset: 40});

      $('#theme-toc', context)
        .once('toc')

        // Once TOC has been built event.
        .bind('jTocBuilt', function (e) {
          var $self = $(this);
          // If items exist.
          if ($self.find('li').length > 0) {
            // Show block (hidden by default.
            $self.closest('.block').show();
            // Add down arrow.
            $self.find('> li > a').addClass('sidebar-toggle__no-content');
          }
          else {
            // No items exist.
            $self.closest('.block').remove();
          }
        })

        // Build the TOC, h3 = sub topic, h4 = component.
        .jtoc(tocSettings);
    }
  };

  /*
   * Make select (particularly multiselect) more user friendly.
   */
  Drupal.behaviors.doeMultiSelect = {
    attach: function(context, settings) {

      // Form elements getting fancy select boxes.
      var selectors = [
        '#views-exposed-form-site-search-page select',
        '#views-exposed-form-assessment-summary-components-page select',
        '#views-exposed-form-site-search-region select',
        '#views-exposed-form-topics-page select'
      ];

      // Search page selects.
      $(selectors.join(','), context)
        .not('#edit-assessment-grade')
        .not('#edit-field-natt-reference')
        .once('sumoselect')
        .SumoSelect({
          placeholder: '- Any -',
          csvDispCount: 4
        })
        .closest('.form-type-select')
        .addClass('select-processed');
    }
  };

  /**
   * Add at a glance toggle.
   */
  Drupal.behaviors.atAGlanceToggle = {
    attach: function (context, settings) {

      // Open a modal window with the at a glance text from the theme framework teaser.
      $('.theme-framework_theme_item', context).once('theme-framework-item', function() {
        var $trigger = $('<span><a>At a glance</a></span>'),
          $content = $('.at-a-glance--content', this);

        // Clicking trigger opens the popup.
        $trigger
          .addClass('at-a-glance__label')
          .magnificPopup({
            mainClass: 'at-a-glance__modal',
            items: {
              src: '<div>' + $content.html() + '</div>',
              type: 'inline'
            },
            callbacks: {}
          });

        $content.before($trigger);
      });

    }
  };

  /*
   * Sidebar toggles (like collapsible fieldset)
   */
  Drupal.behaviors.doeSidebarToggles = {
    attach: function(context, settings) {
      var $window = $(window);

      // View blocks have the title/toggle in the header due to dynamic content.
      $('.toggle-group', context)
        .once('toggle-group', function() {
          // Related topics default to open, all others default closed.
          var $el = $(this),
            defaultOpen = $el.hasClass('view-display-id-related_topics_current');
          $el.sideBarToggle({
            headerSelector: '.view-header',
            contentSelector: '.view-content',
            defaultOpen: defaultOpen,
            subSelector: '.sbs-list',
            subHeaderSelector: 'h3',
            subContentSelector: 'ul'
          });

        })

      // Generic toggle blocks (Use for custom content/beans).
      $('.block-bean .sidebar-toggle', context)
        .once('sidebar-toggle')
        .sideBarToggle({
          headerSelector: '> .sidebar-toggle__header',
          contentSelector: '> .sidebar-toggle__content'
        });

      // When a sidebar is toggled, trigger the sticky sidebar resize/scroll.
      $window.on('sideBarToggle:toggle', function() {
        $window.trigger('stickyHeader:scroll');
      });
    }
  };

  /*
   * View group toggles (like collapsible fieldset)
   */
  Drupal.behaviors.doeViewGroupToggles = {
    attach: function(context, settings) {

      // Use view group headings to toggle content visibility.
      $('.view-group-toggle .view-group', context)
        .once('toggle-group', function() {
          $(this).find('> .view-group__title').click(function(e) {
            $(this).toggleClass('open').parent().find('> .view-group__content').toggle();
          });
        });
    }
  };

  /*
   * Make picture elements popup in modal when clicked.
   */
  Drupal.behaviors.responsiveImageModal = {
    attach: function(context, settings) {

      $('picture', context).once('responsive-image-modal', function () {

          var $img = $(this).find('img');
          var modalSrc = $(this).find('source').filter(function(){
            return !$(this).attr('media');
          }).attr('srcset').split(' ')[0];

          $img.magnificPopup({
            items: {
              src: modalSrc,
              type: 'image'
            }
          });
      });
    }
  };

  /*
   * Show description under an image with a pill button toggle.
   */
  Drupal.behaviors.imageDesciptionToggle = {
    attach: function(context, settings) {
      $('.image__description', context).once('image-desc', function(e){
        var $desc = $(this);
        $('<a>')
          .addClass('button--pill')
          .html('Text alternative')
          .attr('title', 'Click to view the textual representation of this image')
          .click(function(e){
            e.preventDefault();
            $desc.removeClass('element-invisible');
            $(this).remove();
          })
          .insertBefore($desc);
      });
    }
  };

  /*
   * NATT toggle checkbox.
   */
  Drupal.behaviors.searchNattCheckbox = {
    attach: function(context, settings) {
      // Turns NATT selectbox into a single checkbox.
      var $ctx = $('#views-exposed-form-site-search-page', context),
        $natt = $('.views-widget-filter-field_natt_reference', $ctx),
        $select = $('select', $natt),
        off = 'All', on = '1', toggle,
        selectClass = $select.val() === on ? 'checked' : '';
      // Bind to label, on click toggle between 2 select options.
      $natt.find('> label').once('natt-select', function(){
        $(this).addClass(selectClass).click(function(e) {
          $(this).toggleClass('checked');
          toggle = ($select.val() === off) ? on : off;
          $select.val(toggle);
        });
      });
    }
  };

  /*
   * Create the search page region map.
   */
  Drupal.behaviors.regionSearchFilter = {
    attach: function(context, settings) {
      var mapSettings = {};
      // Get the region features and extents passed via drupal_add_js().
      if (settings.doesoe_theme !== undefined && settings.doesoe_theme.mapRegionFeatures !== undefined) {
        mapSettings.regionFeatures = settings.doesoe_theme.mapRegionFeatures;
        mapSettings.regionExtents = settings.doesoe_theme.mapRegionExtents;
      }

      mapSettings.extentFilter = $('#edit-extent', context);
      // Init the map on site and ASC search pages.
      var selector = '#views-exposed-form-site-search-region .views-widget-filter-field_region select';
      $(selector, context).once('region-search').searchMap(mapSettings);
    }
  };

  /*
   * Open map component in modal on large screen or new window on small screen.
   */
  Drupal.behaviors.regionSearch = {
    attach: function(context, settings) {

      // The breakpoint for opening a map in a modal.
      var largeBp = 1000;

      $('.map-open', context)
        .once('map-open')
        .magnificPopup({
          disableOn: largeBp,
          type: 'iframe',
          mainClass: 'map__modal',
          removalDelay: 160,
          preloader: true,
          fixedContentPos: true
        });
    }
  };

  /*
   * Assessment summary "Known As" field metadata.
   */
  Drupal.behaviors.assessmentSummaryKnownAs = {

    fields: {
      '.paragraphs-items-field-prev-known-as' : {
        variation: 'prev',
        type: 'as',
        buttonText: Drupal.t('Title changes from previous report')
      },
      '.paragraphs-items-field-now-known-as' : {
        variation: 'now',
        type: 'as',
        buttonText : Drupal.t('Title changes from current report')
      },
      '.field-prev-known-as' : {
        variation: 'prev',
        type: 'asc',
        buttonText: Drupal.t('Component changes from previous report')
      },
      '.field-now-known-as' : {
        variation: 'now',
        type: 'asc',
        buttonText: Drupal.t('Component changes from current report')
      }
    },

    popupContentClass: '.known-as__popup-content',

    popupLinkClass: 'known-as__popup-link',

    attach: function(context, settings) {
      var self = this;
      $.each(this.fields, function(selector, options) {
        $(selector, context).once('known-as', function () {
          var $button = self.applyButton($(this), options);
          self.applyPopup($(this), $button, options);
        });
      });
    },

    applyButton: function (el, options) {
      var $button = $('<a />').attr('href', '#')
          .addClass(this.popupLinkClass)
          .html(options.buttonText);
      el.prepend($button);
      return $button;
    },

    applyPopup: function (el, button, options) {
      var $popupContent = el.find(this.popupContentClass);
      button.magnificPopup({
        items: {
          src: '<div class="known-as__' + options.variation + '-' + options.type + '">' + $popupContent.html() + '</div>',
          type: 'inline'
        }
      });
    }
  };

  /*
   * Toggle references (para ref bundle - teaser).
   */
  Drupal.behaviors.referenceInlineToggleFull = {
    attach: function(context, settings) {
      $('.reference-inline__short', context)
        .once('reference-inline-toggle')
        .click(function(){
          var $self = $(this);
          $self.parent().find('.reference-inline__full').toggle();
        });
    }
  };

  /*
   * Use listnav plugin to add alphabetical filtering to glossaries.
   */
  Drupal.behaviors.listNavGlossary = {
    attach: function(context, settings) {
      $('ul.listnav-list', context)
        .once('listnav-list')
        .listnav({
          filterSelector: '.term--link',
          includeNums: false,
          allText: 'ALL'
        });
    }
  };

  /*
   * Add search form values to the dom to use when printing the search page.
   */
  Drupal.behaviors.printSearchFilters = {
    attach: function(context, settings) {
      var selectors = [
        '#views-exposed-form-topics-page',
        '#views-exposed-form-site-search-page',
        '#views-exposed-form-report-topic-panel-pane-1',
        '#views-exposed-form-key-findings-by-theme-page',
        '#views-exposed-form-assessment-summary-components-page',
        '#views-exposed-form-assessment-summary-per-theme-page-1',
        '#views-exposed-form-executive-summaries-by-theme-panel-pane-1',
        '#views-exposed-form-key-findings-by-theme-and-year-panel-pane-1',
        '#views-exposed-form-framework-intro-by-theme-framework-panel-pane-1',
      ];
      $(selectors.join(','), context)
        .once('print-search-filters')
        .searchFormValues({
          widgets: ['.views-exposed-widget', '.views-exposed-widget-top'],
        });
    }
  };

  /*
   * Homepage frameworks block tab functionality.
   */
  Drupal.behaviors.homepageFrameworksBlock = {
    attach: function(context, settings) {
      var $view = $('.homepage-feature-frameworks', context);
      var $tabs = $('.views-row-tab', $view);
      var $content = $('.homepage-feature-frameworks__content', $view);
      $tabs.once('homepage-frameworks-block', function () {
        $(this).click(function (e) {
          // Add class to selected tab.
          $tabs.removeClass('tab-active');
          $(this).addClass('tab-active');
          // Update content area with selected tabs content.
          var $thisContent = $(this).next('.views-row-content').find('.frameworks-teaser').clone();
          $content.html($thisContent);
        });
        // Active the first tab on load.
        $tabs.eq(0).trigger('click');
      });
    }
  };

  /*
   * Make image maps responsive.
   */
  Drupal.behaviors.responsiveImageMap = {
    attach: function(context, settings) {
      var $imgMaps = $('img[usemap]', context);
      if ($imgMaps.length > 0) {
        $imgMaps.once('responsive-image-map').rwdImageMaps();
      }
    }
  };

  /*
   * region filter show on map functionality.
   */
  Drupal.behaviors.showRegionSearchOnMap = {
    attach: function(context, settings) {
      var exposedForms = [
        '#views-exposed-form-site-search-page',
        '#views-exposed-form-assessment-summary-components-page'
      ];
      $(exposedForms.join(), context).once('show-region-search-on-map', function () {
        var $form = $(this);
        $form.find('.show-on-map').click(function (e) {
          e.preventDefault();
          $form.attr('action', $(this).attr('href')).submit();
        });
      });
    }
  };

  /*
   * High contrast mode.
   */
  Drupal.behaviors.highContrast = {
    attach: function (context, settings) {
      var $contrast_link = $('#high-contrast a#toggle', context),
          $contrast_hide = $('#high-contrast a#hide', context);

      if (localStorage.highcontrast == 'true') {
        $('body').addClass('palette__highcontrast');
        $('*[class*="row-palette__"]').addClass('row-palette__highcontrast');
        $('*[class*="row-palette--"]').addClass('row-palette--highcontrast');
        $contrast_link.html('Disable high contrast mode');
      }

      if ((new Date().getTime() - parseInt(localStorage.getItem('highcontrast.hide'))) < (7 * 24 * 60 * 60 * 1000)) {
        var top = $('.navbar-tray-open').length > 0 ? 67 : 0;
        $('#high-contrast').hide();
        $('.header__wrapper').css('top', top);
      }

      $contrast_link.once('highcontrast', function () {
        $(this).click(function (e) {
          e.preventDefault();
          if (localStorage.highcontrast == 'true') {
            localStorage.removeItem('highcontrast');
            $(this).html('View site in high contrast');
          }
          else {
            localStorage.setItem('highcontrast', 'true');
            $(this).html('Disable high contrast mode');
          }
          $('body').toggleClass('palette__highcontrast');
          $('*[class*="row-palette__"]').toggleClass('row-palette__highcontrast');
          $('*[class*="row-palette--"]').toggleClass('row-palette--highcontrast');
        });
      });

      $contrast_hide.once('highcontrast_hide').bind('click', function(e) {
        e.preventDefault();
        var top = $('.navbar-tray-open').length > 0 ? 67 : 0;
        $(this).parent().hide();
        $('.header__wrapper').css('top', top);
        localStorage.setItem('highcontrast.hide', new Date().getTime());
      });
    }
  };

  /*
   * Functionality for popup window. See popupSettings for configuration.
   */
  Drupal.behaviors.popupModal = {
    attach: function (context, settings) {
      // Define popup settings.
      var popupSettings = {
        // Days before showing again.
        cookieExpiry:   30,
        // Delay in seconds before showing popup.
        delay:          60,
        // Where the popup content is found on the page.
        popUpSelector:  '#popup-content',
        // Key for the cookie storage.
        cookieKey:      'popupCookie'
      };
      // Must have content, to disable survey/popup remove block from popup region.
      var $popupContent = $(popupSettings.popUpSelector, context);
      if ($popupContent.length === 0) {
        return;
      }

      // If the cookie is present we skip. We rely on the browser expiring the cookie
      // by setting the cookies expires.
      if (!!$.cookie(popupSettings.cookieKey)) {
        return;
      }

      // Tests passed. Popup WILL be displayed!
      setTimeout(function(){
        // Open popup.
        $.magnificPopup.open({
          items: {
            src: $popupContent.html(),
            type: 'inline'
          }
        });
        // Set the cookie to mark as shown so we don't see it again for 'cookieExpiry' days.
        var cookieExpires = new Date();
        cookieExpires.setTime(cookieExpires.getTime() + (popupSettings.cookieExpiry * 86400000));
        $.cookie(popupSettings.cookieKey, 1, { path: '/', expires: cookieExpires });
      }, (popupSettings.delay * 1000));
    }
  };

})(jQuery, Drupal, this, this.document);
