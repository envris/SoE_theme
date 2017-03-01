<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 *
 * @see https://drupal.org/node/1728148
 */
?>

<div class="header__wrapper">
  <header class="header <?php print $header_class;?>" role="banner">
    <h1 id="site-name"><?php print $site_name; ?></h1>
    <div class="header__branding">
      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="header__logo" ><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="header__logo-image" /></a>
      <?php endif; ?>
    </div>
    <div class="header__content">
      <?php print render($page['header']); ?>
      <?php print $search_block; ?>
      <button class="search-mobile-button icon-search" title="search"></button>
      <button class="menu-mobile-button" role="menu" title="main menu">
        <span></span>
        <span></span>
        <span></span>
      </button>
    </div>
  </header>
</div>

<div class="main">

  <div class="main__nav">
    <div class="layout__sidebar-first sidebar__first">
      <?php print render($page['sidebar_first']); ?>
    </div>
  </div>

  <div class="main__content">

    <div class="content-feature">
      <?php if (!$is_front): ?>
        <div class="hero">
          <?php if ($hero): ?>
            <div class="hero__image">
              <?php print render($hero); ?>
            </div>
          <?php endif; ?>
          <?php if ($title || $subtitle): ?>
            <div class="hero__body">
              <?php print render($title_prefix); ?>
              <?php if ($title): ?>
                <h1 class="content__title hero__title"><?php print $title; ?></h1>
              <?php endif; ?>
              <?php print render($title_suffix); ?>
              <?php if ($subtitle): ?>
                <div class="hero__subtitle">
                  <?php print $subtitle; ?>
                </div>
              <?php endif; ?>
              <?php
                $hero_region = render($page['hero_region']);
              ?>
              <?php if ($hero_region): ?>
                <div class="hero__region">
                  <?php print $hero_region; ?>
                </div>
              <?php endif; ?>
            </div>
          <?php endif; ?>
        </div>
      <?php endif; ?>
      <?php print render($page['content_feature']); ?>
    </div>

    <?php $content_top = render($page['content_top']); ?>
    <?php if ($content_top): ?>
      <div class="content-top__wrapper">
        <?php print render($content_top); ?>
      </div>
    <?php endif; ?>

    <div class="layout__content-wrapper">
      <?php print render($page['banner']); ?>

      <div class="layout__content" role="main">
        <a id="main-content"></a>
        <?php print $messages; ?>
        <?php print render($tabs); ?>
        <?php if ($action_links): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
        <?php print render($page['content']); ?>
        <div id="citation" class="element-invisible">
          <?php if (!empty($citation)): ?>
            <?php print $citation; ?>
          <?php endif; ?>
        </div>
        <?php print $feed_icons; ?>
      </div>

      <?php $sidebar_second = render($page['sidebar_second']); ?>
      <?php if ($sidebar_second): ?>
        <div class="layout__sidebar-second sidebar__second">
          <div class="sidebar__second__inner">
            <div class="sidebar__second__content">
              <?php print $sidebar_second; ?>
            </div>
            <div class="sidebar__second__footer">
              <?php print render($page['sidebar_second_footer']); ?>
            </div>
          </div>
        </div>
      <?php endif; ?>
    </div>

  </div>

</div>

<div class="footer">
  <?php
  $footer_top = render($page['footer_top']);
  $footer_first = render($page['footer_first']);
  $footer_second = render($page['footer_second']);
  $footer_bottom = render($page['footer_bottom']);
  ?>
  <?php if ($footer_top): ?>
    <div class="footer__top">
      <?php print $footer_top; ?>
    </div>
  <?php endif; ?>
  <?php if ($footer_first || $footer_second): ?>
    <div class="footer__content">
      <?php if ($footer_first): ?>
        <div class="footer__first">
          <?php print $footer_first; ?>
        </div>
      <?php endif; ?>
      <?php if ($footer_second): ?>
        <div class="footer__second">
          <?php print $footer_second; ?>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>
  <?php if ($footer_bottom): ?>
    <div class="footer__bottom">
      <?php print $footer_bottom; ?>
    </div>
  <?php endif; ?>
</div>

<div id="offscreen">
  <?php $popup = render($page['popup']); ?>
  <?php if ($popup): ?>
    <div id="popup-content">
      <?php print render($popup); ?>
    </div>
  <?php endif; ?>
</div>


<div id="outdated"></div>
