<div id="block-<?php print $block->module .'-'. $block->delta ?>" class="block block-<?php print $block->module .' '. $block_zebra .' '. $block->region ?> clearfix">
  <div class="block-inner">

    <?php if (!empty($block->subject)): ?>
      <h3 class="title block-title"><?php print $block->subject; ?></h3>
    <?php endif; ?>

    <div class="content">
      <?php print $block->content; ?>
    </div>

    <?php print $edit_links; ?>

  </div> <!-- /block-inner -->
</div> <!-- /block -->