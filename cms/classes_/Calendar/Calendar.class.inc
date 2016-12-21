<?php
/**
 * Enter class description
 */
class Calendar extends NamedRecord
{
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ( $ID, 'calendar' );
		
		$this->Read ();
	}
	/**
	 * Insert Calendar 
	 * @return bool
	 */
	function Insert($properties)
	{
		// delete from content DB
		return $this->InsertRecord ( $properties );
	}
	
	/**
	 * Delete Calendar 
	 * @return bool
	 */
	function Delete()
	{
		// delete data
		$calData = new CalendarDataList();
		$calData->DeleteCalendarData($this->m_ID);
		// delete from content DB
		return $this->DeleteRecord ();
	}

}
?>