<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * Enter class description
 */
class CalendarList extends TreeView
{
	/**
	 * Enter description here...
	*/
	function __construct()
	{
		parent::__construct ( null, 'calendarID');

		$sql = "SELECT * FROM `calendar`";
		$this->SetQueryString($sql);
	
	}
}
?>