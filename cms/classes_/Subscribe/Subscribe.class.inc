<?php
/**
* Enter class description here...
*
* @package Notification
*/
class Subscribe extends NamedRecord
{
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	protected $m_status;
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_contentID;
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_lastUserID;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct($ID, 'subscribe');

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
	* Return value of m_status
	*
	* @return bool
	*/
	function GetStatus()
	{
		return $this->m_status;
	}
	
	/**
	* Return value of m_contentID
	*
	* @return int
	*/
	function GetContentID()
	{
		return $this->m_contentID;
	}
	
	/**
	* Return value of m_lastUserID
	*
	* @return int
	*/
	function GetLastUserID()
	{
		return $this->m_lastUserID;
	}
	
	/**
	* Set value to m_status  
	*
	* @param bool $status
	* @param bool $dbupdate
	* @return bool
	*/
	function SetStatus($status, $dbupdate = true)
	{
		return $this->UpdateValue('status', $status, $dbupdate);
	}
	
	/**
	* Set value to m_contentID 
	*
	* @param int $contentID
	* @param bool $dbupdate
	* @return bool
	*/
	function SetContentID($contentID, $dbupdate = true)
	{
		return $this->UpdateValue('contentID', $contentID, $dbupdate);
	}
	
	/**
	* Set value to m_lastUserID 
	*
	* @param int $lastUserID
	* @param bool $dbupdate
	* @return bool
	*/
	function SetLastUserID($lastUserID, $dbupdate = true)
	{
		return $this->UpdateValue('lastUserID', $lastUserID, $dbupdate);
	}
}
?>