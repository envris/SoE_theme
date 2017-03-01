<?php

/**
 * @file
 * Override unformatted display to apply correct markup and classes for ass sum.
 *
 * Changes from default:
 * - Wrapper added.
 * - Title is modified to have the correct element and wrapper.
 */
?>
<div class="ass-sum__component">
  <?php if (!empty($title)): ?>
    <div class="ass-sum__component__title">
      <h4><?php print $title; ?></h4>
      <?php if (!empty($known_as)): ?>
        <?php print $known_as; ?>
      <?php endif; ?>
    </div>
  <?php endif; ?>
  <?php foreach ($rows as $id => $row): ?>
    <div <?php if ($classes_array[$id]): print "class=\"{$classes_array[$id]}\""; endif;?>>
      <?php print $row; ?>
    </div>
  <?php endforeach; ?>
</div>
