<?php
/** 
 * This class has more features for working with menus (structur).
 * the features are`
 * 1. draw menu by defult syles
 * 2. draw menu horizontal or vertical, by styles
 *
 * @version 1.0 
 * @package Menu
 */
class MenuView extends View
{
	/**
	 * Menu structure
	 *
	 * @private array 
	 */
	protected $m_struct;
	
	protected $m_menuID;
	// draw menu horizontal or vertical, by style
	// $vh for vertiacl or horizontal drawing, true for vertical
	// $styles array has styles for links, headers and selected categoryes, and for deept
	

	// constructore
	function __construct(&$structure, $menuID = null)
	{
		parent::__construct ();
		
		$this->m_struct = &$structure;
		$this->m_menuID = $menuID;
	}
	
	/**
	 * Display vertical menu structure, $category is a corrent category , 
	 * $type is type of drawing
	 * 0. for simple draw (Open tree)
	 * 1. for simple draw (Open only corrent category in tree)
	 * 2. for simple draw in select (Open tree)
	 * @param Category $category
	 * @param int $type
	 * @param array $syles
	 * @param int $drawDepth
	 * @param bool $currentToLink
	 */
	function Display($category, $type = 1, $styles = null, $drawDepth = 8, $currentToLink = false, $tagName = 'div', $displayIcon = false, $subTagMain = '', $subTag = '', $toSend = null)
	{
		switch ($type)
		{
			case 1 :
				$this->DisplaySimple ( $this->m_struct, false, true, $category, 1, $drawDepth, $styles, null, $currentToLink, $tagName, $displayIcon, $subTagMain, $subTag, $toSend );
				break;
			case 2 :
				$this->DisplaySimple ( $this->m_struct, true, true, $category, 1, $drawDepth, $styles, null, $currentToLink, $tagName, $displayIcon, $subTagMain, $subTag, $toSend );
				break;
			case 3 :
				$this->DisplaySimple ( $this->m_struct, false, false, $category, 1, $drawDepth, $styles, null, $currentToLink, $tagName, $displayIcon, $subTagMain, $subTag, $toSend );
				break;
			case 4 :
				$this->DisplaySimple ( $this->m_struct, true, false, $category, 1, $drawDepth, $styles, null, $currentToLink, $tagName, $displayIcon, $subTagMain, $subTag, $toSend );
				break;
			case 5 :
				$this->DisplaySimpleSelect ( $this->m_struct );
				break;
			case 6 :
				$this->DisplaySimpleSelect ( $this->m_struct, $category );
				break;
		}
	}
	
