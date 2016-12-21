<?php
// includes
Site::IncludeFile ( 'classes/Category/Category.class.inc' );
Site::IncludeFile ( 'classes/Category/CategoryView.class.inc' );
Site::IncludeFile ( 'classes/Category/SCRelationCMS.class.inc' );
Site::IncludeFile ( 'classes/Category/SFRelationCMS.class.inc' );

Site::IncludeFile ( 'classes/Menu/Menu.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuList.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuView.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuCMS.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuListCMS.class.inc' );

// auto key gen
Site::IncludeFile ( 'classes/Category/Autokeyword.class.inc' );
/**
 * Enter description here...
 * @package Structure
 *
 */
class CategoryCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_menuID;
	
	/**
	 * Enter description here...
	 *
	 * @var CategoryView
	 */
	protected $m_categoryView;
	
	/**
	 * Enter description here...
	 *
	 * @var Category
	 */
	protected $m_category;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @param string $lang
	 * @return CategoryViewCMS
	 */
	function __construct()
	{
		parent::__construct ();
		
		// getting properties form request
		$ID = DB::REQUEST ( 'catID' );
		
		// call paretn constructure
		$this->m_categoryView = new CategoryView ( $ID, false );
		$this->m_category = $this->m_categoryView->GetCategoryPtr ();
		
		// get menu id from request
		$this->m_menuID = $this->m_site->GetValue ( 'menuID' );
		
		$this->ReadNameStrings ( 'classes/Category/langs/', 'category.xml' );
	}
	
	/**
	 * Do category action, 
	 * delete category, 
	 * rename, 
	 * insert new sub category, 
	 * reposition, 
	 * move 
	 * hide or show
	 *
	 * @return bool
	 */
	function DoAction()
	{
		// structure to content relation
		$scr = new SCRelationCMS ();
		$scr->DoAction ( $this->m_category->GetID () );
		
		// menu action
		if (CMS::GetUser ()->GetUserTypeID () == 1)
		{
			$menuListCMS = new MenuListCMS ();
			$menuListCMS->DoAction ();
		}
		
		// hasn`t action
		$actionResult = null;
		
		switch ($this->GetFormAction ())
		{
			case 'rename' :
				$newName = trim ( DB::POST ( 'name' ) );
				$actionResult = $this->m_category->SetName ( $newName, $this->m_displayLang );
				break;
			case 'insert' :
				$newName = DB::POST ( 'insertSubName' );
				if ($newName)
				{
					$actionResult = $this->m_category->InsertSub ( $newName, true, true, true, true, $this->m_menuID );
				}
				break;
			case 'delete' :
				$actionResult = $this->m_category->Delete ();
				break;
			case 'reposition' :
				$npos = DB::POST ( 'newPosition' );
				$actionResult = $this->m_category->Reposition ( $npos );
				break;
			case 'move' :
				$npar = DB::POST ( 'newParent' );
				$actionResult = $this->m_category->Move ( $npar );
				break;
			case 'pathname' :
				$updating = $this->IsRecUpdatable ( $this->m_category, 'pathname' );
				if ($updating)
				{
					$pathname = DB::POST ( 'pathname' );
					$actionResult = $this->m_category->SetPathName ( $pathname );
					
					if ($actionResult)
					{
						$this->m_category->SavePath ();
					}
				}
				break;
			case 'visible' :
				$show = DB::POST ( 'show' );
				$actionResult = $this->m_category->Show ( $show );
				break;
			case 'redirect' :
				$redir = DB::POST ( 'redirect' );
				$actionResult = $this->m_category->SetRedirect ( $redir );
				break;
			case 'contentcount' :
				$ccoutn = DB::POST ( 'contcount' );
				$actionResult = $this->m_category->SetContentCount ( $ccoutn );
				break;
			case 'options' :
				// get new name of category
				$title = DB::POST ( 'title' );
				$desc = DB::POST ( 'description' );
				$actionResult = $this->m_category->SetKeywords ( $title, $this->m_displayLang, 2 );
				$actionResult = $this->m_category->SetKeywords ( $desc, $this->m_displayLang, 3 );
				
				$titleopt = DB::POST ( 'titleopt' );
				$descopt = DB::POST ( 'descopt' );
				$actionResult = $this->m_category->SetKeywordsOpt ( $titleopt, 2 );
				$actionResult = $this->m_category->SetKeywordsOpt ( $descopt, 3 );
				break;
			case 'setsecurity' :
				$security = DB::POST ( 'security' );
				$actionResult = $this->m_category->SetSecurity ( $security );
				break;
			case 'fileRelation' :
				$sfRelation = new SFRelation ( $this->m_category->GetID () );
				
				$ffile = DB::POST ( 'firstFile' );
				$sfile = DB::POST ( 'secondFile' );
				
				// first file
				if ($ffile)
				{
					$actionResult = $sfRelation->Add ( $ffile, 0 );
				}
				else
				{
					$actionResult = $sfRelation->DeleteRelation ( 0 );
				}
				
				// second file
				if ($sfile)
				{
					$actionResult = $sfRelation->Add ( $sfile, 1 );
				}
				else
				{
					$actionResult = $sfRelation->DeleteRelation ( 1 );
				}
				break;
			case 'changecache' :
				$cache = DB::POST ( 'cachevalue' );
				$actionResult = $this->m_category->SetCache ( $cache );
				break;
			case 'selectTemplate' :
				$template = DB::POST ( 'template' );
				$actionResult = $this->m_category->SetTemplate ( $template );
				break;
			case 'teleportAct' :
				$menuID = DB::POST ( 'teleportTo' );
				// set menu ID
				$this->m_category->SetMenuID ( $menuID );
				// change parent to ROOT
				$actionResult = $this->m_category->Move ( 1, $menuID );
				// calculate new paths 
				break;
			case 'changeControls' :
				$userID = CMS::GetUser ()->GetID ();
				if ($userID == 1)
				{
					$isDelete = DB::POST ( 'isDelete' ) ? 1 : 0;
					$isInsert = DB::POST ( 'isInsert' ) ? 1 : 0;
					$isVisible = DB::POST ( 'isVisible' ) ? 1 : 0;
					$isMove = DB::POST ( 'isMove' ) ? 1 : 0;
					$isContent = DB::POST ( 'isContent' ) ? 1 : 0;
					
					$this->m_category->SetValue ( 'isDelete', $isDelete);
					$this->m_category->SetValue ( 'isInsert', $isInsert);
					$this->m_category->SetValue ( 'isVisible', $isVisible);
					$this->m_category->SetValue ( 'isMove', $isMove);
					$this->m_category->SetValue ( 'content_count', $isContent);
					
					$actionResult = true;
				}
				
				break;
			case 'pagecontent':
				$cshowtitle = (isset($_POST["cshowtitle"]) && $_POST["cshowtitle"]=="on" ? 1: 0);
				$ctitle = DB::POST ( 'ctitle' );
				$ctext = DB::POST ( 'ctext' );
				$actionResult = $this->m_category->SetShowContentTitle($cshowtitle);
				$actionResult = $this->m_category->SetContentTitle($ctitle, $this->m_displayLang);
				$actionResult = $this->m_category->SetContentText($ctext, $this->m_displayLang);
				break;
		}
		
		if ($actionResult) // if no error then update
		{
			$this->m_category->Update ();
		}
		
		return $actionResult;
	}
	
	/**
	 * Display category features and return dowen category ID
	 *
	 * @return int
	 */
	function DisplayFeatures()
	{
		$this->DisplayStructFeatures ();
		$this->DisplayContFeatures ();
		
		return $this->m_category->GetID ();
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayContFeatures()
	{
		$scr = new SCRelationCMS ();
		$scr->DisplayFeatures ( $this->m_category->GetID (), $this->m_displayLang );
	}
	
	/**
	 * Draw fithures with catgory (forms)
	 * forms for delete (if this feature allowned), 
	 * insert new sub (if this feature allowned and depth is not beg then maximum depth of menu), 
	 * rename, 
	 * reposition, 
	 * move (if this feature allowned and new parent isn`t sub of corrent), 
	 * visible or hiden
	 * $menuID need for root catrogry
	 * @param string $lang
	 * @param int $menuID
	 * @return bool
	 */
	function DisplayStructFeatures()
	{
		$catID = $this->m_category->GetID ();
		
		// if category and menu have not been selected then display new menu feature  
		if ($catID == null)
		{
//			$isOwner = (CMS::GetUser ()->GetUserTypeID () == 1);
			$menuListCMS = new MenuListCMS ();
			$menuListCMS->DisplayFeatures ();
			
			return false;
		}
		
		$actionName = $this->GetFormActionName ();
		
		$pathname = $this->m_category->GetPathName ();
		$catMenuID = $this->m_category->GetMenuID ();
		
		$name = $this->m_category->GetName ( $this->m_displayLang );
		$keys = $this->m_category->GetKeywords ( $this->m_displayLang );
		$title = $this->m_category->GetTitle ( $this->m_displayLang );
		$desc = $this->m_category->GetDescription ( $this->m_displayLang );
		
		$pageShowContentTitle = $this->m_category->GetShowContentTitle ( $this->m_displayLang );
		$pageContentTitle = $this->m_category->GetContentTitle ( $this->m_displayLang );
		$pageContentText = $this->m_category->GetContentText ( $this->m_displayLang );
		
		// get global names
		$doneS = $this->GetNameString ( 'done' );
		$deleteS = $this->GetNameString ( 'delete' );
		$noEmptyS = $this->GetNameString ( 'noempty' );
		$optionsS = $this->GetNameString ( 'options' );
		
		// dispaly structure title and delete form
		$form = null;
		
		if ($this->m_category->IsDelete ())
		{
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			
			$form = "<div  class='titleColR'>
					 <form method='post' class='clearPD'> 
					 <input type='hidden' name='$actionName' value='delete' />
					 <input type='hidden' name='catID' value='$catID' />
					 <input type='submit' onclick='return confirm(\"$sureDeleteS\");' value='$deleteS' />
					 </form>
					 </div>";
		}
		
		$this->DisplayTitle ( $name, $form, false );
		
		$panelIndex = 0;
		
		print ("<div class='someGTitleBox'>$optionsS</div>") ;
		print ('<div class="someGBox">') ;
		
		// rename
		if ($catID > 1)
		{
			$reading = $this->IsRecReadable ( $this->m_category, 'name' );
			$updating = $this->IsRecUpdatable ( $this->m_category, 'name' );
			
			if ($reading || $updating)
			{
				$rename = $this->GetNameString ( 'rename' );
				
				// tab header
				$this->DisplayTabHeader ( ++ $panelIndex, $rename );
				
				if ($updating)
				{
					$renameJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
					$this->DisplayFormHeadr ( 'rename', 'renameForm', 'post', $renameJS );
					$this->DisplayHidden ( 'catID', $catID );
					print ("<input type=\"text\" value=\"$name\" name=\"name\" id=\"name\" size=\"64\" maxlength=\"256\" />\n") ;
					$this->DisplayFormFooter ( $doneS );
				}
				else
				{
					print ($name) ;
				}
				
				// tab footer
				$this->DisplayTabFooter ();
			}
		}
		
		// path name
		if ($catID > 2)
		{
			// field controll 
			$reading = $this->IsRecReadable ( $this->m_category, 'pathname' );
			$updating = $this->IsRecUpdatable ( $this->m_category, 'pathname' );
			
			// if field is can be read or update then display tab
			if ($reading || $updating)
			{
				$pathnameS = $this->GetNameString ( 'pathname' );
				
				// tab header
				$this->DisplayTabHeader ( ++ $panelIndex, $pathnameS );
				
				// if filed can be updated then display form 
				if ($updating)
				{
					$pathJS = "onsubmit='return SingleForm(\"pathname\", \"$noEmptyS\");'";
					$this->DisplayFormHeadr ( 'pathname', 'pathForm', 'post', $pathJS );
					$this->DisplayHidden ( 'catID', $catID );
					print ("<input type=\"text\" value=\"$pathname\" name=\"pathname\" id=\"pathname\" size=\"36\" maxlength=\"32\">\n") ;
					$this->DisplayFormFooter ( $doneS );
				}
				// display field value
				else
				{
					print ($pathname) ;
				}
				
				// tab footer
				$this->DisplayTabFooter ();
			}
		}
		
		// insert
		$depth = $this->m_category->GetDepth ();
		
		// get menu with structure
		$menu = new Menu ( $catMenuID, true, $this->m_displayLang, true );
		$menDepth = $menu->GetDepth ();
		
		if ($this->m_category->IsInsert () && ($depth < $menDepth || $catID == 1) && $this->IsReadable ( $this->m_category, 'path' ))
		{
			$subcategoryS = $this->GetNameString ( 'subcategory' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $subcategoryS );
			
			$iJS = "onsubmit='return SingleForm(\"insertSubName\", \"$noEmptyS\");'";
			
			$this->DisplayFormHeadr ( 'insert', 'insertForm', 'post', $iJS );
			$this->DisplayHidden ( 'catID', $catID );
			
			if ($this->m_menuID != null)
			{
				print ("<input type='hidden' name='menuID' value='{$this->m_menuID}' />") ;
			}
			
			print ('<input type="text" value="" name="insertSubName" id="insertSubName" size="64" maxlength="256" />') ;
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// move
		if ($this->m_category->IsMove () && $this->IsReadable ( $this->m_category, 'path' ))
		{
			$moveS = $this->GetNameString ( 'move' );
			$selectDestinationS = $this->GetNameString ( 'selectDestination' );
			
			// get menu structure
			$struct = $menu->GetStructure ();
			$drMenu = new MenuView ( $struct, $this->m_displayLang );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $moveS );
			
			$this->DisplayFormHeadr ( 'move' );
			$this->DisplayHidden ( 'catID', $catID );
			print ("<label>$selectDestinationS</label> ") ;
			print ("<select name='newParent'>\n") ;
			print ("<option value='1'>Root</option>\n") ;
			$drMenu->Display ( $catID, 5 );
			print ("</select>\n") ;
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		//move to other menu
		if ($catID > 1 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			// display for header
			$this->DisplayFormHeadr ( 'teleport' );
			
			// get menu list
			$menuList = new MenuList ();
			$menus = $menuList->GetStructure ();
			
			// get category menu ID
			$menuID = $this->m_category->GetMenuID ();
			
			// get category global sub depth
			$subDepth = $this->m_category->GetMaxSubDepth ();
			$subDepth ++;
			
			// names strings
			$teleportTabS = $this->GetNameString ( 'teleportTab' );
			$selectDestinationS = $this->GetNameString ( 'selectDestination' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $teleportTabS );
			
			$this->DisplayFormHeadr ( 'teleportAct' );
			$this->DisplayHidden ( 'catID', $catID );
			print ("<label>$selectDestinationS</label> ") ;
			print ("<select name='teleportTo'>\n") ;
			
			// loop for each menu
			foreach ( $menus as $menu )
			{
				// if menu depth is greate or equal then category sub depth then display it
				if ($subDepth <= $menu ['depth'])
				{
					// current menu for category
					if ($menuID == $menu ['ID'])
					{
						print ("<option value='{$menu['ID']}' selected='selected'>{$menu['name']}</option>\n") ;
					}
					// other menus
					else
					{
						print ("<option value='{$menu['ID']}'>{$menu['name']}</option>\n") ;
					}
				}
			}
			
			print ("</select>\n") ;
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		//choose template for current category
		$templates = $this->m_site->GetTemplates ();
		if ($templates && $catID > 1 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			// get current template
			$tempID = $this->m_category->GetTemplate ();
			
			$templateTabS = $this->GetNameString ( 'templateTab' );
			$selectDestinationS = $this->GetNameString ( 'selectDestination' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $templateTabS );
			
			$this->DisplayFormHeadr ( 'selectTemplate' );
			$this->DisplayHidden ( 'catID', $catID );
			print ("<label>$selectDestinationS</label> ") ;
			print ("<select name='template'>\n") ;
			
			foreach ( $templates as $key => $value )
			{
				if ($tempID == $key)
				{
					print ("<option value='$key' selected='selected'>$value</option>\n") ;
				}
				else
				{
					print ("<option value='$key'>$value</option>\n") ;
				}
			}
			
			print ("</select>\n") ;
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// reposition
		if ($catID > 1 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			$repostionS = $this->GetNameString ( 'reposition' );
			
			// get colegas
			$IDs = $this->m_category->GetColegaIDs ();
			
			$count = count ( $IDs );
			
			if ($count > 0)
			{
				// get names
				$firstS = $this->GetNameString ( 'first' );
				$afterS = $this->GetNameString ( 'after' );
				
				// tab header
				$this->DisplayTabHeader ( ++ $panelIndex, $repostionS );
				
				$this->DisplayFormHeadr ( 'reposition' );
				$this->DisplayHidden ( 'catID', $catID );
				print ('<select name="newPosition">') ;
				print ("<option value='1'>$firstS</option>") ;
				
				for($index = 0; $index < $count; $index ++)
				{
					$cCategory = new Category ( $IDs [$index] );
					$pos = $index + 2;
					print ("<option value='$pos'>$afterS &quot; {$cCategory->GetName($this->m_displayLang)} &quot;</option>") ;
				}
				
				print ('</select>') ;
				$this->DisplayFormFooter ( $doneS );
				
				// tab footer
				$this->DisplayTabFooter ();
			}
		}
		
		// redirect
		if ($catID > 1 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			$redirectS = $this->GetNameString ( 'redirect' );
			
			$subIDs = array ();
			$this->m_category->GetSubIDs ( $subIDs );
			if (count ( $subIDs ) > 0)
			{
				// tab header
				$this->DisplayTabHeader ( ++ $panelIndex, $redirectS );
				
				$this->DisplayFormHeadr ( 'redirect' );
				$this->DisplayHidden ( 'catID', $catID );
				print ("<select name='redirect'>") ;
				
				$donotRedirectS = $this->GetNameString ( 'donotRedirect' );
				$firstRedirectS = $this->GetNameString ( 'firstRedirect' );
				$lastRedirectS = $this->GetNameString ( 'lastRedirect' );
				
				switch ($this->m_category->GetRedirect ())
				{
					case 0 :
						print ("<option value='0' selected>$donotRedirectS</option>\n") ;
						print ("<option value='1'>$firstRedirectS</option>\n") ;
						print ("<option value='2'>$lastRedirectS</option>\n") ;
						break;
					case 1 :
						print ("<option value='0'>$donotRedirectS</option>\n") ;
						print ("<option value='1' selected>$firstRedirectS</option>\n") ;
						print ("<option value='2'>$lastRedirectS</option>\n") ;
						break;
					case 2 :
						print ("<option value='0'>$donotRedirectS</option>\n") ;
						print ("<option value='1'>$firstRedirectS</option>\n") ;
						print ("<option value='2' selected>$lastRedirectS</option>\n") ;
						break;
				}
				
				print ("</select>") ;
				$this->DisplayFormFooter ( $doneS );
				
				// tab footer
				$this->DisplayTabFooter ();
			}
		}
		
		// Visibility
		if ($this->m_category->GetID () > 1 && $this->m_category->IsVsible () && $this->IsReadable ( $this->m_category, 'path' ))
		{
			// get names
			$visibilityS = $this->GetNameString ( 'visibility' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $visibilityS );
			
			$this->DisplayFormHeadr ( 'visible' );
			$this->DisplayHidden ( 'catID', $catID );
			$showS = $this->GetNameString ( 'show' );
			$hideS = $this->GetNameString ( 'hide' );
			
			if ($this->m_category->GetVisible () == 1)
			{
				print ('<label>' . $showS . ': </label>
                            <input name="show" type="radio" value="1" checked />
                            <label>' . $hideS . ': </label>
                            <input name="show" type="radio" value="0" />') ;
			}
			else
			{
				print ('<label>' . $showS . ': </label>
                            <input name="show" type="radio" value="1" />
                            <label>' . $hideS . ': </label>
                            <input name="show" type="radio" value="0" checked />') ;
			}
			
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// change content type
		$concount = $this->m_category->GetRelContentCount ();
		if ($concount > 0 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			// get names
			$contenttypeS = $this->GetNameString ( 'contenttype' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $contenttypeS );
			
			$this->DisplayFormHeadr ( 'contentcount' );
			$this->DisplayHidden ( 'catID', $catID );
			
			$signlContentS = $this->GetNameString ( 'singleContent' );
			$multiContentS = $this->GetNameString ( 'multiContent' );
			
			if ($concount == 1)
			{
				print ('<label>' . $signlContentS . ': </label>
                            <input name="contcount" type="radio" value="1" checked />
                            <label>' . $multiContentS . ': </label>
                            <input name="contcount" type="radio" value="2" />') ;
			}
			else
			{
				print ('<label>' . $signlContentS . ': </label>
                            <input name="contcount" type="radio" value="1" />
                            <label>' . $multiContentS . ': </label>
                            <input name="contcount" type="radio" value="2" checked />') ;
			}
			
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// related files only for 'Owner' users type = 1
		if ($catID > 1 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			// get names
			$relatedFilesS = $this->GetNameString ( 'relatedFiles' );
			$fileBeforeS = $this->GetNameString ( 'fileBefore' );
			$fileAfterS = $this->GetNameString ( 'fileAfter' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $relatedFilesS );
			
			$this->DisplayFormHeadr ( 'fileRelation' );
			$this->DisplayHidden ( 'catID', $catID );
			
			// get related files
			$sfRelation = new SFRelation ( $catID );
			$relFiles = $sfRelation->GetCatRelations ();
			
			// get all files from files folder
			$files = scandir ( '../files' );
			
			$firstOptions = "<option value='0'> - - - </option>";
			$secondOptions = "<option value='0'> - - - </option>";
			
			$firstFile = null;
			$secondFile = null;
			
			if ($relFiles)
			{
				foreach ( $relFiles as $relFile )
				{
					if ($relFile ['show_type'] == 0)
					{
						$firstFile = $relFile ['filename'];
					}
					else
					{
						$secondFile = $relFile ['filename'];
					}
				}
			}
			
			foreach ( $files as $file )
			{
				if ($file != '.' && $file != '..')
				{
					if ($firstFile == $file)
					{
						$firstOptions .= "<option value='$file' selected='selected'>$file</option>\n";
					}
					else
					{
						$firstOptions .= "<option value='$file'>$file</option>\n";
					}
					
					if ($secondFile == $file)
					{
						$secondOptions .= "<option value='$file' selected='selected'>$file</option>\n";
					}
					else
					{
						$secondOptions .= "<option value='$file'>$file</option>\n";
					}
				}
			}
			
			// first file
			print ('<label><strong>' . $fileBeforeS . '</strong></label>
				   <div style="float:left">') ;
			print ('<select name="firstFile">') ;
			print ($firstOptions) ;
			print ('</select>') ;
			print ('</div><br class="clearFloat" />') ;
			
			// second file
			print ('<label><strong>' . $fileAfterS . '</strong></label>
				   <div style="float:left">') ;
			print ('<select name="secondFile">') ;
			print ($secondOptions) ;
			print ('</select>') ;
			print ('</div><br class="clearFloat" />') ;
			
			print ('<label><strong>&nbsp;</strong></label>
					<div>') ;
			print ("<input type='submit' value='$doneS' />") ;
			print ('</div>') ;
			print ('</form>') ;
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// page options   
		if ($catID > 1 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			$pageoptionsS = $this->GetNameString ( 'pageoptions' );
			
			$pageTitleS = $this->GetNameString ( 'pageTitle' );
			$pageKeywordS = $this->GetNameString ( 'pageKeyword' );
			$pageDescS = $this->GetNameString ( 'pageDesc' );
			
			$otherS = $this->GetNameString ( 'other' );
			$defoptS = $this->GetNameString ( 'defopt' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $pageoptionsS );
			
			$this->DisplayFormHeadr ( 'options' );
			$this->DisplayHidden ( 'catID', $catID );
			
			print ("<strong>$pageTitleS </strong>") ;
			print ('<table border="0" cellspacing="0" cellpadding="5">') ;
			print ('<tr><td>') ;
			$titleopt = $this->m_category->GetTitleOpt ();
			if ($titleopt)
			{
				print ('<input name="titleopt" type="radio" value="1" checked /><label>' . $otherS . '</label>
                    	<input name="titleopt" type="radio" value="0"  /><label>' . $defoptS . '</label>');
			}
			else
			{
				print ('<input name="titleopt" type="radio" value="1"  /><label>' . $otherS . '</label>
                    	<input name="titleopt" type="radio" value="0" checked/><label>' . $defoptS . '</label>');
			}
			print ('</td></tr>') ;
			print ('<tr><td>') ;
			print ("<input type='text' value='$title' name='title' size='64' maxlength='255' />\n") ;
			print ('</td></tr>') ;
			print ('</table>') ;
			print ('<br />') ;
			print ("<strong>$pageDescS</strong>") ;
			print ('<table border="0" cellspacing="0" cellpadding="5">') ;
			print ('<tr><td>') ;
			$descopt = $this->m_category->GetDescriptionOpt ();
			if ($descopt)
			{
				print ('<input name="descopt" type="radio" value="1" checked /><label>' . $otherS . '</label>
                    	<input name="descopt" type="radio" value="0"  /><label>' . $defoptS . '</label>');
			}
			else
			{
				print ('<input name="descopt" type="radio" value="1"  /><label>' . $otherS . '</label>
                    	<input name="descopt" type="radio" value="0" checked/><label>' . $defoptS . '</label>');
			}
			print ('</td></tr>');
			print ('<tr><td>');
			print ("<input type='text' value='$desc' name='description' size='64' maxlength='255' />\n");
			print ('</td></tr>');
			print ('</table>');
			print ('<br />');
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// security section
		if ($catID > 2 && $this->m_site->GetSFRoot () && $this->IsReadable ( $this->m_category, 'path' ))
		{
			$securityS = $this->GetNameString ( 'security' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $securityS );
			
			$this->DisplayFormHeadr ( 'setsecurity' );
			$this->DisplayHidden ( 'catID', $catID );
			$security = $this->m_category->GetSecurity ();
			
			$publicS = $this->GetNameString ( 'public' );
			$privateS = $this->GetNameString ( 'private' );
			
			if ($security)
			{
				print ("$publicS <input type='radio' name='security' value='0' />") ;
				print ("$privateS <input type='radio' name='security' value='1' checked />") ;
			}
			else
			{
				print ("$publicS <input type='radio' name='security' value='0' checked />") ;
				print ("$privateS <input type='radio' name='security' value='1' />") ;
			}
			
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// cache section
		if ($catID > 1 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			$cacheTitleS = $this->GetNameString ( 'cacheTitle' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $cacheTitleS );
			
			$this->DisplayFormHeadr ( 'changecache' );
			$this->DisplayHidden ( 'catID', $catID );
			$cache = $this->m_category->GetCache ();
			
			$casheS = $this->GetNameString ( 'cashe' );
			$nocasheS = $this->GetNameString ( 'nocashe' );
			
			if ($cache == 0)
			{
				print ("$casheS <input type='radio' name='cachevalue' value='1' />") ;
				print ("$nocasheS <input type='radio' name='cachevalue' value='0' checked />") ;
			}
			else
			{
				print ("$casheS <input type='radio' name='cachevalue' value='1' checked />") ;
				print ("$nocasheS <input type='radio' name='cachevalue' value='0' />") ;
			}
			
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// page content   
		if ($catID > 1 && $this->IsReadable ( $this->m_category, 'path' ))
		{
			$pagecontentS = $this->GetNameString ( 'pagecontent' );
			$pageContentTitleS = $this->GetNameString ( 'contentTitle' );
			$pagecontentTextS = $this->GetNameString ( 'contentText' );
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $pagecontentS );
			
			$this->DisplayFormHeadr ( 'pagecontent' );
			$this->DisplayHidden ( 'catID', $catID );
			
			print ("<strong>$pageContentTitleS </strong>");
			print ('<table border="0" cellspacing="0" cellpadding="5">');
			print ('<tr><td>');
			print ("<input type='text' value='$pageContentTitle' name='ctitle' size='64' maxlength='255' />&nbsp;&nbsp;&nbsp;<input type='checkbox' name='cshowtitle'".($pageShowContentTitle ? " checked": "")." /> show\n");
			print ('</td></tr>') ;
			print ('</table>') ;
			print ('<br />') ;
			print ("<strong>$pagecontentTextS</strong>") ;
			print ('<table border="0" cellspacing="0" cellpadding="5">') ;
			print ('<tr><td>') ;
			print ("<textarea name='ctext' />$pageContentText</textarea>\n") ;
			print ('</td></tr>') ;
			print ('</table>') ;
			print ('<br />') ;
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// display visible, delete, insert, move controler.
		$user = CMS::GetUser ();
		if ($catID > 1 && $user->GetID () == 1)
		{
			$adminControlsS = $this->GetNameString ( 'adminControls' );
			
			// get values
			$isDel = $this->m_category->IsDelete ();
			$isIns = $this->m_category->IsInsert ();
			$isVis = $this->m_category->IsVsible ();
			$isMov = $this->m_category->IsMove ();
			$isCco = $this->m_category->GetRelContentCount ();
			
			$isDelD = ($isDel) ? " checked='checked'" : '';
			$isInsD = ($isIns) ? " checked='checked'" : '';
			$isVisD = ($isVis) ? " checked='checked'" : '';
			$isMovD = ($isMov) ? " checked='checked'" : '';
			$isCcoD = ($isCco) ? " checked='checked'" : '';
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $adminControlsS );
			
			$this->DisplayFormHeadr ( 'changeControls' );
			$this->DisplayHidden ( 'catID', $catID );
			
			print ("<input type='checkbox' ID='isDelete' name='isDelete'{$isDelD} value='1' /> delete<br />") ;
			print ("<input type='checkbox' ID='isInsert' name='isInsert'{$isInsD} value='1' /> insert<br />") ;
			print ("<input type='checkbox' ID='isVisible' name='isVisible'{$isVisD} value='1' /> visible<br />") ;
			print ("<input type='checkbox' ID='isMove' name='isMove'{$isMovD} value='1' /> move<br />") ;
			print ("<input type='checkbox' ID='isContent' name='isContent'{$isCcoD} value='1' /> content<br />") ;
			print ('<br />') ;
			
			$this->DisplayFormFooter ( $doneS );
			
			// tab footer
			$this->DisplayTabFooter ();
		}
		print ('</div>') ;
		
		// display javascript
		print ('<script type="text/javascript"><!--') ;
		
		for($index = 0; $index <= $panelIndex; $index ++)
			print ("var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n") ;
		
		print ('//--></script>') ;
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
		$catID = $this->m_category->GetID ();
		// get names
		$menuManageS = $this->GetNameString ( 'menuManage' );
		
		// display all emnus
		$m = new MenuListCMS ();
		$m->DisplayMenu ( $menuManageS );
		print ('<br />') ;
		
		// selected menu 
		if ($this->m_menuID > 0)
		{
			// get names
			$rootS = $this->GetNameString ( 'root' );
			
			// get current menu to display
			$meanMenu = new Menu ( $this->m_menuID, true, $this->m_displayLang, true, true );
			
			$menuName = $meanMenu->GetName ();
			print ("<div class='someWTitleBox'>{$menuName}</div>") ;
			print ("<div class='someWBox'>") ;
			
			// display root
			if ($catID != 1)
			{
				print ("<div class='directoryMenu0'><a href='?catID=1'>$rootS</a></div>") ;
			}
			else
			{
				print ("<div class='directoryMenu0'>$rootS</div>") ;
			}
			
			// get menu structure
			$structure = $meanMenu->GetStructure ();
			
			// create new menuView object to display menu strucutre
			$trDraw = new MenuView ( $structure, $this->m_menuID );
			
			/// style for menu
			$style [1] = array ("divclass" => "directoryMenu1" );
			$style [2] = array ("divclass" => "directoryMenu2" );
			$style [3] = array ("divclass" => "directoryMenu3" );
			$style [4] = array ("divclass" => "directoryMenu4" );
			$style [5] = array ("divclass" => "directoryMenu5" );
			$style [6] = array ("divclass" => "directoryMenu6" );
			
			$trDraw->Display ( $this->m_category, 2, $style, $meanMenu->GetDepth () );
			
			print ("</div>\n</div>\n") ;
		}
	}
	
	/**
	 * Create category db (if its doesn`t exist)
	 * static function
	 * 
	 * @return bool
	 */
	function CreateDB()
	{
		$res = parent::CreateDB ();
		
		// create table of menu
		$menu = new MenuCMS ();
		$menu->CreateDB ();
		
		// create table of structure to file relation
		$sfRel = new SFRelationCMS ();
		$sfRel->CreateDB ();
		
		// insert 20 categorys if structure table is empty //
		$sql = 'SELECT (*) FROM `structure`';
		if (DB::QueryToValue ( $sql ) < 1)
		{
			$homeFilds = "";
			$homeValues = "";
			$languages = $this->m_site->GetLanguages ();
			
			foreach ( $languages as $language )
			{
				$homeFilds .= " , `name_$language`";
				$homeValues .= ", 'Root'";
			}
			
			$sql = "INSERT INTO `structure` (`ID`, `parentID`, `isdelete`, `isinsert`, `ismove`, `menuID`, `visible`, `position`, `content_count` $homeFilds) VALUES ('1', '0', '0', '1', '0', '0', '1', '1', '0' $homeValues);";
			$res = DB::Query ( $sql );
			
			for($index = 2; $index <= 20; $index ++)
			{
				$sql = "INSERT INTO `structure` (`ID`, `parentID`, `isdelete`, `isinsert`, `ismove`, `menuID`, `visible`, `position`, `content_count` $homeFilds) VALUES ('$index', '1', '0', '1', '0', '0', '1', '1', '0' $homeValues);";
				$res = DB::Query ( $sql );
			}
		}
		
		return $res;
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$languages = $this->m_site->GetLanguages ();
		
		$lnText = "";
		$keyText = "";
		$titleText = "";
		$descText = "";
		
		foreach ( $languages as $language )
		{
			$lnText .= " `name_$language` varchar(128) NOT NULL default 'category' , \n";
			$keyText .= " `key_$language` varchar(255) , \n";
			$titleText .= " `title_$language` varchar(255) ,\n";
			$descText .= " `desc_$language` varchar(255) , \n";
		}
		
		$sql = "CREATE TABLE IF NOT EXISTS `structure` (
				  `ID` smallint(5) unsigned NOT NULL auto_increment,
				  `isdelete` tinyint(1) NOT NULL default '1',
				  `isinsert` tinyint(1) NOT NULL default '1',
				  `ismove` tinyint(1) NOT NULL default '1',
				  `isvisible` tinyint(1) NOT NULL default '1',
				  `parentID` smallint(5) unsigned NOT NULL default '0',
				  `menuID` tinyint(3) NOT NULL default '0',
				  `visible` tinyint(1) NOT NULL default '1',
				  `position` tinyint(3) NOT NULL default '0',
				  `content_count` tinyint(3) NOT NULL default '1',
				  `file_count` tinyint(3) NOT NULL default '0',
				  `redirect` smallint(5) NOT NULL default '0',
				  `ispathcache` tinyint(1) NOT NULL default '0',
				  `pathname` varchar(32) NOT NULL default '',
				  `path` varchar(255) NOT NULL default '',
				  `cache` tinyint(1) NOT NULL default '1',
				  $lnText
				  $keyText
				  $titleText
				  $descText
				  `security` tinyint(1) NOT NULL default '0',
				  `titleopt` tinyint(1) NOT NULL default '0',
				  `descopt` tinyint(1) NOT NULL default '0',
				  `keyopt` tinyint(1) NOT NULL default '0',
				  `template` tinyint(4) NOT NULL default '1',
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		// test database /////////////////////////////////////////////////
		// main
		$res = $this->TestTable ();
		
		// call related module test
		//menu
		$menu = new MenuCMS ();
		$res &= $menu->SelfTest ();
		
		// create table of structure to file relation
		$sfRel = new SFRelationCMS ();
		$res &= $sfRel->SelfTest ();
		
		// END test database /////////////////////////////////////////////////
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		$tables = array ('structure' );
		
		// add menu table
		$menu = new MenuCMS ();
		$menuTalbes = $menu->GetObjectTables ();
		if ($menuTalbes)
		{
			$tables = array_merge ( $tables, $menuTalbes );
		}
		
		// add menu table
		$sfRel = new SFRelationCMS ();
		$sfRelTalbes = $sfRel->GetObjectTables ();
		if ($sfRelTalbes)
		{
			$tables = array_merge ( $tables, $sfRelTalbes );
		}
		
		return $tables;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param unknown_type $arg
	 * @return unknown
	 */
	function GetAjax($arg)
	{
		$res = null;
		
		$params = parent::RetriveAjaxParams ( $arg );
		
		switch ($params ['action'])
		{
			case 'autoKey' :
				// get carent cateogry ID
				$catID = $this->m_category->GetID ();
				
				// get related content
				$scr = new SCRelation ();
				$catRel = $scr->GetCatRelations ( $catID, $this->m_displayLang );
				
				if (count ( $catRel ) > 0)
				{
					$article = '';
					foreach ( $catRel as $cont )
					{
						$content = new Content ( $cont ['ID'] );
						
						$article .= $content->GetName ( $this->m_displayLang );
						$article .= $content->GetContent ( $this->m_displayLang, false );
					}
					
					// generate auto key gen
					$params ['content'] = $article; //page content 
					//set the length of keywords you like 
					$params ['min_word_length'] = 5; //minimum length of single words 
					$params ['min_word_occur'] = 2; //minimum occur of single words 
					

					$params ['min_2words_length'] = 3; //minimum length of words for 2 word phrases 
					$params ['min_2words_phrase_length'] = 10; //minimum length of 2 word phrases 
					$params ['min_2words_phrase_occur'] = 2; //minimum occur of 2 words phrase 
					

					$params ['min_3words_length'] = 3; //minimum length of words for 3 word phrases 
					$params ['min_3words_phrase_length'] = 10; //minimum length of 3 word phrases 
					$params ['min_3words_phrase_occur'] = 2; //minimum occur of 3 words phrase 
					

					$keyword = new autokeyword ( $params, "UTF-8" );
					
					$res = "<textarea cols='80' rows='10'>words\n";
					$res .= $keyword->parse_words ();
					$res .= "\n2 words phrase\n";
					$res .= $keyword->parse_2words ();
					$res .= "\n3 words phrase\n";
					$res .= $keyword->parse_3words ();
//					$res .= "\nAll together\n";
//					$res .= $keyword->get_keywords ();
					$res .= '</textarea>';
				}
				break;
		}
		
		return $res;
	}
}
?>