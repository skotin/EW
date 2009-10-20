Introduction to Basic

The Basic theme was originally developed for internal use at Raincity Studios for client 
projects. The purpose here is to have a very minimal theme that only contains 
the functions that are used for all websites. This theme is not intended to have 
subthemes, or to be another version of Zen. Basic is only intended to provide an 
extremely clean and flexible start for a Drupal themer.

__________________________________________________________________________________________

Feature List (6.x)

- flexible and simple info file

- Body classes.

	1. front or not-front classes
	2. logged-in or not-logged-in classes
	3. node-type-CONTENT_TYPE class: for example, node-type-page, node-type-story and node-type-forum
	4. two-sidebars, one-sidebar sidebar-left, one-sidebar sidebar-right, or no-sidebars classes
	5. page-URL class
	6. section-FIRST-Arg class
	7. section-node-add, section-node-edit, or section-node-delete classes for node add, edit, and delete pages
	8. Browser class (new), add a clas of the user browser like 'broswer-firefox3'

- Node classes.

	1. node-mine
	2. node-type
	3. node-unpublished
	4. ntype-[node type]
	5. taxonomy
 
- Block Classes

	1. block-[block module]
	2. region-[region]
	3. odd / even
 
- Comment classes

	1. node-author
	2. odd / even
	3. new
	4. mine

- Block editing links. Users with permission to edit blocks will see, when 
hovering over any block, links to edit that block. This is much more intuitive 
than first going to admin/build/blocks.

- Zen Tabs
- Minimal regions : Header / Footer / content top / content bottom / 
sidebar left / sidebar right
- 3/2/1 columns layout
- liquid or fixed layout by just deleting 1 line
- id and classes for all menu items
- folder architecture : css / images
- themable breadcrumb
- themable pager
- themable feed icon
- themable comment wrapper

__________________________________________________________________________________________


Installation

- Download Basic from http://drupal.org/project/basic
- Unpack the downloaded file and place the Basic folder in your Drupal 
installation under one of the following locations:

    * sites/all/themes
    * sites/default/themes
    * sites/example.com/themes 

- Log in as an administrator on your Drupal site and go to 
Administer > Site building > Themes (admin/build/themes) and make Basic the 
default theme.

If you want, you can rename the basic folder to your website name, but in 
version 5, remember to also change the function basic_regions at the begining 
of the template.php file to [name of your theme]_regions. For Drupal 6, 
remember to edit the info file to change the name of the theme.


__________________________________________________________________________________________


Modify the Layout

The purpose of this method is to have a minimal markup for an ideal display. 
For accessibility and search engine optimization, the best order to display a 
page is the following :

1. Header
2. Content
3. Navigation menus
4. Sidebar Left
5. Sideabr Right

To change the Layout, edit the layout.css file in the css folder. You might also
have to edit the page.tpl.php file to change the markup of the page.

We made the markup and css in a way that it is very easy and fast to modify it.
You can change the width of the whole layout or just a specific sidebar by just 
changing very few value in the css. And it'll still work cross-browsers.

For example, to switch from a fluid to a fixed layout, you only have to edit the
width of one element (#page - layout.css - line 30). If you want a fluid layout, just
set the width of #page to 'auto'. If you want to set the width of the layout to 
1000px, just set the width of #page to 1000px.

To change the width of a sidebar, just modify the corresponding values of this sidebar
in layout.css. Each line to modify is commented with its corresponding dependency. 
For example, If you want to change the width of the right sidebar to 250px, change the
value of each line commented "RIGHT Sidebar width" to 250px, just make sure that
the negative values remains negative.

You can also easily change the inner padding of each column by changing the padding
value of the .inner elements, and this, without changing the width of the columns.
You can also change the inner padding of all column (content + sidebars) at once 
by changing the value of .inner (.inner - layout.css).

The Navigation Menu, even though it is loaded after the content, appears before it,
by using negative margins and floating. However, to compensate the space it is
using, the content and the sidebars have to have a top margin equal to the height of 
the Navigation menu. Remember to change all the values commented "Nav menu height" 
if you want to modify the height of the Navigation menu.

Floats and Clears
------------------

As all elements of the content are floating (#content and both sidebars), if you set
an element to clear either left, right or both sides in one of these three containers,
it will only clear the other elements that are in the same container. For example, 
If I have an image floating in the content followed by a div that clears floated 
elements on both sides, the div will not clear the sidebars or any floated elements
in the sidebars. This allows to float elements and clear them without destroying
the layout.


__________________________________________________________________________________________