	/**
	 * Display horizontal menu.
	 * 
	 * The $category is used for knowing how to display each menu item set link or not.
	 * 
	 * The $spliter is used for spliting menu items, It stand between two menu items.
	 * 
	 * The $spanStyle is a style of menu item the has not been selected.	
	 * This is a case when the sub item of it also has not been selected
	 * and $selectedButLink is not set .
	 * 
	 * The $spanStyleSelect is a style of span which is for selected menu item.
	 * This is a case when the $currentToLinked is false, in other case
	 * the spane gets $selectedButLink style and item becomes a link.
	 * 
	 * The $selectedButLink is a style on unselected menu item, but is must has another view
	 * the other linked menu items. It hapens when sub menu item has been selected and there is a need 
	 * to set sign which is the parent.
	 * Also this style is used in case, when the current menu item has been selected but there is a need
	 * to set it a link. This is a case when the caegory has native navigateion, fo example news, product and ect.
	 * 
	 * The $currentToLinked is a boolena control. It is used for linking the menu items even they have been selected.
	 * Use this control with $selectedButLink style.
	 * 
	 * @param Category $category
	 * @param string $spliter
	 * @param string|array $spanStyles
	 * @param string|array $spanStylesSelect
	 * @param string|array $selectedButLink
	 * @param bool $currentToLinked
	 */
	function DisplayHorizontal(&$category, $spliter = ' | ', $spanStyle = null, $spanStyleSelect = null, $selectedButLink = null, $currentToLinked = false, $tagName = 'span')
	{
		$catID = $category->GetID ();
		$siteURL = $this->m_site->GetSiteURL ();
		$lang = $this->m_displayLang;
		
		// selected but link category ID
		$selBytLinkID = null;
		if ($selectedButLink)
		{
			// get global parent
			$depth = $category->GetDepth ();
			
			if ($depth > 1)
			{
				$categories = array ();
				$category->GetPathCategories ( $categories );
				
				$selBytLinkID = $categories [0] ['ID'];
			}
		}
		
		$topCount = count ( $this->m_struct );
		$index = 0;
		if ($this->m_struct)
		{
			foreach ( $this->m_struct as $menuItem )
			{
				$style = ($menuItem ['ID'] == $catID) ? $spanStyleSelect : $spanStyle;
				
				// if selected but link is set then display link in particular style ($selectedButLinkM) //
				// modified style
				$selectedButLinkM = null;
				
				// if style is string ist is just class name
				if (is_string ( $selectedButLink ))
				{
					$selectedButLinkM = $selectedButLink;
				}
				elseif (is_array ( $selectedButLink ))
				{
					// if class is array then nuccessery to get right class name
					if (isset ( $selectedButLink [$index] ))
					{
						$selectedButLinkM = $selectedButLink [$index];
					}
					else
					{
						$styleCount = count ( $selectedButLink );
						$selectedButLinkM = $selectedButLink [$index % $styleCount];
					}
				}
				
				if ($menuItem ['ID'] == $selBytLinkID)
				{
					print ("<$tagName class='$selectedButLinkM'>") ;
				}
				else
				{
					if ($style)
					{
						// modified style
						$styleM = null;
						
						// if style is string ist is just class name
						if (is_string ( $style ))
						{
							$styleM = $style;
						}
						elseif (is_array ( $style ))
						{
							// if class is array then nuccessery to get right class name
							if ($style)
							{
								if (isset ( $style [$index] ))
								{
									$styleM = $style [$index];
								}
								else
								{
									$styleCount = count ( $style );
									$styleM = $style [$index % $styleCount];
								}
							}
						}
						
						if (($menuItem ['ID'] == $catID) && $currentToLinked)
						{
							print ("<$tagName class='$selectedButLinkM'>") ;
						}
						else
						{
							if ($styleM)
							{
								print ("<$tagName class='$styleM'>") ;
							}
							else
							{
								print ("<$tagName>") ;
							}
						}
					}
					else
					{
						print ("<$tagName>") ;
					}
				}
				
				// display name with/or link
				if (($menuItem ['ID'] == $catID) && ($currentToLinked == false))
				{
					print ("{$menuItem['name']}") ;
				}
				else
				{
					$path = '';
					$target = '';
					// external or internal links
					if (strpos ( $menuItem ['path'], 'http://' ) === 0)
					{
						$path = $menuItem ['path'];
						$target = " target='_blank'";
					}
					else
					{
						$path = $siteURL . $lang . '/' . $menuItem ['path'];
					}
					
					print ("<a href='$path'$target>{$menuItem['name']}</a>") ;
				}
				
				print ("</$tagName>") ;
				
				if (++ $index < $topCount && $spliter)
				{
					print ($spliter) ;
				}
			}
		}
	}
	
