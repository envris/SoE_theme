<?php
/**
 * @file
 * Returns the HTML for a single Drupal page while offline.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728174
 */
?><!DOCTYPE html>
<!--[if IEMobile 7]><html class="iem7" <?php print $html_attributes; ?>><![endif]-->
<!--[if lte IE 6]><html class="lt-ie9 lt-ie8 lt-ie7" <?php print $html_attributes; ?>><![endif]-->
<!--[if (IE 7)&(!IEMobile)]><html class="lt-ie9 lt-ie8" <?php print $html_attributes; ?>><![endif]-->
<!--[if IE 8]><html class="lt-ie9" <?php print $html_attributes; ?>><![endif]-->
<!--[if (gte IE 9)|(gt IEMobile 7)]><!--><html <?php print $html_attributes; ?>><!--<![endif]-->

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>

  <?php if ($default_mobile_metatags): ?>
    <meta name="MobileOptimized" content="width">
    <meta name="HandheldFriendly" content="true">
    <meta name="viewport" content="width=device-width">
  <?php endif; ?>
  <meta http-equiv="cleartype" content="on">

  <?php print $styles; ?>
  <?php print $scripts; ?>
  <?php if ($add_respond_js): ?>
    <!--[if lt IE 9]>
    <script src="<?php print $base_path . $path_to_zen; ?>/js/html5-respond.js"></script>
    <![endif]-->
  <?php elseif ($add_html5_shim): ?>
    <!--[if lt IE 9]>
    <script src="<?php print $base_path . $path_to_zen; ?>/js/html5.js"></script>
    <![endif]-->
  <?php endif; ?>
</head>
<body class="<?php print $classes; ?> static-404" <?php print $attributes;?>>


<div class="header__wrapper">
  <header class="header " role="banner">
    <div class="header__branding">
      <a href="/" title="Home" rel="home" class="header__logo" ><img src="<?php print $theme_path; ?>/images/logo-white.png" alt="Home" class="header__logo-image" /></a>
    </div>
  </header>
</div>

<div class="main">

  <div class="main__content">

    <div class="content-feature">
      <h1>We'll be back soon</h1>
    </div>

    <div class="layout__content-wrapper" style="text-align: center; font-size: 1.2em">
      <?php print $content; ?>
    </div>

  </div>

</div>

<div class="footer">
  <div class="footer__content">
    <div class="footer__first">
      <div class="region region-footer-first">
        <div id="block-bean-footer-freetext" class="block block-bean contextual-links-region first last odd bean--footer-freetext footer-freetext">

          <div class="block__content">

            <div class="field field-name-field-bean-body field-type-text-long field-label-hidden"><div class="field-items"><div class="field-item even"><p><img src="<?php print $theme_path; ?>/images/logo-footer.png" /></p>
                  <p>The Australia state of the environment 2016 report was prepared by an independent committee using a range of best available information to support assessments of environmental condition, pressures, management effectiveness, resilience, risks and outlooks.</p>
                </div></div></div>  </div>

        </div>
      </div>
    </div>
    <div class="footer__second">
      <div class="region region-footer-second">
        <div id="block-bean-subscribe-box-text" class="block block-bean contextual-links-region first odd bean--subscribe-box-text">

          <div class="block__content">

            <div class="field field-name-field-bean-body field-type-text-long field-label-hidden"><div class="field-items"><div class="field-item even"><p>STAY UP TO DATE WITH THE STATE OF ENVIRONMENT</p>
                </div></div></div>  </div>

        </div>
        <div id="block-bean-subscribe-box-form" class="block block-bean contextual-links-region even bean--subscribe-box-form">

          <div class="block__content">

            <div class="field field-name-field-bean-body field-type-text-long field-label-hidden"><div class="field-items"><div class="field-item even"><form action="//environment.us14.list-manage.com/subscribe/post?u=69945d53ab5d3c5cdddaee231&amp;id=e842d25793" method="post" name="mc-embedded-subscribe-form" id="mc-embedded-subscribe-form" target="_blank" novalidate>
                    <label for="mce-EMAIL" class="element-invisible">Email Address</label>
                    <input type="text" placeholder="Your email address..." value="" name="EMAIL" id="mce-EMAIL" class="form-text">
                    <div style="position: absolute; left: -5000px;" aria-hidden="true">
                      <input type="text" value="" name="b_69945d53ab5d3c5cdddaee231_e842d25793" tabindex="-1">
                    </div>
                    <div class="form-actions">
                      <input type="submit" value="Subscribe" name="subscribe" id="mc-embedded-subscribe" class="form-submit">
                    </div>
                  </form></div></div></div>  </div>

        </div>
      </div>
    </div>
  </div>
  <div class="footer__bottom">
    <div class="region region-footer-bottom">
      <div id="block-bean-acknowledgment-of-country" class="block block-bean contextual-links-region first odd bean--acknowledgment-of-country">


        <div class="block__content">

          <div class="field field-name-field-bean-body field-type-text-long field-label-hidden"><div class="field-items"><div class="field-item even"><h4>Acknowledgment of Country</h4>
                <p>The authors acknowledge the traditional owners of Country throughout Australia and their continuing connection to land, sea and community; we pay respect to them and their cultures and to their elders both past and present.</p>
              </div></div></div>  </div>

      </div>
      <div id="block-bean-copyright" class="block block-bean contextual-links-region last even bean--copyright">

        <div class="block__content">

          <div class="field field-name-field-bean-body field-type-text-long field-label-hidden"><div class="field-items"><div class="field-item even"><p>© Australia State of the Environment 2016</p>
              </div></div></div>  </div>

      </div>
    </div>
  </div>
</div>

<?php print $bottom; ?>

</body>
</html>
