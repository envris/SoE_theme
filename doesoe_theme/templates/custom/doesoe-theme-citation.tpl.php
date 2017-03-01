<?php
/**
 * @file
 * Citation template.
 *
 * Available variables:
 * - $report_year: The most current report year.
 * - $theme_name: The theme name.
 * - $page_title: The current page title.
 * - $page_title: The authors name separated by comma.
 * - $date: The current date in 'j F Y' format.
 */
?>
<?php print $authors; ?> (<?php print $report_year; ?>). <?php print $theme_name; ?>: <?php print $page_title; ?>. In: <?php print $report_name; ?>, <?php print $url; ?>, <?php print $doi; ?>
