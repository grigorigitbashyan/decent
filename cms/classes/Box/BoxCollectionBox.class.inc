<?php

/**
 * Relation between box collection and boxes
 *
 * @package Box
 */
class BoxCollectionBox
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_boxCollectionID;
	
	function __construct($boxCollectionID = null)
	{
		$this->m_boxCollectionID = $boxCollectionID;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $boxID
	 */
	function AddBox($boxID)
	{
		// get last position and incriment it
		$position = 1;
		
		$boxes = $this->GetRelatedBoxList();
		if($boxes)
		{
			$lastBox = array_pop($boxes);
			$position = $lastBox['position'] + 1;
		}
		
		
		$sql = "INSERT INTO `boxcollection_box`
				(`boxcollectionID` , `boxID` ,`position`)
				VALUES
				($this->m_boxCollectionID, $boxID, $position)";
			
		return DB::Query($sql);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $recodID
	 * @return bool
	 */
	function DeleteBox($recodID)
	{
		$sql = "DELETE FROM `boxcollection_box` WHERE `ID` = $recodID";
		return DB::Query($sql);
	}
	
	/**
	 * $direction true means to up, false to down
	 *
	 * @param int $recodID
	 * @param bool $direction
	 */
	function Reposition($recodID, $direction)
	{
		// get record position
		$sql = "SELECT `position` FROM `boxcollection_box` WHERE `ID` = $recodID";
		$oldPosition = DB::QueryToValue($sql);

		// get second record
		if($direction)
		{
			$sql = "SELECT * FROM `boxcollection_box` 
					WHERE `boxcollectionID` = $this->m_boxCollectionID AND `position` < $oldPosition
					ORDER BY `position`  DESC LIMIT 1";
		}
		else 
		{
			$sql = "SELECT * FROM `boxcollection_box` 
					WHERE `boxcollectionID` = $this->m_boxCollectionID AND `position` > $oldPosition
					ORDER BY `position` LIMIT 1";
		}
		
		$row = DB::QueryToRow($sql);
		
		if($row)
		{
			$secondID = $row['ID'];;
			$newPosition = $row['position'];;
			// change position
			$sql = "UPDATE `boxcollection_box` SET `position` = $newPosition WHERE `ID` = $recodID";
			DB::Query($sql);
			
			$sql = "UPDATE `boxcollection_box` SET `position` = $oldPosition WHERE `ID` = $secondID";
			DB::Query($sql);
		}
		
	}
	
	/**
	 * Enter description here...
	 *
	 * @return array
	 */
	function GetRelatedBoxList()
	{
		$sql = "SELECT * FROM `boxcollection_box` WHERE `boxcollectionID` = $this->m_boxCollectionID ORDER BY `position`";
		return DB::QueryToArray($sql);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $boxID
	 * @return bool
	 */
	static function IsBoxRelated($boxID)
	{
		$sql = "SELECT * FROM `boxcollection_box` WHERE `boxID` = $boxID";
		$res = DB::QueryToArray($sql);
		
		return (count($res) > 0);
	}
}
?>