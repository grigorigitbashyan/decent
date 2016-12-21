<?php
class ActionResult
{
	/**
	 * Enter description here...
	 *
	 * @var mixed
	 */
	protected $m_oldValue;
	/**
	 * Enter description here...
	 *
	 * @var mixed
	 */
	protected $m_newValue;
	/**
	 * 0 means that the action has been completed successfully, otherwise the number is errorCode
	 *
	 * @var int
	 */
	protected $m_status;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_moduleID;
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_actionID;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_recordID;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_fieldID;
	
	
	/**
	 * action type ID
	 * 1 for insert
	 * 2 for update
	 * 3 for delete
	 *
	 * @var int
	 */
	protected $m_actionTypeID;
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		
	}
	
	/**
	 * Enter description here...
	 *
	 * @param unknown_type $currentValue
	 */
	public function SetOldValue($currentValue)
	{
		$this->m_oldValue = $currentValue;		
	}
	
	/**
	 * Enter description here...
	 *
	 * @param mixed $newValue
	 */
	public function SetNewValue($newValue)
	{
		$this->m_newValue = $newValue;		
	}
	/**
	 * Enter description here...
	 *
	 * @param int $recID
	 */
	public function SetRecordID($recID)
	{
		$this->m_recordID = $recID;
	}
	/**
	 * Enter description here...
	 *
	 * @param string $fldID
	 */
	public function SetFieldID($fldID)
	{
		$this->m_fieldID = $fldID;
	}
	/**
	 * Enter description here...
	 *
	 * @param string $modID
	 */
	public function SetModuleID($modID)
	{
		$this->m_moduleID = $modID;
	}	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	public function GetStatus()
	{
		return $this->m_status;
	}	
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	public function GetModuleID()
	{
		return $this->m_moduleID;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $status
	 */
	public function SetStatus($status = 0)
	{
		$this->m_status = $status;
	}
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	public function GetActionID()
	{
		return $this->m_actionID;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	public function GetRecordID()
	{
		return $this->m_recordID;
	}
	/**
	 * Enter description here...
	 *
	 * @return mixed
	 */
	public function GetOldValue()
	{
		return $this->m_oldValue ;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return mixed
	 */
	public function GetNewValue()
	{
		return $this->m_newValue ;
	}
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	public function GetFieldID()
	{
		return $this->m_fieldID;
	}
	
	/**
	 * returns true if old and new values are equal.
	 *
	 * @return bool
	 */
	public function isChanged()
	{
		if(is_string($this->m_oldValue) && is_string($this->m_newValue))
		{
			return strcmp($this->m_oldValue, $this->m_newValue);
		}
		else
		{
			return ($this->m_oldValue ==$this->m_newValue);
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $actionID
	 */
	function SetActionID($actionID)
	{
		$this->m_actionID = $actionID;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	public function GetActionTypeID()
	{
		return $this->m_actionTypeID;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $typeID
	 */
	public function SetActionTypeID($typeID)
	{
		$this->m_actionTypeID = $typeID;
	}
}
?>