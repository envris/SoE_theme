<?php
/**
 * @file
 * Preprocessors for panels.
 */

/**
 * Implements hook_preprocess_panels_pane().
 */
function doesoe_theme_preprocess_panels_pane(&$vars) {
  // Add a theme intro for atmo sub themes.
  if (isset($vars['pane']->configuration['bean_delta']) && $vars['pane']->configuration['bean_delta'] == 'framework-intro-by-theme') {
    $intro = _doesoe_theme_atmosphere_sub_theme_intro_text('Frameworks', 'theme/!term/by-framework');
    $vars['content']['bean']['framework-intro-by-theme']['field_bean_body'][0]['#markup'] .= $intro;
  }
}
