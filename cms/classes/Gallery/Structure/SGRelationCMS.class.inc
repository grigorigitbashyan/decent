<?php
Site::IncludeFile ( 'classes/Gallery/Structure/SGRelation.class.inc' );
Site::IncludeFile ( 'classes/Gallery/GalleryList.class.inc' );
Site::IncludeFile ( 'classes/Gallery/Pattern/GalleryPatternList.class.inc' );

class SGRelationCMS extends CMSObject
{
	/**
	 * Enter description here...
	 *
	 * @var SGRelation
	 */
	protected $m_sgRelation;
	
	protected $m_ID;
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->m_ID = DB::REQUEST ( 'catID' );
		
		if ($this->m_ID)
		{
			$this->m_sgRelation = new SGRelation ( $this->m_ID );
		}
		
		$this->ReadNameStrings ( 'classes/Gallery/langs/', 'gallery.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		if ($this->m_ID < 2)
		{
			return false;
		}
		
		// get names
		$doneS = $this->GetNameString ( 'done' );
		$title = 'Gallery Relation';
		// get gallery list
		$gallList = new GalleryList ( );
		
		// get pattern list
		$gallPattList = new GalleryPatternList ( );
		
		$currGallID = null;
		$currPattID = null;
		// single case
		if ($this->m_sgRelation)
		{
			////////////////////////////
			$rels = $this->m_sgRelation->GetStructure ();
			
			if (count ( $rels ) > 0)
			{
				$currGallID = $rels [0] ['galleryID'];
				$currPattID = $rels [0] ['patternID'];
			}
		}
		
		///////////////////////
		print ( "<div class=\"someGTitleBox\">$title</div><div class=\"someGBox\">
				<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr>	<td>" );
		
		// display from
		$this->DisplayFormHeadr ( 'manageGalleryPattner' );
		$this->DisplayHidden('catID', $this->m_ID);
		
		print ( '<select name="galleryID">' );
		$gallList->DisplaySelect ( '...', $currGallID );
		print ( '</select>' );
		
		$pattCount = $gallPattList->GetItemCount ();
		
		// display hidden it it is only one pattern
		if ($pattCount == 1)
		{
			$item = $gallPattList->GetStructure ();
			$this->DisplayHidden ( 'patternID', $item [0] ['ID'] );
		}
		else
		{
			print ( '<select name="patternID">' );
			$gallPattList->DisplaySelect ( null, $currPattID );
			print ( '</select>' );
		}
		$this->DisplayFormFooter ( $doneS );
		
		print ( '</td></tr></table></div>' );
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = 'CREATE TABLE `structure_gallery` (
			`ID` SMALLINT NOT NULL AUTO_INCREMENT PRIMARY KEY ,
			`categoryID` SMALLINT NOT NULL ,
			`galleryID` SMALLINT NOT NULL ,
			`patternID` SMALLINT NOT NULL
			);';
		
		return $sql;
	}
	
	function DoAction()
	{
		$ar = null;
		switch ($this->GetFormAction())
		{
			case 'manageGalleryPattner':
				$galleryID = DB::POST('galleryID');
				$patternID = DB::POST('patternID');
				
				// add gallery
				$ar = $this->m_sgRelation->Manage($galleryID, $patternID);
				break;
		}
		
		if($ar)
		{
			$this->m_sgRelation = new SGRelation($this->m_ID);
		}
		
		return $ar;
	}
}
?>