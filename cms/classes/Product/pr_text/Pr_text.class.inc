<?php
/**
* Enter class description here...
*
* @package Item
*/
class Pr_text extends Record
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
	protected $m_subID;

	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_value;
	
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
	 * @param string $lang
	 */
	function __construct($ID = null, $productID = null, $fieldID = null, $lang = null, $group = 0)
	{
		if ($ID == null && $productID && $fieldID)
		{
			// get according ID
			$subID = ($lang) ? $lang : '0';
			
			$sql = "SELECT `ID` FROM `pr_text` WHERE `productID` = $productID AND `fieldID` = $fieldID AND `subID` = '$subID' AND `group` = $group";
			$result = DB::QueryToArray ( $sql );
			
			$ID = $result ['0'] ['ID'];
		}
		
		parent::__construct ( $ID, 'pr_text' );
		
		$this->Read ();
	}

	/**
	* Delete record from database
	*
	* @return bool
	*/
	function Delete()
	{
		 return $this->DeleteRecord();
	}
	/**
	* Update record
	*
	* @param array $properties
	* @return bool
	*/
	function Update($properties)
	{
		 return $this->UpdateValues($properties);
	}
	/**
	* Insert new record
	*
	* @param array $properties
	* @return bool
	*/
	function Insert($properties = null)
	{
		 return $this->InsertRecord($properties);
	}

	// Get functions ///////////////////////////////////////////////////
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
	* Return value of m_subID
	*
	* @return int
	*/
	function GetSubID()
	{
		return $this->m_subID;
	}

	/**
	* Return value of m_value
	*
	* @return string
	*/
	function GetValue($toBr = false)
	{
		return ($toBr == true) ? nl2br($this->m_value) : $this->m_value;  
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
		return $this->UpdateValue('productID', $productID, $dbupdate);
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
		return $this->UpdateValue('fieldID', $fieldID, $dbupdate);
	}

	/**
	* Set value to m_subID  
	*
	* @param int $subID
	* @param bool $dbupdate
	* @return bool
	*/
	function SetSubID($subID, $dbupdate = true)
	{
		return $this->UpdateValue('subID', $subID, $dbupdate);
	}

	/**
	* Set value to m_value  
	*
	* @param string $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetValue($value, $dbupdate = true)
	{
		return $this->UpdateValue('value', $value, $dbupdate);
	}
}
?>