	function DisplayHorizontalWithSubs(&$category, $spliter = ' | ', $spanStyle = null, $spanStyleSelect = null, $selectedButLink = null, $currentToLinked = false, $tagName = 'span', $startDepth = 1, $innerTag1 = null, $innerTag1Class = null, $innerTag2 = null, $innerTag2Class = null, $innerTagMain = null, $innerTagMainClass = null, $innerTagMainActiveClass = null, $showTop=0)
	{
		$catID = $category->GetID ();
		$siteURL = $this->m_site->GetSiteURL ();
		$lang = $this->m_displayLang;
		
		// selected but link category ID
		$selBytLinkID = null;
		if ($selectedButLink)
		{
			// get global parent
			$depth = $category->GetDepth ();
			
			if ($depth > 1)
			{
				$categories = array ();
				$category->GetPathCategories ( $categories );
				
				$selBytLinkID = $categories [0] ['ID'];
			}
		}
		
		$topCount = count ( $this->m_struct );
		$index = 0;
		if ($this->m_struct)
		{
			foreach ( $this->m_struct as $menuItem )
			{
				$style = ($menuItem ['ID'] == $catID) ? $spanStyleSelect : $spanStyle;
				$style = str_replace("@@index@@", $index+1, $style);
				// if selected but link is set then display link in particular style ($selectedButLinkM) //
				// modified style
				$selectedButLinkM = null;
				
				// if style is string ist is just class name
				if (is_string ( $selectedButLink ))
				{
					$selectedButLinkM = $selectedButLink;
				}
				elseif (is_array ( $selectedButLink ))
				{
					// if class is array then nuccessery to get right class name
					if (isset ( $selectedButLink [$index] ))
					{
						$selectedButLinkM = $selectedButLink [$index];
					}
					else
					{
						$styleCount = count ( $selectedButLink );
						$selectedButLinkM = $selectedButLink [$index % $styleCount];
					}
				}
				
				if ( $tagName!=null && $tagName!="" )
				{
					print ("<$tagName class='".$style."'>") ;
				}
				
				// display name with/or link
				if (($menuItem ['ID'] == $catID) && ($currentToLinked == false))
				{
					$path = '';
					$target = '';
					// external or internal links
					if (strpos ( $menuItem ['path'], 'http://' ) === 0)
					{
						$path = $menuItem ['path'];
						$target = " target='_blank'";
					}
					else
					{
						$path = $siteURL . $lang . '/' . $menuItem ['path'];
					}
					
					$firstMenuLink = $menuItem['name'];
				}
				else
				{
					$path = '';
					$target = '';
					// external or internal links
					if (strpos ( $menuItem ['path'], 'http://' ) === 0)
					{
						$path = $menuItem ['path'];
						$target = " target='_blank'";
					}
					else
					{
						$path = $siteURL . $lang . '/' . $menuItem ['path'];
					}
					
					$firstMenuLink = "<a href='$path'$target>{$menuItem['name']}</a>";
				}
				
				//	added by Grigor
				if ( $innerTag1!==null )
				{
					if ( $innerTag1Class!==null) {
						
						print ("<$innerTag1 class='$innerTag1Class'>");
					} else {
						
						print ("<$innerTag1>");
					}
				}
				
				if ( $innerTag2!==null )
				{
					if ( $innerTag2Class!==null ) {
						
						print ("<$innerTag2 class='$innerTag2Class'>");
					} else {
						
						print ("<$innerTag2>");
					}
					print $firstMenuLink;
					print ("</$innerTag2>");
				}
				
				if ( is_array($menuItem["ref"]) ) {
					
					
					$subMenuIDS = "";
					foreach ( $menuItem["ref"] as $subMenuItem )
					{
						$subMenuIDS[] = $subMenuItem ['ID'];
						if ( $catID== $subMenuItem ['ID']) {
							
							if ( $innerTagMainActiveClass!==null) {
								
								print ("<$innerTagMain class='$innerTagMainActiveClass'>") ;
							} else {
								
								print ("<$innerTagMain>") ;
							}
							
							print $subMenuItem['name'];
						} else {
							
							if ( $innerTagMainClass!==null ) {
								
								print ("<$innerTagMain class='$innerTagMainClass'>") ;
							} else {
								
								print ("<$innerTagMain>") ;
							}
							
							$path = '';
							$target = '';
							// external or internal links
							if (strpos ( $subMenuItem ['path'], 'http://' ) === 0)
							{
								$path = $subMenuItem ['path'];
								$target = " target='_blank'";
							}
							else
							{
								$path = $siteURL . $lang . '/' . $subMenuItem ['path'];
							}
							
							print ("<a href='$path'$target>{$subMenuItem['name']}</a>") ;
						}
						
						print ("</$innerTagMain>") ;
					}
				}
				
				if ( $innerTag1!==null )
				{
					print ("</$innerTag1>");
				}
				
				//	added by Grigor
				if ( $tagName!=null && $tagName!="" )
				{
					print ("</$tagName>") ;
				}
				
				if (++ $index < $topCount && $spliter)
				{
					print ($spliter) ;
				}
			}
		}
	}
	
