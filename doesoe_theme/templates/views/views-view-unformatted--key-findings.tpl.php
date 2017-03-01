<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<div class="view-group row-palette--<?php print (isset($row_palette) ? $row_palette : ''); ?>">
  <?php if (!empty($title)): ?>
    <h3 class="view-group__title">
      <?php print $title; ?>
      <span class="cite">
        <a href="#" data-citation-src="#citation" data-citation-content="<?php print (isset($cite_text) ? trim($cite_text) : ''); ?>" data-citation-palette="<?php print (isset($row_palette) ? $row_palette : ''); ?>" title="Cite this content">
          <i class="icon-ribbon"></i>Cite</a>
      </span>
    </h3>
  <?php endif; ?>
  <div class="view-group__content">
    <?php foreach ($rows as $id => $row): ?>
      <div <?php if ($classes_array[$id]): print "class=\"{$classes_array[$id]}\""; endif;?>>
        <?php print $row; ?>
      </div>
    <?php endforeach; ?>
  </div>
</div>
