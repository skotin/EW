<?php
// $Id: views-view-field.tpl.php,v 1.1 2008/05/16 22:22:32 merlinofchaos Exp $
 /**
  * This template is used to print a single field in a view. It is not
  * actually used in default Views, as this is registered as a theme
  * function which has better performance. For single overrides, the
  * template is perfectly okay.
  *
  * Variables available:
  * - $view: The view object
  * - $field: The field handler object that can process the input
  * - $row: The raw SQL result that can be used
  * - $output: The processed output that will normally be used.
  *
  * When fetching output from the $row, this construct should be used:
  * $data = $row->{$field->field_alias}
  *
  * The above will guarantee that you'll always get the correct data,
  * regardless of any changes in the aliasing that might happen if
  * the view is modified.
  */
?>

<?php /*echo "<pre>"*/?>
<?php/* print $view-> result[0]->node_title;*/ ?>


   <?php // find an image to display
        if ($page == 0) {
        $find= "/src=\"(.+?)\"/i";
        $text2search= $view->result[0]->node_revisions_body;
        preg_match($find, $text2search, $matches);
        if ($matches[1]) {
            $imgttl = check_plain($title);
            $found_image = "<a href='$node_url'><img src='".$matches[1]."' style='float: left; padding: 5px;' alt='$imgttl' title='$imgttl' class='teaser'></a>";
        }
        else { $found_image = ""; }
        }
     ?>
     <?php
            $img_name=explode('/', $matches[1]);
            $imgr = array_reverse($img_name);
            $img = $imgr[0];
            $imagecache_path =  file_create_url(file_directory_path().'/'.$img);
            print theme('imagecache', 'teasr', $img, '', 'Иллюстрация', array('class' => 'teaser'));
     ?>
<?php
 print l($view->result[0]->node_revisions_teaser." <img src=\"themes/ost2/images/readmore.gif\" class=\"readmore\" />", 'node/'.$view->result[0]->nid,  array('attributes' => array(), 'html'=>TRUE));

// print_r($view->field['view_node']);
// print_r($view);
//    $view->field['teaser_1']->render($row);
?>

<?php /*echo "</pre>"*/?>

