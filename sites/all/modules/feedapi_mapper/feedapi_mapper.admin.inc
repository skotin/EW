<?php
// $Id: feedapi_mapper.admin.inc,v 1.1.2.12 2009/09/29 18:47:12 alexb Exp $
/**
 * @file
 * Administration-related functions for FeedAPI Mapper
 */

/**
 * Theming function for outputting result of _feedapi_mapper_get_field_mappers_descriptions().
 * @param $descriptions
 *   Result of _feedapi_mapper_get_field_mappers_descriptions().
 * @return
 *   HTML output.
 */
function theme_feedapi_mapper_descriptions($descriptions) {
  $output = '<dl>';
  foreach ($descriptions as $module => $description) {
    $output .= '<dt><strong>'. t('!module_name module', array('!module_name' => $module)) .'</strong></dt>';
    $output .= '<dd>'. $description .'</dd>';
  }
  $output .= '</dl>';
  return $output;
}

/**
 * Theming function for showing the current mapping.
 */
function theme_feedapi_mapper_form($form) {
  $output = '';
  _feedapi_mapper_load_mappers();
  $type_url_str = str_replace('_', '-', $form['#node']->type);
  if (isset($form['#node']->nid)) {
    $delete_path = 'node/' . $form['#node']->nid .'/map/delete/';
    $toggle_unique_path = 'feedapi-mapper/' . $form['#node']->nid .'/unique/';
  }
  else {
    $delete_path = 'admin/content/node-type/'. $type_url_str .'/map/delete/';
    $toggle_unique_path = 'feedapi-mapper/'. $type_url_str .'/unique/';
  }
  if (isset($form['#mapping']['mapping'])) {
    $active_processors = _feedapi_mapper_get_active_processors($form['#node']->type);
    foreach ($form['#mapping']['mapping'] as $feed_path => $node_path) {
      $target = unserialize($node_path);
      if (function_exists($target[0] .'_feedapi_mapper')) {
        $unique_supported = FALSE;
        foreach ($active_processors as $processor) {
          $unique_supported = ($unique_supported | call_user_func($target[0] .'_feedapi_mapper', 'unique supported', $form['#node'], $processor, NULL, $target[1]));
        }
        if ($unique_supported == TRUE) {
          $unique = $form['#mapping']['unique'][$feed_path] ? t('Yes') : t('No');
          $unique = l($unique, $toggle_unique_path . base64_encode($feed_path) .'/'. drupal_get_token($feed_path));
        }
        else {
          $unique = t('N/A');
        }
      }
      $rows[] = array(
        // @todo: Set proper messages.
        isset($form['#feed_map'][$feed_path]) ? $form['#feed_map'][$feed_path] : '',
        isset($form['#field_map'][$node_path]) ? $form['#field_map'][$node_path] : t('<em>Error: missing target.</em>'),
	      $unique,
        l(t('Delete'), $delete_path . base64_encode($feed_path)),
      );
    }
  }

  $rows[] = array(
    drupal_render($form['source']),
    drupal_render($form['target']),
    drupal_render($form['unique']),
    drupal_render($form['add']),
  );

  $form['mapping']['table']['#value'] = theme('table', array(t('Source'), t('Target'), t('Unique')), $rows);

  $output .= drupal_render($form);

  return $output;
}

/**
 * Page callback for deleting field mapings.
 */
function feedapi_mapper_delete_form($form_state, $param, $encoded_key) {
  $form = array();
  $key = base64_decode($encoded_key);
  if (is_string($param)) {
    $node = new stdClass();
    $node->type = str_replace('-', '_', $param);
    $path = 'admin/content/node-type/'. $param .'/map';
  }
  else {
    $node = $param;
    $path = "node/{$node->nid}/map";
  }

  $form['#node'] = $node;
  $form['#redirect'] = $path;
  $form['#source'] = $key;

  return confirm_form($form,
    t('Delete this mapping?'),
    $path,
    t('Are you sure you would like to delete the mapping for the %path feed element? This action cannot be undone.', array('%path' => join('->', unserialize($key)))),
    t('Delete'), t('Cancel')
  );
}

