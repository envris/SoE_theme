<?php

/**
 * @file
 * Display Suite 1 column template.
 */
?>
<div class="paragraph-iframe">
  <<?php print $ds_content_wrapper; print $layout_attributes; ?> class="ds-1col <?php print $classes;?> clearfix">

  <div class="paragraph-iframe__iframe">
    <?php if (isset($iframe)): ?>
      <iframe style="<?php print render($iframe_style);?>"
              src="<?php print render($iframe);?>" allowFullScreen
              mozAllowFullScreen webkitAllowFullScreen>
      </iframe>
    <?php endif; ?>
  </div>

  <div class="paragraph-iframe__link">
    <?php if (isset($iframe_link)): ?>
      <?php print render($iframe_link); ?>
    <?php endif; ?>
  </div>


    <?php print $ds_content; ?>
  </<?php print $ds_content_wrapper ?>>

  <?php if (!empty($drupal_render_children)): ?>
    <?php print $drupal_render_children ?>
  <?php endif; ?>
</div>