	function DisplayHorizontalWithSubsNew(&$category, $spliter = ' | ', $spanStyle = null, $spanStyleSelect = null, $selectedButLink = null, $currentToLinked = false, $tagName = 'span', $startDepth = 1, $innerTag1 = null, $innerTag1Class = null, $innerTag2 = null, $innerTag2Class = null, $innerTagMain = null, $innerTagMainClass = null, $innerTagMainActiveClass = null, $showTop=0)
	{
		$catID = $category->GetID ();
		$siteURL = $this->m_site->GetSiteURL ();
		$lang = $this->m_displayLang;
		
		// selected but link category ID
		$selBytLinkID = null;
		if ($selectedButLink)
		{
			// get global parent
			$depth = $category->GetDepth ();
			
			if ($depth > 1)
			{
				$categories = array ();
				$category->GetPathCategories ( $categories );
				
				$selBytLinkID = $categories [0] ['ID'];
			}
		}
		
		$topCount = count ( $this->m_struct );
		$index = 0;
		if ($this->m_struct)
		{
			foreach ( $this->m_struct as $menuItem )
			{
				$style = ($menuItem ['ID'] == $catID) ? $spanStyleSelect : $spanStyle;
				
				// if selected but link is set then display link in particular style ($selectedButLinkM) //
				// modified style
				$selectedButLinkM = null;
				
				// if style is string ist is just class name
				if (is_string ( $selectedButLink ))
				{
					$selectedButLinkM = $selectedButLink;
				}
				elseif (is_array ( $selectedButLink ))
				{
					// if class is array then nuccessery to get right class name
					if (isset ( $selectedButLink [$index] ))
					{
						$selectedButLinkM = $selectedButLink [$index];
					}
					else
					{
						$styleCount = count ( $selectedButLink );
						$selectedButLinkM = $selectedButLink [$index % $styleCount];
					}
				}
				
				if ($menuItem ['ID'] == $selBytLinkID)
				{
					print ("<$tagName class='$selectedButLinkM'>") ;
				}
				else
				{
					if ($style)
					{
						// modified style
						$styleM = null;
						
						// if style is string ist is just class name
						if (is_string ( $style ))
						{
							$styleM = $style;
						}
						elseif (is_array ( $style ))
						{
							// if class is array then nuccessery to get right class name
							if ($style)
							{
								if (isset ( $style [$index] ))
								{
									$styleM = $style [$index];
								}
								else
								{
									$styleCount = count ( $style );
									$styleM = $style [$index % $styleCount];
								}
							}
						}
						
						if (($menuItem ['ID'] == $catID) && $currentToLinked)
						{
							print ("<$tagName class='$selectedButLinkM'>") ;
						}
						else
						{
							if ($styleM)
							{
								print ("<$tagName class='$styleM'>") ;
							}
							else
							{
								print ("<$tagName>") ;
							}
						}
					}
					else
					{
						print ("<$tagName>") ;
					}
				}
				
				// display name with/or link
				if (($menuItem ['ID'] == $catID) && ($currentToLinked == false))
				{
					$path = '';
					$target = '';
					// external or internal links
					if (strpos ( $menuItem ['path'], 'http://' ) === 0)
					{
						$path = $menuItem ['path'];
						$target = " target='_blank'";
					}
					else
					{
						$path = $siteURL . $lang . '/' . $menuItem ['path'];
					}
					
					print ("<a href='$path'$target class='mm".($index+1)."'>{$menuItem['name']}</a>") ;
				}
				else
				{
					$path = '';
					$target = '';
					// external or internal links
					if (strpos ( $menuItem ['path'], 'http://' ) === 0)
					{
						$path = $menuItem ['path'];
						$target = " target='_blank'";
					}
					else
					{
						$path = $siteURL . $lang . '/' . $menuItem ['path'];
					}
					
					print ("<a href='$path'$target class='mm".($index+1)."'>{$menuItem['name']}</a>") ;
				}
				
				//	added by Grigor
				
				if ( is_array($menuItem["ref"]) ) {
					
					if ( $innerTag1!==null ) {
						
						if ( $innerTag1Class!==null) {
							
							print ("<$innerTag1 class='$innerTag1Class'>");
						} else {
							
							print ("<$innerTag1>");
						}
					}
					
					if ( $innerTag2!==null ) {
						
						if ( $innerTag2Class!==null ) {
							
							print ("<$innerTag2 class='$innerTag2Class'>");
						} else {
							
							print ("<$innerTag2>");
						}
					}
					$subMenuIDS = "";
					$sID = 0;
					foreach ( $menuItem["ref"] as $subMenuItem )
					{
						$subMenuIDS[$sID][] = $subMenuItem['ID'];
						if ( is_array($subMenuItem["ref"]) )
						{
							foreach ( $subMenuItem["ref"] as $subsubMenuItem )
							{
								$subMenuIDS[$sID][] = $subsubMenuItem['ID'];
							}
						}
						if ( $catID==$subMenuItem['ID']) {
							
							if ( $innerTagMainActiveClass!==null) {
								
								print ("<$innerTagMain class='$innerTagMainActiveClass'>") ;
							} else {
								
								print ("<$innerTagMain>") ;
							}
						} else {
							
							if ( $innerTagMainClass!==null ) {
								
								print ("<$innerTagMain class='$innerTagMainClass'>") ;
							} else {
								
								print ("<$innerTagMain>") ;
							}
						}
						
						$path = '';
						$target = '';
						// external or internal links
						if (strpos ( $subMenuItem['path'], 'http://' ) === 0)
						{
							$path = $subMenuItem['path'];
							$target = " target='_blank'";
						}
						else
						{
							$path = $siteURL . $lang . '/' . $subMenuItem['path'];
						}
						
						print ("<a href='$path'$target>{$subMenuItem['name']}</a>");
						
						print ("</$innerTagMain>");
						
						$sID++;
					}
					
					if ( $innerTag2!==null ) {
						
						print ("</$innerTag2>");
					}
					
					//	show top article ->	start
					if ( $showTop==1 && is_array($subMenuIDS) ) {
						
						for ( $SMi=0; $SMi<count($subMenuIDS); $SMi++ ) {
							
							$SMStr = " ( PO.`relatedID` = 100000 ";
							for ( $SsMi=0; $SsMi<count($subMenuIDS[$SMi]); $SsMi++ ) {
								
								$SMStr .= " or PO.`relatedID` = ".$subMenuIDS[$SMi][$SsMi]." ";
							}
							$SMStr .= " ) ";
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
									`pr_options` PO on (PM.`ID` = PO.`productID`)
								join
									`structure` S on (PO.`relatedID` = S.`ID`) 
								JOIN 
								  	`pr_field` PF ON (PO.`fieldID` = PF.`ID`) 
								where 
									$SMStr and (PF.`type` = 2 || PF.`type` = 8)
								order by 
									rand()";
							$ProdArr = DB::QueryToArray($query);
							
							if ( $ProdArr[0]!="" ) {
								
								$tempPr = new Pr_main($ProdArr[0]["ID"]);
								$prCatID = $tempPr->GetCategoryID();
								$tempPrList = new ProductList($prCatID);
								if ( count($tempPrList->GetStructure())>1 )
								{
									?>
									<ul class="a<?php echo $SMi;?>" style="display:none;">
					                    <div>
					                        <p><img src="<?php echo $ProdArr[0]["picture"];?>" /></p>
					                        <span><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$ProdArr[0]["link"];?>"><?php echo $ProdArr[0]["name"];?></a></span>
					                    </div>
					                </ul>
									<?php
								}
								else 
								{
									?>
									<ul class="a<?php echo $SMi;?>" style="display:none;">
					                    <div>
					                        <p><img src="<?php echo $ProdArr[0]["picture"];?>" /></p>
					                        <span><a href="<?php echo $this->m_site->GetSiteURL().$this->m_displayLang.'/'.$ProdArr[0]["path"];?>"><?php echo $ProdArr[0]["name"];?></a></span>
					                    </div>
					                </ul>
									<?php
								}
							}
						}
					}
					//	show top article ->	end
					
					if ( $innerTag1!==null ) {
						
						print ("</$innerTag1>");
					}
				}
				
				//	added by Grigor
				
				print ("</$tagName>") ;
				
				if (++ $index < $topCount && $spliter)
				{
					print ($spliter) ;
				}
			}
		}
	}
	
