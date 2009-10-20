$Id: README.txt,v 1.2.2.3 2009/09/29 19:00:09 alexb Exp $

Feed Element Mapper
===================

Compatible
==========

* FeedAPI Node processor >= 1.8
  http://drupal.org/project/feedapi
* FeedAPI Data processor
  http://drupal.org/project/feedapi_data

About
=====

Add-on module for FeedAPI that maps elements on a feed item such as tags or the 
author name to taxonomy or CCK fields. These mappings are configurable by point 
and click.

Tutorials
=========

Blog post with screen cast explaining how to use Feed Element Mapper 1.0.

http://www.developmentseed.org/blog/2007/oct/30/pick-it-feed-stick-it-node

API Documentation
=================

For implementing mappers refer to API documentation in feedapi_mapper.api.php

Exportables
===========

You can export mappings to code by going to node/[id]/map/export and copying 
the code from the text area on this site.

This code can be imported to another site via a default hook: 

function my_module_feedapi_mapper_default() {
  $feedapi_mapper = new ;
  $feedapi_mapper->disabled = FALSE; /* Edit this to true to make a default feedapi_mapper disabled initially */
  $feedapi_mapper->api_version = 1;
  $feedapi_mapper->param = '';
  $feedapi_mapper->mapping = '';
  $feedapi_mapper->unique_elements = '';
  return array('my_name' => $feedapi_mapper);
}

Exportables are using CTools/export, so you will also need to import 
hook_ctools_plugin_api():

function my_module_ctools_plugin_api($module, $api) {
  if ($module == 'feedapi_mapper' && $api == 'feedapi_mapper_default') {
    return array(
      'version' => 1,
      'file' => 'my_module.defaults.inc',
      'path' => drupal_get_path('module', 'my_module'),
    );
  }
}
