<?php
// $Id: feedapi_mapper_feedapi_node.inc,v 1.1.2.3 2009/08/17 15:02:26 aronnovak Exp $

/**
 * On behalf implementation of hook_feedapi_mapper() for feedapi_node module.
 *
 * @see hook_feedapi_mapper()
 */
function feedapi_node_feedapi_mapper($op, $feed_node, $active_processor, $node = NULL, $feed_element = array(), $field_name = '', $sub_field = '') {

  // Checks processor compatibility.
  if ($active_processor != 'feedapi_node') {
    return;
  }

  if ($op == 'describe') {
    return t('Maps a feed element to item URL and GUID.');
  }
  else if ($op == 'list') {
    return array('guid' => t('GUID'), 'original_url' => t('URL'));
  }
  else if ($op == 'map') {
    if (in_array($field_name, array('guid', 'original_url'))) {
      if (!empty($feed_element)) {
        $node->feedapi->feed_item->options->{$field_name} = $feed_element;
      }
      return $node;
    }
  }
  else if ($op == 'unique') {
    $nids = array();
    // Check for any duplicates, decide in _feedapi_node_save() whether to look for in-feed duplicates or any duplicates.
    $field_name = $field_name == 'original_url' ? 'url' : $field_name;
    $result = db_query("SELECT nid, feed_nid FROM {feedapi_node_item} i JOIN {feedapi_node_item_feed} f ON i.nid = f.feed_item_nid WHERE %s = '%s'", $field_name, $feed_element);
    while ($item = db_fetch_array($result)) {
      $nids[$item['nid']][] = $item['feed_nid'];
    }
    return $nids;
  }
  else if ($op == 'unique supported') {
    // Both fields support testing for unique items.
    return TRUE;
  }
}
