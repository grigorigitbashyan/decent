<?php
/**
 * Enter class description
 */
class Subscriber extends NamedRecord
{
	/**
	 * Description
	 *
	 * @var char
	 */
	protected $m_code;
	
	/**
	 * Description
	 *
	 * @var char
	 */
	protected $m_email;
	
	/**
	 * Description
	 *
	 * @var char
	 */
	protected $m_statistic;
	
	/**
	 * Description
	 *
	 * @var char
	 */
	protected $m_additional;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null, $IDName = 'ID')
	{
		parent::__construct ( $ID, 'subscriber', $IDName );
		
		$this->Read ();
		
		$this->m_IDFiled = 'ID';
	}
	/**
	 * Insert Subscriber 
	 * @return bool
	 */
	function Insert($properties)
	{
		// delete from content DB
		return $this->InsertRecord ( $properties );
	}
	
	/**
	 * Delete Subscriber 
	 * @return bool
	 */
	function Delete()
	{
		// delete from content DB
		return $this->DeleteRecord ();
	}
	
	/**
	 * Set value of m_code
	 *
	 * @param char $code
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetCode($code, $dbupdate = true)
	{
		return $this->UpdateValue ( 'code', $code, $dbupdate );
	}
	
	/**
	 * Set value of m_email
	 *
	 * @param char $email
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetEmail($email, $dbupdate = true)
	{
		return $this->UpdateValue ( 'email', $email, $dbupdate );
	}
	
	/**
	 * Set value of m_statistic
	 *
	 * @param char $statistic
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetStatistic($statistic, $dbupdate = true)
	{
		return $this->UpdateValue ( 'statistic', $statistic, $dbupdate );
	}
	
	/**
	 * Set value of m_additional
	 *
	 * @param char $additional
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetAdditional($additional, $dbupdate = true)
	{
		return $this->UpdateValue ( 'additional', $additional, $dbupdate );
	}
	
	/**
	 * Return value of m_code
	 *
	 * @return char
	 */
	function GetCode()
	{
		return $this->m_code;
	}
	
	/**
	 * Return value of m_email
	 *
	 * @return char
	 */
	function GetEmail()
	{
		return $this->m_email;
	}
	
	/**
	 * Return value of m_statistic
	 *
	 * @return char
	 */
	function GetStatistic()
	{
		return $this->m_statistic;
	}
	
	/**
	 * Return value of m_additional
	 *
	 * @return char
	 */
	function GetAdditional()
	{
		return $this->m_additional;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param unknown_type $email
	 * @return bool true if email exist
	 */
	function CheckEmail($email)
	{
		$email = trim($email);
		$sql = "SELECT count(*) FROM `subscriber` WHERE `email`='$email'";
		return DB::QueryToValue($sql);
	}

}
?>