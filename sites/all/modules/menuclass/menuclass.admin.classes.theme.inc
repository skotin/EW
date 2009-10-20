<?php
// $Id: menuclass.admin.classes.theme.inc,v 1.2 2009/01/05 14:47:29 daniboy Exp $

/**
 * @file Menu Class's administration methods for Classes. 
 */

/**
 * Themer for the menuclass_admin_classes_list form.
 *
 * @ingroup themeable
 */
function theme_menuclass_admin_classes_list($form) {
  
  $header = array(t('Class definition'), t('Weight'), t('Operations'));
  $rows = array();
  foreach (element_children($form['definitions']) as $classid) {
    // Don't take form control structures.
    if (is_array($form['definitions'][$classid])) {
      $form['weights'][$classid]['#attributes']['class'] = 'menuclass-class-weight';
      $rows[] = array(
        'data' => array(drupal_render($form['definitions'][$classid]), drupal_render($form['weights'][$classid]), drupal_render($form['operations'][$classid])),
        'class' => 'draggable',
      );
    }
  }
  
  $output = '';
  $output .= theme('table', $header, $rows, array('id' => 'menuclass-classes'));
  $output .= drupal_render($form);

  drupal_add_tabledrag('menuclass-classes', 'order', 'sibling', 'menuclass-class-weight', NULL, NULL, FALSE);

  return $output;
}