	/**
	 * Display Open tree , or open onli corrent category path tree
	 *
	 * $syles most have 2-4 param position, selectet class and no selected class
	 * $style[1] = array("divclass" => "class1", "divclassS" => ..., "divclassSL" => ..., "linkclass" => ...);
	 * $style[1] = array("divclass" => "class1", "divclassS" => "linkclass1");
	 * 
	 * @param array $categoryis
	 * @param string $lang
	 * @param bool $byID
	 * @param boolean $bOpenTree
	 * @param Category $category
	 * @param int $corrDeep
	 * @param array $syles
	 * @param array $path
	 */
	function DisplaySimple(&$categoryis, $byID = false, $bOpenTree = true, $categoryS = 0, $corrDeep = 1, $drawDepth = null, $styles = null, $path = null, $currentToLink = false, $tagName = 'div', $displayIcon = false, $subTagMain = '', $subTag = '', $toSend = null)
	{
		if (($categoryis == null) || ($drawDepth && ($corrDeep > $drawDepth)))
			return;
		
		$ID = $categoryS->GetID ();
		
		// site root
		$siteRoot = $this->m_site->GetSiteURL ();
		
		if (! $path && $ID != null) // get path of corrent category (selected category)
		{
			$path = array ();
			$categoryS->GetPathCategories ( $path );
		}
		
		// get syles
		$st = ($styles [$corrDeep]) ? $styles [$corrDeep] : $styles [count ( $styles )];
		$divClass = (isset ( $st ['divclass'] )) ? $st ['divclass'] : null;
		$divSelectClass = (isset ( $st ['divclassS'] )) ? $st ['divclassS'] : $divClass;
		$divCurrentToLinkClass = (isset ( $st ['divclassSL'] )) ? $st ['divclassSL'] : $divClass;
		
		$pathIDs = array();
		if ($path){
			foreach ($path as $item)
			{
				$pathIDs[] = $item['ID'];
			}
		}
		
		foreach ( $categoryis as $category )
		{
			$catName = $category ['name'];
			
			// display icon
			if ($displayIcon)
			{
				$defName = 'icon.jpg';
				if (file_exists ( "media/images/icons/{$category['ID']}.jpg" ))
				{
					$defName = "{$category['ID']}.jpg";
				}
				
//				print ("<div class='mainMenuIcon'><img src='media/images/icons/$defName' alt='' /></div>") ;
			}
			
			// draw category
			if ( $ID==$category['ID'] )
			{
				$divSelectClassM = $divSelectClass;
				
				if ( $currentToLink )
				{
					$divSelectClassM = ($divCurrentToLinkClass) ? $divCurrentToLinkClass : $divClass;
					$linkClass = (isset ( $st ['linkclass'] )) ? " class='{$st['linkclass']}'" : "";
					
					if ($byID)
					{
						$categoryPath = "?catID={$category['ID']}";
						
						// add menu ID
						if ($this->m_menuID)
						{
							$categoryPath .= "&submenuID={$this->m_menuID}";
						}
					}
					else
					{
						$categoryPath = $siteRoot . $this->m_displayLang . '/' . $category ['path'];
					}
					
					print ("\t<$tagName class='{$divSelectClassM}'>");
					print ("<a href='{$categoryPath}' $linkClass>{$catName}</a>");
				}
				else
				{
					// if the class for selected div is set then put it to div
					if ($divSelectClass)
					{
						print ("\t<$tagName class='{$divSelectClass}'>");
					}
					else
					{
						print ("\t<$tagName>");
					
					}
					
					print "$catName\n";
				}
			}
			else
			{
				// display in different style path categories
				if (in_array($category ['ID'], $pathIDs))
				{
					print ("\t<$tagName class='{$divCurrentToLinkClass}'>");
				}
				else
				{
					if ($divClass)
					{
						print ("\t<$tagName class='{$divClass}'>");
//						print ("\t<$tagName>");
					}
					else
					{
						print ("\t<$tagName>");
					}
				}
				
				if ($byID)
				{
					$categoryPath = "?catID={$category['ID']}";
					
					// add menu ID
					if ($this->m_menuID)
					{
						$categoryPath .= "&submenuID={$this->m_menuID}";
					}
				}
				else
				{
					$categoryPath = $siteRoot . $this->m_displayLang . '/' . $category ['path'];
				}
				
				$linkClass = (isset ( $st ['linkclass'] )) ? " class='{$st['linkclass']}'" : "";
				
				print ("<a href='{$categoryPath}' $linkClass>{$catName}</a>") ;
			}
			
			print ("</$tagName>\n") ;
			
			if ( $category['ref']!=null )
			{
				if ( $bOpenTree || $path[$corrDeep-1]['ID']==$category['ID'] || $path[$corrDeep]['ID']==$category['ID'] )
				{
					if ( $subTagMain!="" )
					{
						print ("<$subTagMain class='".$styles[$corrDeep + 1]["divclass"]."'>\n") ;
						$this->DisplaySimpleSub ( $category ['ref'], $byID, $bOpenTree, $categoryS, $corrDeep + 1, $drawDepth, $styles, $path, $currentToLink, $tagName, false, $subTagMain, $subTag );
						print ("</$subTagMain>\n") ;
					}
					else 
					{
						$this->DisplaySimple ( $category ['ref'], $byID, $bOpenTree, $categoryS, $corrDeep + 1, $drawDepth, $styles, $path, $currentToLink, $tagName, false );
					}
				}
			}
			elseif ( ($category['ID']==58 || $category['ID']==59 || $category['ID']==60 || $category['ID']==61 || $category['ID']==62 || $category['ID']==63 || $category['ID']==86 || $category['ID']==87 || $category['ID']==88 || $category['ID']==89 || $category['ID']==90) && ($bOpenTree || $path[$corrDeep - 1]['ID'] == $category['ID'] || $path[$corrDeep]['ID'] == $category['ID']) ) 
			{
				if ( $subTagMain!="" )
				{
					$catID = $category ['ID'];
					$catPath = $category ['path'];
					$this->m_displayLang;
					$sql = "
							select 
								PM.`typeID`, 
								PM.`random`, 
								PM.`name_".$this->m_displayLang."` as `name`, 
								PM.`desc_".$this->m_displayLang."` as `desc`, 
								PM.`short_desc_".$this->m_displayLang."` as `short_desc`, 
								PM.`picture`, 
								PM.`date` 
							from 
								`pr_main` PM 
							join 
								`pr_options` PO on (PM.`ID` = PO.`productID`) 
							where 
								PO.`fieldID` = 3 and PO.`relatedID` = $catID 
							order by 
								PM.`ID` asc
					";
					
					$prArr = DB::QueryToArray($sql);
					if ( $prArr[0]!="" )
					{
						print ("<$subTagMain class='".$styles[$corrDeep + 1]["divclass"]."'>\n");
						for ( $i=0; $i<count($prArr); $i++ )
						{
							if ( $toSend==$prArr[$i]["random"] )
							{
								print ("<$subTag class='".$styles[$corrDeep + 1]["divclassS"]."'>\n");
//								print ("<$subTag>\n");
								print ("".$prArr[$i]["name"]."");
								print ("</$subTag>\n");
							}
							else 
							{
								print ("<$subTag class='".$styles[$corrDeep + 1]["divclass"]."'>\n");
//								print ("<$subTag>\n");
								print ("<a href='".$this->m_displayLang."/".$catPath.$prArr[$i]["random"]."'>".$prArr[$i]["name"]."</a>");
								print ("</$subTag>\n");
							}
						}
						print ("</$subTagMain>\n");
					}
				}
			}
		}
		
		return;
	}
	
