<?php
/**
 * Enter class description
 */
class ModuleAccess extends Record
{
	/**
	* Description
	*
	* @var char
	*/
	protected $m_module;

	/**
	* Description
	*
	* @var int
	*/
	protected $m_userTypeID;

	/**
	* Description
	*
	* @var int
	*/
	protected $m_access;

	/**
	 * Enter description here...
	 *
	 * @param int $ID
	*/
	function __construct($ID = null)
	{
		parent::__construct($ID, 'module_access');
	
		$this->Read();
	}
	/**
	 * Insert ModuleAccess 
	* @return bool
	*/
	function Insert($properties)
	{
		// delete from content DB
		return $this->InsertRecord($properties);	}

	/**
	 * Delete ModuleAccess 
	* @return bool
	*/
	function Delete()
	{
		// delete from content DB
		return $this->DeleteRecord();	}

	/**
	 * Set value of m_module
	 *
	 * @param char $module
	 * @param bool $dbupdate
	 * @return char
	*/
	function SetModule($module, $dbupdate = true)
	{
		$this->UpdateValue('module', $module, $dbupdate);
	}

	/**
	 * Set value of m_userTypeID
	 *
	 * @param int $userTypeID
	 * @param bool $dbupdate
	 * @return int
	*/
	function SetUserTypeID($userTypeID, $dbupdate = true)
	{
		$this->UpdateValue('userTypeID', $userTypeID, $dbupdate);
	}

	/**
	 * Set value of m_access
	 *
	 * @param int $access
	 * @param bool $dbupdate
	 * @return int
	*/
	function SetAccess($access, $dbupdate = true)
	{
		$this->UpdateValue('access', $access, $dbupdate);
	}

	/**
	 * Return value of m_module
	 *
	 * @return char
	*/
	function GetModule()
	{
		return $this->m_module;
	}

	/**
	 * Return value of m_userTypeID
	 *
	 * @return int
	*/
	function GetUserTypeID()
	{
		return $this->m_userTypeID;
	}

	/**
	 * Return value of m_access
	 *
	 * @return int
	*/
	function GetAccess()
	{
		return $this->m_access;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $status
	 */
	function SetReadStatus($status)
	{
		
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $status
	 */
	function SetWriteStatus($status)
	{
		
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $status
	 */
	function SetInsertStatus($status)
	{
		
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $status
	 */
	function SetDeleteStatus($status)
	{
		
	}
}
?>