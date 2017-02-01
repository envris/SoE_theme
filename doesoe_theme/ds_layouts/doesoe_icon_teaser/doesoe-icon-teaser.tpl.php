<?php
/**
 * @file
 * Icon teaser template file.
 */
?>
<<?php print $layout_wrapper; print $layout_attributes; ?> class="ds-icon-teaser <?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <<?php print $side_wrapper ?> class="ds-icon-teaser__side <?php print $side_classes; ?>">
    <i class="ds-icon-teaser__content-type-icon"></i>
    <?php print $side; ?>
  </<?php print $side_wrapper ?>>

  <<?php print $main_content_wrapper ?> class="ds-icon-teaser__content <?php print $main_content_classes; ?>">
    <?php print $main_content; ?>
    <div class="ds-icon-teaser__below-content">
      <?php print $below_content; ?>
    </div>
  </<?php print $main_content_wrapper ?>>

</<?php print $layout_wrapper ?>>

<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
