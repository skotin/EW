<?php
// $Id: feedapi_mapper_link.inc,v 1.2.2.2 2009/08/07 15:43:43 aronnovak Exp $

/**
 * On behalf implementation of hook_feedapi_mapper for link module (CCK).
 * 
 * @see hook_feedapi_mapper()
 */
function link_feedapi_mapper($op, $feed_node, $active_processor, $node = NULL, $feed_element = array(), $field_name = '', $sub_field = '') {
  
  // Checks processor compatibility.
  if ($active_processor != 'feedapi_node') {
    return;
  }

  // Test for the node field that we would like to map to.
  if ($op == 'describe') {
    return t('Maps a URL to this link CCK field.');
  }
  else if ($op == 'list') {
    $info = content_types(feedapi_mapper_get_item_node_type($feed_node));
    $fields = array();
    if (@count($info['fields'])) {
      foreach ($info['fields'] as $field_name => $field) {
        if (in_array($field['type'], array('link'))) {
          $fields[$field_name] = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
        }
      }
    }
    if (count($fields)) {
      return $fields;
    }
    return FALSE;
  }
  else if ($op == 'map') {
    // Here is where the actual mapping happens.
    // When we are called at this point, $field_name contains the name of the field the user has
    // decided to map to and $field_element is the feed item element the user has decided to map.
    // We just need to put the two things together. The data structure here depends a lot on
    // CCK. We stick the value in $feed_element into $node->$field_name[0]['value'].
    if (!is_array($feed_element)) {
      if (isset($node->$field_name)) {
        $field = $node->$field_name;
      }
      else {
        $field = array();
      }
      $field[count($field)]['url'] = link_validate_url($feed_element) ? $feed_element : NULL;
      $node->$field_name = $field;
    }
    else {
      $node->{$field_name} = array();
      foreach ($feed_element as $k => $url) {
        $node->{$field_name}[$k]['url'] = link_validate_url($url) ? $url : NULL;
      }
    }
    return $node;
  }
}
