<?php
/**
* Enter class description here...
*
* @package Item
*/
class Pr_field extends NamedRecord
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
	* @protected int
	*/
	protected $m_subID;

	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_multilang;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_editor;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_fields;
	
	/**
	* Enter description here...
	*
	* @protected text
	*/
	protected $m_pattern;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_subtype;

	/**
	* Enter description here...
	*
	* @param int $ID
	* @return Pr_field
	*/
	function __construct($ID = null)
	{
		 parent::__construct($ID, 'pr_field');

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

	// Get functions ///////////////////////////////////////////////////
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
	* Return value of m_subID
	*
	* @return int
	*/
	function GetSubID()
	{
		return $this->m_subID;
	}

	/**
	* Return value of m_multilang
	*
	* @return int
	*/
	function GetMultilang()
	{
		return $this->m_multilang;
	}
	
	/**
	* Return value of m_editor
	*
	* @return int
	*/
	function GetEditor()
	{
		return $this->m_editor;
	}
	
	/**
	* Return value of m_fields
	*
	* @return string
	*/
	function GetFields()
	{
		return $this->m_fields;
	}
	
	/**
	* Return value of m_pattern
	*
	* @return text
	*/
	function GetFieldPattern()
	{
		return $this->m_pattern;
	}
	
	/**
	* Return value of m_subtype
	*
	* @return text
	*/
	function GetSubtype()
	{
		return $this->m_subtype;
	}

	// Set functions ////////////////////////////////////////////////////
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
	* Set value to m_subID  
	*
	* @param int $subID
	* @param bool $dbupdate
	* @return bool
	*/
	function SetSubID($subID, $dbupdate = true)
	{
		// if ID starts with 'm', it means thet it has been related to menu
		if(is_string($subID) && substr($subID, 0, 1) == 'm')
		{
			$subID = - intval(substr($subID, 1));
		}

		return $this->UpdateValue('subID', $subID, $dbupdate);
	}

	/**
	* Set value to m_multilang  
	*
	* @param int $multilang
	* @param bool $dbupdate
	* @return bool
	*/
	function SetMultilang($multilang, $dbupdate = true)
	{
		return $this->UpdateValue('multilang', $multilang, $dbupdate);
	}
	
	/**
	* Set value to m_editor
	*
	* @param int $editor
	* @param bool $dbupdate
	* @return bool
	*/
	function SetEditor($editor, $dbupdate = true)
	{
		return $this->UpdateValue('editor', $editor, $dbupdate);
	}
	
	/**
	* Set value to m_fields
	*
	* @param int $editor
	* @param bool $dbupdate
	* @return bool
	*/
	function SetFields($fields, $dbupdate = true)
	{
		return $this->UpdateValue('fields', $fields, $dbupdate);
	}
	
	/**
	* Set value to m_pattern
	*
	* @param text $pattern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetFieldPattern($pattern, $dbupdate = true)
	{
		return $this->UpdateValue('pattern', $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_subtype
	*
	* @param string $subtype
	* @param bool $dbupdate
	* @return bool
	*/
	function SetSubtype($subtype, $dbupdate = true)
	{
		return $this->UpdateValue('subtype', $subtype, $dbupdate);
	}
}
?>