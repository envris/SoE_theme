<?php
/**
 * @file
 * Icon element.
 */
?>
<span class="icon icon-<?php print check_plain($icon); ?>"<?php (!empty($title) ? print ' title="' . $title . '"' : ''); ?> aria-hidden="true"></span>
<?php if (!empty($title)) : ?>
  <span class="element-invisible"><?php print $title; ?></span>
<?php endif; ?>
