<?php
// $Id: menuclass.admin.sets.inc,v 1.2 2009/01/05 14:47:29 daniboy Exp $

/**
 * @file Menu Class's administration methods for Class Sets. 
 */

// Class sets related constants.
define('MENUCLASS_SET_TITLE_SIZE', 32);

/**
 * Menu callback; display the list of class sets and a form to add a
 * new set.
 */
function menuclass_admin_sets_page() {
  drupal_set_title(t('Class sets'));
  $output = '';
  
  // Output a table with the current sets and with operations on those sets.
  $headers = array(t('Set title'), t('Operations'));
  $rows = array();
  $sets = menuclass_get_sets();
  if (!empty($sets)) {
    foreach ($sets as $csid => $title) {
      
      $operations = theme_links(array(
        array('href' => MENUCLASS_PATH."/$csid/list", 'title' => t("List items")),
        array('href' => MENUCLASS_PATH."/$csid/rename", 'title' => t("Rename")),
        array('href' => MENUCLASS_PATH."/$csid/delete", 'title' => t("Delete")),
      ));
      $rows[] = array($title, $operations);
      
    }
    $output .= theme_table($headers, $rows);
  }
  else {
    $output .= t('There are no sets.');
  }
  
  // Form to add a new set.
  $output .= drupal_get_form('menuclass_admin_set_add');
  
  return $output;
}

/**
 * Form to add a new class set.
 * 
 * @ingroup forms
 * @see menuclass_admin_set_add_validate()
 * @see menuclass_admin_set_add_submit()
 */
function menuclass_admin_set_add(&$form_state) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('New set'),
    '#description' => t('Write a unique title to add a new set.'),
    '#maxlength' => MENUCLASS_SET_TITLE_SIZE,
    '#required' => TRUE,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add a new set'),
  );
  
  return $form;
}

/**
 * Validation for adding a new class set.
 * 
 * @see menuclass_admin_set_add()
 */
function menuclass_admin_set_add_validate($form, &$form_state) {
  $new_title = check_plain($form_state['values']['title']);
  
  // Check if a set by this name already exists.
  if ((bool)(db_result(db_query("SELECT csid FROM {menu_class_set} WHERE title = '%s'", $new_title)))) {
    form_set_error('title', t('The set <em>!title</em> already exists.', array('!title' => $new_title)));
  }
}

/**
 * Submittion for adding a new class set.
 * 
 * @see menuclass_admin_set_add()
 */
function menuclass_admin_set_add_submit($form, &$form_state) {
  $set = new stdClass();
  $set->title = check_plain($form_state['values']['title']);
  if (drupal_write_record('menu_class_set', $set)) {
    drupal_set_message(t('The set <em>!set</em> has been added.', array('!set' => $set->title)));
  }
  else {
    form_set_error('title', t('Unknown error!'));
  }
}

/**
 * Form to rename a set.
 * 
 * @param $csid
 *   The ID of the class set we want to rename.
 * 
 * @ingroup forms
 * @see menuclass_admin_set_rename_validate()
 * @see menuclass_admin_set_rename_submit()
 */
function menuclass_admin_set_rename(&$form_state, $csid) {
  if ($title = menuclass_get_set($csid)) {
    drupal_set_title(t('Rename the set <em>!title</em>.', array('!title' => $title)));
    
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Rename set'),
      '#default_value' => check_plain($title),
      '#description' => t('Write a new and unique title to rename the <em>!title</em> set.', array('!title' => $title)),
      '#maxlength' => MENUCLASS_SET_TITLE_SIZE,
      '#required' => TRUE,
    );
    
    $form['csid'] = array(
      '#type' => 'hidden',
      '#value' => (int)$csid,
    );
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Rename set'),
    );
    
    $form['cancel'] = array(
      '#value' => l(t('Cancel'), MENUCLASS_PATH),
    );
    
    return $form;
  }
  else {
    drupal_set_message(t('There is no set #!csid', array('!csid' =>$csid)), 'warning');
    drupal_goto(MENUCLASS_PATH);
  }
}

/**
 * Validation for renaming a set.
 * 
 * @see menuclass_admin_set_rename()
 */
function menuclass_admin_set_rename_validate($form, &$form_state) {
  $csid = (int)$form_state['values']['csid'];
  $new_title = check_plain($form_state['values']['title']);
  
  // Check if we are trying to rename the set to itself
  if ($new_title == menuclass_get_set($csid)) {
    form_set_error('title', t('Cannot rename the <em>!title</em> set to <em>!title</em>.', array('!title' => $new_title)));
    return;
  }
  
  // Check if a set by this name already exists.
  if ((bool)(db_result(db_query("SELECT csid FROM {menu_class_set} WHERE title = '%s'", $new_title)))) {
    form_set_error('title', t('Another set with the name <em>!title</em> already exists.', array('!title' => $new_title)));
  }
}

/**
 * Submittion for renaming a set.
 * 
 * @see menuclass_admin_set_rename()
 */
function menuclass_admin_set_rename_submit($form, &$form_state) {
  $set = new stdClass();
  $set->csid = (int)$form_state['values']['csid'];
  $set->title = check_plain($form_state['values']['title']);
  if (drupal_write_record('menu_class_set', $set, 'csid')) {
    drupal_set_message(t('The set has been renamed to <em>!title</em>.', array('!title' => $set->title)));
    drupal_goto(MENUCLASS_PATH);
  }
  else {
    form_set_error('title', t('Unknown error!'));
  }
}

/**
 * Form to delete a set.
 * 
 * @param $csid
 *   The ID of the class set we want to delete.
 * 
 * @ingroup forms
 * @see menuclass_admin_set_delete_submit()
 */
function menuclass_admin_set_delete(&$form_state, $csid) {
  if ($title = menuclass_get_set($csid)) {
    drupal_set_title(t('Delete the set <em>!title</em>.', array('!title' => $title)));
    
    $form['csid'] = array(
      '#type' => 'hidden',
      '#value' => (int)$csid,
    );
    
    $form = confirm_form($form,
      t('Are you sure you want to delete the set <em>!title</em>?', array('!title' => $title)),
      MENUCLASS_PATH,
      t('All classes that belong to this set will be deleted as well.'));
    
    return $form;
  }
  else {
    drupal_set_message(t('There is no set #!csid', array('!csid' =>$csid)), 'warning');
    drupal_goto(MENUCLASS_PATH);
  }
}

/**
 * Submittion for deleting a set.
 * 
 * @see menuclass_admin_set_delete()
 */
function menuclass_admin_set_delete_submit($form, &$form_state) {
  $csid = (int)$form_state['values']['csid'];
  $title = menuclass_get_set($csid);
  
  // Delete all references to classes in this set
  db_query('DELETE FROM {menu_links_class} WHERE classid IN (SELECT classid FROM {menu_class_definition} WHERE {csid} = %d)', $csid);
  $references = db_affected_rows();
  
  // Delete all classes in this set
  db_query('DELETE FROM {menu_class_definition} WHERE csid = %d', $csid);
  $classes = db_affected_rows();
  
  // Delete set
  db_query('DELETE FROM {menu_class_set} WHERE csid = %d', $csid);
  
  drupal_set_message(t('Deleted the set <em>!title</em>. Removed !references references and !classes class definitions.', array('!title' => $title, '!references' => $references, '!classes' => $classes)));
  drupal_goto(MENUCLASS_PATH);
}

