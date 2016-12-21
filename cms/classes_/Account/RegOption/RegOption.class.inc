<?php
/**
 * Enter description here...
 *
 * @package Account
 */
class RegOption extends Record
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_value;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID)
	{
		parent::__construct ( $ID, 'registration_option' );
		
		$this->Read();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	function GetValue()
	{
		return $this->m_value;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $value
	 */
	function SetValue($value)
	{
		return $this->UpdateValue('value', $value);
	}
}
?>