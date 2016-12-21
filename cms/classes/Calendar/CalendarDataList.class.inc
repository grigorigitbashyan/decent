<?php
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );
/**
 * Enter class description
 */
class CalendarDataList extends TreeView
{
	protected $m_calendarID;
	
	/**
	 * Enter description here...
	 */
	function __construct($calendarID)
	{
		parent::__construct ( null, 'calendar_dataID' );
		
		$this->m_calendarID = $calendarID;
		
		$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name` FROM `calendar_data` 
				WHERE `calendarID` = $this->m_calendarID";
		$this->SetQueryString ( $sql );
	}
	
	/**
	 * @param array|int $IDs
	 * @return bool
	 */
	function Delete($IDs)
	{
		$res = true;
		if (is_array ( $IDs ))
		{
			foreach ( $IDs as $ID )
			{
				$calData = new CalendarData ( $ID );
				$res &= $calData->Delete ();
			}
		}
		else
		{
			$calData = new CalendarData ( $IDs );
			$res &= $calData->Delete ();
		}
		
		return $res;
	}
	
	/**
	 * This function deletes all data of selected calendar.
	 * 
	 * @param int $calendarID
	 * @return bool
	 */
	function DeleteCalendarData($calendarID)
	{
		$sql = "DELETE FROM `calendar_data` WHERE `calendarID` = $calendarID";
		return DB::Query ( $sql );
	}
	
	/**
	 * @param array|int $IDs
	 * @param date $lastDate
	 * @return null
	 */
	function SetLastDate($IDs, $lastDate)
	{
		if (is_array ( $IDs ))
		{
			foreach ( $IDs as $ID )
			{
				$calData = new CalendarData ( $ID );
				$calData->SetLastDate ( $lastDate );
			}
		}
		else
		{
			$calData = new CalendarData ( $IDs );
			$calData->SetLastDate ( $lastDate );
		}
	}
	
	/**
	 * @param $dataIDs
	 * @param $clendarID
	 * @param $dataTypeID
	 * @param $startDate
	 * @param lastDate
	 * 
	 * @return bool
	 */
	function Add($dataIDs, $clendarID, $dataTypeID, $startDate, $lastDate = null)
	{
		$res = true;
		
		if (! $lastDate)
		{
			$lastDate = $startDate;
		}
		
		$newCalendar = new CalendarData ( );
		
		$properties = array ('calendarID' => $clendarID, 'startDate' => $startDate, 'dataTypeID' => $dataTypeID, 'lastDate' => $lastDate );
		
		if (is_array ( $dataIDs ))
		{
			foreach ( $dataIDs as $dataID )
			{
				$properties ['dataID'] = $dataID;
				$res = $newCalendar->Insert ( $properties );
			}
		}
		else
		{
			$properties ['dataID'] = $dataIDs;
			$res = $newCalendar->Insert ( $properties );
		}
		
		return $res;
	}
	
	function GetMonthData($year, $month, $deteType = 1)
	{
		$res = array ();
		
		/////////////////////
		$deteTypeCond = ($deteType) ? " AND `dataTypeID` = $deteType" : '';
		$lastDay = date ( 't', mktime ( 0, 0, 0, $month, 1, $year ) );
		
		$sql = "SELECT `dataID`, `startDate`, `lastDate` FROM `calendar_data` 
				WHERE `calendarID` = $this->m_calendarID AND
				  ((`startDate` <= '{$year}-{$month}-01'{$deteTypeCond} AND `lastDate` >= '{$year}-{$month}-01') 
				  ||  (`startDate` <= '{$year}-{$month}-{$lastDay}' AND `lastDate` >= '{$year}-{$month}-{$lastDay}') 
				  ||  (`startDate` > '{$year}-{$month}-01' AND `lastDate` < '{$year}-{$month}-{$lastDay}'))";
		
		$results = DB::QueryToArray ( $sql );
		
		if ($results)
		{
			foreach ( $results as $row )
			{
				if ($row ['startDate'] <= "{$year}-{$month}-01")
				{
					$currDate = "{$year}-{$month}-01";
					$day = 1;
					while ( $currDate <= $row ['lastDate'] && ($currDate <= "{$year}-{$month}-{$lastDay}") )
					{
						
						$res [$day] = true;
						
						$day ++;
						$currDate = ($day < 10) ? "{$year}-{$month}-0{$day}" : "{$year}-{$month}-{$day}";
					
					}
				}
				elseif ($row ['lastDate'] >= "{$year}-{$month}-{$lastDay}")
				{
					$startDate = substr ( $row ['startDate'], 8 );
					for($day = $startDate; $day <= $lastDay; $day ++)
					{
						$res [$day] = true;
					}
				}
				else
				{
					$startDate = intval ( substr ( $row ['startDate'], 8 ) );
					$lDay = intval ( substr ( $row ['lastDate'], 8 ) );
					
					for($day = $startDate; $day <= $lDay; $day ++)
					{
						$res [$day] = true;
					}
				}
			}
		}
		
		return $res;
	}
}
?>