<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $picture ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>




  <div class="content clear-block">
    <?php print $content ?>
  </div>
   <div class="signature">
   <?php
   $user_load = user_load($array = array('uid' => $node->uid));
   if($user_load->signature) {
   echo '<div id="user_sig">'.$user_load->signature.'</div>';
   }
   ?>

   </div>

   <div class="clear-block">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>

<!--    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>-->
  </div>


</div>
