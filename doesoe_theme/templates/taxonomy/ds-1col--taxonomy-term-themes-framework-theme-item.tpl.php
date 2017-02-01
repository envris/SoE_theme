<?php

/**
 * @file
 * Display Suite 1 column template.
 */
?>
<<?php print $ds_content_wrapper; print $layout_attributes; ?> class="ds-1col <?php print $classes;?> clearfix">

<?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
<?php endif; ?>

<?php if (isset($icon)) :?>
  <div class="theme-framework_theme_item__side">
    <div class="theme-framework_theme_item__icon"><?php print drupal_render($icon); ?></div>
  </div>
<?php endif; ?>
<div class="theme-framework_theme_item__content">
  <?php print $ds_content; ?>
  <?php if (isset($glance)) :?>
    <div class="theme-framework_theme_item__at-a-glance"><?php print $glance; ?></div>
  <?php endif; ?>
  <?php if (isset($assessment_summaries_link)) :?>
    <div class="theme-framework_theme_item__link"><?php print $assessment_summaries_link; ?></div>
  <?php endif; ?>
  <?php if (isset($topics_link)) :?>
    <div class="theme-framework_theme_item__link"><?php print $topics_link; ?></div>
  <?php endif; ?>
</div>

</<?php print $ds_content_wrapper ?>>

<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
