<?php
/**
 * @file
 * Content two-col template file.
 */
?>
<<?php print $layout_wrapper; print $layout_attributes; ?> class="ds-content__two-col <?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <<?php print $col_1_wrapper ?> class="ds-content__two-col__col-1 <?php print $col_1_classes; ?>">
    <?php print $col_1; ?>
  </<?php print $col_1_wrapper ?>>

  <<?php print $col_2_wrapper ?> class="ds-content__two-col__col-2 <?php print $col_2_classes; ?>">
    <?php print $col_2; ?>
  </<?php print $col_2_wrapper ?>>

</<?php print $layout_wrapper ?>>

<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
