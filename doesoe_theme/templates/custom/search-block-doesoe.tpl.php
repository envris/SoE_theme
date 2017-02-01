<?php
/**
 * @file
 * Displays the doesoe search form block.
 *
 * This form emulates the core drupal search block markup for use with a custom
 * views search.
 */
?>
<div class="<?php print $classes; ?>">
  <div class="block__content">
    <?php print render($search_form); ?>
  </div>
</div>
