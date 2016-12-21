<?php
/**
* Enter class description here...
*
* @package Box
*/
class Box extends NamedRecord
{
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_type;

	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_param;

	/**
	* Enter description here...
	*
	* @param int $ID
	* @return Box
	*/
	function __construct($ID = null)
	{
		 parent::__construct($ID, 'box');

		 $this->Read();
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

	/**
	* Return value of m_type
	*
	* @return int
	*/
	function GetType()
	{
		return $this->m_type;
	}

	/**
	* Return value of m_param
	*
	* @return string
	*/
	function GetParam()
	{
		return $this->m_param;
	}
	
	/**
	* Set value to m_type  
	*
	* @param int $type
	* @param bool $dbupdate
	* @return bool
	*/
	function SetType($type, $dbupdate = true)
	{
		return $this->UpdateValue('type', $type, $dbupdate);
	}

	/**
	* Set value to m_param  
	*
	* @param string $param
	* @param bool $dbupdate
	* @return bool
	*/
	function SetParam($param, $dbupdate = true)
	{
		return $this->UpdateValue('param', $param, $dbupdate);
	}
}
?>