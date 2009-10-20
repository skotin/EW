<?php
// $Id: menuclass.admin.classes.inc,v 1.2 2009/01/05 14:47:29 daniboy Exp $

/**
 * @file Menu Class's administration methods for Classes. 
 */

// Classes related constants.
define('MENUCLASS_CLASS_MAX_LENGTH', 32);
define('MENUCLASS_CLASS_REGEX', '/^[a-zA-Z][a-zA-Z0-9-]*$/');

/**
 * Menu callback; display the list of classes that belong to a set
 * and a form to add a new class to that set.
 */
function menuclass_admin_classes_page($csid) {
  if ($set = menuclass_get_set($csid)) {
    drupal_set_title(t('Class definitions in !set', array('!set' => $set)));
    $output = '';
    
    // Output the class definitions reordering form.
    $output .= drupal_get_form('menuclass_admin_classes_list', $csid);
    
    // Output the class addition form.
    $output .= drupal_get_form('menuclass_admin_class_add', $csid);
    
    return $output;
  }
  else {
    drupal_set_message(t('There is no set #!csid', array('!csid' =>$csid)), 'warning');
    drupal_goto(MENUCLASS_PATH);
  }
}

/**
 * From to list and reorder class definitions from a class set.
 * 
 * @param $csid
 *   The class set's ID that we want to list and reorder classes of.
 * 
 * @ingroup form
 * @see menuclass_admin_classes_list_submit()
 * @see theme_menuclass_admin_classes_list()
 */
function menuclass_admin_classes_list(&$form_state, $csid) {
  $classes = menuclass_get_classes($csid);
  
  if (!empty($classes)) {
    $form['weights'] = array('#tree' => TRUE);
    
    foreach ($classes as $class) {
      $form['definitions'][$class->classid] = array(
        '#value' => check_plain($class->definition),
      );
      
      $form['weights'][$class->classid] = array(
        '#type' => 'weight',
        '#default_value' => $class->weight,
      );
      
      $form['operations'][$class->classid] = array(
        '#value' => theme_links(array(
          array('href' => MENUCLASS_PATH."/class/$class->classid/redefine", 'title' => t("Redefine")),
          array('href' => MENUCLASS_PATH."/class/$class->classid/delete", 'title' => t("Delete")),
        )),
      );
    }
    
    $form['csid'] = array(
      '#type' => 'hidden',
      '#value' => (int)$csid,
    );
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Reorder class definitions'),
    );
  }
  else {
    $form['empty'] = array(
      '#value' => t('There are no classes defined for this menu.'),
    );
  }
  
  return $form;
}

/**
 * Submittion for listing and reordering class definitions from a
 * class set.
 *
 * @see menuclass_admin_classes_list()
 */
function menuclass_admin_classes_list_submit($form, &$form_state) {
  foreach ($form_state['values']['weights'] as $classid => $weight) {
    $class = new stdClass();
    $class->classid = $classid;
    $class->weight = $weight;
    drupal_write_record('menu_class_definition', $class, 'classid');
  }
  drupal_set_message(t('The class definitions ordering has been saved.'));
}

/**
 * Form to add a new class.
 * 
 * @param $csid
 *   The class set's ID that we want to add a class to.
 * 
 * @ingroup forms
 * @see menuclass_admin_class_add_validate()
 * @see menuclass_admin_class_add_submit()
 */
function menuclass_admin_class_add(&$form_state, $csid) {
  $set = menuclass_get_set($csid);
  
  $form['definition'] = array(
    '#type' => 'textfield',
    '#title' => t('New definition'),
    '#description' => t('Write a unique definition to add to the new !set set. Each class name must start with a letter and contain only letters, numbers and hyphens. (This can be a space seperated list.)', array('!set' => $set)),
    '#maxlength' => MENUCLASS_CLASS_MAX_LENGTH,
    '#required' => TRUE,
  );
  
  $form['csid'] = array(
    '#type' => 'hidden',
    '#value' => (int)$csid,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add a new definition'),
  );
  
  return $form;
}

