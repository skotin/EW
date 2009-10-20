<?php
// $Id: views-view-unformatted.tpl.php,v 1.6 2008/10/01 20:52:11 merlinofchaos Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>


<?php $exclude = views_get_view('general_article_view');
  $exclude->args = $view->args;
  $exclude->execute();
  $genart= $exclude->result[0]->nid;

  ?>

<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php foreach ($rows as $id => $row): ?>
  <div class="<?php print $classes[$id]; ?>">
  <?php $art=$view->result[$id]->nid; ?>
  <?php  if($genart!=$art): ?>
  <?php print $row; ?>
  <?php endif; ?>

  </div>
<?php endforeach; ?>


