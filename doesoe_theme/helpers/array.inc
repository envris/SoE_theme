<?php
/**
 * @file
 * Array helpers.
 */

/**
 * Move an item from one position to another in an array while preserving keys.
 *
 * Copied from here: http://stackoverflow.com/a/28831998
 *
 * @param array $array
 *   The array to act on.
 * @param int|string $original_pos
 *   The key of the item to move.
 * @param int $new_pos
 *   The new position of the origninal item.
 */
function _doesoe_theme_array_move(&$array, $original_pos, $new_pos) {
  $p1 = array_splice($array, $original_pos, 1);
  $p2 = array_splice($array, 0, $new_pos);
  $array = array_merge($p2, $p1, $array);
}
