<?php
// $Id: feedapi_mapper_emvideo.inc,v 1.2.2.2 2009/08/07 15:43:43 aronnovak Exp $

/**
 * Implementation of hook_feedapi_mapper().
 *
 * @see hook_feedapi_mapper()
 */
function emvideo_feedapi_mapper($op, $feed_node, $active_processor, $node = NULL, $feed_element = array(), $field_name = '', $sub_field = '') {

  // Checks processor compatibility.
  if ($active_processor != 'feedapi_node') {
    return;
  }

  switch ($op) {
    case 'describe':
      // Describe what we are doing in this mapper. This shows up as help text on the mapping page.
      return t('Maps a link to a video to the Embedded Media Video CCK field. Use the original_url element for mapping to this field.');
    case 'list':
      $info = content_types(feedapi_mapper_get_item_node_type($feed_node));
      $fields = array();
      if (@count($info['fields'])) {
        foreach ($info['fields'] as $field_name => $field) {
          if (in_array($field['type'], array('emvideo'))) {
            $fields[$field_name] = isset($field['widget']['label']) ? $field['widget']['label'] : $field_name;
          }
        }
      }
      if (count($fields)) {
        return $fields;
      }
      return FALSE;
    case 'map':
      // Here is where the actual mapping happens.
      if (isset($node->{$field_name})) {
        $items = $node->{$field_name};
      }
      else {
        $items = array();
      }
      if (is_string($feed_element) && valid_url($feed_element)) {
        // straight link usually from options->original_url or options->guid
        $items[]['embed'] = $feed_element;
      }
      elseif (is_array($feed_element)) {
        foreach ($feed_element as $element) {
          if (is_array($element)) {
            // is this an options->enclosure-media->type array
            if (valid_url($element['link'])) {
              $items[]['embed'] = $element['link'];
            }
            elseif (valid_url($element['player'])) {
              $items[]['embed'] = $element['player'];
            }
          }
          elseif (is_string($element) && valid_url($element)) {
            // not sure when this would be called but for completeness
            $items[]['embed'] = $element;
          }
        }
      }
      emvideo_field('submit', $node, $field_name, $items, FALSE, FALSE);
      $node->$field_name = $items;
      return $node;
  }
}
