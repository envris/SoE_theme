<?php
/**
 * @file
 * Display Suite 1 column template for Key findings teaser.
 */
?>

<<?php print $ds_content_wrapper; print $layout_attributes; ?> class="ds-1col <?php print $classes;?> clearfix key-finding">

<?php if (isset($icon)) : ?>
  <div class="key-finding-teaser__icon">
    <?php print render($icon); ?>
  </div>
<?php endif; ?>

<div class="key-finding-teaser__content">
  <?php print $ds_content; ?>
</div>

</<?php print $ds_content_wrapper ?>>
