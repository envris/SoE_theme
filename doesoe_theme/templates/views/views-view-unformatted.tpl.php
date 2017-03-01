<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 *
 * Changes from default: If content is grouped (ie. has a title) then an
 * additional wrapper is added around the rows.
 */
?>
<?php if (!empty($title)): ?>
<div class="view-group">
  <h3 class="view-group__title"><?php print $title; ?></h3>
  <div class="view-group__content">
<?php endif; ?>
  <?php foreach ($rows as $id => $row): ?>
    <div <?php if ($classes_array[$id]): print "class=\"{$classes_array[$id]}\""; endif;?>>
      <div class="views-row-inner"><?php print $row; ?></div>
    </div>
  <?php endforeach; ?>
<?php if (!empty($title)): ?>
  </div>
</div>
<?php endif; ?>
