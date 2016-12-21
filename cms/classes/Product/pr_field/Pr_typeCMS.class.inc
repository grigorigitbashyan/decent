<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_type/Pr_type.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/ProductList.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_typeCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_type
	 */
	protected $m_pr_type;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->Update ();
		
		$this->ReadNameStrings ( 'classes/Product/langs/', 'product.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function DoAction()
	{
		$ar = null;
		
		$action = $this->GetFormAction ();
		
		$ID = $this->m_pr_type->GetID ();
		
		switch ($action)
		{
			case 'rename' : // rename Pr_type
				if (($newName = DB::POST ('name') ))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_pr_type->SetName ( $newName );
					}
				}
				break;
			case 'delete' : // delete Pr_type
				$ar = $this->m_pr_type->Delete ();
				
				$prTypeFieldListCMS = new Pr_typefieldListCMS ( $ID );
				$prTypeFieldListCMS->DeleteTypeFields ();
				break;
			case 'pattern' :
				if (($newPattern = DB::POST ('newpattern')))
				{
					// set new pattern
					$ar = $this->m_pr_type->SetPattern ( $newPattern, $this->m_displayLang );
				}
				break;
			case 'patternlist' :
				if (($newPattern = DB::POST ('newpattern')))
				{
					// set new pattern
					$ar = $this->m_pr_type->SetPatternList ( $newPattern, $this->m_displayLang );
					$ar &= $this->m_pr_type->SetPatternList1 ( DB::POST ('newpattern1'), $this->m_displayLang );
					$ar &= $this->m_pr_type->SetPatternListHeader( DB::POST ('newpatternheader'), $this->m_displayLang );
					$ar &= $this->m_pr_type->SetPatternListFooter( DB::POST ('newpatternfooter'), $this->m_displayLang );
				}
				break;
			case 'patterngroup' :
				if (($newPattern = DB::POST ('newpattern')))
				{
					// set new pattern
					$ar = $this->m_pr_type->SetPatternGroup ( $newPattern, $this->m_displayLang );
				}
				break;
			case 'patternsearch' :
				if (($newPattern = DB::POST ('newpattern')))
				{
					// set new pattern
					$ar = $this->m_pr_type->SetPatternSearch ( $newPattern, $this->m_displayLang );
				}
				break;
			case 'patterngallery' :
				if (($newPattern = DB::POST ('newpattern')))
				{
					// set new pattern
					$ar = $this->m_pr_type->SetPatternGallery( $newPattern, $this->m_displayLang );
				}
				break;
			case 'patternrelation' :
				if (($newPattern = DB::POST ('newpattern')))
				{
					// set new pattern
					$ar = $this->m_pr_type->SetPatternRelation( $newPattern, $this->m_displayLang );
					
					$ar &= $this->m_pr_type->SetPatternRelationHeader( DB::POST ('newpatternheader'), $this->m_displayLang );
					$ar &= $this->m_pr_type->SetPatternRelationFooter( DB::POST ('newpatternfooter'), $this->m_displayLang );
				}
				break;
			case 'patternother' :
				if (($newPattern = DB::POST ('newpattern')))
				{
					// set new pattern
					$ar = $this->m_pr_type->SetPatternOther( $newPattern, $this->m_displayLang );
					
					$ar &= $this->m_pr_type->SetPatternOtherHeader( DB::POST ('newpatternheader'), $this->m_displayLang );
					$ar &= $this->m_pr_type->SetPatternOtherFooter( DB::POST ('newpatternfooter'), $this->m_displayLang );
				}
				break;
			case 'patternwishlist' :
				if (($newPattern = DB::POST ('newpattern')))
				{
					// set new pattern
					$ar = $this->m_pr_type->SetPatternWishlist( $newPattern, $this->m_displayLang );
					
					$ar &= $this->m_pr_type->SetPatternWishlistHeader( DB::POST ('newpatternheader'), $this->m_displayLang );
					$ar &= $this->m_pr_type->SetPatternWishlistFooter( DB::POST ('newpatternfooter'), $this->m_displayLang );
				}
				break;
			case 'ptrelation':
				$ptIDs = DB::POST('ptIDs');
				if ( !is_array($ptIDs) )
				{
					$ptIDs = array();
				}
				
				$this->m_pr_type->DeletePTRelations();		//	delete existing relations
				$this->m_pr_type->AddPTRelations($ptIDs);	//	add relations
				break;
			case 'setRequirements':
				$textprice = DB::POST('textprice');
				$picprice = DB::POST('picprice');
				$contactprice = DB::POST('contactprice');
				$bonus = DB::POST('bonus');
				$mintextcount = DB::POST('mintextcount');
				$minpiccount = DB::POST('minpiccount');
				$mincontactcount = DB::POST('mincontactcount');
				$bonustextcount = DB::POST('bonustextcount');
				$bonuspiccount = DB::POST('bonuspiccount');
				$bonuscontactcount = DB::POST('bonuscontactcount');
				
				$this->m_pr_type->SetTextPrice($textprice);
				$this->m_pr_type->SetPicPrice($picprice);
				$this->m_pr_type->SetContactPrice($contactprice);
				$this->m_pr_type->SetBonus($bonus);
				$this->m_pr_type->SetMinTextCount($mintextcount);
				$this->m_pr_type->SetMinPicCount($minpiccount);
				$this->m_pr_type->SetMinContactCount($mincontactcount);
				$this->m_pr_type->SetBonusTextCount($bonustextcount);
				$this->m_pr_type->SetBonusPicCount($bonuspiccount);
				$this->m_pr_type->SetBonusContactCount($bonuscontactcount);
				break;
			case 'setOrder':
				$order = DB::POST('order');
				$this->m_pr_type->SetOrder($order);
				break;
			case 'setImageParams':
				$iconW = DB::POST('iconW');
				$iconH = DB::POST('iconH');
				$coverW = DB::POST('coverW');
				$coverH = DB::POST('coverH');
				$thumbnailW = DB::POST('thumbnailW');
				$thumbnailH = DB::POST('thumbnailH');
				$iconC = DB::POST('iconC') ? 1: 0;
				$coverC = DB::POST('coverC') ? 1: 0;
				$thumbnailC = DB::POST('thumbnailC') ? 1: 0;
				
				$iconI = $iconW.";".$iconH;
				$coverI = $coverW.";".$coverH;
				$thumbnailI = $thumbnailW.";".$thumbnailH;
				$this->m_pr_type->SetIconSize($iconI);
				$this->m_pr_type->SetCoverSize($coverI);
				$this->m_pr_type->SetThumbnailSize($thumbnailI);
				$this->m_pr_type->SetIconCrop($iconC);
				$this->m_pr_type->SetCoverCrop($coverC);
				$this->m_pr_type->SetThumbnailCrop($thumbnailC);
				break;
			case 'structure_relation':
				$structureID = DB::POST('structureID');
				$ar = $this->m_pr_type->SetStructureID($structureID);
				break;
			case 'setTabs':
				$tabs = DB::POST('tabs');
				$tabs = serialize($tabs);
				$this->m_pr_type->SetTabs($tabs);
				break;
		}
		
		//if there is a any action then update Pr_type  
		if ($ar)
		{
			$this->Update ();
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
	
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// get names
		$doneS = $this->GetNameString ( 'done' );
		$deleteS = $this->GetNameString ( 'delete' );
		$renameS = $this->GetNameString('rename');
		
		$selectS = $this->GetNameString ( 'select' );
		$allS = $this->GetNameString ( 'all' );
		$noneS = $this->GetNameString ( 'none' );
		
		/////////////////////////////////////
		$ID = $this->m_pr_type->GetID ();
		$objName = $this->m_pr_type->GetName ();
		
		$overLink = 'pr_typeID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		
		$delForm = null;
		
		// get count of products according this type
		$prductsList = new Pr_mainList ( $ID );
		
		$proCount = $prductsList->GetAllCount ();
		
		if ($proCount < 1)
		{
			// delete form javascript
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
		
			$delForm = "<div  class='titleColR'>
    					<form method='POST' $dJS>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='$overLink' value='$ID'>
    					<input type='submit' value='$deleteS'>
    					</form>
    					</div>";
		}
		
		// display form title
		$this->DisplayTitle ( $objName, $delForm, false );
		
		// diplay rename form
		$noEmptyS = $this->GetNameString ( 'noempty' );
		
		// display tab control
		$panelIndex = 0;
		$optionsS = $this->GetNameString ( 'options' );
		
		print ( "<div class='someGTitleBox'>$optionsS</div>" );
		print ( '<div class="someGBox">' );
		
		// tab header
		$this->DisplayTabHeader ( ++ $panelIndex, $renameS );
		
		$renameJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
		$this->DisplayFormHeadr ( 'rename', 'renameForm', 'post', $renameJS );
		$this->DisplayHidden ( $overLink , $ID );
		print ( "<input type='text' value='$objName' name='name' id='name' size='64' maxlength='256' />\n" );
		$this->DisplayFormFooter ( $doneS );
		
		// tab footer
		$this->DisplayTabFooter ();
			
		// tab header
		$this->DisplayTabHeader ( ++ $panelIndex, 'Single pattern' );
		
		// display single pattern form
		$this->DisplayFormHeadr ( 'pattern' );
		$this->DisplayHidden ( $overLink, $ID );
		$pattern = $this->m_pr_type->GetPattern ( $this->m_displayLang );
		print ( 'Single pattern<br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern' id='newpattern' class='mceNoEditor'>$pattern</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ( $doneS );
		
		// tab footer
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'list pattern');
		
		// display list pattern form
		$this->DisplayFormHeadr ( 'patternlist' );
		$this->DisplayHidden ( $overLink, $ID );
		$patternList = $this->m_pr_type->GetPatternList ( $this->m_displayLang );
		$patternList1 = $this->m_pr_type->GetPatternList1 ( $this->m_displayLang );
		$patternListHeader = $this->m_pr_type->GetPatternListHeader ( $this->m_displayLang );
		$patternListFooter = $this->m_pr_type->GetPatternListFooter ( $this->m_displayLang );
		print ( 'List pattern. It is allowed to use any option in @@FieldName@@.<br />You can not use options in HEADER & FOOTER.<br />Comments must be like: &lt;!--FieldName @@FieldName@@ FieldName--&gt;<br />' );
		print ( "<textarea rows='10' cols='80' name='newpatternheader' id='newpatternheader' class='mceNoEditor'>$patternListHeader</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern' id='newpattern' class='mceNoEditor'>$patternList</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern1' id='newpattern1' class='mceNoEditor'>$patternList1</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='10' cols='80' name='newpatternfooter' id='newpatternheader' class='mceNoEditor'>$patternListFooter</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ( $doneS );
		
		// tab footer
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'group pattern');
		
		// display group pattern form
		$this->DisplayFormHeadr ( 'patterngroup' );
		$this->DisplayHidden ( $overLink, $ID );
		$patternGroup = $this->m_pr_type->GetPatternGroup ( $this->m_displayLang );
		print ( 'List pattern. It is allowed to use only ID, random, name, subRootPath, subRootName, date, picture and short_desc in @@. <br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern' id='newpattern' class='mceNoEditor'>$patternGroup</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ( $doneS );
		
		// tab footer
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'search pattern');
		
		// display search pattern form
		$this->DisplayFormHeadr ( 'patternsearch' );
		$this->DisplayHidden ( $overLink, $ID );
		$patternSearch = $this->m_pr_type->GetPatternSearch ( $this->m_displayLang );
		print ( 'Search pattern. It is allowed to use only ID, random, name, fullPath, date, picture and short_desc in @@. <br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern' id='newpattern' class='mceNoEditor'>$patternSearch</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ( $doneS );
		
		// tab footer
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'gallery pattern');
		
		// display gallery pattern form
		$this->DisplayFormHeadr ( 'patterngallery' );
		$this->DisplayHidden ( $overLink, $ID );
		$patternGallery = $this->m_pr_type->GetPatternGallery ( $this->m_displayLang );
		print ( 'Gallery pattern. It is allowed to use only path and desc in @@. <br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern' id='newpattern' class='mceNoEditor'>$patternGallery</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'relation pattern');
		$this->DisplayFormHeadr ( 'patternrelation' );
		$this->DisplayHidden ( $overLink, $ID );
		$patternRelation = $this->m_pr_type->GetPatternRelation ( $this->m_displayLang );
		$patternRelationHeader = $this->m_pr_type->GetPatternRelationHeader ( $this->m_displayLang );
		$patternRelationFooter = $this->m_pr_type->GetPatternRelationFooter ( $this->m_displayLang );
		print ( 'Relation pattern. It is allowed to use only path and desc in @@. <br />' );
		print ( "<textarea rows='10' cols='80' name='newpatternheader' id='newpatternheader' class='mceNoEditor'>$patternRelationHeader</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern' id='newpattern' class='mceNoEditor'>$patternRelation</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='10' cols='80' name='newpatternfooter' id='newpatternfooter' class='mceNoEditor'>$patternRelationFooter</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'others pattern');
		$this->DisplayFormHeadr ( 'patternother' );
		$this->DisplayHidden ( $overLink, $ID );
		$patternOther = $this->m_pr_type->GetPatternOther ( $this->m_displayLang );
		$patternOtherHeader = $this->m_pr_type->GetPatternOtherHeader ( $this->m_displayLang );
		$patternOtherFooter = $this->m_pr_type->GetPatternOtherFooter ( $this->m_displayLang );
		print ( 'Relation pattern. It is allowed to use only path and desc in @@. <br />' );
		print ( "<textarea rows='10' cols='80' name='newpatternheader' id='newpatternheader' class='mceNoEditor'>$patternOtherHeader</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern' id='newpattern' class='mceNoEditor'>$patternOther</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='10' cols='80' name='newpatternfooter' id='newpatternfooter' class='mceNoEditor'>$patternOtherFooter</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'Wishlist pattern');
		$this->DisplayFormHeadr ( 'patternwishlist' );
		$this->DisplayHidden ( $overLink, $ID );
		$patternWishlist = $this->m_pr_type->GetPatternWishlist ( $this->m_displayLang );
		$patternWishlistHeader = $this->m_pr_type->GetPatternWishlistHeader( $this->m_displayLang );
		$patternWishlistFooter = $this->m_pr_type->GetPatternWishlistFooter( $this->m_displayLang );
		print ( "<textarea rows='10' cols='80' name='newpatternheader' id='newpatternheader' class='mceNoEditor'>$patternWishlistHeader</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='15' cols='80' name='newpattern' id='newpattern' class='mceNoEditor'>$patternWishlist</textarea>" );
		print ( '<br /><br />' );
		print ( "<textarea rows='10' cols='80' name='newpatternfooter' id='newpatternfooter' class='mceNoEditor'>$patternWishlistFooter</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'Relation to Structure');
		$this->DisplayFormHeadr ( 'structure_relation' );
		$this->DisplayHidden ( $overLink, $ID );
		
		$menuList = new MenuList();
		$menuListArr = $menuList->GetStructure();
		
		$structureID = $this->m_pr_type->GetStructureID();
		?>
		<select name="structureID">
			<option value="0">...</option>
			<?php
			for ( $m=0; $m<count($menuListArr); $m++ )
			{
				$menuID = $menuListArr[$m]["ID"];
				?>
				<option value="menu_<?php echo $menuID;?>" disabled><?php echo $menuListArr[$m]["name"];?></option>
				<?php
				$menu = new Menu($menuID, true, "en", true);
				$menuItem = $menu->GetStructure();
				
				$structure = array ();
				$menu->GetStructureFromDB($structure, 1, "en", true);
				for ( $s=0; $s<count($structure); $s++ )
				{
					?>
					<option value="<?php echo $structure[$s]["ID"];?>"<?php echo ($structureID==$structure[$s]["ID"] ? ' selected': '');?>>&nbsp;&nbsp;&nbsp;<?php echo $structure[$s]["name"];?></option>
					<?php
				}
			}
			?>
		</select>
		<?php
		
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter();
		
		// tab header
		$this->DisplayTabHeader(++$panelIndex, 'Order');
		$this->DisplayFormHeadr ( 'setOrder' );
		$this->DisplayHidden ( $overLink, $ID );
		$order = $this->m_pr_type->GetOrder();
		print ( "<input type='text' name='order' id='order' value='$order' />" );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter();
		
		//	icon & cover params
		$this->DisplayTabHeader(++$panelIndex, 'Set Icon, Cover & Thumbnail parameters');
		$this->DisplayFormHeadr ( 'setImageParams' );
		$this->DisplayHidden ( $overLink, $ID );
		$iconI = $this->m_pr_type->GetIconSize();
		$iconI = explode(";", $iconI);
		$coverI = $this->m_pr_type->GetCoverSize();
		$coverI = explode(";", $coverI);
		$thumbnailI = $this->m_pr_type->GetThumbnailSize();
		$thumbnailI = explode(";", $thumbnailI);
		$iconC = $this->m_pr_type->GetIconCrop();
		$coverC = $this->m_pr_type->GetCoverCrop();
		$thumbnailC = $this->m_pr_type->GetThumbnailCrop();
		print ( "<b>Icon</b><br />" );
		print ( "Width: <input type='text' name='iconW' id='iconW' value='".$iconI[0]."' /> Height: <input type='text' name='iconH' id='iconH' value='".$iconI[1]."' /> Crop <input type='checkbox' name='iconC' id='iconC' ".($iconC ? "checked": "")."/><br /><br />" );
		print ( "<b>Cover</b><br />" );
		print ( "Width: <input type='text' name='coverW' id='coverW' value='".$coverI[0]."' /> Height: <input type='text' name='coverH' id='coverH' value='".$coverI[1]."' /> Crop <input type='checkbox' name='coverC' id='coverC' ".($coverC ? "checked": "")."/><br /><br />" );
		print ( "<b>Thumbnail</b><br />" );
		print ( "Width: <input type='text' name='thumbnailW' value='".$thumbnailI[0]."' /> Height: <input type='text' name='thumbnailH' value='".$thumbnailI[1]."' /> Crop <input type='checkbox' name='thumbnailC' ".($thumbnailC ? "checked": "")."/><br /><br />" );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter();
		
		//	tab access
		$this->DisplayTabHeader(++$panelIndex, 'Tabs Access & Redirection');
		$this->DisplayFormHeadr ( 'setTabs' );
		$this->DisplayHidden ( $overLink, $ID );
		$tabs = $this->m_pr_type->GetTabs();
		if ( $tabs )
		{
			$tabs = unserialize($tabs);
		}
		else 
		{
			$tabs = array();
		}
		print ( "<table cellpadding='2' width='100%'>" );
		print ( "<tr><td valign='middle' width='150'>Short Description:</td><td valign='middle'><input type='checkbox' name='tabs[]' value='short' ".(in_array('short', $tabs) ? "checked": "")."/></td></tr>" );
		print ( "<tr><td valign='middle'>Controller:</td><td valign='middle'><input type='checkbox' name='tabs[]' value='controller' ".(in_array('controller', $tabs) ? "checked": "")."/></td></tr>" );
		print ( "<tr><td valign='middle'>View Count:</td><td valign='middle'><input type='checkbox' name='tabs[]' value='viewcount' ".(in_array('viewcount', $tabs) ? "checked": "")."/></td></tr>" );
		print ( "<tr><td valign='middle'>META options:</td><td valign='middle'><input type='checkbox' name='tabs[]' value='meta' ".(in_array('meta', $tabs) ? "checked": "")."/></td></tr>" );
		print ( "<tr><td valign='middle'>Author:</td><td valign='middle'><input type='checkbox' name='tabs[]' value='author' ".(in_array('author', $tabs) ? "checked": "")."/></td></tr>" );
		print ( "<tr><td colspan='2'><hr size='1'></td></tr>" );
		print ( "<tr><td valign='middle'>Redirect to first article:</td><td valign='middle'><input type='checkbox' name='tabs[]' value='redirect' ".(in_array('redirect', $tabs) ? "checked": "")."/></td></tr>" );
		print ( "</table>" );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter();
		
		print ( '</div>' );
		
		// display javascript
		print ( '<script type="text/javascript"><!--' );
		
		for($index = 0; $index <= $panelIndex; $index ++)
			print ( "var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n" );
		
		print ( '//--></script>' );
		
		
		//	display relation to other product types
		$productList = new ProductList();
		$allPT = $productList->GetProductTypes("`ID`", "asc");
		if ( count($allPT) )
		{
			$ptRelations = $this->m_pr_type->GetPTRelations();
			if ( !is_array($ptRelations) )
			{
				$ptRelations = array();
			}
			
			$this->DisplayFormHeadr ( 'ptrelation', 'ptfield' );
			$this->DisplayHidden ( $overLink, $ID );
			?>
			<br />
			<h2>Relation to Product Types</h2>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
				<tr>
					<td>
						<div class="titleColR">
							<span style="padding-right: 20px">
								<strong><?php echo $selectS?></strong>
								<a href="#" onclick="chekeBoxes('ptfield', 'ptIDs', true); return false"><?php echo $allS?></a>,
								<a href="#" onclick="chekeBoxes('ptfield', 'ptIDs', false); return false"><?php echo $noneS?></a>
							</span> 
							<input name="" type="submit" value="<?php echo $doneS?>" />
						</div>
						<br class="clearFloat" />
					</td>
				</tr>
			</table>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" style="border-left: 1px solid #777777; border-right: 1px solid #777777">
				<tr>
					<td>
						<?php
						for ( $i=0; $i<count($allPT); $i++ )
						{
							if ( $allPT[$i]["ID"]!=$ID )
							{
								?>
								<div class='<?php echo ($i%2==0 ? "tableUneven": "tableEven");?>'>
									<div class='tableCHB'><input name='ptIDs[]' id='ptIDs' type='checkbox' value='<?php echo $allPT[$i]["ID"];?>'<?php echo (in_array($allPT[$i]["ID"], $ptRelations) ? " checked": "");?> /></div>
									<div class='tableT'><?php echo $allPT[$i]["name"];?></div>
									<br class='clearFloat' />
								</div>
								<?php
							}
						}
						?>
					</td>
				</tr>
			</table>
			</form>
			<?php
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function Delete()
	{
		return $this->m_pr_type->Delete ();
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Update()
	{
		$ID = DB::REQUEST ('pr_typeID');
		$this->m_pr_type = new Pr_type ( $ID );
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$langs = $this->m_site->GetLanguages ();
		
		$addS = '';
		
		foreach ( $langs as $lang )
		{
			$addS .= "`pattern_$lang` text,
					  `patternlist_$lang` text,
					  `patterngroup_$lang` text,
					  `patterngallery_$lang` text,";
		}
		
		$sql = "CREATE TABLE `pr_type` (
			  `ID` smallint(5) unsigned NOT NULL auto_increment,
			  `name` varchar(255) NOT NULL,
			  $addS
			  PRIMARY KEY  (`ID`)
			)DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>