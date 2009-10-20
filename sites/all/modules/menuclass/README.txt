$Id: README.txt,v 1.2 2009/01/05 14:47:29 daniboy Exp $

README.txt
==========
This module contains the back-end to add custom HTML classes to menu
items.



Installing
==========
#1 Put this module's directory in your site's modules directory (or
   in sites/all/modules) and activate it using Drupal's module
   administration.

#2 Add the following code to your theme's template.php
<code>
/**
 * Implementation of theme_menu_item_link().
 *
 * Integrates Menu Class
 */
function phptemplate_menu_item_link($link) {
  if (function_exists('menuclass_to_link')) {
    menuclass_to_link($link);
  }
  return theme_menu_item_link($link);
}
</code>
   
   If you already have a theme override of theme_menu_item_link() in
   your template php, add this code to the overriding function before
   rendering the links with l().
<code>
if (function_exists('menuclass_to_link')) {
  menuclass_to_link($link);
}
</code>
   
   If your overriding function doesn't use l() to render the links to
   HTML code, you can probably find out what to do all by yourself. 
   
   /----------------------------------------------------------------\
   | This step is very important! Without this step, no classes     |
   | will be added to your menu items.                              |
   \----------------------------------------------------------------/
   
   It should be noted that this step must be done for each active
   theme that you want to affect.



Usage
=====
Define Sets
Administer >> Menus >> Class Sets
-------------
Sets contain definitions of classes, each set can be activated for
different menus. Sets exist to prevent too classes from appearing in
menus where they aren't needed.


Define Classes
Administer >> Menus >> Class Sets >> List items (For each set)
--------------
Here you can add, remove and redefine classes that belong to a set.
The class definition is the actual text that will be appended to the
menu item, to the link's "class" attribute.


Activate sets
Administer >> Menus >> (Select a menu) >> Class sets selection
--------------------------------------------------------------
Activate the sets that you want this menu to use.


Add class to menu items
Administer >> Menus >> (Select a menu) >> Edit (For each menu item)
-------------------------------------------------------------------
For items that can have classes added to them, a new field in the
form will appear where you can choose a class definition from each
set that is active for the menu.

A core issue in Drupal prevents us from adding classes to new menu
items, making it possible only to add classes to existing menu items,
Which means that in order to add a class for a new item, one must
first create that menu item and then edit it again. Our apologies.



Known Issues
============
Does not work with the following modules:
  # DHTML Menu




// So Say We All.
