<?php
/**
 * Enter description here...
 *
 */
class Value extends Record 
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_tryID;
	/**
	 * Enter description here...
	 *
	 * @var mixed
	 */
	protected $m_value;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID)
	{
		parent::__construct($ID, 'value');
		
		$this->m_tryID = $ID;
		$this->Read();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param mixed $value
	 * @return bool
	 */
	function SetValue($value)
	{
		if(is_array($value) || is_object($value))
		{
			$value = serialize($value);	
		}
		
		// update
		if($this->m_ID)
		{
			return $this->UpdateValue('value', $value);
		}
		
		// insert
		$properties = array('ID' => $this->m_tryID, 'value' => $value);
		$this->m_value = $value;
		return $this->InsertRecord($properties);
	}
	
	
	/**
	 * Enter description here...
	 *
	 * @param bool $unserialize use true value for arrays and objects
	 * @return mixed
	 */
	function GetValue($unserialize = false)
	{
		if($unserialize)
		{
			return unserialize($this->m_value);
		}
		
		return $this->m_value;
	}
}
?>