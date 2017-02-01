<?php
/**
 * @file
 * Link teaser template file.
 */
?>
<a href="<?php print url('node/' . $node->nid); ?>" <?php print $layout_attributes; ?> class="ds-link-teaser <?php print $classes;?> clearfix">

  <span class="ds-link-teaser__side <?php print $side_classes; ?>">
    <i class="ds-link-teaser__content-type-icon"></i>
    <?php print $side; ?>
  </span>

  <span class="ds-link-teaser__content <?php print $main_content_classes; ?>">
    <?php print $main_content; ?>
  </span>

</a>

<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
