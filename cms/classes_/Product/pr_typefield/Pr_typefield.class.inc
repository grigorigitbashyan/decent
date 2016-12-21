<?php
/**
* Enter class description here...
*
* @package Item
*/
class Pr_typefield extends Record
{
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_producttypeID;

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
	protected $m_single;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_order;

	/**
	* Enter description here...
	*
	* @param int $ID
	* @return Pr_typefield
	*/
	function __construct($ID = null)
	{
		 parent::__construct($ID, 'pr_typefield');

		 $this->Read();
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
	* Return value of m_producttypeID
	*
	* @return int
	*/
	function GetProducttypeID()
	{
		return $this->m_producttypeID;
	}

	/**
	* Return value of m_filedID
	*
	* @return int
	*/
	function GetFiledID()
	{
		return $this->m_filedID;
	}

	/**
	* Return value of m_single
	*
	* @return int
	*/
	function GetSingle()
	{
		return $this->m_single;
	}
	
	/**
	* Return value of m_order
	*
	* @return int
	*/
	function GetOrder()
	{
		return $this->m_order;
	}

	// Set functions ////////////////////////////////////////////////////


	/**
	* Set value to m_producttypeID  
	*
	* @param int $producttypeID
	* @param bool $dbupdate
	* @return bool
	*/
	function SetProducttypeID($producttypeID, $dbupdate = true)
	{
		return $this->UpdateValue('producttypeID', $producttypeID, $dbupdate);
	}

	/**
	* Set value to m_filedID  
	*
	* @param int $filedID
	* @param bool $dbupdate
	* @return bool
	*/
	function SetFiledID($filedID, $dbupdate = true)
	{
		return $this->UpdateValue('filedID', $filedID, $dbupdate);
	}

	/**
	* Set value to m_single  
	*
	* @param int $single
	* @param bool $dbupdate
	* @return bool
	*/
	function SetSingle($single, $dbupdate = true)
	{
		return $this->UpdateValue('single', $single, $dbupdate);
	}
	
	/**
	* Set value to m_order
	*
	* @param int $single
	* @param bool $dbupdate
	* @return bool
	*/
	function SetOrder($order, $dbupdate = true)
	{
		return $this->UpdateValue('order', $order, $dbupdate);
	}
}
?>