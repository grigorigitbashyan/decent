<?php
Site::IncludeFile ( 'classes/Common/ContentCaching.class.inc' );
// includes
Site::IncludeFile ( 'classes/Category/Category.class.inc' );
Site::IncludeFile ( 'classes/Content/Content.class.inc' );

Site::IncludeFile ( 'classes/UserManagement/UserManagementCMS.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserTypeListCMS.class.inc' );

Site::IncludeFile('classes/Common/ContentCaching.class.inc');
Site::IncludeFile('classes/Common/ValueCMS.class.inc');
Site::IncludeFile('classes/Folder/FolderXP.class.inc');

Site::IncludeFile('classes/Analitics/Analitics.class.inc');
Site::IncludeFile('classes/Analitics/AnaliticsList.class.inc');

Site::IncludeFile('classes/Common/Image.class.inc');
/**
 * Enter description here...
 *
 * @package Main
 */
class ControlPanelCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_subPanel;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_userStatus;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		// geting current sub category of control panel
		$this->m_subPanel = $this->m_site->GetValue ( 'subpanel' );
		
		// get current user
		$user = UserManagement::GetCurrentUser ();
		
		if ($user)
		{
			$this->m_userStatus = $user->GetUserTypeID ();
		}
		
		// read name strings
		$this->ReadNameStrings ( 'classes/Common/langs/', 'controlPanel.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return unknown
	 */
	function DoAction()
	{
		$userCMS = new UserManagementCMS ( );
		$res = $userCMS->DoAction ();
		
		switch ($this->GetFormAction ())
		{
			case 'websitename' :
				$newName = DB::POST ( 'title' );
				
				$category = new Category ( 1 );
				$category->SetName ( $newName, $this->m_displayLang, false );
				break;
			case 'metadata' :
				$rootCat = new Category ( 1 );
				
				// get values from form
				if (($title = DB::POST ( 'title' )))
				{
					$rootCat->SetKeywords ( $title, $this->m_displayLang, 2 );
				}
				// set keywords
				if (($keywords = DB::POST ( 'keywords' )))
				{
					$rootCat->SetKeywords ( $keywords, $this->m_displayLang, 1 );
				}
				// set description
				if (($description = DB::POST ( 'description' )))
				{
					$rootCat->SetKeywords ( $description, $this->m_displayLang, 3 );
				}
				
				$picture = $_FILES["picture"];
				if ( $picture["size"]>0 )
				{
					$name = str_replace(" ", "", $picture['name']);
					
					$path_parts = pathinfo($picture['name']);
					$esxt = $path_parts['extension'];
					$esxt = strtolower($esxt);
					
					$pos = strripos($name, $esxt);
					$name = rand(11111, 99999).".".$esxt;
					
					$types = array('jpg', 'jpeg', 'png');
					$res = array_search($esxt, $types);
					if ( is_int($res) )
					{
						$fileAddress = "../media/images/".$name;
						if ( !file_exists($fileAddress) )
						{
							$oldPicture = $rootCat->GetPicture();
							if ( $oldPicture)
							{
								$oldPicture = "../".$oldPicture;
								if ( file_exists($oldPicture) )
								{
									unlink($oldPicture);
								}
							}
							
							$res = FolderXP::Upload($picture['tmp_name'], $fileAddress);
							
							//	resize
							$maxWidth = $this->m_site->GetMaxImageWidth();
							$im = new Image();
							$im->ChangeSizeInBox($fileAddress, $maxWidth);
							
							$fileAddress = substr($fileAddress, 3);
							$rootCat->SetPicture($fileAddress);
						}
					}
				}
				elseif ( isset($_POST["delete"]) && $_POST["delete"]=="on" ) 
				{
					$oldPicture = $rootCat->GetPicture();
					if ( $oldPicture)
					{
						$oldPicture = "../".$oldPicture;
						if ( file_exists($oldPicture) )
						{
							unlink($oldPicture);
						}
					}
					
					$rootCat->SetPicture("");
				}
				break;
			case 'status' :
				// read values form form
				$status = DB::POST ( 'status' );
				$content = new Content ( 1 );
				$content->UpdateVisibility ( $status );
				break;
			case 'sitemapxml' :
				// get text from post
				if ( $str = $_POST['content'] )
				{
					// open robots.txt file anr write new content
					file_put_contents ( '../sitemap.xml', $str );
				}
				break;
			case 'robotstxt' :
				// get text from post
				if ( $str = $_POST['content'] )
				{
					// open robots.txt file anr write new content
					file_put_contents ( '../robots.txt', $str );
				}
				break;
			case 'analitics':
				$title = trim($_POST['title']);
				$code = $_POST['code'];
				$place = intval($_POST['place']);
				if ( $title && $code )
				{
					$aFileName = rand(1111, 9999).".html";
					file_put_contents("../media/analytics/".$aFileName, $code);
					
					$properties["title"] = $title;
					$properties["url"] = $aFileName;
					$properties["place"] = $place;
					
					$analitics = new Analitics();
					$analitics->Insert($properties);
				}
				break;
			case 'analitics_edit':
				$item = intval($_POST['item']);
				$analitics = new Analitics($item);
				if ( isset($_POST["delete"]) )
				{
					unlink("../media/analytics/".$analitics->GetUrl());
					$analitics->Delete();
				}
				else 
				{
					$title = trim($_POST['title']);
					$code = $_POST['code'];
					$place = intval($_POST['place']);
					if ( $title && $code )
					{
						$analitics->SetTitle($title);
						$analitics->SetPlace($place);
						file_put_contents("../media/analytics/".$analitics->GetUrl(), $code);
					}
				}
				break;
			case 'cmsLang' :
				if (($cmslanguage = DB::POST ( 'cmslanguage' )))
				{
					$user = CMS::GetUser ();
					$user->SetCMSLang ( $cmslanguage );
				}
				break;
			case 'uploadRestoreDB' :
				//	get file content
				//	$sql = file_get_contents ( $_FILES ['gl2_img'] ['tmp_name'] );
				break;
			case 'addNewLanguage' :
				if ($this->m_userStatus == 1)
				{
					$newLang = DB::POST ( 'newLanguage' );
					$newLangName = DB::POST ( 'newLanguageName' );
					if ($newLang && $newLangName && strlen ( trim ( $newLang ) ) == 2 && strlen ( trim ( $newLangName ) ) > 0)
					{
						$newLang = trim ( $newLang );
						$newLangName = trim ( $newLangName );
					}
					else
					{
						break;
					}
					
					$langs = $this->m_site->GetLanguages ();
					
					if (array_key_exists ( $newLang, $langs ))
					{
						break;
					}
					// updateing database
					
					// get all tables 
					$dbName = Site::GetDBName ();
					$tables = DB::GetDBTables ( $dbName );
					
					foreach ( $tables as $tableName )
					{
						// show fields
						$resultF = DB::QueryToArray ( "SHOW COLUMNS FROM $tableName" );
						
						if (! $resultF)
						{
							echo 'Could not run query: ' . DB::GetError ();
							exit ();
						}
						if (count ( $resultF ) > 0)
						{
							$currentTableFields = array ();
							
							foreach ( $resultF as $row )
							{
								$fieldName = $row ['Field'];
								foreach ( $langs as $lang )
								{
									if (substr ( $fieldName, strlen ( $fieldName ) - 3 ) == "_$lang")
									{
										$subName = substr ( $fieldName, 0, strlen ( $fieldName ) - 3 );
										
										$currentTableFields [$subName] = $row;
										$currentTableFields [$subName] ['Field'] = "{$subName}_{$newLang}";
									}
								}
							}
							
							if (count ( $currentTableFields ) > 0)
							{
								$updateResult = true;
								foreach ( $currentTableFields as $newField )
								{
									$null = ($newField ['Null'] == 'YES') ? 'NULL' : 'NOT NULL';
									$default = ($newField ['Default']) ? "DEFAULT '{$newField['Default']}" : '';
									
									$sql = "ALTER TABLE `$tableName` ADD `{$newField['Field']}` {$newField['Type']} $null $default";
									
									$updateResult &= DB::Query ( $sql, true );
								}
							}
						}
					}
					//	updateing configuration XML
					
					// opten XML
					$cinfFilePath = Site::GetConfigFilePath ();
					
					//change file permition
					chmod ( $cinfFilePath, 0640 );
					
					$config = simplexml_load_file ( $cinfFilePath );
					
					// Navigate to languages
					$languages = $config->languages;
					
					// add new new language element
					$newLangElement = $languages->addChild ( 'language' );
					$newLangElement->addChild ( 'linkID', "$newLang" );
					$newLangElement->addChild ( 'linkName', "$newLangName" );
					
					// save the file
					$config->asXML ( $cinfFilePath );
					
					// call Site functions to update new information
					Site::LoadStatics ();
				}
				break;
			case 'updateLangXML' :
				if (($fileName = DB::POST ( 'updateXML' )))
				{
					if (! file_exists ( $fileName ))
					{
						die ( "The '$fileName' file does not exist!" );
					}
					
					chmod ( $fileName, 0753 );
					
					$xmlWrite = simplexml_load_file ( $fileName );
					
					foreach ( $xmlWrite as $child )
					{
						$ID = ( string ) $child ['id'];
						
						if (($value = DB::POST ( 'editxml_' . $ID )))
						{
							$child [0] = $value;
						}
					}
					
					$xmlWrite->asXML ( $fileName );
				}
				break;
			case 'cacheseconds' :
				$seconds = intval ( DB::POST ( 'seconds' ) );
				
				if (is_int ( $seconds ))
				{
					$fileName = 'config/cache.xml';
					
					// xml load
					$xml = simplexml_load_file ( $fileName );
					$xml->seconds = $seconds;
					
					// xml write
					$xml->asXML ( $fileName );
				}
				break;
			case 'deleteOldCached' :
				$cache = new ContentCaching ( '../' );
				$cache->DeleteOldFiles (false);
				break;
			case 'changeFTP':
				
				$properties = array();
				
				$properties['ftpuser'] = DB::POST('filed1');
				$properties['ftppass'] = DB::POST('filed2');
				$properties['ftpurl'] = DB::POST('filed3');
				$properties['ftproot'] = DB::POST('ftproot');
				
				// save properties to Value table
				$valueObj = new Value(1);
				$valueObj->SetValue($properties);
				break;
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
		// user box
		if (DB::GET_POST ( 'userID' ) || DB::GET_POST ( 'usertype' ))
		{
			$this->m_subPanel = '';
		}
		
		// read names
		$userTitleS = $this->GetNameString ( 'userTitle' );
		$userStatsS = $this->GetNameString ( 'userStats' );
		$siteOptionsS = $this->GetNameString ( 'siteOptions' );
		$seoToolsS = $this->GetNameString ( 'seoTools' );
		$backupS = $this->GetNameString ( 'backup' );
		
		// user accounts
		print ( "<div class='someWTitleBox'>$userTitleS</div>" );
		print ( '<div class="someWBox">' );
		$userCMS = new UserManagementCMS ( );
		$userCMS->DisplayMenu ();
		print ( '</div>' );
		print ( '<br />' );
		
		// website options
		print ( '<br />' );
		print ( "<div class='someWTitleBox'>$siteOptionsS</div>" );
		print ( '<div class="someWBox">' );
		
		// web site options
		$websiteNameS = $this->GetNameString ( 'websiteName' );
		$this->printMenuItem ( $websiteNameS, 'websitename' );
		
		// mediadata
		$metadataS = $this->GetNameString ( 'metadata' );
		$this->printMenuItem ( $metadataS, 'metadata' );
		
		// Status
		$statusS = $this->GetNameString ( 'status' );
		$this->printMenuItem ( $statusS, 'status' );
		
		if ($this->m_userStatus == 1)
		{
			$addLanguageS = $this->GetNameString ( 'addLanguage' );
			$this->printMenuItem ( $addLanguageS, 'addLanguage' );
		}
		
		$addLanguageS = 'Update Languages'; //$this->GetNameString ( 'addLanguage' );
		$this->printMenuItem ( $addLanguageS, 'updateLangXML' );
		
		$cacheS = 'Cache options'; //$this->GetNameString ( 'addLanguage' );
		$this->printMenuItem ( $cacheS, 'updateCache' );
		print ( '</div>' );
		
		
		// website options
		$ftpSettingsS = $this->GetNameString ( 'ftpSettings' );
		print ( '<br />' );
		print ( "<div class='someWTitleBox'>$ftpSettingsS</div>" );
		print ( '<div class="someWBox">' );
		// FTP
		$ftpS = $this->GetNameString ( 'ftp' );
		$this->printMenuItem ( $ftpS, 'ftp' );
		print('</div>');
		
		// website options
		print ( '<br />' );
		print ( "<div class='someWTitleBox'>$backupS</div>" );
		print ( '<div class="someWBox">' );
		
		// backup options
		$backupDBS = $this->GetNameString ( 'backupDB' );
		$this->printMenuItem ( $backupDBS, 'backupDB' );
		
//		$restoreDBS = $this->GetNameString('restoreDB');
//		$this->printMenuItem ( $restoreDBS, 'restoreDB' );
		
		print ( '</div>' );
		print ( '<br />' );
		
		print ( "<div class='someWTitleBox'>$seoToolsS</div>" );
		print ( '<div class="someWBox">' );
		
		//sitemap.xml
		$this->printMenuItem ( 'sitemap.xml', 'sitemapxml' );
		
		// robots.txt
		$this->printMenuItem ( 'robots.txt', 'robotstxt' );
		
		// analitics
		$this->printMenuItem ( 'Analitics', 'analytics' );
		print ( '</div>' );
		
		print ( '<br />
	         <div class="someWTitleBox">Siteius CMS</div>
	         <div class="someWBox">' );
		
		$cmsOptionsS = $this->GetNameString ( 'cmsOptions' );
		$this->printMenuItem ( $cmsOptionsS, 'cmsoptions' );
		
		$helpS = $this->GetNameString ( 'help' );
		$aboutS = $this->GetNameString ( 'about' );
		
		print ( '<div class="directoryMenu0">
	            	<a href="help/index.html" target="aboutwecms" onclick="onwmc(this.href,\'aboutwecms\',\'640\',\'480\',\'yes\');return false">' . $helpS . '</a>
	            </div>
	            <div class="directoryMenu0">
	            	<a href="help/aboutWECMS.html" target="aboutwecms" onclick="onwmc(this.href,\'aboutwecms\',\'280\',\'320\',\'no\');return false">' . $aboutS . '</a>
	            </div>
	         </div>
	    <!--end navigation-->' );
	}
	
	/**
	 * This function displays left box links.
	 * 
	 * @param string $name
	 * @param string $link
	 */
	private function printMenuItem($name, $link)
	{
		print ( '<div class="directoryMenu0">' );
		if ($link == $this->m_subPanel)
		{
			print ( $name );
		}
		else
		{
			print ( "<a href='?subpanel=$link'>$name</a>" );
		}
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// user features
		if ( isset($_GET['userID']) || isset($_GET['usertype']) )
		{
			// mediadata
			$userCMS = new UserManagementCMS ( );
			$userCMS->DisplayFeatures ();
			
			return;
		}
		
		$doneS = $this->GetNameString ( 'done' );
		
		switch ($this->m_subPanel)
		{
			case 'websitename' :
				$websiteNameS = $this->GetNameString ( 'websiteName' );
				$defWebsiteNameS = $this->GetNameString ( 'defWebsiteName' );
				$this->DisplayTitle ( $websiteNameS );
				
				$content = new Category ( 1 );
				$name = $content->GetName ( $this->m_displayLang );
				
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'websitename' );
				print ( '<div class="metadataFormLabel"><label><strong>' . $defWebsiteNameS . '</strong></label></div>' );
				print ( '<div style="float:left"><input name="title" value="' . $name . '" type="text" size="56" maxlength="256" style="font-size:14px" /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '<div class="metadataFormLabel"><label><strong>&nbsp;</strong></label></div>' );
				print ( '<div style="float:left"><input name="" type="submit" value="' . $doneS . '" /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '</form>' );
				print ( '</div>' );
				break;
			case 'metadata' :
				// display title
				$metadataS = $this->GetNameString ( 'metadata' );
				$defTitleS = $this->GetNameString ( 'defTitle' );
				$defDescS = $this->GetNameString ( 'defDesc' );
				$defKeysS = $this->GetNameString ( 'defKeys' );
				
				$this->DisplayTitle ( $metadataS );
				
				// get values from database
				$rootCat = new Category ( 1 );
				
				$title = $rootCat->GetTitle ( $this->m_displayLang );
				$keywords = $rootCat->GetKeywords ( $this->m_displayLang );
				$description = $rootCat->GetDescription ( $this->m_displayLang );
				$picture = $rootCat->GetPicture();
				
				// display form
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'metadata', '', 'post', ' enctype="multipart/form-data"' );
				print ( '<div class="metadataFormLabel"><label><strong>' . $defTitleS . '</strong></label></div>' );
				print ( '<div style="float:left"><input name="title" value="' . $title . '" type="text" size="56" maxlength="256" style="font-size:14px" /></div>' );
				print ( '<br class="clearFloat" />' );
				
				print ( '<div class="metadataFormLabel"><label><strong>' . $defDescS . '</strong></label></div>' );
				print ( '<div style="float:left"><textarea name="description" cols="40" rows="5">' . $description . '</textarea></div>' );
				print ( '<br class="clearFloat" /><br />' );
				
				print ( '<div class="metadataFormLabel"><label><strong>' . $defKeysS . '</strong></label></div>' );
				print ( '<div style="float:left"><textarea name="keywords" cols="40" rows="5">' . $keywords . '</textarea></div>' );
				print ( '<br class="clearFloat" /><br />' );
				
				print ( '<div class="metadataFormLabel"><label><strong>Picture</strong></label></div>' );
				print ( '<div style="float:left"><input name="picture" type="file" /></div>' );
				print ( '<br class="clearFloat" />' );
				
				if ( $picture && file("../".$picture) )
				{
					print ( '<div class="metadataFormLabel">&nbsp;</div>' );
					print ( '<div style="float:left"><img src="../'.$picture.'" width="200" /><br /><input type="checkbox" name="delete"> delete</div>' );
					print ( '<br class="clearFloat" />' );
					print ( '<br class="clearFloat" />' );
				}
				
				print ( '<div class="metadataFormLabel"><label><strong>&nbsp;</strong></label></div>' );
				print ( '<div style="float:left"><input name="" type="submit" value="' . $doneS . '" /></div>' );
				print ( '<br class="clearFloat" />' );
				
				print ( '</form>' );
				print ( '</div>' );
				
				break;
			case 'status' :
				// display title
				
				$statusS = $this->GetNameString ( 'status' );
				$onlineS = $this->GetNameString ( 'online' );
				$offlineS = $this->GetNameString ( 'offline' );
				$offlineMessageS = $this->GetNameString ( 'offlineMessage' );
				
				$this->DisplayTitle ( $statusS );
				
				// read values vrom database
				$content = new Content ( 1 );
				
				$title = $content->GetName ( $this->m_displayLang );
				$contentType = $content->GetPurpose ();
				$status = $content->GetTitleShow ();
				$online = ($status) ? 'checked' : '';
				$offline = ($status) ? '' : 'checked';
				
				// display form
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'status' );
				print ( '<div class="userFormLabel"><label><strong>' . $onlineS . '</strong></label></div>' );
				print ( '<div style="float:left"><input name="status" type="radio" value="1" ' . $online . ' /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '<div class="userFormLabel"><label><strong>' . $offlineS . '</strong></label></div>' );
				print ( '<div style="float:left"><input name="status" type="radio" value="0" ' . $offline . '/></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '<div class="userFormLabel"><label><strong>' . $offlineMessageS . '</strong></label></div>' );
				print ( "<div style='float:left'><a href='?structure=content&contentTypeID=$contentType&contID=1'> $title</a></div>" );
				print ( '<br class="clearFloat" /><br />' );
				print ( '<div class="userFormLabel"><label><strong>&nbsp;</strong></label></div>' );
				print ( '<div style="float:left"><input name="" type="submit" value="' . $doneS . '" /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '</form>' );
				print ( '</div>' );
				
				break;
			case 'sitemapxml' :
				// open sitemap.xml file and display form to edit
				$sitemapS = $this->GetNameString ( 'sitemap' );
				$this->DisplayTitle ( $sitemapS );
				
				// open file or generate new xml
				
				$content = "";
				// generation
				if ($this->GetFormAction () == 'sitemapGeneration')
				{
					Site::IncludeFile ( 'classes/Menu/Menu.class.inc' );
					Site::IncludeFile ( 'classes/Menu/MenuList.class.inc' );
					
					function createSiteMapXML($structure, $addToLink, $date, $freq = 'daily', $depth = 1)
					{
						$content = '';
						
						if (count ( $structure ) > 0)
						{
							foreach ( $structure as $item )
							{
								$priority = 1.0;
								
								if ($item ['ID'] != 2)
								{
									$priority = pow ( 0.8, $depth );
								}
								
								if ($item ['ID'] != 2)
								{
									$priorityS = sprintf ( "%.4s", $priority );
									// get item and make xml
									$content .= "<url>\n";
									$content .= "\t<loc>{$addToLink}{$item['path']}</loc>\n";
									$content .= "\t<priority>$priorityS</priority>\n";
									$content .= "\t<lastmod>$date</lastmod>\n";
									$content .= "\t<changefreq>$freq</changefreq>\n";
									$content .= "</url>\n";
								}
								
								// if there is a sub item calculate for it
								if (isset ( $item ['ref'] ))
								{
									$content .= createSiteMapXML ( $item['ref'], $addToLink, $date, $freq, $depth + 1 );
								}
								
								//	get related products for current menu
								$query = "select `ID` from `options` where `category_id` = ".$item['ID'];
								$optionID = DB::QueryToValue($query);
								if ( $optionID )
								{
									$query = "
										SELECT PM.`random` 
										FROM `pr_main` PM 
										JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
										JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID` and OD.`optionsID` = $optionID) 
										WHERE PM.`status` = 1 
										GROUP BY PM.`ID`";
								}
								else 
								{
									$query = "
										SELECT PM.`random`  
										FROM `pr_main` PM 
										JOIN `pr_options` PO ON (PM.`ID` = PO.`productID` AND PO.`relatedID` = ".$item['ID'].") 
										JOIN `pr_field` PF ON (PO.`fieldID` = PF.`ID`) 
										WHERE (PF.`type` = 2 || PF.`type` = 8) and PM.`status` = 1";
								}
								
								$retArr = DB::QueryToArray($query);
								
								if ( count($retArr)>1 )
								{
									$priority = pow ( 0.6, $depth );
									for ( $i=0; $i<count($retArr); $i++ )
									{
										$priorityS = sprintf ( "%.4s", $priority );
										// get item and make xml
										$content .= "<url>\n";
										$content .= "\t<loc>{$addToLink}{$retArr[$i]["random"]}/</loc>\n";
										$content .= "\t<priority>$priorityS</priority>\n";
										$content .= "\t<lastmod>$date</lastmod>\n";
										$content .= "\t<changefreq>$freq</changefreq>\n";
										$content .= "</url>\n";
									}
								}
							}
						}
						
						return $content;
					}
					
					// get all menus
					$menuList = new MenuList ( );
					$menus = $menuList->GetStructure ();
					
					// make link for sitemap
					$siteURL = $this->m_site->GetSiteURL ();
					$defLanguage = $this->m_site->GetDefaultLanguage ();
					$langs = $this->m_site->GetLanguages ();
					
					// loop for all menus
					$freq = DB::POST ( 'freq' );
					$date = date ( "c" );
					
					foreach ( $langs as $lang )
					{
						$addToLink = $siteURL . $lang . '/';
						
						$temp = '';
						foreach ( $menus as $menu )
						{
							if ($menu ['ID'] > 1)
							{
								// get menu structure
								$currMenu = new Menu ( $menu ['ID'], true, $lang );
								$structure = $currMenu->GetStructure ();
								$temp .= createSiteMapXML ( $structure, $addToLink, $date, $freq );
							}
						}
						
						// home item
						// home url
						$homeURL = (strcmp ( $defLanguage, $lang )) ? $siteURL . $lang . '/' : $siteURL;
						$home = "<url>\n";
						$home .= "\t<loc>$homeURL</loc>\n";
						$home .= "\t<priority>1</priority>\n";
						$home .= "\t<lastmod>$date</lastmod>\n";
						$home .= "\t<changefreq>$freq</changefreq>\n";
						$home .= "</url>\n";
						
						$content .= $home . $temp;
					}
					
					$front = '<?xml version="1.0" encoding="UTF-8"?>';
					$front .= "\n" . '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">' . "\n";
					
					$content = $front . $content . '</urlset>';
				}
				//open file
				else
				{
					$filename = '../sitemap.xml';
					if (is_file ( $filename ))
					{
						$content = file_get_contents ( $filename );
					}
				}
				
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'sitemapxml' );
				print ( "<textarea name='content' cols='60' rows='15'>$content</textarea>" );
				print ( '<br />' );
				print ( '<br />' );
				print ( '<input type="submit" value="' . $doneS . '" />' );
				print ( '</form>' );
				print ( '</div>' );
				
				// form for XML generation
				$generateS = $this->GetNameString ( 'generate' );
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'sitemapGeneration' );
				print ( '<select name="freq">' );
				print ( '<option value="daily">daily</option>' );
				print ( '<option value="weekly">weekly</option>' );
				print ( '<option value="monthly">monthly</option>' );
				print ( '</select>' );
				print ( '<input type="submit" value="' . $generateS . '" />' );
				print ( '</form>' );
				print ( '</div>' );
				
				break;
			case 'robotstxt' :
				// open robots.txt file and display form to edit
				$robotsS = $this->GetNameString ( 'robots' );
				$generateS = $this->GetNameString ( 'generate' );
				
				$this->DisplayTitle ( $robotsS );
				
				$content = "";
				// generatie or read from file
				if ($this->GetFormAction () == 'robotstxtGeneration')
				{
					// get site url
					$siteURL = $this->m_site->GetSiteURL ();
					
					// remove last slash and http://
					$siteURLSlesh = substr ( $siteURL, 0, strlen ( $siteURL ) - 1 );
					$pos = strpos ( $siteURLSlesh, 'http://' );
					if ($pos !== false)
					{
						$siteURLSlesh = substr ( $siteURLSlesh, $pos + 7 );
					}
					
					// pattern
					$content = "User-agent: *\n";
					$content .= "Disallow: /cms\n";
					$content .= "Host: $siteURLSlesh\n";
					$content .= "Sitemap: {$siteURL}sitemap.xml\n";
				}
				else
				{
					$filename = '../robots.txt';
					if (is_file ( $filename ))
					{
						$content = file_get_contents ( $filename );
					}
				}
				
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'robotstxt' );
				print ( "<textarea name='content' cols='60' rows='15'>$content</textarea>" );
				print ( '<br />' );
				print ( '<br />' );
				print ( '<input type="submit" value="' . $doneS . '" />' );
				print ( '</form>' );
				print ( '</div>' );
				
				// generation
				$this->DisplayFormHeadr ( 'robotstxtGeneration' );
				$this->DisplayFormFooter ( $generateS );
				
				break;
			case 'analytics':
				$this->DisplayTitle ( "Analytics" );
				
				// display form
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'analitics' );
				print ( '<div class="metadataFormLabel"><label><strong>Title</strong></label></div>' );
				print ( '<div style="float:left"><input name="title" value="" type="text" size="56" maxlength="256" style="font-size:14px" /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '<div class="metadataFormLabel"><label><strong>Code</strong></label></div>' );
				print ( '<div style="float:left;padding-bottom:10px;"><textarea name="code" cols="100" rows="15"></textarea></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '<div class="metadataFormLabel"><label><strong>Place</strong></label></div>' );
				?>
				<div style="float:left">
					<select name="place">
						<option value="1">Head</option>
						<option value="2">Body</option>
					</select>
				</div>
				<?php
				print ( '<br class="clearFloat" /><br />' );
				print ( '<div class="metadataFormLabel"><label><strong>&nbsp;</strong></label></div>' );
				print ( '<div style="float:left"><input name="" type="submit" value="Add" /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '</form>' );
				print ( '</div>' );
				
				$analiticsList = new AnaliticsList();
				$analiticsListArr = $analiticsList->GetStructure();
				if ( is_array($analiticsListArr) )
				{
					$this->DisplayTitle ( "Already exist" );
					for ( $i=0; $i<count($analiticsListArr); $i++ )
					{
						if ( ($i+1)==count($analiticsListArr) )
						{
							print ( '<div class="someContent">' );
						}
						else 
						{
							print ( '<div class="someContent" style="border-bottom:1px solid #999;">' );
						}
						$this->DisplayFormHeadr ( 'analitics_edit' );
						$this->DisplayHidden("item", $analiticsListArr[$i]["ID"]);
						print ( '<div class="metadataFormLabel"><label><strong>Title</strong></label></div>' );
						print ( '<div style="float:left"><input name="title" value="'.$analiticsListArr[$i]["title"].'" type="text" size="56" maxlength="256" style="font-size:14px" /></div>' );
						print ( '<br class="clearFloat" />' );
						print ( '<div class="metadataFormLabel"><label><strong>Code</strong></label></div>' );
						print ( '<div style="float:left"><textarea name="code" cols="100" rows="15">'.file_get_contents("../media/analytics/".$analiticsListArr[$i]["url"]).'</textarea></div>' );
						print ( '<br class="clearFloat" />' );
						print ( '<div class="metadataFormLabel"><label><strong>Place</strong></label></div>' );
						?>
						<div style="float:left">
							<select name="place">
								<option value="1"<?php echo ($analiticsListArr[$i]["place"]==1 ? " selected": "");?>>Head</option>
								<option value="2"<?php echo ($analiticsListArr[$i]["place"]==2 ? " selected": "");?>>Body</option>
							</select>
						</div>
						<?php
						print ( '<br class="clearFloat" /><br />' );
						print ( '<div class="metadataFormLabel"><label><strong>&nbsp;</strong></label></div>' );
						print ( '<div style="float:left"><input name="edit" type="submit" value="Edit" /> <input name="delete" type="submit" value="Delete" /></div>' );
						print ( '<br class="clearFloat" />' );
						print ( '</form>' );
						print ( '</div>' );
					}
				}
				break;
			case 'cmsoptions' :
				// cms options
				$cmsOptionsS = $this->GetNameString ( 'cmsOptions' );
				$this->DisplayTitle ( $cmsOptionsS );
				
				$cmsLang = CMS::GetCMSLang ();
				$cmsLangs = CMS::GetCMSLangs ();
				
				$cmsLanguage = $this->GetNameString ( 'cmsLanguage' );
				
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'cmsLang' );
				
				print ( '<div class="metadataFormLabel"><label><strong>' . $cmsLanguage . '</strong></label></div>' );
				print ( '<div style="float:left">' );
				
				print ( '<select name="cmslanguage">' );
				foreach ( $cmsLangs as $key => $value )
				{
					if ($cmsLang == $key)
					{
						print ( "<option value='$key' selected='selected'>$value</option>" );
					}
					else
					{
						print ( "<option value='$key'>$value</option>" );
					}
				}
				print ( '</select></div><br class="clearFloat" />' );
				print ( '<div class="metadataFormLabel">
				<label><strong>&nbsp;</strong></label></div>' );
				
				print ( '<div style="float:left">' );
				$this->DisplayFormFooter ( $doneS );
				print ( '</div>' );
				break;
			case 'backupDB' :
				$backupDBS = $this->GetNameString ( 'backupDB' );
				$this->DisplayTitle ( $backupDBS );
				
				$siteURL = $this->m_site->GetSiteURL ();
				?>
<form method='post' name='' class='clearPD' action=''
	onsubmit='window.open("<?php echo 
				$siteURL?>cms/files/backupDB.php", "Bakup"); return false;'><input
	type='hidden' name='subscribecmsAction' value='start'> <input
	type='hidden' name='SubscribeListID' value='1' /> <input type='submit'
	value='Start'></form>
<?php
				break;
			case 'restoreDB' :
				$restoreDBS = $this->GetNameString ( 'restoreDB' );
				$this->DisplayTitle ( $restoreDBS );
				$this->DisplayDBUploadForm ();
				break;
			case 'addLanguage' :
				if ($this->m_userStatus == 1)
				{
					$this->DisplayTitle ( 'Adding new language' );
					
					$langs = $this->m_site->GetLanguagesName ();
					
					print ( 'Current languages:<br /><br />' );
					foreach ( $langs as $key => $langelement )
					{
						print ( "$key: $langelement<br />" );
					}
					print ( '<div class="someContent">' );
					$this->DisplayFormHeadr ( 'addNewLanguage' );
					print ( '<br />' );
					print ( "2 chars for language ID<br />" );
					print ( "<input type='text' name='newLanguage' />" );
					print ( '<br />' );
					print ( "language name<br />" );
					print ( "<input type='text' name='newLanguageName' />" );
					print ( '<br />' );
					print ( '<br />' );
					print ( '<input type="submit" value="' . $doneS . '" />' );
					print ( '</form>' );
					print ( '</div>' );
				}
				
				break;
			case 'updateLangXML' :
				$this->DisplayTitle ( 'Edit file' );
				
				// get current file name
				$fileName = DB::POST ('updateXML');
				
				// get all files
				$files = scandir ( "../langs/{$this->m_displayLang}/" );
				
				$firstOptions = "<option value='0'> - - - </option>";
				
				foreach ( $files as $file )
				{
					$path_parts = pathinfo ( "../langs/{$this->m_displayLang}/$file" );
					$ext = $path_parts ['extension'];
					$ext = strtolower ( $ext );
					
					if ($ext == 'xml')
					{
						if ($file != '.' && $file != '..')
						{
							if ("../langs/{$this->m_displayLang}/$file" == $fileName)
							{
								$firstOptions .= "<option value='../langs/{$this->m_displayLang}/$file' selected>$file</option>\n";
							}
							else
							{
								$firstOptions .= "<option value='../langs/{$this->m_displayLang}/$file'>$file</option>\n";
							}
						}
					}
				}
				
				// display files form
				print ( '<form method="post">' );
				print ( "<select name='updateXML'>$firstOptions</select>" );
				print ( "<input type='submit' value='$doneS'/>" );
				print ( '</form>' );
				
				// display select
				if ($fileName)
				{
					if (! file_exists ( $fileName ))
					{
						die ( "The '$fileName' file does not exist!" );
					}
					$xml = simplexml_load_file ( $fileName );
					// display xml
					$this->DisplayFormHeadr ( 'updateLangXML' );
					$this->DisplayHidden ( 'updateXML', $fileName );
					foreach ( $xml->name as $nameItem )
					{
						$ID = ( string ) $nameItem ['id'];
						$name = ( string ) $nameItem;
						$lnght = strlen ( $name );
						print ( "$ID<br />" );
						
						$IDName = "editxml_{$ID}";
						if ($lnght > 64)
						{
							$rows = ($lnght / 64 > 1) ? floor ( $lnght / 64 ) + 1 : 1;
							print ( "<textarea id='$IDName' name='$IDName' cols='60' rows='$rows'>$name</textarea>" );
						}
						else
						{
							print ( "<input type='text' id='$IDName' name='$IDName' size='100' value='$name'/>" );
						}
						print ( "<br />" );
					}
					$this->DisplayFormFooter ( $doneS );
				}
				break;
			case 'updateCache' :
				$restoreDBS = 'Caching options'; //$this->GetNameString ( 'restoreDB' );
				$this->DisplayTitle ( $restoreDBS );
				
				$cacheSecondsS = 'cache seconds';
				// read seconds
				$xml = simplexml_load_file ( 'config/cache.xml' );
				$seconds = ( int ) $xml->seconds;
				
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'cacheseconds' );
				print ( '<div class="metadataFormLabel"><label><strong>' . $cacheSecondsS . '</strong></label></div>' );
				print ( '<div style="float:left"><input name="seconds" value="' . $seconds . '" type="text" size="10" maxlength="10" style="font-size:14px" /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '<div style="float:left"><input type="submit" value="' . $doneS . '" /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '</form>' );
				print ( '</div>' );
				
				print ( '<div class="someContent">' );
				$this->DisplayFormHeadr ( 'deleteOldCached' );
				print ( '<div class="metadataFormLabel"><label><strong>' . $cacheSecondsS . '</strong></label></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '<div style="float:left"><input type="submit" value="' . 'Delete old files' . '" /></div>' );
				print ( '<br class="clearFloat" />' );
				print ( '</form>' );
				print ( '</div>' );

				break;
			case 'ftp':
				$this->DisplayFtpFeatures();
				break;
		}
		
		print ( "&nbsp;" );
	}
	
	/**
	 * Create user module tables
	 *
	 */
	function CreateDB()
	{
		$userLiet = new UserTypeListCMS ( );
		$userLiet->CreateDB ();
		
		$userCMS = new UserManagementCMS ( );
		$userCMS->CreateDB ();
		
		$valueCMS = new ValueCMS();
		$valueCMS->CreateDB();
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayDBUploadForm()
	{
		$maxFileSize = ini_get ( 'upload_max_filesize' );
		$typeStr = 'txt';
		
		$actionName = $this->GetFormActionName ();
		$uploadS = $this->GetNameString ( 'restoreDB' );
		$fileUploadS = $this->GetNameString ( 'fileUpload' );
		?>
<form action="" method="POST" class="clearPD"
	enctype='multipart/form-data'><input type="hidden"
	name="<?php echo $actionName?>" value="uploadRestoreDB">
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL"><label><strong><?php echo $fileUploadS?></strong> </label>
		<input type="file" name="gl2_img" size="48" /> <span>&nbsp;[ Max = <?php echo $maxFileSize?> ] <?php echo $typeStr?></span>
		</div>
		<div class="titleColR"><input name="" type="submit"
			value="<?php echo $uploadS?>" /></div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
</form>
<?php
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		// chack database
		$userLiet = new UserTypeListCMS ( );
		$res = $userLiet->SelfTest ();
		
		$userCMS = new UserManagementCMS ( );
		$res &= $userCMS->SelfTest ();
		
		$valueCMS = new ValueCMS ( );
		$res &= $valueCMS->SelfTest ();
		
		// chack files

		// cache xml
		if (! file_exists ( 'config/cache.xml' ))
		{
			$res = false;
			print ( 'The config file <b>cache.xml</b> does not exist<br />' );
		}
	
		$catchCnt = new ContentCaching('../');
		$cacheFolder = $catchCnt->GetCacheFolder();
		
		// if dir does not exit and it is allowed to make it then try to do that.
		if(!file_exists($cacheFolder) && $this->CanBeCorrected())
		{
			FolderXP::Mkdir($cacheFolder, -1);	
		}
		
		// check is dir exit
		if(!file_exists($cacheFolder))
		{
			$res = false;
			print("The folder <b>$cacheFolder</b> does not exist!\n<br />");
		}
		
		return $res;
	}
	
	function GetObjectTables()
	{
		// chack database
		$userLiet = new UserTypeListCMS ( );
		$resType = $userLiet->GetObjectTables ();
		
		$userCMS = new UserManagementCMS ( );
		$resUser = $userCMS->GetObjectTables ();
		
		return array_merge($resType, $resUser);
	}
	
	/*
	 * This function displays FTP settings form
	 */
	function DisplayFtpFeatures()
	{
		$ftpS = $this->GetNameString('ftp');
		$this->DisplayTitle($ftpS);
		
		$doneS = $this->GetNameString('done');
		
		// get values for ftp
		$valueObj = new Value(1);
		$ftpSettings = $valueObj->GetValue(true);

		$userName = (isset($ftpSettings['ftpuser'])) ? $ftpSettings['ftpuser'] : '';
		$ftpURL = (isset($ftpSettings['ftpurl'])) ? $ftpSettings['ftpurl'] : '';
		$ftp_user_pass = (isset($ftpSettings['ftppass'])) ? $ftpSettings['ftppass'] : '';
		$userDir = (isset($ftpSettings['ftproot'])) ? $ftpSettings['ftproot'] : '';

		// display form
		
		print('<script type=\'text/javascript\'>
					function Validation()
					{
						field1 = TextFildValid(\'filed1\', null, \'filed1HTML\');
						field2 = TextFildValid(\'filed2\', null, \'filed2HTML\');
						field3 = TextFildValid(\'filed3\', null, \'filed3HTML\');
						return (field1 && field2 && field3 );
					}
				</script>');

		print('<div style="padding: 10px">');
		$this->DisplayFormHeadr('changeFTP', '', 'post', ' onsubmit=\'return Validation();\'');
		print<<<FTPUSERORM
				<div style="padding: 3px">
						<input type="text" id="filed1" name="filed1" value="$userName" />
						<span id="filed1HTML"> username</span>
				</div>
				<div style="padding: 3px">
						<input type="password" id="filed2" name="filed2" value="" /> 
						<span id="filed2HTML"> password</span>
				</div>
				<div style="padding: 3px">
						<input type="text" id="filed3" name="filed3" value="$ftpURL" /> 
						<span id="filed3HTML"> ftp URL</span>
				</div>
				<div style="padding: 3px"><input type="text" name="ftproot" value="$userDir" /> root dir</div>
FTPUSERORM;
		$this->DisplayFormFooter($doneS);
		print('</div>');
		
		// test connection
		// set up basic connection
		$conn_id = ftp_connect($ftpURL); 
	
		// login with username and password
		$login_result = ftp_login($conn_id, $userName, $ftp_user_pass); 
		
		$prRes = ($login_result) ? 'OK' : 'Error!';
		print("FTP test result: $prRes");
		ftp_close($conn_id);
	}
}
?>