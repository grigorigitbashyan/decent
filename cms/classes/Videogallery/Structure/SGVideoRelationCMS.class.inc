<?php
Site::IncludeFile ( 'classes/Videogallery/Structure/SGVideoRelation.class.inc' );
Site::IncludeFile ( 'classes/Videogallery/VideogalleryList.class.inc' );
Site::IncludeFile ( 'classes/Videogallery/Pattern/VideogalleryPatternList.class.inc' );

class SGVideoRelationCMS extends CMSObject
{
	/**
	 * Enter description here...
	 *
	 * @var SGVideoRelation
	 */
	protected $m_sgVideoRelation;
	
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
			$this->m_sgVideoRelation = new SGVideoRelation ( $this->m_ID );
		}
		
		$this->ReadNameStrings ( 'classes/Videogallery/langs/', 'videogallery.xml' );
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
		$title = 'Videogallery Relation';
		// get videogallery list
		$gallList = new VideogalleryList ( );
		
		// get pattern list
		$gallPattList = new VideogalleryPatternList ( );
		
		$currGallID = null;
		$currPattID = null;
		// single case
		if ($this->m_sgVideoRelation)
		{
			////////////////////////////
			$rels = $this->m_sgVideoRelation->GetStructure ();
			
			if (count ( $rels ) > 0)
			{
				$currGallID = $rels [0] ['videogalleryID'];
				$currPattID = $rels [0] ['patternID'];
			}
		}
		
		///////////////////////
		print ( "<div class=\"someGTitleBox\">$title</div><div class=\"someGBox\">
				<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr>	<td>" );
		
		// display from
		$this->DisplayFormHeadr ( 'manageVideogalleryPattner' );
		$this->DisplayHidden('catID', $this->m_ID);
		
		print ( '<select name="videogalleryID">' );
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
		$sql = 'CREATE TABLE `structure_videogallery` (
			`ID` SMALLINT NOT NULL AUTO_INCREMENT PRIMARY KEY ,
			`categoryID` SMALLINT NOT NULL ,
			`videogalleryID` SMALLINT NOT NULL ,
			`patternID` SMALLINT NOT NULL
			);';
		
		return $sql;
	}
	
	function DoAction()
	{
		$ar = null;
		switch ($this->GetFormAction())
		{
			case 'manageVideogalleryPattner':
				$videogalleryID = DB::POST('videogalleryID');
				$patternID = DB::POST('patternID');
				
				// add videogallery
				$ar = $this->m_sgVideoRelation->Manage($videogalleryID, $patternID);
				break;
		}
		
		if($ar)
		{
			$this->m_sgVideoRelation = new SGVideoRelation($this->m_ID);
		}
		
		return $ar;
	}
}
?>