/**
 * Submission callback for feedapi_mapper_delete_form.
 */
function feedapi_mapper_delete_form_submit($form, &$form_state) {
  $param = isset($form['#node']->nid) && _feedapi_mapper_scope($form['#node']->nid) ? $form['#node']->nid : $form['#node']->type;
  $node = new stdClass();
  $field = is_numeric($param) ? 'nid' : 'type';
  $node->{$field} = $param;
  feedapi_mapper_override_mapping($node);
  feedapi_mapper_delete_mapping($param, $form['#source']);
  drupal_set_message(t('The mapping entry has been deleted successfully'));
}

/**
 * Reverts the mapping to the version that lies in the .inc file.
 */
function feedapi_mapper_revert($form_state, $param) {
  $form = array();
  $node = new stdClass();
  $node->type = str_replace('-', '_', $param);

  $form['#node'] = $node;
  $form['#redirect'] = $_REQUEST['redirect'];

  return confirm_form($form,
    t('Revert this mapping?'),
    $path,
    t('Are you sure you would like to revert the mapping to the exported one? The changes that you made in this mapping will be lost.'),
    t('Revert'), t('Cancel')
  );
}

/**
 * Submission callback for feedapi_mapper_revert.
 */
function feedapi_mapper_revert_submit($form, &$form_state) {
  feedapi_mapper_delete_mapping($form['#node']->type);
}

/**
 * Toggles the value of the unique-ness of the given key
 * @see theme_feedapi_mapper_form()
 */
function feedapi_mapper_toggle_unique($param, $encoded_key, $token) {
  $key = base64_decode($encoded_key);
  if (!drupal_valid_token($token, $key)) {
    drupal_set_message(t('Invalid request'), 'error');
    drupal_goto('');
  }
  if (!is_numeric($param)) {
    $node = new stdClass();
    $node->type = str_replace('-', '_', $param);
    $path = 'admin/content/node-type/'. $param .'/map';
  }
  elseif (!_feedapi_mapper_scope($param)) {
    $node = node_load($param);
    $path = "node/{$param}/map";
    $param = $node->type;
  }
  else {
    $node = node_load($param);
    $path = "node/{$node->nid}/map";
    $param = $node->nid;
  }
  $mapping = feedapi_mapper_override_mapping($node);
  feedapi_mapper_save_unique($param, $key, !$mapping['unique'][$key]);
  drupal_goto($path);
}

/**
 * Form callback confirmation form for fall back to default.
 */
function feedapi_mapper_default_form($form_state, $feed_node) {
  $form = array();
  $form['#node'] = $feed_node;
  $form['#redirect'] = "node/{$feed_node->nid}/map";

  return confirm_form($form,
    t('Use content-type settings?'),
    "node/{$feed_node->nid}/map",
    t('Are you sure you would like to delete this mapping and fall back to the !default? This action cannot be undone.', array('!default' => l(t('content-type settings'), 'admin/content/node-type/'. str_replace('_', '-', $feed_node->type) .'/map'))),
    t('Delete and use content-type settings'),
    t('Cancel')
  );
}

/**
 * Submission callback for feedapi_mapper_default_form.
 */
function feedapi_mapper_default_form_submit($form, &$form_state) {
  feedapi_mapper_delete_mapping($form['#node']->nid);
  drupal_set_message(t('Deleted mapping for this node. Fall back to content-type default.'));
}

/**
 * Form callback confirmation form for restrict mapping to the given node.
 */
function feedapi_mapper_restrict_form($form_state, $feed_node) {
  $form = array();
  $form['#node'] = $feed_node;
  $form['#redirect'] = "node/{$feed_node->nid}/map";

  return confirm_form($form,
    t('Change to per-node?'),
    "node/{$feed_node->nid}/map",
    t('Would you like to apply the mapping to the feed "!title" only?', array('!title' => $feed_node->title)),
    t('Change to per-node'),
    t('Cancel')
  );
}