	/**
	 * Display Open tree , or open onli corrent category path tree
	 *
	 * $syles most have 2-4 param position, selectet class and no selected class
	 * $style[1] = array("divclass" => "class1", "divclassS" => ..., "divclassSL" => ..., "linkclass" => ...);
	 * $style[1] = array("divclass" => "class1", "divclassS" => "linkclass1");
	 * 
	 * @param array $categoryis
	 * @param string $lang
	 * @param bool $byID
	 * @param boolean $bOpenTree
	 * @param Category $category
	 * @param int $corrDeep
	 * @param array $syles
	 * @param array $path
	 */
	function DisplaySimpleSub(&$categoryis, $byID = false, $bOpenTree = true, $categoryS = 0, $corrDeep = 1, $drawDepth = null, $styles = null, $path = null, $currentToLink = false, $tagName = 'div', $displayIcon = false, $subTagMain = 'div', $subTag = 'span')
	{
		if (($categoryis == null) || ($drawDepth && ($corrDeep > $drawDepth)))
			return;
		
		$ID = $categoryS->GetID ();
		
		// site root
		$siteRoot = $this->m_site->GetSiteURL ();
		
		if (! $path && $ID != null) // get path of corrent category (selected category)
		{
			$path = array ();
			$categoryS->GetPathCategories ( $path );
		}
		
		// get syles
		$st = ($styles [$corrDeep]) ? $styles [$corrDeep] : $styles [count ( $styles )];
		$divClass = (isset ( $st ['divclass'] )) ? $st ['divclass'] : null;
		$divSelectClass = (isset ( $st ['divclassS'] )) ? $st ['divclassS'] : $divClass;
		$divCurrentToLinkClass = (isset ( $st ['divclassSL'] )) ? $st ['divclassSL'] : $divClass;
		
		$pathIDs = array();
		if ($path)
		{
			foreach ($path as $item)
			{
				$pathIDs[] = $item['ID'];
			}
		}
		
		foreach ( $categoryis as $category )
		{
			$catName = $category ['name'];
			
			// display icon
			if ($displayIcon)
			{
				$defName = 'icon.jpg';
				if (file_exists ( "media/images/icons/{$category['ID']}.jpg" ))
				{
					$defName = "{$category['ID']}.jpg";
				}
				
//				print ("<div class='mainMenuIcon'><img src='media/images/icons/$defName' alt='' /></div>") ;
			}
			
			// draw category
			if ( $ID==$category['ID'] )
			{
				$divSelectClassM = $divSelectClass;
				
				if ($currentToLink)
				{
					$divSelectClassM = ($divCurrentToLinkClass) ? $divCurrentToLinkClass : $divClass;
					$linkClass = (isset ( $st ['linkclass'] )) ? " class='{$st['linkclass']}'" : "";
					
					if ($byID)
					{
						$categoryPath = "?catID={$category['ID']}";
						
						// add menu ID
						if ($this->m_menuID)
						{
							$categoryPath .= "&submenuID={$this->m_menuID}";
						}
					}
					else
					{
						$categoryPath = $siteRoot . $this->m_displayLang . '/' . $category ['path'];
					}
					
					print ("\t<$subTag class='{$divSelectClassM}'>") ;
					print ("<a href='{$categoryPath}' $linkClass>{$catName}</a>") ;
				}
				else
				{
					// if the class for selected div is set then put it to div
					if ($divSelectClass)
					{
						print ("\t<$subTag class='{$divSelectClass}'>") ;
					}
					else
					{
						print ("\t<$subTag>") ;
					
					}
					
					print "$catName\n";
				}
			}
			else
			{
				// display in different style path categories
				if (in_array($category ['ID'], $pathIDs))
				{
					print ("\t<$subTag class='{$divCurrentToLinkClass}'>") ;
				}
				else
				{
					if ($divClass)
					{
//						print ("\t<$subTag class='{$divClass}'>") ;
						print ("\t<$subTag>") ;
					}
					else
					{
						print ("\t<$subTag>") ;
					}
				}
				
				if ($byID)
				{
					$categoryPath = "?catID={$category['ID']}";
					
					// add menu ID
					if ($this->m_menuID)
					{
						$categoryPath .= "&submenuID={$this->m_menuID}";
					}
				}
				else
				{
					$categoryPath = $siteRoot . $this->m_displayLang . '/' . $category ['path'];
				}
				
				$linkClass = (isset ( $st ['linkclass'] )) ? " class='{$st['linkclass']}'" : "";
				
				print ("<a href='{$categoryPath}' $linkClass>{$catName}</a>") ;
			}
			
			print ("</$subTag>\n") ;
			
			if ($category ['ref'] != null)
			{
				if ($bOpenTree || $path [$corrDeep - 1] ['ID'] == $category ['ID'] || $path [$corrDeep] ['ID'] == $category ['ID'])
				{
					$this->DisplaySimple ( $category ['ref'], $byID, $bOpenTree, $categoryS, $corrDeep + 1, $drawDepth, $styles, $path, $currentToLink, $tagName, $subTagMain, $subTag );
				}
			}
		}
		
		return;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $categoryis
	 * @param string $lang
	 * @param int $ID
	 * @param string $displayEmp
	 */
	function DisplaySimpleSelect(&$categoryis, $selectedID = null, $corrDeep = 1, $displayEmp = null)
	{
		if ($categoryis && count ( $categoryis ) > 0)
		{
			// display empty option 
			if ($displayEmp)
			{
				print ("<option value=''>$displayEmp</option>\n") ;
			}
			
			foreach ( $categoryis as $category )
			{
				$slc = null;
				
				if ($selectedID == $category ['ID'])
				{
					$slc = "selected='selected' ";
				}
				// draw category
				$bspace = "";
				$bspace = str_pad ( $bspace, 3 * 6 * $corrDeep, "&nbsp;" ); // 3 is count of space
				print ("<option value='{$category['ID']}' $slc>$bspace{$category['name']}</option>\n") ;
				
				// draw sub categoryes
				if ($category ['ref'] != null)
				{
					$this->DisplaySimpleSelect ( $category ['ref'], $selectedID, $corrDeep + 1 );
				}
			}
		}
	}
	
	/**
	 * Created for multi value structure tree
	 *
	 * @param array $categoryis
	 * @param string $lang
	 * @param int $ID
	 * @param string $displayEmp
	 */
	function DisplayMultiSelect(&$categoryis, $selectedIDs = null, $corrDeep = 1, $displayEmp = null)
	{
		if ($categoryis && count ( $categoryis ) > 0)
		{
			// display empty option 
			if ($displayEmp)
			{
				print ("<option value=''>$displayEmp</option>\n") ;
			}
			
			foreach ( $categoryis as $category )
			{
				$slc = null;
				if ( in_array($category ['ID'],$selectedIDs ))
				{
					$slc = "selected='selected' ";
				}
				// draw category
				$bspace = "";
				$bspace = str_pad ( $bspace, 3 * 6 * $corrDeep, "&nbsp;" ); // 3 is count of space
				print ("<option value='{$category['ID']}' $slc>$bspace{$category['name']}</option>\n") ;
			
				// draw sub categoryes
				if ($category ['ref'] != null)
				{
					$this->DisplayMultiSelect ( $category ['ref'], $selectedIDs, $corrDeep + 1 );
				}
			}
		}
	}
}

?>