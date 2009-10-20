<?php
// $Id: feedapi_mapper_location.inc,v 1.2.2.2 2009/08/07 15:43:43 aronnovak Exp $

/**
 * On behalf implementation of hook_feedapi_mapper() for location module.
 * 
 * @see hook_feedapi_mapper()
 */
function location_feedapi_mapper($op, $feed_node, $active_processor, $node = NULL, $feed_element = array(), $field_name = '', $sub_field = '') {

  // Checks processor compatibility.
  if ($active_processor != 'feedapi_node') {
    return;
  }

  if ($op == 'describe') {
    return t('Maps pairs of geographic coordinates to the location of a node.');
  }
  else if ($op == 'list') {
    $sub_fields = array('latitude' => t('Latitude'), 'longitude' => t('Longitude'));
    $info = content_types($node_type);
    $fields = array();
    if (@count($info['fields'])) {
      foreach ($info['fields'] as $field_name => $field) {
        if (in_array($field['type'], array('location'))) {
          $fields[$field_name] = $sub_fields;
        }
      }
    }
    if (count($fields)) {
      return $fields;
    }
    return FALSE;
  }
  else if ($op == 'map') {
    if (is_array($feed_element)) {
      foreach ($feed_element as $i => $val) {
        $node->{$field_name}[$i][$sub_field] = $val;
      }
    }
    return $node;
  }
}