/**
 * Submission callback for feedapi_mapper_restrict_form.
 */
function feedapi_mapper_restrict_form_submit($form, &$form_state) {
  $mapping = feedapi_mapper_load_mapping($form['#node']);
  feedapi_mapper_save_mapping($form['#node']->nid, $mapping);
}

/**
 * Callback function for hook_menu().
 */
function feedapi_mapper_page($node) {
  if (is_string($node)) {
    $node_type = $node;
    $node = new stdClass();
    $node->type = str_replace('-', '_', $node_type);
  }
  $names = node_get_types('names');
  drupal_set_title(check_plain($node->title ? $node->title : $names[$node->type]));
  $output = drupal_get_form('feedapi_mapper_form', $node);
  return $output;
}

/**
 * Mapping form.
 */
function feedapi_mapper_form($form_state, $feed_node) {

  $field_map = _feedapi_mapper_get_field_mappers($feed_node);

  // Get elements of feed items.
  $elements = array();
  if ($merged_item = _feedapi_mapper_get_items_merged($feed_node)) {
    $merged_item = _feedapi_mapper_simplify_raw($merged_item, $feed_node->feed->parsers);
    $elements = _feedapi_mapper_get_feed_elements($merged_item);
  }
  $elements = array_merge($elements, _feedapi_mapper_get_standard_elements());
  asort($elements);
  $feed_map = array_flip($elements);

  // Load stored mapping.
  $mapping = feedapi_mapper_load_mapping($feed_node);

  $form['#node'] = $feed_node;

  $form['#feed_map'] = $feed_map;
  $form['#field_map'] = $field_map;
  // Flatten array for theme_feedapi_mapper_form().
  foreach ($form['#field_map'] as $k => $v) {
    if (is_array($v)) {
      unset($form['#field_map'][$k]);
      $form['#field_map'] = array_merge($form['#field_map'], $v);
    }
  }
  $form['#mapping'] = $mapping;

  // Scope settings: if there is is a node we are on and there is no mapping at all,
  // we are operating on the content-type defaults. If not it's per-node.
  if ($feed_node->nid && _feedapi_mapper_scope($feed_node->nid)) {
    $scope = t('This mapping is specific to this feed node.');
    $scope_op = l(t('Use content-type mapping'), 'node/'. $feed_node->nid .'/map/default');
    $form['#scope'] = TRUE;
  }
  elseif (isset($feed_node->nid)) {
    $scope = t('This mapping applies to all content-types. Any changes will affect all feeds with the same content-type.');
    $scope_op = l(t('Change to per node'), 'node/'. $feed_node->nid .'/map/restrict');
    $form['#scope'] = FALSE;
  }

  if ($mapping['export_type'] === EXPORT_IN_DATABASE) {
    $status = t('Normal (This mapping is defined in the database).');
    $status_op = '&nbsp;';
  }
  if ($mapping['export_type'] === EXPORT_IN_CODE) {
    $status = t('Default (This mapping is defined in code).');
    $status_op = '&nbsp;';
  }
  if ($mapping['export_type'] === (EXPORT_IN_CODE | EXPORT_IN_DATABASE)) {
    $destination = isset($feed_node->nid) ? "node/{$feed_node->nid}/map" : "admin/content/node-type/{$feed_node->type}/map";
    $status = t('Overridden (There is a default mapping defined in code but it is overridden in the database)');
    $status_op = l(t('Revert'), 'admin/content/node-type/'. str_replace('_', '-', $feed_node->type) .'/map/revert', array('query' => array('destination' => $destination)));
  }

  $form['status_scope'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status and scope'),
  );
  $rows = array();
  $rows[] = array(
    '<strong>'. t('Status') .'</strong>', $status, $status_op,
  );
  $rows[] = array(
    '<strong>'. t('Scope') .'</strong>', $scope, $scope_op,
  );
  $form['status_scope']['value']['#value'] = theme('table', array(), $rows);

  // Create a placeholder for mapping form elements. This will be populated in
  // theme_feedapi_mapper_form().
  $form['mapping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mapping'),
  );

  // Filter out maps that already exist from our list since we don't support
  // multiple mappings for sources.
  $feed_map_options = array_merge(array(t('Select a source')), $feed_map);
  if (isset($mapping['mapping'])) {
    foreach ($mapping['mapping'] as $feed_path => $node_path) {
      if (isset($feed_map_options[$feed_path])) {
        unset($feed_map_options[$feed_path]);
      }
    }
  }
  $field_map_options = array_merge(array(t('Select a target')), $field_map);

  $form['source'] = array(
    '#type' => 'select',
    '#options' => $feed_map_options,
  );
  $form['target'] = array(
    '#type' => 'select',
    '#options' => $field_map_options,
  );
  $form['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  // Print descriptions for available mapping targets.
  // This is done by iterating through $field_map and extracting module names from it.
  $descriptions = _feedapi_mapper_get_field_mappers_descriptions($feed_node);
  $descriptions_filtered = array();
  foreach ($field_map as $key => $target) {
    if (is_array($target)) {
      $key = unserialize(key($target));
    }
    else {
      $key = unserialize($key);
    }
    if (is_array($key)) {
      $module = $key[0];
      $descriptions_filtered[$module] = $descriptions[$module];
    }
  }

  if ($merged_item) {
    $form['feed_item'] = array(
      '#type' => 'fieldset',
      '#title' => t('Feed item example'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('All feed items of this feed merged into one. Here you can see which feed item elements are available for mapping. As this view is derived from the actual feed items on this feed, there might be more mappable elements than those listed here.'),
    );
    $form['feed_item']['item'] = array(
      '#type' => 'markup',
      '#value' => '<pre>'. print_r(_feedapi_mapper_truncate_array($merged_item), TRUE) .'</pre>',
    );
  }

  if ($descriptions = theme('feedapi_mapper_descriptions', $descriptions_filtered)) {
    $form['descriptions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Description of available mappers'),
      '#description' => t('This is a list of mappers available for the <em>feed item</em> content type for this feed.'),
      '#tree' => TRUE,
    );
    $form['descriptions']['descriptions'] = array(
      '#type' => 'markup',
      '#value' => $descriptions,
    );
  }

  return $form;
}

