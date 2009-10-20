<?php
// $Id: menuclass.admin.menu.inc,v 1.2 2009/01/05 14:47:29 daniboy Exp $

/**
 * @file Menu Class's administration methods for the actual menus.
 */

/**
 * Form to choose which class sets will be active on a menu.
 * 
 * @param $menu
 *   The menu object of the menu that we want to choose class sets on.
 * 
 * @ingroup forms
 * @see menuclass_admin_menu_set_submit()
 */
function menuclass_admin_menu_set(&$form_state, $menu) {
  $menu_name = $menu['menu_name'];
  $menu_title = $menu['title'];
  
  $form['sets'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Active class sets for !menu', array('!menu' => $menu_title)),
    '#default_value' => variable_get("menuclass_active_sets_{$menu_name}", array()),
    '#description' => t('Select the class sets that you want to enable for this menu.'),
    '#options' => menuclass_get_sets(),
  );
  
  $form['menu_name'] = array(
    '#type' => 'hidden',
    '#value' => $menu_name,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change active sets'),
  );
  
  return $form;
}

/**
 * Submittion for choosing which class sets will be active on a menu.
 *
 * @see menuclass_admin_menu_set
 */
function menuclass_admin_menu_set_submit($form, &$form_state) {
  $menu_name = $form_state['values']['menu_name'];
  
  $current_sets = variable_get("menuclass_active_sets_{$menu_name}", array());
  $new_sets = $form_state['values']['sets'];
  
  $references = 0;
  foreach ($new_sets as $csid => $active) {
    // Check if a set was active on this menu before, and not it's
    // being deactivated.
    if ($active < $current_sets[$csid]) {
      
      // Fetch all pairs of menu link ID and class definition ID that
      // belong to this menu and to the deactivated class set using
      // black magic and a JOIN SQL query.
      $query =  "SELECT mlc.mlid, mlc.classid FROM {menu_links_class} mlc ";
      $query .= "LEFT JOIN {menu_class_definition} mcd ON mlc.classid = mcd.classid ";
      $query .= "LEFT JOIN {menu_links} ml ON ml.mlid = mlc.mlid ";
      $query .= "WHERE ml.menu_name = '%s' AND mcd.csid = %d";
      $result = db_query($query, $menu_name, $csid);
      
      // Now delete those pairs.
      while ($pair = db_fetch_object($result)) {
        if (db_query("DELETE FROM {menu_links_class} WHERE mlid = %d AND classid = %d LIMIT 1", $pair->mlid, $pair->classid)) {
          $references++;
        }
      }
      
    }
  }
  
  if ($references) {
    drupal_set_message(format_plural($references,
      'Removed 1 class reference.',
      'Removed @count class references.'));
  }
  variable_set("menuclass_active_sets_{$menu_name}", $new_sets);
}

