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
	
	function GetSpecialProducts($order1 = "date", $order1Type = "desc", $order2 = null, $order2Type = null, $tagName = null)
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
						`pr_options` PO2 on (PO2.`productID` = PO.`productID`)
					join
						`structure` S on (PO2.`relatedID` = S.`ID`) 
					where 
						PM.`special` = 1 and PM.`status` = 1 and PO2.`fieldID` = 24
					group by 
						PM.`ID` 
					order by 
						PM.`$order1` $order1Type";
		$ProdArr = DB::QueryToArray($query);
		
		foreach ( $ProdArr as $prodItem )
		{
			$this->DisplayItem($prodItem, $tagName);
		}
	}
	
	function GetHomeGalleryProductsList($limit=5)
	{
		$query = "
					select 
						PM.`ID`, 
						PM.`random`, 
						PM.`name_$this->m_displayLang` as name, 
						PM.`short_desc_$this->m_displayLang` as short_desc, 
						PM.`picture`, 
						PM.`cover`, 
						PM.`date`, 
						PT.`name` as `typename`, 
						S.`path` 
					from 
						`pr_main` PM 
					join 
						`pr_type` PT on (PT.`ID` = PM.`typeID`)
					join 
						`pr_options` PO on (PO.`productID` = PM.`ID`)
					join
						`structure` S on (PO.`relatedID` = S.`ID`) 
					where 
						PO.`fieldID` = 6 and PM.`homeGallery` = 1 and PM.`status` = 1 
					group by 
						PM.`ID` 
					order by 
						PM.`date` desc, PM.`ID` desc
					limit 0, ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
	
	function GetHomeGalleryProductsItems($divClass = null, $divId = null)
	{
		$query = "
					select 
						PM.`ID`, 
						PM.`random` as link, 
						PM.`name_$this->m_displayLang` as name, 
						PM.`short_desc_$this->m_displayLang` as short_desc, 
						PM.`picture`, 
						PM.`cover`, 
						PM.`date`, 
						S.`path` 
					from 
						`pr_main` PM 
					join 
						`pr_options` PO2 on (PO2.`productID` = PM.`ID`)
					join
						`structure` S on (PO2.`relatedID` = S.`ID`) 
					where 
						PO2.`fieldID` = 24 and PM.`homeGallery` = 1 and PM.`status` = 1 
					group by 
						PM.`ID` 
					order by 
						PM.`date` desc, PM.`ID` desc
					limit 0, 6";
		$ProdArr = DB::QueryToArray($query);
		$index = 1;
		if ( $ProdArr[0]!="" )
		{
			foreach ( $ProdArr as $prodItem )
			{
				?>
				<div id="<?php echo $divId.$index;?>" class="<?php echo $divClass;?>">
                    <div class="title"><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>" ><?php echo $prodItem["name"];?></a></div>
                    <div class="image"><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>" ><img src="<?php echo $prodItem["cover"];?>" width="400" height="300" alt="" /></a></div>
                    <div class="desc"><?php echo $prodItem["short_desc"];?><div class="read-more"><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$prodItem["path"].$prodItem["link"];?>"><?php echo ($this->m_displayLang=="en" ? "Continue" : "გაგრძელება");?>...</a></div></div>
                </div>
				<?php
				$index++;
			}
		}
	}
	
	function GetMostViewedProducts($tagName = null, $limit=5)
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
						`pr_options` PO on (PO.`productID` = PM.`ID`)
					join
						`structure` S on (PO.`relatedID` = S.`ID`) 
					where 
						PO.`fieldID` = 24 and PM.`status` = 1 
					group by 
						PM.`ID` 
					order by 
						PM.`viewCount` desc, PM.`date` desc 
					limit
						0, ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		$index = 1;
		foreach ( $ProdArr as $prodItem )
		{
			$this->DisplayMostViewedItem($prodItem, $tagName, $index);
			$index++;
		}
	}
	
	function GetRandomProducts($tagName = null, $limit=5)
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
						`pr_options` PO on (PO.`productID` = PM.`ID`)
					join
						`structure` S on (PO.`relatedID` = S.`ID`) 
					where 
						PO.`fieldID` = 24 and PM.`status` = 1 
					group by 
						PM.`ID` 
					order by 
						rand() 
					limit
						0, ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		$index = 1;
		foreach ( $ProdArr as $prodItem )
		{
			$this->DisplayMostViewedItem($prodItem, $tagName, $index);
			$index++;
		}
	}
	
	function GetLastProducts($tagName=null, $limit="0, 4", $display=false)
	{
		$query = "
					select 
						PM.`ID`, 
						PM.`random`, 
						PM.`name_$this->m_displayLang` as name, 
						PM.`short_desc_$this->m_displayLang` as short_desc, 
						PM.`picture`, 
						PM.`cover`, 
						PM.`date`, 
						PT.`name` as `typename`, 
						S.`path` 
					from 
						`pr_main` PM 
					join 
						`pr_type` PT on (PT.`ID` = PM.`typeID`)
					join 
						`pr_options` PO on (PO.`productID` = PM.`ID`)
					join
						`structure` S on (PO.`relatedID` = S.`ID`) 
					where 
						PO.`fieldID` = 6 and PM.`homeGallery` = 0 and PM.`status` = 1 
					group by 
						PM.`ID` 
					order by 
						PM.`date` desc, PM.`ID` desc
					limit ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		if ( $display )
		{
			foreach ( $ProdArr as $prodItem )
			{
				$this->DisplayLastItem($prodItem, $tagName);
			}
		}
		else 
		{
			return $ProdArr;
		}
	}
	
	function GetProductsByTypes($typeIDs, $fieldIDs=0, $orderBy="PM.`date`", $orderType="desc", $limit = 4, $relatedID=0, $special=0)
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
			$conStr = "(";
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
			$conStr .= ")";
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
		
		$specialStr = "";
		if ( $special==0 || $special==1 )
		{
			$specialStr = " and PM.`special` = ".$special;
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
					PM.`status` = 1 and $conStr $specialStr 
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
					PM.`status` = 1 and $conStr $relatedIDStr $specialStr 
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
	
	function GetLastProductsByCategory($tagName = null, $category, $limit = 4, $order="date", $display=true)
	{
		if ( $order=="random" )
		{
			$orderStr = " rand()";
		}
		else 
		{
			$orderStr = " PM.`$order` desc, PM.`ID` desc";
		}
		$query = "
			select 
				PM.`ID`, 
				PM.`typeID`, 
				PM.`random` as link, 
				PM.`name_$this->m_displayLang` as name, 
				PM.`short_desc_$this->m_displayLang` as short_desc, 
				PM.`picture`, 
				PM.`date`, 
				S.`name_$this->m_displayLang` as path_name, 
				S.`path` 
			from 
				`pr_main` PM 
			join 
				`pr_options` PO on (PO.`productID` = PM.`ID`)
			join
				`structure` S on (PO.`relatedID` = S.`ID`) 
			JOIN 
			  	`pr_field` PF ON (PO.`fieldID` = PF.`ID`) 
			where 
				(PF.`type` = 2 || PF.`type` = 8) and PO.`relatedID` = $category and PM.`status` = 1 
			order by 
				$orderStr 
			limit 0, ".$limit;
		$ProdArr = DB::QueryToArray($query);
		
		if ( $display )
		{
			if ( $tagName!=null )
			{
				print ('<'.$tagName.' class="latestArticles_'.$category.'">');
			}
			
			foreach ( $ProdArr as $prodItem )
			{
				$this->DisplayLastItemByCat($prodItem, $tagName);
			}
			
			if ( $tagName!=null )
			{
				print ('</'.$tagName.'>');
			}
		}
		else 
		{
			return $ProdArr;
		}
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
}
?>