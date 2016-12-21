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
class ProductViewSite extends View
{
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_catPath;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_catID;
	
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	protected $m_displaySingle;
	
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
	function __construct($catID, $catPath, $displaySingle = true)
	{
		parent::__construct ();
		
		$this->m_catID = $catID;
		$this->m_catPath = $catPath;
		$this->m_displaySingle = $displaySingle;
		$this->m_productView = null;
		
		$this->ReadNameStrings('langs/', 'product.xml', $this->m_displayLang);
	}
	
	/**
	 * This function swithch what to display, list or item
	 *
	 * @param array $params
	 * @param string $addLink
	 */
	function Display($params, $displayGroup = false)
	{
		if ( !isset($params["sortby"]) )
		{
			$params['sorttype'] = 'DESC';
			$params['sortby'] = 'date';
		}
		
		$params['region'] = (isset($_GET["region"]) ? $_GET["region"]: "");
		// if params contain number
		$this->SetProductView ( $params );
		
		if ( !$this->DisplayItem() )
		{
			$this->DisplayList($params, $displayGroup);
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 */
	protected function DisplayList($params, $displayGroup = false)
	{
		// get number of products in one page
		$countInPage = $this->GetProductCountInPage ();
		
		//	sort
		if ( !isset($params["sortby"]) )
		{
			$params['sorttype'] = 'DESC';
			$params['sortby'] = 'date';
			if ( isset($_GET["sortby"]) && $_GET["sortby"]=="recomended" )
			{
				$params['sortby'] = $_GET["sortby"];
			}
			
			if ( isset($_GET["sortby"]) && $_GET["sortby"]=="popular" )
			{
				$params['sortby'] = "viewCount";
			}
		}
		
		$params['region'] = (isset($_GET["region"]) ? $_GET["region"]: "");
		$params['date'] = (isset($_GET["date"]) ? $_GET["date"]: "");
		
		// read rpoduct count in one page
		$productList = new Pr_mainListView ( $this->m_catID, $this->m_catPath, $countInPage, $params );
		
		$listObj = $productList->GetProductList ();
		
		$count = $listObj->GetAllCount ();
		
		//	added by Grigor - checking content relations with current page
		$rel = new SCRelation ( );
		
		// get category related content list
		$relIDs = $rel->GetCatRelations ( $this->m_catID, $this->m_displayLang, false );
		
		// get related contents count
		$countIDs = count($relIDs);
		
		if ( $count==0 && $params[0]=="" )
		{
			if ($displayGroup)
			{
				$this->DisplayGroup ();
			}
		}
		else
		{
			$tag = "";
			if ( $params[0] )
			{
				$tempOD = new Optionsdata($params[0], "key");
				$tag = $tempOD->GetName($this->m_displayLang);
			}
			
			$productList->Display ($tag);
			
			if ( $count>$countInPage )
			{
				?>
				<br clear="all">
				<a id="loadMore" class="more-button h-centered" data-page="1" data-maxpage="<?php echo ceil($count/$countInPage);?>">еще</a>
				<?php
			}
//			$productList->DisplayNavigation ( $afterLink );
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
	 * @param array $params
	 * @return string|null
	 */
	function GetProductAuthor($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetAuthorSiteName();
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetPublishedDate($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetPublishedDate();
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetUpdatedDate($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetUpdatedDate();
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductTags($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetFieldValue(71, "ru", true);
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductSEOTitle($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetSEOTitle ( $this->m_displayLang );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return bool|null
	 */
	function GetProductSEOTitleOpt($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetSEOTitleOpt ();
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductSEODesc($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetSEODescription ( $this->m_displayLang );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return bool|null
	 */
	function GetProductSEODescOpt($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetSEODescriptionOpt ();
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductSEOKeywords($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetSEOKeywords ( $this->m_displayLang );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return bool|null
	 */
	function GetProductSEOKeywordsOpt($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetSEOKeywordsOpt ();
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductID($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetID ( );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductShortDesc($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetDescShort ( $this->m_displayLang );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductDesc($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetDesc ( $this->m_displayLang );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductPicture($params)
	{
		$this->SetProductView ( $params );
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetPicture();
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductCover($params)
	{
		$this->SetProductView ( $params );
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetCover();
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductCity($params)
	{
		$this->SetProductView ( $params );
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			$city = $pr->GetFieldValue(18, $this->m_displayLang, true);
			if ( $city=="" )
			{
				for ( $i=1; $i<11; $i++ )
				{
					$city = $pr->GetFieldValue(18, $this->m_displayLang, true, null, $i);
					if ( $city!="" )
					{
						break;
					}
				}
			}
			return $city;
		}
		
		return null;
	}
	
	/**
	 * This function returns boolen value of product controller, 
	 * which can be used to display some information before displaying product 
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductCotroller($params)
	{
		$this->SetProductView ( $params );
		
		if ($this->m_productView)
		{
			$pr = $this->m_productView->GetProduct ();
			return $pr->GetProductcontroller ();
		}
		
		return null;
	}
	
	/**
	 * This function returns boolen value of product controller, 
	 * which can be used to display some information before displaying product 
	 *
	 * @param array $params
	 * @return string|null
	 */
	function GetProductCountInCurrentPage($params)
	{
		// get number of products in one page
		$countInPage = $this->GetProductCountInPage();
		
		// read rpoduct count in one page
		$productList = new Pr_mainListView($this->m_catID, $this->m_catPath, $countInPage, $params);
		
		$listObj = $productList->GetProductList();
		
		$count = $listObj->GetAllCount();
		
		return $count;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $prID
	 */
	protected function DisplayItem()
	{
		if ($this->m_productView)
		{
			$controller = $this->m_productView->GetProduct ()->GetProductController ();
			// display product if there is no controller or it is allowed
			if (! $controller || isset ( $_POST ['allowProduct'] ))
			{
				$this->m_productView->Display ();
			}
			else
			{
				$allowMessageS = $this->GetNameString('allowMessage');
				$yesS = $this->GetNameString('yes');
				$noS = $this->GetNameString('no');
				
				print ( '<div class="productControllerMain">' );
				print ( '<div class="productControllerMessage">' );
				print ( $allowMessageS );
				print ( '</div>' );
				print ( '<div class="productControllerAllow">' );
				print ( '<form method="post">' );
				print ( "<input type='submit' name='allowProduct' value='$yesS' />" );
				print ( '</form>' );
				print ( '</div>' );
				print ( '<div class="productControllerNotAllow">' );
				print ( "<form method='post' action='{$this->m_catPath}'>" );
				print ( "<input type='submit' name='notAllowProduct' value='$noS' />" );
				print ( '</form>' );
				print ( '</div>' );
				print ( '<br clear="all">' );
				print ( '</div>' );
			}
			
			return true;
		}
		
		return false;
	}
	
	function DisplayGroup()
	{
		// get sub meni items
		$cat = new Category($this->m_catID);
		$menuID = $cat->GetMenuID();
		
		$subMenu = array();
		$menu = new Menu($menuID, true, $this->m_displayLang);
		$menu->GetSubMenu($this->m_catID, $subMenu, $menu->GetStructure());
		
		$prTypeView = new pr_typeView();
		
		if ( $subMenu )
		{
			foreach ( $subMenu as $menuItem )
			{
				$currCatName = $menuItem['name'];
				$currCatPath = $menuItem['path'];
				
				$currCatPath = $this->m_displayLang.'/'.$currCatPath;
				
				$productList = new Pr_mainListView($menuItem['ID'], $menuItem['path'], 100);
				$prList = $productList->GetProductList();
				if ( $prList->GetAllCount() > 0 )
				{
					$products = $prList->GetStructure();
					$product = new Pr_main($products[0]['ID']);
					
					$prView = $prTypeView->MakeGroupView($product, $currCatPath, $currCatName, $products);
					
					print $prView;
				}
				elseif ( is_array($menuItem["ref"]) )
				{
					for ( $ref=0; $ref<count($menuItem["ref"]); $ref++ )
					{
						$productList = new Pr_mainListView($menuItem["ref"][$ref]['ID'], $menuItem['path'], 1);
						$prList = $productList->GetProductList();
						if ( $prList->GetAllCount() > 0 )
						{
							$products = $prList->GetStructure();
							$product = new Pr_main($products[0]['ID']);
							
							$prView = $prTypeView->MakeGroupView($product, $currCatPath, $currCatName, $products);
							
							print $prView;
							
							break;
						}
					}
				}
			}
		}
		else 
		{
			$query = "select `ID` from `options` where `category_id` = ".$this->m_catID;
			$optionID = DB::QueryToValue($query);
			if ( $optionID>0 )
			{
				$query = "
					SELECT OD.* FROM `optionsdata` OD 
					join `pr_options` PO on (PO.`relatedID` = OD.`ID`) 
					join `pr_main` PM on (PM.`ID` = PO.`productID` and PM.`status` = 1) 
					where OD.`optionsID` = ".$optionID."
					group by OD.`ID` 
					order by count(OD.`ID`) desc
				";
				$subMenu = DB::QueryToArray($query);
				
				foreach ( $subMenu as $subMenuSingle )
				{
					$query = "
						SELECT PM.`ID`, PM.`typeID`, PM.`name_{$this->m_displayLang}` as `name`, PM.`desc_{$this->m_displayLang}` as `desc`, PM.`short_desc_{$this->m_displayLang}` as `short_desc`, PM.`picture`, PM.`date` as `sdate`, PM.`date`, PM.`random` 
						FROM `pr_main` PM 
						JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
						JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID` and OD.`ID` = '".$subMenuSingle["ID"]."' and OD.`optionsID` = ".$optionID.")
						WHERE PM.`status` = 1 
						ORDER BY rand()";
					$ProdArr = DB::QueryToArray($query);
					if ( count($ProdArr)>0 )
					{
						$product = new Pr_main($ProdArr[0]['ID']);
						$currCatPath = $this->m_displayLang.'/'.$cat->GetPath().$subMenuSingle["key"];
						$currCatName = $subMenuSingle["name_".$this->m_displayLang];
						$prView = $prTypeView->MakeGroupView($product, $currCatPath, $currCatName, $ProdArr);
						
						print $prView;
					}
				}
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 */
	protected function SetProductView($params)
	{
		if ( $this->m_productView )
		{
			return;
		}
		
		if ( count($params) > 0 && is_array($params) )
		{
			$productID = 0;
			foreach ( $params as $param )
			{
				if ( strlen($param) > 0 )
				{
					$productID = $param;
					if ( is_string($productID) && strlen($productID)==8 )
					{
						$this->m_productView = new Pr_mainView($productID);
						if ( !$this->m_productView->GetProduct()->getID() )
						{
							$this->m_productView = null;
						}
					}
				}
			}
		}
		
		/*if ( !$this->m_productView )
		{
			// get number of products in one page
			$countInPage = $this->GetProductCountInPage();
			
			// read rpoduct count in one page
			$productList = new Pr_mainListView($this->m_catID, $this->m_catPath, $countInPage, $params);
			
			$listObj = $productList->GetProductList();
			
			$count = $listObj->GetAllCount();
			if ( $count==1 )
			{
				$item = $listObj->GetStructure();
				$this->m_productView = new Pr_mainView($item[0]['ID']);
			}
		}*/
	}
	
	/**
	 * Return number of products in one page, is is configurible from product.xml 
	 *
	 * @return int
	 */
	protected function GetProductCountInPage()
	{
		// open config file
		$xml = simplexml_load_file ( 'cms/config/product.xml' );
		
		// read number
		if (isset ( $xml->countInPage ))
		{
			return ( int ) $xml->countInPage;
		}
		
		return 10;
	}
}
?>