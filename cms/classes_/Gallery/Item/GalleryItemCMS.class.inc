<?php
//TODO corrent the include
Site::IncludeFile ( 'classes/Gallery/Item/GalleryItem.class.inc' );

/**
 * Enter class description
 */
class GalleryItemCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var GalleryItem
	 */
	protected $m_gallery_item;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_gallery_item = new GalleryItem ( $ID );
		
		$this->ReadNameStrings ( 'classes/Gallery/langs/', 'gallery_item.xml' );
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
		
		$str = "CREATE TABLE `gallery_item` (
			  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
			  `galleryID` int(11) NOT NULL,
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
				$ar = $this->m_gallery_item->SetName ( $value, $this->m_displayLang );

				$value = DB::POST ( 'Desc' );
				$ar |= $this->m_gallery_item->SetDesc ( $value, $this->m_displayLang );

				$value = DB::POST ( 'Link' );
				
				if(DB::POST('toalllink'))
				{
					$langs = $this->m_site->GetLanguages();
					foreach ($langs as $lang)
					{
						$ar |= $this->m_gallery_item->SetLink ( $value, $lang );
					}
				}
				else 
				{
					$ar |= $this->m_gallery_item->SetLink ( $value, $this->m_displayLang );
				}
				break;
			case 'changeFile' :
				$value = DB::POST ( 'File' );
				$ar = $this->m_gallery_item->SetPath ( $value );
				break;
			case 'changePosition' :
				$value = DB::POST ( 'Position' );
				$ar = $this->m_gallery_item->SetPosition ( $value );
				break;
			case 'delete' :
				$this->m_gallery_item->Delete ();
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
		if (! $this->m_gallery_item->GetID ())
		{return false;}
		$gallery_itemID = $this->m_gallery_item->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		$nameS = $this->GetNameString ( 'title' ); 
		$descS = $this->GetNameString ( 'desc' );
		$linkS = $this->GetNameString ( 'link' );
		$toallLangS = $this->GetNameString ( 'toalllang' );
		
		// values
		$GalleryIDV = $this->m_gallery_item->GetGalleryID ();
		$NameV = $this->m_gallery_item->GetName ( $this->m_displayLang );
		$DescV = $this->m_gallery_item->GetDesc ( $this->m_displayLang );
		$LinkV = $this->m_gallery_item->GetLink ( $this->m_displayLang );
		$pathV = $this->m_gallery_item->GetPath();

		// display image
		print('<div class="userFormLabel ">&nbsp;</div>
                <div><img src="'.$pathV.'" height="100" alt=" " /></div>
                <br clear="all" />');
		//////////////////////////////////////
		//Forms
		$this->DisplayFormHeadr ( 'update' );
		$this->DisplayHidden ( 'galleryID', $GalleryIDV );
		$this->DisplayHidden ( 'gallery_itemID', $gallery_itemID );

		$reading = $this->IsRecReadable ( $this->m_gallery_item, 'name' );
		$updating = $this->IsRecUpdatable ( $this->m_gallery_item, 'name' );
		
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
		$reading = $this->IsRecReadable ( $this->m_gallery_item, 'desc' );
		$updating = $this->IsRecUpdatable ( $this->m_gallery_item, 'desc' );
		
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
		
		// link
		$reading = $this->IsRecReadable ( $this->m_gallery_item, 'link' );
		$updating = $this->IsRecUpdatable ( $this->m_gallery_item, 'link' );
		
		if ($reading || $updating)
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
	
		print('<div class="userFormLabel ">&nbsp;</div>');
		$this->DisplayFormFooter ( $doneS );

		return true;
	}
}
?>