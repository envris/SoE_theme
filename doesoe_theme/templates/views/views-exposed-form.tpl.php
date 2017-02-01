<?php

/**
 * @file
 * This template handles the layout of the views exposed filter form.
 *
 * Variables available:
 * - $widgets: An array of exposed form widgets. Each widget contains:
 * - $widget->label: The visible label to print. May be optional.
 * - $widget->operator: The operator for the widget. May be optional.
 * - $widget->widget: The widget itself.
 * - $sort_by: The select box to sort the view using an exposed form.
 * - $sort_order: The select box with the ASC, DESC options to define order. May be optional.
 * - $items_per_page: The select box with the available items per page. May be optional.
 * - $offset: A textfield to define the offset of the view. May be optional.
 * - $reset_button: A button to reset the exposed filter applied. May be optional.
 * - $button: The submit button for the form.
 *
 * Changes from default:
 * - Separate containers for elements vs actions.
 * - Smacss selectors.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($q)): ?>
  <?php
    // This ensures that, if clean URLs are off, the 'q' is added first so that
    // it shows up first in the URL.
    print $q;
  ?>
<?php endif; ?>
<div class="views-exposed-form__wrapper">
  <?php
    // Use a preprocess hook to move elements into the top region, add a title
    // or move submit to top.
    if (!empty($widgets_top) || !empty($form_title) || !empty($top_actions) || !empty($toggle_text)):
  ?>

    <?php if (!empty($form_top_prefix)): ?>
      <div class="views-exposed-form-top__prefix"><?php print $form_top_prefix; ?></div>
    <?php endif; ?>

    <div class="views-exposed-form-top">
      <div class="views-exposed-widgets-top clearfix">
        <div class="views-exposed-widget-top__container">

          <?php if (!empty($form_title)): ?>
            <h1 class="views-exposed-form-top__title"><?php print $form_title; ?></h1>
          <?php endif; ?>

          <div class="views-exposed-widget-top__elements">
            <?php if (!empty($widgets_top)): ?>
              <?php foreach ($widgets_top as $id => $widget): ?>
                <div id="<?php print $widget->id; ?>-wrapper" class="views-exposed-widget-top views-exposed-widget-top__element-wrapper views-widget-<?php print $id; ?>">
                  <?php if (!empty($widget->label)): ?>
                    <label for="<?php print $widget->id; ?>">
                      <?php print $widget->label; ?>
                    </label>
                  <?php endif; ?>
                  <?php if (!empty($widget->operator)): ?>
                    <div class="views-operator views-exposed-widget-top__operator">
                      <?php print $widget->operator; ?>
                    </div>
                  <?php endif; ?>
                  <div class="views-widget views-exposed-widget-top__element">
                    <?php print $widget->widget; ?>
                  </div>
                  <?php if (!empty($widget->description)): ?>
                    <div class="description views-exposed-widget-top__description">
                      <?php print $widget->description; ?>
                    </div>
                  <?php endif; ?>
                </div>
              <?php endforeach; ?>
            <?php endif; ?>
          </div>

          <div class="views-exposed-widget-top__actions">
            <?php if(!empty($toggle_text)): ?>
              <div class="views-exposed-widget-top__toggle-wrapper">
                <span class="views-exposed-widget-top__toggle"><?php print $toggle_text; ?> <i></i></span>
              </div>
            <?php endif; ?>
            <?php if(!empty($top_actions)): ?>
              <div class="views-exposed-widget-top views-exposed-widget-top__submit">
                <?php print $button; ?>
                <?php
                // Alternate actions dropdown.
                if (!empty($alt_actions)): ?>
                  <div class="views-exposed-widget-top__alt-actions dropdown-wrapper">
                    <i></i>
                    <ul class="dropdown">
                      <?php foreach ($alt_actions as $action => $action_title): ?>
                        <li class="alt-action" data-action="<?php print $action; ?>"><?php print $action_title; ?></li>
                      <?php endforeach; ?>
                    </ul>
                  </div>
                <?php endif; ?>
              </div>
            <?php endif; ?>
          </div>

        </div>
      </div>
    </div>
  <?php endif; ?>

  <?php if (!empty($form_prefix)): ?>
    <div class="views-exposed-form__prefix"><?php print $form_prefix; ?></div>
  <?php endif; ?>

  <div class="views-exposed-form">
    <div class="views-exposed-widgets clearfix">
      <div class="views-exposed-widget__container">

        <?php if (!empty($filters_title)): ?>
          <h3 class="views-exposed-form__filters-title"><?php print $filters_title; ?><i></i></h3>
        <?php endif; ?>

        <div class="views-exposed-widget__elements">
          <?php foreach ($widgets as $id => $widget): ?>
            <div id="<?php print $widget->id; ?>-wrapper" class="views-exposed-widget views-exposed-widget__element-wrapper views-widget-<?php print $id; ?>">
              <?php if (!empty($widget->label)): ?>
                <label for="<?php print $widget->id; ?>">
                  <?php print $widget->label; ?>
                </label>
              <?php endif; ?>
              <?php if (!empty($widget->operator)): ?>
                <div class="views-operator views-exposed-widget__operator">
                  <?php print $widget->operator; ?>
                </div>
              <?php endif; ?>
              <div class="views-widget views-exposed-widget__element">
                <?php print $widget->widget; ?>
              </div>
              <?php if (!empty($widget->description)): ?>
                <div class="description views-exposed-widget__description">
                  <?php print $widget->description; ?>
                </div>
              <?php endif; ?>
            </div>
          <?php endforeach; ?>
          <?php if (!empty($sort_by)): ?>
            <div class="views-exposed-widget views-exposed-widget__sort">
              <?php print $sort_by; ?>
            </div>
            <div class="views-exposed-widget views-exposed-widget__order">
              <?php print $sort_order; ?>
            </div>
          <?php endif; ?>
          <?php if (!empty($items_per_page)): ?>
            <div class="views-exposed-widget views-exposed-widget__per-page">
              <?php print $items_per_page; ?>
            </div>
          <?php endif; ?>
          <?php if (!empty($offset)): ?>
            <div class="views-exposed-widget views-exposed-widget__offset">
              <?php print $offset; ?>
            </div>
          <?php endif; ?>
        </div>

        <div class="views-exposed-widget__actions">
          <div class="views-exposed-widget views-exposed-widget__submit">
            <?php print $button; ?>
          </div>
          <?php if (!empty($reset_button)): ?>
            <div class="views-exposed-widget views-exposed-widget__reset">
              <?php print $reset_button; ?>
            </div>
          <?php endif; ?>
        </div>

      </div>
    </div>
  </div>

  <?php if (!empty($form_suffix)): ?>
    <div class="views-exposed-form__suffix"><?php print $form_suffix; ?></div>
  <?php endif; ?>

</div>