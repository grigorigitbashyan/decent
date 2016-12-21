<?php
Site::IncludeFile('classes/Common/CMSListObject.class.inc');
//TODO corrent the include
Site::IncludeFile('classes/CalendarData/CalendarDataCMS.class.inc');
Site::IncludeFile('classes/CalendarData/CalendarDataList.class.inc');

/**
 * Enter class description
 */
class CalendarDataListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct (new CalendarDataList(), 'CalendarData');
		$this->ReadNameStrings ( 'classes/CalendarData/langs/', 'CalendarData.xml' );
	}
	/**
	* All actions must be done hear
	*
	* @return bool
	*/
	function DoAction()
	{
		return parent::DoAction(true, true);
	}

	/**
	* Display menu
	*
	*/
	function DisplayMenu()
	{
		$moduleNameS = 'CalendarData';//$this->GetNameString ( 'moduleName' );
		parent::DisplayMenu($moduleNameS);
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		// display CalendarData
		if(!parent::DisplayFeatures())
		{
			// get language strings
			$doneS = $this->GetNameString('done');

			// display title
			$this->DisplayTitle('New CalendarData');

			// display for for new CalendarData
			$this->DisplaySingleTextForm('insert', null, 'name', null, null, $doneS);
		}
	}
}
?>