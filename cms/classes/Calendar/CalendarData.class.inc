<?php
/**
 * Enter class description
 */
class CalendarData extends Record
{
	/**
	* Description
	*
	* @var int
	*/
	protected $m_calendarID;

	/**
	* Description
	*
	* @var int
	*/
	protected $m_dataTypeID;

	/**
	 * @var int
	 */
	protected $m_dataID;
	
	/**
	* Description
	*
	* @var date
	*/
	protected $m_startDate;

	/**
	* Description
	*
	* @var date
	*/
	protected $m_lastDate;

	/**
	 * Enter description here...
	 *
	 * @param int $ID
	*/
	function __construct($ID = null)
	{
		parent::__construct($ID, 'calendar_data');
	
		$this->Read();
	}
	/**
	 * Insert CalendarData 
	* @return bool
	*/
	function Insert($properties)
	{
		// delete from content DB
		return $this->InsertRecord($properties);	}

	/**
	 * Delete CalendarData 
	* @return bool
	*/
	function Delete()
	{
		// delete from content DB
		return $this->DeleteRecord();	}

	/**
	 * Set value of m_calendarID
	 *
	 * @param int $calendarID
	 * @param bool $dbupdate
	 * @return int
	*/
	function SetCalendarID($calendarID, $dbupdate = true)
	{
		$this->UpdateValue('calendarID', $calendarID, $dbupdate);
	}

	/**
	 * Set value of m_dataTypeID
	 *
	 * @param int $dataTypeID
	 * @param bool $dbupdate
	 * @return int
	*/
	function SetDataTypeID($dataTypeID, $dbupdate = true)
	{
		$this->UpdateValue('dataTypeID', $dataTypeID, $dbupdate);
	}

	/**
	 * Set value of m_dataID
	 *
	 * @param int $dataID
	 * @param bool $dbupdate
	 * @return int
	*/
	function SetDataID($dataID, $dbupdate = true)
	{
		$this->UpdateValue('dataID', $dataID, $dbupdate);
	}
	
	/**
	 * Set value of m_startDate
	 *
	 * @param date $startDate
	 * @param bool $dbupdate
	 * @return date
	*/
	function SetStartDate($startDate, $dbupdate = true)
	{
		$this->UpdateValue('startDate', $startDate, $dbupdate);
	}

	/**
	 * Set value of m_lastDate
	 *
	 * @param date $lastDate
	 * @param bool $dbupdate
	 * @return date
	*/
	function SetLastDate($lastDate, $dbupdate = true)
	{
		$this->UpdateValue('lastDate', $lastDate, $dbupdate);
	}

	/**
	 * Return value of m_calendarID
	 *
	 * @return int
	*/
	function GetCalendarID()
	{
		return $this->m_calendarID;
	}

	/**
	 * Return value of m_dataTypeID
	 *
	 * @return int
	*/
	function GetDataTypeID()
	{
		return $this->m_dataTypeID;
	}

	/**
	 * Return value of m_dataTypeID
	 *
	 * @return int
	*/
	function GetDataID()
	{
		return $this->m_dataID;
	}
	
	/**
	 * Return value of m_startDate
	 *
	 * @return date
	*/
	function GetStartDate()
	{
		return $this->m_startDate;
	}

	/**
	 * Return value of m_lastDate
	 *
	 * @return date
	*/
	function GetLastDate()
	{
		return $this->m_lastDate;
	}

}
?>