/**
 * Validation for adding a new class.
 * 
 * @see menuclass_admin_class_add()
 */
function menuclass_admin_class_add_validate($form, &$form_state) {
  $csid = (int)$form_state['values']['csid'];
  $new_definition = _menuclass_remove_whitespace(check_plain($form_state['values']['definition']));
  
  // Check if there is actually a set to add to.
  if (!($set = menuclass_get_set($csid))) {
    drupal_set_message(t('Trying to add a class to a set that does not exist.'), 'error');
    drupal_goto(MENUCLASS_PATH);
  }
  
  
  // Check whether every class name in this definition conforms to
  // the allowed standard.
  $class_names = explode(' ', $new_definition);
  foreach ($class_names as $class_name) {
    if (!preg_match(MENUCLASS_CLASS_REGEX, $class_name)) {
      form_set_error('definition', t('Invalid class name in the definition. Each class must start with a letter and contain only letters, numbers and hyphens.'));
      return;
    }
  }
  
  // Check if a definition like this one already exists.
  if ((bool)(db_result(db_query("SELECT classid FROM {menu_class_definition} WHERE csid = %d AND definition = '%s'", $csid, $new_definition)))) {
    form_set_error('title', t('The definition <code>!definition</code> already exists in the set <em>!set</em>.', array('!definition' => $new_definition, '!set' => $set)));
  }
  
  // Save the de-whitespaced definition to the form state
  $form_state['values']['definition'] = $new_definition;
}

/**
 * Submittion for adding a new class.
 * 
 * @see menuclass_admin_class_add()
 */
function menuclass_admin_class_add_submit($form, &$form_state) {
  $class = new stdClass();
  $class->csid = (int)check_plain($form_state['values']['csid']);
  $class->definition = check_plain($form_state['values']['definition']);
  if (drupal_write_record('menu_class_definition', $class)) {
    drupal_set_message(t('The class definition <code>!definition</code> has been added.', array('!definition' => $class->definition)));
  }
  else {
    form_set_error('title', t('Unknown error!'));
  }
}

/**
 * Form to redefine a class.
 * 
 * @param $classid
 *   The ID of the class we want to redefine.
 * 
 * @ingroup forms
 * @see menuclass_admin_class_redefine_validate()
 * @see menuclass_admin_class_redefine_submit()
 */
function menuclass_admin_class_redefine(&$form_state, $classid) {
  if ($class = menuclass_get_class($classid)) {
    drupal_set_title(t('Redefine the class <code>!definition</code>.', array('!definition' => $class->definition)));
    
    $form['notice'] = array(
      '#value' => t('<strong>Notice</strong>: Redefining a class will change all existing references to the new definition.'),
    );
    
    $form['definition'] = array(
      '#type' => 'textfield',
      '#title' => t('Redefine class'),
      '#default_value' => check_plain($class->definition),
      '#description' => t('Write a unique definition to redefine. Each class name must start with a letter and contain only letters, numbers and hyphens. (This can be a space seperated list.)', array('!set' => $set)),
      '#maxlength' => MENUCLASS_CLASS_MAX_LENGTH,
      '#required' => TRUE,
    );
    
    $form['classid'] = array(
      '#type' => 'hidden',
      '#value' => (int)$classid,
    );
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Redefine class'),
    );
    
    $form['cancel'] = array(
      '#value' => l(t('Cancel'), MENUCLASS_PATH."/$class->csid/list"),
    );
    
    return $form;
  }
  else {
    drupal_set_message(t('There is no class #!classid', array('!classid' =>$classid)), 'warning');
    drupal_goto(MENUCLASS_PATH);
  }
}

/**
 * Validation for redefining a class.
 * 
 * @see menuclass_admin_class_redefine()
 */
