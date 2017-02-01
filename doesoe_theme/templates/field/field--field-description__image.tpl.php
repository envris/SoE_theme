<?php
/**
 * @file
 * Adjust image description field markup to avoid unnecessary wrappers.
 */
?>
<span class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php foreach ($items as $delta => $item): ?>
      <?php print render($item); ?>
    <?php endforeach; ?>
</span>
