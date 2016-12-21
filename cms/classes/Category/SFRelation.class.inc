<?php
/**
 * Enter description here...
 *
 * @package Structure
 */
class SFRelation
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
	function Add($fileName, $showtype = 0)
	{
		$showtype = intval ( $showtype );
		
		$sql = "SELECT count(*) FROM `structure_file` WHERE `structureID` = {$this->m_catID} AND `show_type` = $showtype";
		$count = DB::QueryToValue ( $sql );
		
		if ($count > 0) // if relation has olredy exist then update it set new contentID
		{
			$sql = "UPDATE `structure_file` SET `filename` = '$fileName' WHERE `structure_file`.`structureID` ={$this->m_catID} AND `show_type`= $showtype LIMIT 1 ;";
		}
		else // else inser new relation
		{
			$sql = "INSERT INTO `structure_file` (`structureID`, `filename`, `show_type`) VALUES ({$this->m_catID}, '$fileName', $showtype);";
		}
		
		$res = DB::Query ( $sql );
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $showtype
	 * @return bool
	 */
	function DeleteRelation($showtype = 0)
	{
		$sql = "DELETE FROM `structure_file` WHERE `structureID` = {$this->m_catID} AND `show_type` = $showtype";
		$res = DB::Query ( $sql );
		
		return $res;
	}
	/**
	 * Return all related files
	 *
	 * @return array|null
	 */
	function GetCatRelations()
	{
		if ($this->m_catID > 0)
		{
			$sql = "SELECT * FROM `structure_file` WHERE `structureID` = {$this->m_catID}";
			return DB::QueryToArray ( $sql );
		}
		
		return null;
	}
}
?>