<?php
/**
 * This is a Product to calendar (CMS) class.
 *
 */
class ProductToCalendarCMS extends CalendarExtantionCMS 
{
	/**
	 * Construct
	 *
	 * @param int $calendarID
	 */
	function __construct($calendarID = 1)
	{
		parent::__construct($calendarID);
	}
	
	/**
	 * @see CMSObject::DisplayFeatures();
	 *
	 */
	function DisplayFeatures()
	{
		parent::DisplayFeatures();
		print('Hello From ProductToCalendarCMS!<br />');
	}
	
	function GetRelatedItems($date)
	{
		
	}
}
?>