<?php

/**
 * @file
 * Display Suite 1 column template.
 */
?>
<<?php print $ds_content_wrapper; print $layout_attributes; ?> class="ds-1col key-finding-headline <?php print $classes;?> clearfix">

  <?php if (isset($icon)) : ?>
    <div class="key-finding-headline__icon">
      <?php print render($icon); ?>
    </div>
  <?php endif; ?>

  <div class="key-finding-headline__content">
    <?php print $ds_content; ?>
  </div>

</<?php print $ds_content_wrapper ?>>
