<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');

class SGVideoRelation extends TreeView
{
	protected $m_catID;
	
	function __construct($catID)
	{
		parent::__construct();
		
		$this->m_catID = $catID;
		$sql = "SELECT * FROM `structure_videogallery` WHERE categoryID={$this->m_catID}";
		$this->SetQueryString($sql);	
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $videogalleryID
	 * @param int $patternID
	 * @return bool
	 */
	function Manage($videogalleryID, $patternID)
	{
		$sql = '';
		if($videogalleryID)
		{
			// check
			$sqlCh = "SELECT * FROM `structure_videogallery` WHERE `categoryID`={$this->m_catID}";
			if(DB::QueryToRow($sqlCh))
			{
				$sql = "UPDATE `structure_videogallery` SET `patternID` = '$patternID', `videogalleryID`={$videogalleryID} 
						WHERE `categoryID`={$this->m_catID}";
			}
			else 
			{
				$sql = "INSERT INTO `structure_videogallery` (`categoryID` ,`videogalleryID` ,`patternID`)
						VALUES ('{$this->m_catID}', '$videogalleryID', '$patternID');";
			}
		}
		// remove
		else
		{
			$sql = "DELETE FROM `structure_videogallery` WHERE `categoryID`={$this->m_catID}";
		}
		
		return DB::Query($sql);
	}
}
?>