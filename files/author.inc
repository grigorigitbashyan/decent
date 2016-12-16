<?php
global $lang, $siteURL, $params, $prAll, $siteUserObj, $isLogin;

$articlesCat = new Category(124);

$tempUser = new Site_user($params[0], "random");
$authorID = $tempUser->GetID();
$authorName = $tempUser->GetFullName();
								
if ( $authorID )
{
	$userArticlesArr = $prAll->GetUserArticles($authorID, "0, 100");
	$userFollowers = $tempUser->GetMyFollowers();
	?>
	<div class="row">
		<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 text-center">
			<img src="images/avatar-large.png" alt="" class="photo-profile" width="300" height="300" />
			<h1><?php echo $authorName;?></h1>
			<?php
			if ( $isLogin )
			{
				$userID = $siteUserObj->GetID();
				if ( $userID==$authorID )
				{
					?>
					<em><?php echo count($userFollowers);?> followers</em>
					<?php
				}
				else 
				{
					$query = "select * from `user_tags` where `userID` = $userID and `tagID` = $authorID and `tagType` = 'author'";
					$res = DB::QueryToRow($query);
					if ( is_array($res) )
					{
						?>
						<a id="follow" class="underline" style="cursor:pointer;" data-id="<?php echo $authorID;?>">Un-follow</a> - <em id="followersCount"><?php echo count($userFollowers);?> followers</em>
						<?php
					}
					else 
					{
						?>
						<a id="follow" class="underline" style="cursor:pointer;" data-id="<?php echo $authorID;?>">Follow</a> - <em id="followersCount"><?php echo count($userFollowers);?> followers</em>
						<?php
					}
				}
			}
			else 
			{
				?>
				<em><?php echo count($userFollowers);?> followers</em>
				<?php
			}
			?>
			<hr />
		</div>
	</div>
	<div>
		<?php
		for ( $i=0; $i<count($userArticlesArr); $i++ )
		{
			?>
			<div class="row">
				<div class="col-lg-2 col-md-2 col-sm-2 col-xs-12 info-article">
					<span class="city color-blue">Nevada Center</span>
					<i class="time"> - <?php echo ComputeDate($userArticlesArr[$i]["published_date"]);?> </i>
					<?php
					if ( !$userArticlesArr[$i]["payment"] )
					{
						?>
						<span class="btn btn-default">Free</span>
						<?php
					}
					?>
				</div>
				<div class="col-lg-10 col-md-10 col-sm-10 col-xs-12 title-article">
					<a href="<?php echo $userArticlesArr[$i]["link"];?>">
						<h2><?php echo $userArticlesArr[$i]["name"];?></h2>
					</a>
					<?php
					if ( $userArticlesArr[$i]["picture1"]!="" && $userArticlesArr[$i]["picture1"]!="img/thumbnail.gif" )
					{
						?>
						<a href="<?php echo $userArticlesArr[$i]["link"];?>" class="title-img hidden-xs"><img src="<?php echo $userArticlesArr[$i]["picture1"];?>" class="pull-right"/></a>
						<?php
					}
					?>
				</div>
			</div>
			<?php
		}
		?>
		<!--<button type="button" class="btn btn-default more">Load more</button>-->
	</div>
	<?php
}
?>