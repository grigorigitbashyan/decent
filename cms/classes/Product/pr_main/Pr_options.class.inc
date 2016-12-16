<?php
/**
 * Enter class description here...
 *
 * @package Item
 */
class Pr_options extends Record
{
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_productID;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_fieldID;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_relatedID;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_group;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @param int $productID
	 * @param int $fieldID
	 */
	function __construct($ID = null, $productID = null, $fieldID = null, $group = 0, $relatedID = 0)
	{
		if ($ID == null && $productID && $fieldID)
		{
			// get according ID
			if ( $relatedID )
			{
				$sql = "SELECT `ID` FROM `pr_options` WHERE `productID` = $productID AND `fieldID` = $fieldID AND `group` = $group AND `relatedID` = $relatedID";
			}
			else 
			{
				$sql = "SELECT `ID` FROM `pr_options` WHERE `productID` = $productID AND `fieldID` = $fieldID AND `group` = $group";
			}
			$result = DB::QueryToArray($sql);
			
			$ID = $result['0']['ID'];
		}
		
		parent::__construct ( $ID, 'pr_options' );
		
		$this->Read ();
	}
	
	/**
	 * Update record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Update($properties)
	{
		return $this->UpdateValues ( $properties );
	}
	/**
	 * Insert new record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Insert($properties = null)
	{
		return $this->InsertRecord ( $properties );
	}
	
	// Get functions ///////////////////////////////////////////////////
	
	/**
	 * Adriada; added
	 * Return IDs for multi select values
	 * 
	 * Enter description here ...
	 */
	function GetIDs()
	{
		$sql = "SELECT `ID` 
				FROM `{$this->m_tableName}` 
				WHERE `fieldID` = {$this->m_fieldID} AND `productID` = {$this->m_productID};";
		return DB::QueryToVector($sql);
	}

	/**
	 * Return value of m_productID
	 *
	 * @return int
	 */
	function GetProductID()
	{
		return $this->m_productID;
	}
	
	/**
	 * Return value of m_fieldID
	 *
	 * @return int
	 */
	function GetFieldID()
	{
		return $this->m_fieldID;
	}
	
	/**
	 * Return value of m_relatedID
	 *
	 * @return int
	 */
	function GetRelatedID()
	{
		return $this->m_relatedID;
	}
	
	/**
	 * Return value of m_group
	 *
	 * @return int
	 */
	function GetGroup()
	{
		return $this->m_group;
	}
	
	/**
	 * Return relatedIDs for multi value
	 * Enter description here ...
	 */
	function GetRelatedIDs()
	{
		$sql = "SELECT `relatedID` 
				FROM `{$this->m_tableName}` 
				WHERE `fieldID` = {$this->m_fieldID} AND `productID` = {$this->m_productID};";
		return DB::QueryToVector($sql);
	}
	
	// Set functions ////////////////////////////////////////////////////
	/**
	 * Set value to m_productID  
	 *
	 * @param int $productID
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetProductID($productID, $dbupdate = true)
	{
		return $this->UpdateValue ( 'productID', $productID, $dbupdate );
	}
	
	/**
	 * Set value to m_fieldID  
	 *
	 * @param int $fieldID
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetFieldID($fieldID, $dbupdate = true)
	{
		return $this->UpdateValue ( 'fieldID', $fieldID, $dbupdate );
	}
	
	/**
	 * Set value to m_relatedID  
	 *
	 * @param int $relatedID
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetRelatedID($relatedID, $dbupdate = true)
	{
		return $this->UpdateValue ( 'relatedID', $relatedID, $dbupdate );
	}
	
	/**
	 * Delete all exists multi options
	 * Enter description here ...
	 */
	function DeleteMultiValues($group=0)
	{
		$sql = "DELETE 
				FROM `{$this->m_tableName}` 
				WHERE `fieldID` = {$this->m_fieldID} AND `productID` = {$this->m_productID} AND `group` = $group;";
		
		return DB::Query( $sql );
	}
	
}
?>