function menuclass_admin_class_redefine_validate($form, &$form_state) {
  $classid = (int)$form_state['values']['classid'];
  $class = menuclass_get_class($classid);
  $new_definition = _menuclass_remove_whitespace(check_plain($form_state['values']['definition']));
  
  // Check if we are trying to redefine the class to itself
  if ($new_definition == $class->definition) {
    form_set_error('definition', t('Cannot redefine <code>!definition</code> to itself.', array('!definition' => $new_definition)));
    return;
  }
  
  // Check whether every class name in this definition conforms to
  // the allowed standard.
  $class_names = explode(' ', $new_definition);
  foreach ($class_names as $class_name) {
    if (!preg_match(MENUCLASS_CLASS_REGEX, $class_name)) {
      form_set_error('definition', t('Invalid class name in the definition. Each class must start with a letter and contain only letters, numbers and hyphens.'));
      return;
    }
  }
  
  // Check if a definition like this one already exists.
  if ((bool)(db_result(db_query("SELECT classid FROM {menu_class_definition} WHERE csid = %d AND definition = '%s'", $class->csid, $new_definition)))) {
    form_set_error('title', t('The definition <code>!definition</code> already exists in the this set.', array('!definition' => $new_definition)));
  }
  
  // Save the de-whitespaced definition to the form state
  $form_state['values']['definition'] = $new_definition;
  
  // Save the class set's ID to the form state
  $form_state['values']['csid'] = $class->csid;
}

/**
 * Submittion for redefining a class.
 * 
 * @see menuclass_admin_class_redefine()
 */
function menuclass_admin_class_redefine_submit($form, &$form_state) {
  $class = new stdClass();
  $class->classid = (int)$form_state['values']['classid'];
  $class->definition = check_plain($form_state['values']['definition']);
  if (drupal_write_record('menu_class_definition', $class, 'classid')) {
    drupal_set_message(t('The class has been redefined to <code>!definition</code>.', array('!definition' => $class->definition)));
    
    $csid = (int)$form_state['values']['csid'];
    drupal_goto(MENUCLASS_PATH."/$csid/list");
  }
  else {
    form_set_error('title', t('Unknown error!'));
  }
}

/**
 * Form to delete a class.
 * 
 * @param $classid
 *   The ID of the class we want to redefine.
 * 
 * @ingroup forms
 * @see menuclass_admin_class_delete_submit()
 */
function menuclass_admin_class_delete(&$form_state, $classid) {
  if ($class = menuclass_get_class($classid)) {
    drupal_set_title(t('Delete the class definition <code>!definition</code>.', array('!definition' => $class->definition)));
    
    $form['classid'] = array(
      '#type' => 'hidden',
      '#value' => (int)$classid,
    );
    
    $form = confirm_form($form,
      t('Are you sure you want to delete the class definition <code>!definition</code>?', array('!definition' => $class->definition)),
      MENUCLASS_PATH,
      t('All references to this class will be deleted as well.'));
    
    return $form;
  }
  else {
    drupal_set_message(t('There is no class definition #!classid', array('!classid' =>$classid)), 'warning');
    drupal_goto(MENUCLASS_PATH);
  }
}

/**
 * Submittion for deleting a set.
 * 
 * @see menuclass_admin_class_delete()
 */
function menuclass_admin_class_delete_submit($form, &$form_state) {
  $classid = (int)$form_state['values']['classid'];
  $class = menuclass_get_class($classid);
  
  // Delete all references to the class
  db_query('DELETE FROM {menu_links_class} WHERE classid = %d', $classid);
  $references = db_affected_rows();
  
  // Delete the class definition
  db_query('DELETE FROM {menu_class_definition} WHERE classid = %d', $classid);
  
  drupal_set_message(t('Deleted the class definition <code>!definition</code>. Removed !references references.', array('!definition' => $class->definition, '!references' => $references)));
  drupal_goto(MENUCLASS_PATH."/$class->csid/list");
}

