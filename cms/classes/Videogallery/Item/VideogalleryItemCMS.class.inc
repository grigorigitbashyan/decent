<?php
//TODO corrent the include
Site::IncludeFile ( 'classes/Videogallery/Item/VideogalleryItem.class.inc' );

/**
 * Enter class description
 */
class VideogalleryItemCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var VideogalleryItem
	 */
	protected $m_videogallery_item;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_videogallery_item = new VideogalleryItem ( $ID );
		
		$this->ReadNameStrings ( 'classes/Videogallery/langs/', 'videogallery_item.xml' );
	}
	/**
	 * Return sql statement to create table 
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$langs = $this->m_site->GetLanguages ();
		
		$strLang = '';
		
		foreach ( $langs as $lang )
		{
			$strLang .= "`name_{$lang}` varchar(255) DEFAULT NULL,
			  `desc_{$lang}` text,
			  `link_{$lang}` varchar(255) DEFAULT NULL,";
		}
		
		$str = "CREATE TABLE `videogallery_item` (
			  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
			  `videogalleryID` int(11) NOT NULL,
			  $strLang
			  `path` varchar(255) NOT NULL,
			  `position` smallint(6) NOT NULL DEFAULT '0',
			  `isuploaded` tinyint(1) NOT NULL DEFAULT '1',
			  PRIMARY KEY (`ID`)
			) ENGINE=InnoDB  DEFAULT CHARSET=utf8;";
			
		return $str;
	}
	
	/**
	 * All actions must be done hear
	 *
	 * @return bool
	 */
	function DoAction()
	{
		$ar = null;
		
		switch ($this->GetFormAction ())
		{
			case 'update' :
				$value = DB::POST ( 'Name' );
				$ar = $this->m_videogallery_item->SetName ( $value, $this->m_displayLang );

				$value = DB::POST ( 'Desc' );
				$ar |= $this->m_videogallery_item->SetDesc ( $value, $this->m_displayLang );
				
				$value = DB::POST( 'viewCount' );
				$ar |= $this->m_videogallery_item->SetViewCount( $value );
				
				$value = DB::POST( 'videoCode' );
				$ar |= $this->m_videogallery_item->SetVideoCode( $value );

				$value = DB::POST ( 'Link' );
				
				if(DB::POST('toalllink'))
				{
					$langs = $this->m_site->GetLanguages();
					foreach ($langs as $lang)
					{
						$ar |= $this->m_videogallery_item->SetLink ( $value, $lang );
					}
				}
				else 
				{
					$ar |= $this->m_videogallery_item->SetLink ( $value, $this->m_displayLang );
				}
				
				$value = DB::POST( 'hide' );
				$value = ($value == "on" ? 1 : 0);
				$ar |= $this->m_videogallery_item->SetHide( $value );
				
				
				$videogalleryFolderPath = $this->m_site->GetVideogalleryImageRoot ( true, true );
				$videoID = DB::POST( 'videogallery_itemID' );
				
				//		DELETE VIDEOS
				if ( DB::POST( 'del_demo' ) ) {
					
					unlink($videogalleryFolderPath["demo"] . $videoID . ".flv");
				}
				
				if ( DB::POST( 'del_mobile' ) ) {
					
					unlink($videogalleryFolderPath["other"] . "mobile/" . $videoID . ".3gp");
				}
				
				if ( DB::POST( 'del_full' ) ) {
					
					if ( file_exists($videogalleryFolderPath["other"] . "full/" . $videoID . ".mpeg") ) {
						
						unlink($videogalleryFolderPath["other"] . "full/" . $videoID . ".mpeg");
					}
					
					if ( file_exists($videogalleryFolderPath["other"] . "full/" . $videoID . ".avi") ) {
						
						unlink($videogalleryFolderPath["other"] . "full/" . $videoID . ".avi");
					}
				}
				
				//		UPLOAD VIDEOS
				//		DEMO VERSION
				// get upload file properties
				$name = str_replace ( " ", "", $_FILES ['gl2_img_demo'] ['name'] );
				$size = str_replace ( " ", "", $_FILES ['gl2_img_demo'] ['size'] );
				
				// image size is 0 , then return 
				if ($size > 0)
				{
					// get uploaded file extension
					$path_parts = pathinfo ( $_FILES ['gl2_img_demo'] ['name'] );
					$ext = $path_parts ['extension'];
					$ext = strtolower ( $ext );
					
					// replase uper chares to lower
					$pos = strripos ( $name, $ext );
					$name = substr ( $name, 0, $pos ) . $ext;
					
					//////////////////////////////////////////////////////
					
					// all posible extentions
					$types = array ('flv');
					$res = array_search ( $ext, $types );
					$fileAddress = $videogalleryFolderPath["demo"] . $videoID . "." . $types[$res];
					
					if ( is_int ( $res ) && !file_exists ( $fileAddress ))
					{
						FolderXP::Upload ( $_FILES ['gl2_img_demo'] ['tmp_name'], $fileAddress );
					}
				}
				
				//		MOBILE VERSION
				// get upload file properties
				$name = str_replace ( " ", "", $_FILES ['gl2_img_mobile'] ['name'] );
				$size = str_replace ( " ", "", $_FILES ['gl2_img_mobile'] ['size'] );
				
				// image size is 0 , then return 
				if ($size > 0)
				{
					// get uploaded file extension
					$path_parts = pathinfo ( $_FILES ['gl2_img_mobile'] ['name'] );
					$ext = $path_parts ['extension'];
					$ext = strtolower ( $ext );
					
					// replase uper chares to lower
					$pos = strripos ( $name, $ext );
					$name = substr ( $name, 0, $pos ) . $ext;
					
					//////////////////////////////////////////////////////
					
					// all posible extentions
					$types = array ('3gp');
					$res = array_search ( $ext, $types );
					$fileAddress = $videogalleryFolderPath["other"] . "mobile/" . $videoID . "." . $types[$res];
					
					if ( is_int ( $res ) && !file_exists ( $fileAddress ))
					{
						FolderXP::Upload ( $_FILES ['gl2_img_mobile'] ['tmp_name'], $fileAddress );
					}
				}
				
				//		FULL VERSION
				// get upload file properties
				$name = str_replace ( " ", "", $_FILES ['gl2_img_full'] ['name'] );
				$size = str_replace ( " ", "", $_FILES ['gl2_img_full'] ['size'] );
				
				// image size is 0 , then return 
				if ($size > 0)
				{
					// get uploaded file extension
					$path_parts = pathinfo ( $_FILES ['gl2_img_full'] ['name'] );
					$ext = $path_parts ['extension'];
					$ext = strtolower ( $ext );
					
					// replase uper chares to lower
					$pos = strripos ( $name, $ext );
					$name = substr ( $name, 0, $pos ) . $ext;
					
					// all posible extentions
					$types = array ('avi', 'mpeg');
					$res = array_search ( $ext, $types );
					$fileAddress = $videogalleryFolderPath["other"] . "full/" . $videoID . "." . $types[$res];
					
					if ( is_int ( $res ) && !file_exists ( $fileAddress ))
					{
						if ( file_exists($videogalleryFolderPath["other"] . "full/" . $videoID . ".mpeg") ) {
							
							unlink($videogalleryFolderPath["other"] . "full/" . $videoID . ".mpeg");
						}
						if ( file_exists($videogalleryFolderPath["other"] . "full/" . $videoID . ".avi") ) {
							
							unlink($videogalleryFolderPath["other"] . "full/" . $videoID . ".avi");
						}
						
						FolderXP::Upload ( $_FILES ['gl2_img_full'] ['tmp_name'], $fileAddress );
					}
				}
				break;
			case 'changeFile' :
				$value = DB::POST ( 'File' );
				$ar = $this->m_videogallery_item->SetPath ( $value );
				break;
			case 'changePosition' :
				$value = DB::POST ( 'Position' );
				$ar = $this->m_videogallery_item->SetPosition ( $value );
				break;
			case 'delete' :
				$this->m_videogallery_item->Delete ();
				break;
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
	 * Module features must be displayed hear
	 *
	 */
	function DisplayFeatures()
	{
		if (! $this->m_videogallery_item->GetID ())
		{return false;}
		$videogallery_itemID = $this->m_videogallery_item->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		$nameS = $this->GetNameString ( 'title' ); 
		$descS = $this->GetNameString ( 'desc' );
		$linkS = $this->GetNameString ( 'link' );
		$toallLangS = $this->GetNameString ( 'toalllang' );
		
		$videoIDS = $this->GetNameString ( 'videoID' );
		$viewCountS = $this->GetNameString ( 'viewCount' );
		$videoCodeS = $this->GetNameString ( 'videoCode' );
		$hideS = $this->GetNameString ( 'hide' );
		
		// values
		$VideogalleryIDV = $this->m_videogallery_item->GetVideogalleryID ();
		$NameV = $this->m_videogallery_item->GetName ( $this->m_displayLang );
		$DescV = $this->m_videogallery_item->GetDesc ( $this->m_displayLang );
		$LinkV = $this->m_videogallery_item->GetLink ( $this->m_displayLang );
		$pathV = $this->m_videogallery_item->GetPath();
		
		$videoIDV = $this->m_videogallery_item->GetVideoID();
		$viewCountV = $this->m_videogallery_item->GetViewCount();
		$videoCodeV = $this->m_videogallery_item->GetVideoCode();
		$hideV = $this->m_videogallery_item->GetHide();

		// display image
		print('<div class="userFormLabel ">&nbsp;</div>
                <div><img src="http://i1.ytimg.com/vi/'.$videoCodeV.'/default.jpg" height="100" alt=" " /></div>
                <br clear="all" />');
		
		//////////////////////////////////////
		//Forms
		$this->DisplayFormHeadr ( 'update', '', 'post', null, null, ' enctype="multipart/form-data"' );
		$this->DisplayHidden ( 'videogalleryID', $VideogalleryIDV );
		$this->DisplayHidden ( 'videogallery_itemID', $videogallery_itemID );

		$reading = $this->IsRecReadable ( $this->m_videogallery_item, 'name' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery_item, 'name' );
		if ($reading || $updating)
		{
			print('<div class="userFormLabel "><label><strong>'.$nameS.'</strong></label></div><div>');
			if ($updating)
			{
				print ("<input type='text' value='$NameV' name='Name' id='Name' size='48' maxlength='255' />\n") ;
			}
			else
			{
				print ($NameV) ;
			}
			print('</div><br clear="all" />');
		}
		
		// description
		$reading = $this->IsRecReadable ( $this->m_videogallery_item, 'desc' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery_item, 'desc' );
		if ($reading || $updating)
		{
			print('<div class="userFormLabel "><label><strong>'.$descS.'</strong></label></div><div>');
			if ($updating)
			{
				print ("<textarea name='Desc' id='Desc' cols=\"42\" rows=\"5\">$DescV</textarea>\n") ;
			}
			else
			{
				print ($DescV) ;
			}
			print('</div><br clear="all" />');
		}
		
		// code
		$reading = $this->IsRecReadable ( $this->m_videogallery_item, 'videoCode' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery_item, 'videoCode' );
		if ($reading || $updating)
		{
			print('<div class="userFormLabel "><label><strong>'.$videoCodeS.'</strong></label></div><div>');
			print('<input type="text" name="videoCode" value="'.$videoCodeV.'" />');
			print('</div><br clear="all" />');
		}
		
		// link
		$reading = $this->IsRecReadable ( $this->m_videogallery_item, 'link' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery_item, 'link' );
		if (0)
		{
			print('<div class="userFormLabel "><label><strong>'.$linkS.'</strong></label></div><div>');
			if ($updating)
			{
				print ("<input type='text' value='$LinkV' name='Link' id='Link' size='48' maxlength='255' />\n") ;
				print('<input name="toalllink" type="checkbox" value="1" checked="checked" /> <strong>'.$toallLangS.'</strong>');
			}
			else
			{
				print ($LinkV) ;
			}
			print('</div><br clear="all" />');
		}
		
		// videoID
		/*$reading = $this->IsRecReadable ( $this->m_videogallery_item, 'videoID' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery_item, 'videoID' );
		if ($reading || $updating)
		{
			print('<div class="userFormLabel "><label><strong>'.$videoIDS.'</strong></label></div><div>');
			print ($videoIDV) ;
			print('</div><br clear="all" />');
		}*/
		
		// viewCount
		$reading = $this->IsRecReadable ( $this->m_videogallery_item, 'viewCount' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery_item, 'viewCount' );
		if ($reading || $updating)
		{
			print('<div class="userFormLabel "><label><strong>'.$viewCountS.'</strong></label></div><div>');
			print('<input type="text" name="viewCount" value="'.$viewCountV.'" />');
			print('</div><br clear="all" />');
		}
		
		// hide
		$reading = $this->IsRecReadable ( $this->m_videogallery_item, 'hide' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery_item, 'hide' );
		if ($reading || $updating)
		{
			print('<div class="userFormLabel "><label><strong>'.$hideS.'</strong></label></div><div>');
			if ($updating)
			{
				if ( $hideV ) {
					
					print('<input name="hide" type="checkbox" checked="checked" />');
				} else {
					
					print('<input name="hide" type="checkbox" />');
				}
				
			}
			else
			{
				print ($LinkV) ;
			}
			print('</div><br clear="all" />');
		}
	
		print('<div class="userFormLabel ">&nbsp;</div>');
		$this->DisplayFormFooter ( $doneS );

		return true;
	}
}
?>