/**
 * Validate hook.
 */
function feedapi_mapper_form_validate($form, &$form_state) {
  if (empty($form_state['values']['source'])) {
    form_set_error('source', t('You need to specify a valid source from the dropdown list.'));
  }
  elseif (empty($form_state['values']['target'])) {
    form_set_error('target', t('You need to specify a valid target from the dropdown list.'));
  }
}

/**
 * Submit hook.
 */
function feedapi_mapper_form_submit($form, &$form_state) {
  $param = ($form['#node']->nid && $form['#scope']) ? $form['#node']->nid : $form['#node']->type;
  feedapi_mapper_add_mapping($param, $form_state['values']['source'], $form_state['values']['target'], FALSE);
}

/**
 * Callback function for hook_menu().
 */
function feedapi_mapper_export_page($form_state, $param) {
  if (is_object($param)) {
    if (_feedapi_mapper_scope($param->nid)) {
      drupal_set_message(t('Only per content-type mappings can be exported.'), 'warning');
      drupal_goto("node/{$param->nid}/map");
    }
    $param = $param->type;
  }
  ctools_include('export');
  $mapping = array();
  $mapping = ctools_export_object('feedapi_mapper', array_pop(ctools_export_load_object('feedapi_mapper', 'conditions', array('param' => $param))));    
  $form['show'] = array(
    '#title' => t('PHP code'),
    '#type' => 'textarea',
    '#default_value' => $mapping,
    '#description' => t('Use this code for implementing hook_feedapi_mapper_default().
                         See <a href="@install">README.txt</a> for more information',
			array('@install' => url(drupal_get_path('module', 'feedapi_mapper') . '/README.txt'))),
  );
  return $form;
}
