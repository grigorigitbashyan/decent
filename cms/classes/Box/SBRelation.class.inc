<?php
/**
 * Enter description here...
 *
 * @package Box
 */
class SBRelation
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_catID;
	
	/**
	 * Enter description here...
	 *
	 * @param int $catID
	 */
	function __construct($catID = null)
	{
		$this->m_catID = $catID;
	}
	
	/**
	 * Add new relation category and file and type
	 *
	 * @param string $contID
	 * @param int $contID
	 * @return bool
	 */
	function Add($boxCollID, $align = 0)
	{
		$align = intval ( $align );
		
		// get category
		$sql = "SELECT count(*) FROM `structure_boxcollection_box` WHERE `structureID` = {$this->m_catID} AND `align` = $align";
		$count = DB::QueryToValue ( $sql );
		
		if ($count > 0) // if relation has olredy exist then update it set new contentID
		{
			$sql = "UPDATE `structure_boxcollection_box` SET `boxcollectionID` = '$boxCollID' WHERE `structureID` = {$this->m_catID} AND `align` = $align LIMIT 1 ;";
			$res = DB::Query ( $sql );
			return $res;
		}
		else // else inser new relation
		{
			$sql = "INSERT INTO `structure_boxcollection_box` (`structureID`, `boxcollectionID`, `align`) VALUES ('$this->m_catID', '$boxCollID', '$align');";
			$res = DB::Query ( $sql );
			return $res;
		}
	}
	
	/**
	 * Delete relation
	 *
	 * @param int $boxCollID
	 * @param int $align
	 * @return bool
	 */
	function Delete($align)
	{
		$sql = "DELETE FROM `structure_boxcollection_box` 
    			WHERE `structureID` = $this->m_catID AND `align` = $align";
		
		$res = DB::Query ( $sql );
		return $res;
	}
	
	/**
	 * Return Id of box that has been related to category 
	 * @param int $align
	 * @param bool $global
	 * 
	 * @return array|null
	 */
	function GetCatRelationID($align = 0, $global = false)
	{
		if ($this->m_catID > 0)
		{
			$catID = ($global) ? 1 : $this->m_catID;
			$sql = "SELECT `boxcollectionID` FROM `structure_boxcollection_box` WHERE structureID = {$catID} AND `align` = $align";
			return DB::QueryToValue ( $sql );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $boxCollectionID
	 * @return bool
	 */
	static function IsBoxCollectionRelated($boxCollectionID)
	{
		$sql = "SELECT * FROM `structure_boxcollection_box` WHERE `boxcollectionID` = $boxCollectionID";
		$res = DB::QueryToArray ( $sql );
		
		return (count ( $res ) > 0);
	}
}
?>