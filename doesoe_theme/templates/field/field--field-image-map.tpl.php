<?php
/**
 * @file
 * Turn a image map field into image map markup.
 */
?>
<map class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php foreach ($items as $delta => $item): ?>
      <?php print render($item); ?>
    <?php endforeach; ?>
</map>
