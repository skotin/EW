  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
    <?php if ($picture) {
      print $picture;
    }?>
    <?php if ($page == 0) { ?><h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a></h2><?php }; ?>
    <?php
      // Load author details
      $author = user_load(array('uid' => $node->uid));
    ?>
    <span class="submitted">From
    <?php
      // We override the default user link here, because we want the link
      // to point to the user's gallery, rather than their account page.
      print l($author->name, "photos/user/$node->uid");
    ?>
    on
    <?php 
      // Let's format a nice date without the time (Aug 16, 2006)
      print format_date($node->created, 'custom', 'M d, Y'); 
    ?>
    </span>
    <span class="taxonomy">Tags: 
    <?php
      // Rather than simply print $terms, we want to control where
      // the taxonomy links lead; therefore, we'll compile the list
      // manually.
      $term_links = array();
      foreach ($node->taxonomy as $term) {
        $term_links[] = l($term->name, "photos/tags/$term->name");
      }
      // "implode" makes the terms comma-separated
      print implode(', ', $term_links);
     ?></span>
    <div class="content"><?php print $content?></div>
    <?php if ($links) { ?><div class="links">&raquo; <?php print $links?></div><?php }; ?>
  </div>
