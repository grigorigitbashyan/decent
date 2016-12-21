<?php
/**
* Enter class description here...
*
* @package ItemOptions
*/
class Optionsdata extends NamedRecord
{
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_optionsID;

	/**
	 * Key for options
	 *
	 * @var string
	 */
	protected $m_key;
	
	/**
	* Enter description here...
	*
	* @param int $ID
	* @return Optionsdata
	*/
	function __construct($ID = null, $field = "ID")
	{
		parent::__construct($ID, 'optionsdata', $field);
		
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
	* Return value of m_oprionsID
	*
	* @return int
	*/
	function GetOptionsID()
	{
		return $this->m_optionsID;
	}

	/**
	* Set value to m_oprionsID  
	*
	* @param int $oprionsID
	* @param bool $dbupdate
	* @return bool
	*/
	function SetOptionsID($optionsID, $dbupdate = true)
	{
		return $this->UpdateValue('optionsID', $optionsID, $dbupdate);
	}
	
	/**
	 * get key
	 *
	 * @return string
	 */
	function GetKey()
	{
		return $this->m_key;
	}
	
	/**
	 * Set key
	 *
	 * @param string $key
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetKey($key, $dbupdate = true)
	{
		return $this->UpdateValue('key', $key, $dbupdate);
	}
}
?>