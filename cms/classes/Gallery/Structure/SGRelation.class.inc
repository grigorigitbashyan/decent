<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');

class SGRelation extends TreeView
{
	protected $m_catID;
	
	function __construct($catID)
	{
		parent::__construct();
		
		$this->m_catID = $catID;
		$sql = "SELECT * FROM `structure_gallery` WHERE categoryID={$this->m_catID}";
		$this->SetQueryString($sql);	
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $galleryID
	 * @param int $patternID
	 * @return bool
	 */
	function Manage($galleryID, $patternID)
	{
		$sql = '';
		if($galleryID)
		{
			// check
			$sqlCh = "SELECT * FROM `structure_gallery` WHERE `categoryID`={$this->m_catID}";
			if(DB::QueryToRow($sqlCh))
			{
				$sql = "UPDATE `structure_gallery` SET `patternID` = '$patternID', `galleryID`={$galleryID} 
						WHERE `categoryID`={$this->m_catID}";
			}
			else 
			{
				$sql = "INSERT INTO `structure_gallery` (`categoryID` ,`galleryID` ,`patternID`)
						VALUES ('{$this->m_catID}', '$galleryID', '$patternID');";
			}
		}
		// remove
		else
		{
			$sql = "DELETE FROM `structure_gallery` WHERE `categoryID`={$this->m_catID}";
		}
		
		return DB::Query($sql);
	}
}
?>