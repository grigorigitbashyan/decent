<?php
$includes = array (
	'classes/Product/pr_main/Pr_mainListView.class.inc', 
	'classes/Product/pr_type/Pr_typeView.class.inc', 
	'classes/Product/pr_main/Pr_mainView.class.inc' 
);

Site::IncludeFiles ( $includes );

/**
 * This class is used for displaying product(s) in website
 *
 * @package Item
 */
class ProductAll extends View
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_catID;
	protected $m_catIDStr;
	
	/**
	 * Enter description here...
	 *
	 * @var Pr_mainView
	 */
	protected $m_productView;
	
	/**
	 * Enter description here...
	 *
	 * @param int $catID
	 * @param string $catPath
	 * @param bool $displaySingle
	 */
	function __construct($catID=0)
	{
		parent::__construct ();
		
		$this->m_catID = $catID;
		
		if ( $this->m_catID == 0 ) {
			
			$this->m_catIDStr = "";
		} elseif ( is_array($this->m_catID) ) {
			
			$this->m_catIDStr = "";
		} else {
			
			$this->m_catIDStr = "";
		}
	}
	
	function GetProducts($order1 = "date", $order1Type = "desc", $order2 = null, $order2Type = null, $limit = 10, $tagName = null)
	{
		$query = "
			select 
				PM.`ID`, 
				PM.`random` as link, 
				PM.`name_$this->m_displayLang` as name, 
				PM.`short_desc_$this->m_displayLang` as short_desc, 
				PM.`picture`, 
				PM.`date`, 
				S.`path` 
			from 
				`pr_main` PM 
			join 
				`pr_options` PO on (PM.`ID` = PO.`productID`)
			join
				`structure` S on (PO.`relatedID` = S.`ID`) 
			where 
				PM.`status` = 1 
			order by 
				PM.`$order1` $order1Type limit 0, $limit";
		$ProdArr = DB::QueryToArray($query);
		
		foreach ( $ProdArr as $prodItem )
		{
			$this->DisplayItem($prodItem, $tagName);
		}
	}
	
	function GetMostViewedProducts($limit="0, 3", $forDays=7, $exceptionIDs=null)
	{
		$selDate = time() - $forDays*86400;
		$selDate = date("Y-m-d", $selDate);
		
		$exceptionIDs_str = "";
		if ( is_array($exceptionIDs) )
		{
			$exceptionIDs = implode(", ", $exceptionIDs);
			$exceptionIDs_str = " and `ID` not in ($exceptionIDs)";
		}
		
		$query = "
			select 
				`ID`, 
				`random` as `link`, 
				`name_$this->m_displayLang` as `name`, 
				`short_desc_$this->m_displayLang` as `short_desc`, 
				`desc_$this->m_displayLang` as `desc`, 
				`picture`, 
				`picture1`, 
				`picture2`, 
				`cover`, 
				`published_date`, 
				`updated_date`, 
				`viewCount`, 
				`googleViewCount`, 
				`googleUniqueViewCount` 
			from 
				`pr_main` 
			where 
				`status` = 1 and `picture` <> 'img/thumbnail.gif' and `date` > '$selDate' $exceptionIDs_str
			order by 
				`viewCount` desc, `ID` desc
			limit ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	
	function GetRandomProducts($limit="0, 3", $forDays=7, $exceptionIDs=null)
	{
		$selDate = time() - $forDays*86400;
		$selDate = date("Y-m-d", $selDate);
		
		$exceptionIDs_str = "";
		if ( is_array($exceptionIDs) )
		{
			$exceptionIDs = implode(", ", $exceptionIDs);
			$exceptionIDs_str = " and `ID` not in ($exceptionIDs)";
		}
		
		$query = "
			select 
				`ID`, 
				`random` as `link`, 
				`name_$this->m_displayLang` as `name`, 
				`short_desc_$this->m_displayLang` as `short_desc`, 
				`desc_$this->m_displayLang` as `desc`, 
				`picture`, 
				`picture1`, 
				`picture2`, 
				`cover`, 
				`published_date`, 
				`updated_date`, 
				`viewCount`, 
				`googleViewCount`, 
				`googleUniqueViewCount` 
			from 
				`pr_main` 
			where 
				`status` = 1 and `picture` <> 'img/thumbnail.gif' and `published_date` > '$selDate' $exceptionIDs_str
			order by 
				rand() 
			limit ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	
	function GetSiteUserProducts($userID, $limit=3, $exceptionIDs=null)
	{
		//	get user last activity date
		$query = "select `lasttime` from `user_tags` where `userID` = $userID and `tagType` = 'tag' order by `lasttime` desc limit 0, 1";
		$lastTime = DB::QueryToValue($query);
		
		//	count one week from user last activity date
		$lastTime = $lastTime - 7*86400;
		
		//	get 3 most popular user tags from counted perioud
		$tagsArr = "";
		$query = "select * from `user_tags` where `userID` = $userID and `tagType` = 'tag' and `lasttime` > $lastTime order by `count` desc limit 0, 3";
		$retArr = DB::QueryToArray($query);
		for ( $i=0; $i<count($retArr); $i++ )
		{
			$tagsArr[] = $retArr[$i]["tagID"];
		}
		
		//	select 3 most popular articles from selected tags from last week
		if ( is_array($tagsArr) )
		{
//			$fromDate = time() - 7*86400 - 1;
			$fromDate = time() - 365*86400 - 1;
			$fromDate = date("Y-m-d", $fromDate);
			
			$tagsArr = implode(", ", $tagsArr);
			
			$exceptionIDs_str = "";
			if ( is_array($exceptionIDs) )
			{
				$exceptionIDs = implode(", ", $exceptionIDs);
				$exceptionIDs_str = " and PM.`ID` not in ($exceptionIDs)";
			}
			
			$query = "
				select 
					PM.`ID`, 
					PM.`random` as `link`, 
					PM.`name_$this->m_displayLang` as `name`, 
					PM.`short_desc_$this->m_displayLang` as `short_desc`, 
					PM.`desc_$this->m_displayLang` as `desc`, 
					PM.`picture`, 
					PM.`picture1`, 
					PM.`picture2`, 
					PM.`cover`, 
					PM.`published_date`, 
					PM.`updated_date`, 
					PM.`viewCount`, 
					PM.`googleViewCount`, 
					PM.`googleUniqueViewCount` 
				from 
					`pr_main` PM 
				join 
					`pr_options` PO on (PM.`ID` = PO.`productID` and PO.`fieldID` = 71)
				where 
					PM.`status` = 1 and PO.`relatedID` in ($tagsArr) and PM.`published_date` > '$fromDate' and PM.`ID` not in (select `productID` from `viewedArticles` where `userID` = $userID) $exceptionIDs_str 
				group by 
					PM.`ID`
				order by 
					PM.`viewCount` desc
				limit 0, ".$limit;
		}
		else 
		{
			$toDate = time() - 7*86400 - 1;
			$toDate = date("Y-m-d", $toDate);
			
//			$fromDate = time() - 30*86400;
			$fromDate = time() - 365*86400;
			$fromDate = date("Y-m-d", $fromDate);
			
			$exceptionIDs_str = "";
			if ( is_array($exceptionIDs) )
			{
				$exceptionIDs = implode(", ", $exceptionIDs);
				$exceptionIDs_str = " and `ID` not in ($exceptionIDs)";
			}
			
			$query = "
				select 
					`ID`, 
					`random` as `link`, 
					`name_$this->m_displayLang` as `name`, 
					`short_desc_$this->m_displayLang` as `short_desc`, 
					`desc_$this->m_displayLang` as `desc`, 
					`picture`, 
					`picture1`, 
					`picture2`, 
					`cover`, 
					`published_date`, 
					`updated_date`, 
					`viewCount`, 
					`googleViewCount`, 
					`googleUniqueViewCount` 
				from 
					`pr_main` 
				where 
					`status` = 1 and `picture` <> 'img/thumbnail.gif' and `published_date` > '$fromDate' and `published_date` < '$toDate' $exceptionIDs_str 
				order by 
					`viewCount` desc
				limit 0, ".$limit;
		}
		
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	
	function GetSiteUserProductsByAuthor($userID, $exceptionIDs=null)
	{
		//	get user last activity date
		$query = "select `lasttime` from `user_tags` where `userID` = $userID and `tagType` = 'author' order by `lasttime` desc limit 0, 1";
		$lastTime = DB::QueryToValue($query);
		
		//	count one week from user last activity date
		$lastTime = $lastTime - 7*86400;
		
		//	get user authors from counted perioud
		$tagsArr = "";
		$query = "select * from `user_tags` where `userID` = $userID and `tagType` = 'author' and `lasttime` > $lastTime order by `count` desc";
		$retArr = DB::QueryToArray($query);
		for ( $i=0; $i<count($retArr); $i++ )
		{
			$tagsArr[] = $retArr[$i]["tagID"];
		}
		
		$exceptionIDs_str = "";
		if ( is_array($exceptionIDs) )
		{
			$exceptionIDs = implode(", ", $exceptionIDs);
			$exceptionIDs_str = " and `ID` not in ($exceptionIDs)";
		}
		
		//	select 3 most popular articles from selected tags from last week
		$ProdArr = "";
		if ( is_array($tagsArr) )
		{
//			$fromDate = time() - 7*86400 - 1;
			$fromDate = time() - 365*86400 - 1;
			$fromDate = date("Y-m-d", $fromDate);
			
			$tagsArr = implode(", ", $tagsArr);
			
			$query = "
				select 
					`ID`, 
					`random` as `link`, 
					`name_$this->m_displayLang` as `name`, 
					`short_desc_$this->m_displayLang` as `short_desc`, 
					`desc_$this->m_displayLang` as `desc`, 
					`picture`, 
					`picture1`, 
					`picture2`, 
					`cover`, 
					`published_date`, 
					`updated_date`, 
					`viewCount`, 
					`googleViewCount`, 
					`googleUniqueViewCount` 
				from 
					`pr_main` 
				where 
					`status` = 1 and `userID` in ($tagsArr) and `published_date` > '$fromDate' and `ID` not in (select `productID` from `viewedArticles` where `userID` = $userID) $exceptionIDs_str 
				group by 
					`userID`
				order by 
					`viewCount` desc
				limit 0, 3";
			
			$ProdArr = DB::QueryToArray($query);
		}
		
		return $ProdArr;
	}
	
	function GetLastProducts($limit="0, 4", $cover=false)
	{
		$coverStr = "";
		if ( $cover )
		{
			$coverStr = " and `cover` <> 'img/cover.gif'";
		}
		
		$query = "
			select 
				`ID`, 
				`random` as `link`, 
				`name_$this->m_displayLang` as `name`, 
				`short_desc_$this->m_displayLang` as `short_desc`, 
				`desc_$this->m_displayLang` as `desc`, 
				`picture`, 
				`picture1`, 
				`picture2`, 
				`cover`, 
				`published_date`, 
				`updated_date`, 
				`viewCount`, 
				`payment`
			from 
				`pr_main` 
			where 
				`typeID` = 1 and `status` = 1 $coverStr
			order by 
				`date` desc, `ID` desc
			limit ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	
	function GetUserArticles($userID, $limit="0, 9")
	{
		$query = "
			select 
				`ID`, 
				`random` as `link`, 
				`name_$this->m_displayLang` as `name`, 
				`short_desc_$this->m_displayLang` as `short_desc`, 
				`desc_$this->m_displayLang` as `desc`, 
				`picture`, 
				`picture1`, 
				`picture2`, 
				`cover`, 
				`published_date`, 
				`updated_date`, 
				`viewCount`, 
				`payment`
			from 
				`pr_main` 
			where 
				`status` = 1 and `typeID` = 1 and `userID` = $userID 
			order by 
				`date` desc, `ID` desc
			limit ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	function GetUserArticlesViewCount($userID)
	{
		$query = "
			select 
				SUM(`googleUniqueViewCount`) as `total` 
			from 
				`pr_main` 
			where 
				`status` = 1 and `userID` = $userID";
		
		return DB::QueryToValue($query);
	}
	function GetUserMostViewedArticle($userID, $cover=true)
	{
		$coverStr = "";
		if ( $cover )
		{
			$coverStr = " and `cover` <> '' and `cover` <> 'img/cover.gif'";
		}
		$query = "
			select 
				`ID`, 
				`random` as `link`, 
				`name_$this->m_displayLang` as `name`, 
				`short_desc_$this->m_displayLang` as `short_desc`, 
				`desc_$this->m_displayLang` as `desc`, 
				`picture`, 
				`picture1`, 
				`picture2`, 
				`cover`, 
				`published_date`, 
				`updated_date`, 
				`viewCount`, 
				`googleViewCount`, 
				`googleUniqueViewCount` 
			from 
				`pr_main` 
			where 
				`status` = 1 and `userID` = $userID $coverStr
			order by 
				`googleUniqueViewCount` desc
			limit 0, 1";
		$ProdArr = DB::QueryToRow($query);
		
		return $ProdArr;
	}
	
	function GetProductsByTypes($typeIDs, $fieldIDs=0, $orderBy="PM.`date`", $orderType="desc", $limit = 4, $relatedID=0)
	{
		$orderStr = "";
		if ( $orderBy=="random" )
		{
			$orderStr = " rand()";
		}
		else 
		{
			$orderStr = " $orderBy $orderType, PM.`ID` desc";
		}
		
		if ( is_array($typeIDs) )
		{
			$conStr = "";
			for ( $i=0; $i<count($typeIDs); $i++ )
			{
				if ( $i==0 )
				{
					$conStr .= "(PM.`typeID` = ".$typeIDs[$i]." and PO2.`fieldID` = ".$fieldIDs[$i].")";
				}
				else 
				{
					$conStr .= " or (PM.`typeID` = ".$typeIDs[$i]." and PO2.`fieldID` = ".$fieldIDs[$i].")";
				}
			}
		}
		else 
		{
			if ( $fieldIDs==0 )
			{
				$conStr = "PM.`typeID` = $typeIDs";
			}
			else 
			{
				$conStr = "PM.`typeID` = $typeIDs and PO2.`fieldID` = $fieldIDs";
			}
		}
		
		$relatedIDStr = "";
		if ( $relatedID )
		{
			$relatedIDStr = " and PO2.`relatedID` = ".$relatedID;
		}
		
		if ( $fieldIDs==0 )
		{
			$query = "
				select 
					PM.`ID`, 
					PM.`random` as `link`, 
					PM.`name_$this->m_displayLang` as `name`, 
					PM.`subtitle_$this->m_displayLang` as `subtitle`, 
					PM.`short_desc_$this->m_displayLang` as `short_desc`, 
					PM.`desc_$this->m_displayLang` as `desc`, 
					PM.`picture`, 
					PM.`cover`, 
					PM.`date`
				from 
					`pr_main` PM 
				where 
					$conStr 
				order by 
					$orderStr 
				limit 0, ".$limit;
		}
		else 
		{
			$query = "
				select 
					PM.`ID`, 
					PM.`random` as `link`, 
					PM.`name_$this->m_displayLang` as `name`, 
					PM.`subtitle_$this->m_displayLang` as `subtitle`, 
					PM.`short_desc_$this->m_displayLang` as `short_desc`, 
					PM.`desc_$this->m_displayLang` as `desc`, 
					PM.`picture`, 
					PM.`cover`, 
					PM.`date` 
				from 
					`pr_main` PM 
				join 
					`pr_options` PO2 on (PO2.`productID` = PM.`ID`)
				where 
					$conStr $relatedIDStr 
				order by 
					$orderStr 
				limit 0, ".$limit;
		}
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	
	function GetLastProductsWithException($exceptTypeIDs=null, $orderBy="PM.`date`", $orderType="desc", $limit = 15)
	{
		$orderStr = "";
		if ( $orderBy=="random" )
		{
			$orderStr = " rand()";
		}
		else 
		{
			$orderStr = " $orderBy $orderType, PM.`ID` desc";
		}
		
		if ( $exceptTypeIDs!=null )
		{
			if ( is_array($exceptTypeIDs) )
			{
				$conStr = " and (PM.`typeID` > 0 ";
				for ( $i=0; $i<count($exceptTypeIDs); $i++ )
				{
					$conStr .= " and PM.`typeID` <> ".$exceptTypeIDs[$i]."";
				}
				$conStr .= ")";
			}
			else 
			{
				$conStr = " and PM.`typeID` <> $exceptTypeIDs";
			}
		}
		
		$query = "
			SELECT PM.`ID`, PM.`typeID`, PM.`name_{$this->m_displayLang}` as `name`, PM.`desc_{$this->m_displayLang}` as `desc`, PM.`short_desc_{$this->m_displayLang}` as `short_desc`, PM.`picture`, PM.`date` as `sdate`, PM.`date`, PM.`random`, CONCAT_WS('', S.`path`, OD.`key`) as `path`, OD.`name_{$this->m_displayLang}` as `path_name`
			FROM `pr_main` PM 
			JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
			JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID`)
			JOIN `options` O ON (O.`ID` = OD.`optionsID` and O.`category_id` > 0) 
			JOIN `structure` S ON (S.`ID` = O.`category_id`) 
			WHERE PM.`status` = 1 $conStr 
			GROUP BY PM.`ID` 
			ORDER BY ".$orderStr;
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	
	function GetProductsByType($typeID, $fieldID, $orderBy="PM.`date`", $orderType="desc", $limit = 4)
	{
		$orderStr = "";
		if ( $orderBy=="random" )
		{
			$orderStr = " rand()";
		}
		else 
		{
			$orderStr = " $orderBy $orderType, PM.`ID` desc";
		}
		
		$query = "
			select 
				PM.`ID`, 
				PM.`random` as `link`, 
				PM.`name_$this->m_displayLang` as `name`, 
				PM.`short_desc_$this->m_displayLang` as `short_desc`, 
				PM.`desc_$this->m_displayLang` as `desc`, 
				PM.`picture`, 
				PM.`cover`, 
				PM.`date`, 
				S.`name_$this->m_displayLang` as path_name, 
				S.`path` 
			from 
				`pr_main` PM 
			join 
				`pr_options` PO2 on (PO2.`productID` = PM.`ID`)
			join
				`structure` S on (PO2.`relatedID` = S.`ID`) 
			where 
				PM.`typeID` = $typeID and PM.`status` = 1 and PO2.`fieldID` = $fieldID 
			order by 
				$orderStr 
			limit 0, ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	
	/**
	 * This function swithch what to display, list or item
	 *
	 * @param array $params
	 * @param string $addLink
	 */
	function Display($params, $displayGroup = false)
	{
		// if params contain number
		$this->SetProductView ( $params );
		
		if (!$this->DisplayItem ())
		{
			$this->DisplayList ( $params, $displayGroup );
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductName($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetName ( $this->m_displayLang );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $prID
	 */
	protected function DisplayItem($prodItem, $tagName = null)
	{
		if ( $tagName!=null ) {
			
			print ('<'.$tagName.'>');
		}
		?>
		<div><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>" title=""><img src="<?php echo $prodItem["picture"];?>" alt="<?php echo $prodItem["name"];?>" /></a></div>
        <p><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>"><?php echo $prodItem["name"];?></a></p>
		<?php
		if ( $tagName!=null ) {
			
			print ('</'.$tagName.'>');
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $prID
	 */
	protected function DisplayLastItem($prodItem, $tagName = null)
	{
		if ( $tagName!=null ) {
			
			print ('<'.$tagName.' class="latest-article-item-set">');
		}
		?>
		<div class="latest-article-item-path"><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"];?>" title=""><?php echo $prodItem["path_name"];?></a></div>
        <div class="latest-article-item">
            <div class="latest-article-item-img"><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>" title=""><img src="<?php echo $prodItem["picture"];?>" alt="<?php echo $prodItem["name"];?>" width="120" /></a></div>
            <div class="latest-article-item-title"><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>"><?php echo $prodItem["name"];?></a></div>
            <div class="latest-article-item-desc"><?php echo $prodItem["short_desc"];?></div>
        </div>
        <div class="read-more"><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>"><?php echo ($this->m_displayLang=="en" ? "Continue" : "გაგრძელება");?>...</a></div>
		<?php
		if ( $tagName!=null ) {
			
			print ('</'.$tagName.'>');
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $prID
	 */
	protected function DisplayLastItemByCat($prodItem, $tagName = null)
	{
		?>
           <div class="latestArticleCatItemTitle"><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>"><?php echo $prodItem["name"];?></a></div>
		<?php
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $prID
	 */
	protected function DisplayMostViewedItem($prodItem, $tagName = null, $index=1)
	{
		if ( $tagName!=null ) {
			
			print ('<'.$tagName.'>');
		}
		?>
		<a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>"><?php echo $prodItem["name"];?></a>
		<?php
		if ( $tagName!=null ) {
			
			print ('</'.$tagName.'>');
		}
	}
	
	function DisplayGroup()
	{
		// get sub meni items
		$cat = new Category ( $this->m_catID );
		$menuID = $cat->GetMenuID ();
		
		$subMenu = array ();
		$menu = new Menu ( $menuID, true, $this->m_displayLang );
		$menu->GetSubMenu ( $this->m_catID, $subMenu, $menu->GetStructure () );
		
		$prTypeView = new pr_typeView ( );
		
		if ($subMenu)
		{
			foreach ( $subMenu as $menuItem )
			{
				$currCatName = $menuItem ['name'];
				$currCatPath = $menuItem ['path'];
				
				$currCatPath = $this->m_displayLang . '/' . $currCatPath;
				
				$productList = new Pr_mainListView ( $menuItem ['ID'], $menuItem ['path'], 1 );
				$prList = $productList->GetProductList ();
				
				if ($prList->GetAllCount () > 0)
				{
					$products = $prList->GetStructure ();
					
					$product = new Pr_main ( $products [0] ['ID'] );
					
					$prView = $prTypeView->MakeGroupView ( $product, $currCatPath, $currCatName );
					
					print $prView;
				}
			}
		}
	}
	
	
	function GetArticlesCountForTag($tagID)
	{
		$query = "
			SELECT PM.`ID`, PM.`typeID`, PM.`name_{$this->m_displayLang}` as `name`, PM.`desc_{$this->m_displayLang}` as `desc`, PM.`short_desc_{$this->m_displayLang}` as `short_desc`, PM.`picture`, PM.`picture1`, PM.`picture2`, PM.`picture3`, PM.`picture4`, PM.`cover`, PM.`date` as `sdate`, PM.`published_date`, PM.`updated_date`, PM.`random`, PM.`viewCount`, PM.`googleViewCount`, PM.`googleUniqueViewCount` 
			FROM `pr_main` PM 
			JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
			JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID` and OD.`ID` = $tagID and OD.`optionsID` = 28) 
			WHERE PM.`status` = 1 
			GROUP BY PM.`ID`";
		
		return count(DB::QueryToArray($query));
	}
	function GetAuthorsCountForTag($tagID)
	{
		$query = "
			SELECT PM.`ID`, PM.`typeID`, PM.`name_{$this->m_displayLang}` as `name`, PM.`desc_{$this->m_displayLang}` as `desc`, PM.`short_desc_{$this->m_displayLang}` as `short_desc`, PM.`picture`, PM.`picture1`, PM.`picture2`, PM.`picture3`, PM.`picture4`, PM.`cover`, PM.`date` as `sdate`, PM.`published_date`, PM.`updated_date`, PM.`random`, PM.`viewCount`, PM.`googleViewCount`, PM.`googleUniqueViewCount` 
			FROM `pr_main` PM 
			JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
			JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID` and OD.`ID` = $tagID and OD.`optionsID` = 28) 
			WHERE PM.`status` = 1 
			GROUP BY PM.`userID`";
		
		return count(DB::QueryToArray($query));
	}
	function GetMostViewedArticleForTag($tagID, $coverExist = true)
	{
		$coverStr = "";
		if ( $coverExist )
		{
			$coverStr = " and PM.`cover` <> '' and PM.`cover` <> 'img/cover.gif'";
		}
		$query = "
			SELECT PM.`ID`, PM.`typeID`, PM.`name_{$this->m_displayLang}` as `name`, PM.`desc_{$this->m_displayLang}` as `desc`, PM.`short_desc_{$this->m_displayLang}` as `short_desc`, PM.`picture`, PM.`picture1`, PM.`picture2`, PM.`picture3`, PM.`picture4`, PM.`cover`, PM.`date` as `sdate`, PM.`published_date`, PM.`updated_date`, PM.`random`, PM.`viewCount`, PM.`googleViewCount`, PM.`googleUniqueViewCount` 
			FROM `pr_main` PM 
			JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
			JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID` and OD.`ID` = $tagID and OD.`optionsID` = 28) 
			WHERE PM.`status` = 1 $coverStr
			GROUP BY PM.`ID`
			ORDER BY `googleUniqueViewCount` desc
			LIMIT 0, 1";
		
		return DB::QueryToRow($query);
	}
	function GetSubscribersForTag($tagID)
	{
		$query = "select * from `user_tags` where `tagID` = $tagID and `tagType` = 'tag' and `follow` = 1";
		
		return DB::QueryToArray($query);
	}
	
	
	public function SearchProducts($tags=null, $searchWord=null, $limit="0, 10")
	{
		if ( is_array($tags) && count($tags)>0 )
		{
			$tags = implode(", ", $tags);
			if ( $searchWord )
			{
				$query = "
					select 
						PM.`ID`, 
						PM.`random` as `link`, 
						PM.`name_en` as `name`, 
						PM.`short_desc_en` as `short_desc`, 
						PM.`desc_en` as `desc`, 
						PM.`picture`, 
						PM.`picture1`, 
						PM.`picture2`, 
						PM.`cover`, 
						PM.`published_date`, 
						PM.`updated_date`, 
						PM.`viewCount`, 
						PM.`payment`
					from 
						`pr_main` PM 
					join 
						`pr_options` PO on (PO.`productID` = PM.`ID` and PO.`fieldID` = 1 and PO.`relatedID` in ($tags))
					where 
						PM.`typeID` = 1 and PM.`status` = 1 and (PM.`name_en` like '%$searchWord%' or `desc_en` like '%$searchWord%') 
					group by
						PM.`ID` 
					order by 
						PM.`date` desc, `ID` desc 
					limit 
						$limit
					";
				$lastPrArr = DB::QueryToArray($query);
			}
			else 
			{
				$query = "
					select 
						PM.`ID`, 
						PM.`random` as `link`, 
						PM.`name_en` as `name`, 
						PM.`short_desc_en` as `short_desc`, 
						PM.`desc_en` as `desc`, 
						PM.`picture`, 
						PM.`picture1`, 
						PM.`picture2`, 
						PM.`cover`, 
						PM.`published_date`, 
						PM.`updated_date`, 
						PM.`viewCount`, 
						PM.`payment`
					from 
						`pr_main` PM 
					join 
						`pr_options` PO on (PO.`productID` = PM.`ID` and PO.`fieldID` = 1 and PO.`relatedID` in ($tags))
					where 
						PM.`typeID` = 1 and PM.`status` = 1 
					group by
						PM.`ID` 
					order by 
						PM.`date` desc, `ID` desc 
					limit 
						$limit
					";
				$lastPrArr = DB::QueryToArray($query);
			}
		}
		elseif ( $searchWord )
		{
			$query = "
				select 
					PM.`ID`, 
					PM.`random` as `link`, 
					PM.`name_en` as `name`, 
					PM.`short_desc_en` as `short_desc`, 
					PM.`desc_en` as `desc`, 
					PM.`picture`, 
					PM.`picture1`, 
					PM.`picture2`, 
					PM.`cover`, 
					PM.`published_date`, 
					PM.`updated_date`, 
					PM.`viewCount`, 
					PM.`payment`
				from 
					`pr_main` PM 
				join 
					`pr_options` PO on (PO.`productID` = PM.`ID` and PO.`fieldID` = 1)
				where 
					PM.`typeID` = 1 and PM.`status` = 1 and (PM.`name_en` like '%$searchWord%' or `desc_en` like '%$searchWord%') 
				group by
					PM.`ID` 
				order by 
					PM.`date` desc, `ID` desc 
				limit 
					$limit
				";
			$lastPrArr = DB::QueryToArray($query);
		}
		else 
		{
			$lastPrArr = $this->GetLastProducts($limit);
		}
		
		return $lastPrArr;
	}
}
?>