<?php
// $Id: views-view.tpl.php,v 1.10 2008/09/22 20:50:58 merlinofchaos Exp $
/**
 * @file views-view.tpl.php
 * Main view template
 *
 * Variables available:
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 * - $admin_links: A rendered list of administrative links
 * - $admin_links_raw: A list of administrative links suitable for theme('links')
 *
 * @ingroup views_templates
 */
?>
<div class="view view-<?php print $css_name; ?> view-id-<?php print $name; ?> view-display-id-<?php print $display_id; ?> view-dom-id-<?php print $dom_id; ?>">
  <?php if ($admin_links): ?>
    <div class="views-admin-links views-hide">
      <?php print $admin_links; ?>
    </div>
  <?php endif; ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($exposed): ?>
    <div class="view-filters">
      <?php print $exposed; ?>
    </div>
  <?php endif; ?>

  <?php if ($attachment_before): ?>
    <div class="attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>
   <?php
echo "<div id=\"author-blok\">";?>
<?php
if ((arg(0) == 'node') && is_numeric(arg(1))) {
$args[0] = arg(1);
$node_id = $args[0];
$node = node_load($node_id);
$user_id = $node->uid;
} else if ((arg(0) == 'blog') && is_numeric(arg(1))) {
$args[0] = arg(1);
$user_id = $args[0];
}

$user_load = user_load($array = array('uid' => $user_id));
echo '<img src="/'.$user_load->picture.'" title="'.$user_load->name.'" /> <h3>'.$user_load->profile_displayName;
echo "'s Blog</h3>";
if($user_load->signature) {
echo '<div id="user_sig">'.$user_load->signature.'</div>';
}
?>
<?php echo "</div>";
?>
  <?php if ($rows): ?>
    <div class="view-content">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
  <?php endif; ?>

  <?php if ($attachment_after): ?>
    <div class="attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div> <?php // class view ?>
