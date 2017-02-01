<?php
/**
 * @file
 * Assessment summary list header.
 */
?>
<div class="ass-sum__header">

  <div class="ass-sum__header__title">
    <strong><?php print t('Component'); ?></strong>
    <div class="ass-sum__header__sub"></div>
  </div>

  <?php
  /* This mimics the layout of the content below the header */
  ?>
  <div class="ass-sum__header__items">

    <div class="ass-sum__header__summary">
      <strong><?php print t('Summary'); ?></strong>
      <div class="ass-sum__header__sub"></div>
    </div>

    <div class="ass-sum__header__year">
      <?php /* placeholder */ ?>
    </div>

    <div class="ass-sum__header__grade">
      <strong><?php print t('Grade'); ?></strong>
      <div class="ass-sum__header__grades ass-sum__header__sub scale-<?php print $grade_scale; ?>">
        <?php foreach (explode(',', $grades) as $grade): ?>
          <span><?php print trim($grade); ?></span>
        <?php endforeach; ?>
      </div>
    </div>

    <div class="ass-sum__header__confidence">
      <strong><?php print t('Confidence'); ?></strong>
      <div class="ass-sum__header__confidences ass-sum__header__sub">
        <span><?php print t('In Grade'); ?></span>
        <span><?php print t('In Trend'); ?></span>
      </div>
    </div>

    <div class="ass-sum__header__compare">
      <strong><?php print t('Comparability'); ?></strong>
      <div class="ass-sum__header__compates ass-sum__header__sub">
        <span><?php print t('To previous years'); ?></span>
      </div>
    </div>

  </div>

</div>
