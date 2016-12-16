<?php
// version 1.0
global $lang, $siteURL, $params, $prAll;

$articlesCountInPage = 2;

$searchWord = trim(DB::POST('searchWord'));
$tags = $_POST["tags"];

$lastPrArrAll = $prAll->SearchProducts($tags, $searchWord, "0, 10000");
$lastPrArr = $prAll->SearchProducts($tags, $searchWord, "0, ".$articlesCountInPage);

if ( is_array($lastPrArr) )
{
	for ( $i=0; $i<count($lastPrArr); $i++ )
	{
		?>
		<div class="row" style="min-height:170px;">
			<div class="col-lg-2 col-md-2 col-sm-2 col-xs-12 info-article">
				<span class="city color-blue">Nevada Center</span>
				<i class="time"> - <?php echo ComputeDate($lastPrArr[$i]["published_date"]);?></i>
				<?php
				if ( !$lastPrArr[$i]["payment"] )
				{
					?>
					<span class="btn btn-default">Free</span>
					<?php
				}
				?>
			</div>
			<div class="col-lg-10 col-md-10 col-sm-10 col-xs-12 title-article">
				<a href="<?php echo $lastPrArr[$i]["link"];?>">
					<h2><?php echo $lastPrArr[$i]["name"];?></h2>
				</a>
				<?php
				if ( $lastPrArr[$i]["picture1"]!="" && $lastPrArr[$i]["picture1"]!="img/thumbnail.gif" )
				{
					?>
					<a href="<?php echo $lastPrArr[$i]["link"];?>" class="title-img hidden-xs"><img src="<?php echo $lastPrArr[$i]["picture1"];?>" class="pull-right"/></a>
					<?php
				}
				?>
			</div>
		</div>
		<?php
	}
	
	if ( count($lastPrArrAll)>$articlesCountInPage )
	{
		?>
		<button type="button" class="btn btn-default more" id="searchMore" data-page="1" data-search="1">Load more</button>
		<?php
	}
}
else 
{
	?>
	<h2>No results</h2>
	<?php
}
?>