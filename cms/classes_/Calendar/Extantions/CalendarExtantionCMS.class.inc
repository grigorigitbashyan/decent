<?php
Site::IncludeFile('classes/Calendar/CalendarDataList.class.inc');
/**
 * This class is base class for all Calendar extentions that must be used in CMS.
 *
 */
abstract class CalendarExtantionCMS extends CMSObject
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_calendarID;
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_currentDate;
	
	/**
	 * Construct
	 *
	 * @param int $calendarID
	 */
	function __construct($calendarID = 1)
	{
		parent::__construct();
		
		$this->m_calendarID = $calendarID;
		
		$this->m_currentDate = DB::REQUEST('currDate');
		
		if(!$this->m_currentDate)
		{
			$this->m_currentDate = DB::REQUEST('subcurrDate');
		}
	}
	
	/**
	 * Display calendar title.
	 *
	 */
	function DisplayFeatures()
	{
		static $hasAlreadyDisplayed = false;
		
		if(!$hasAlreadyDisplayed)
		{
			$this->DisplayTitle($this->m_currentDate);
		}
		
		$hasAlreadyDisplayed = true;
	}
	
	/**
	 * @return bool
	 */
	function DoAction()
	{
		$res = true;
		
		$calDataList = new CalendarDataList($this->m_calendarID);
		
		switch ($this->GetFormAction())
		{
			case 'add':
				// add new IDs
				$clendarID = $this->m_calendarID;
				$startDate = $this->m_currentDate;
				$dataIDs = DB::REQUEST('dataIDs');
				
				// retrive dataTypeID
				$dataTypeID = 1;
				
				//
				$calDataList->Add($dataIDs, $clendarID, $dataTypeID, $startDate);
				break;
			case 'relatedAction':
				
				$IDs = DB::POST('dataIDs');

				// delete selected IDs
				if(DB::POST('delete'))
				{
					$calDataList->Delete($IDs);
				}
				
				// set last date for IDs
				if(DB::POST('setLastDate'))
				{
					
					$lastDate = DB::POST('lastDate');
					$calDataList->SetLastDate($IDs, $lastDate);
				}
				break;
		}
		
		return $res;
	}
	
	function DisplayFeatureAddHeader()
	{
		print("<form method=\"POST\">\n");
		print("<input type=\"submit\" name=\"submit\" value=\"Add\"/>\n");
	}
	
	function DisplayFeatureModifyHeader()
	{
		
	}
	
	function DisplayFeatureFooter()
	{
		print("</form>\n");
	}
}
?>