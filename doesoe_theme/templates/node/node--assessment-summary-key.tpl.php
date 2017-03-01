<?php
/**
 * @file
 * Template for an assessment summary key node.
 */
?>
<div class="<?php print $classes; ?>">

  <div class="ass-sum-key__title">
    <span class="icon-information" aria-hidden="true" title="<?php print t('Assessment Summary Key'); ?>"></span>
    <h2><?php print t('Assessment Summary Key'); ?></h2>
  </div>

  <div class="ass-sum-key__layout__col-1">

    <h3><?php print t('Grades'); ?></h3>

    <div class="ass-sum-key__grades">
      <?php print render($content); ?>
    </div>

  </div>

  <div class="ass-sum-key__layout__col-2">

    <h3><?php print t('Recent Trends'); ?></h3>

    <ul class="ass-sum-key__trends">
      <li>
        <span class="ass-sum-key__icon icon-trend-increase"></span>
        <p><?php print t('Improving'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-trend-no-change"></span>
        <p><?php print t('Stable'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-trend-decrease"></span>
        <p><?php print t('Deteriorating'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-trend-unknown"></span>
        <p><?php print t('Unclear'); ?></p>
      </li>
    </ul>

    <h3><?php print t('Confidence'); ?></h3>

    <ul class="ass-sum-key__confidence">
      <li>
        <span class="ass-sum-key__icon icon-confidence-5"></span>
        <p><?php print t('<strong>Adequate:</strong> Adequate high-quality evidence <strong>and</strong> high level of consensus'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-confidence-4"></span>
        <p><?php print t('<strong>Somewhat adequate:</strong> Adequate high-quality evidence <strong>or</strong> high level of consensus'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-confidence-3"></span>
        <p><?php print t('<strong>Limited:</strong> Limited evidence <strong>or</strong> limited consensus'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-confidence-2"></span>
        <p><?php print t('<strong>Very limited:</strong> Limited evidence <strong>and</strong> limited consensus'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-confidence-1"></span>
        <p><?php print t('<strong>Low:</strong> Evidence and consensus too low to make an assessment'); ?></p>
      </li>
    </ul>

    <h3><?php print t('Comparability'); ?></h3>

    <ul class="ass-sum-key__comparability">
      <li>
        <span class="ass-sum-key__icon icon-compare-4"></span>
        <p><?php print t('<strong>Comparable:</strong> Grade and trend are comparable to the previous assessment'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-compare-3"></span>
        <p><?php print t('<strong>Somewhat comparable:</strong> Grade and trend are somewhat comparable to the previous assessment'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-compare-2"></span>
        <p><?php print t('<strong>Not comparable:</strong> Grade and trend are not comparable to the previous assessment'); ?></p>
      </li>
      <li>
        <span class="ass-sum-key__icon icon-compare-1"></span>
        <p><?php print t('Not previously assessed'); ?></p>
      </li>
    </ul>

  </div>

</div>
