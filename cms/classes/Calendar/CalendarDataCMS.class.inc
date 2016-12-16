<?php
Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );
//TODO corrent the include
Site::IncludeFile ( 'classes/Calendar/CalendarData.class.inc' );

/**
 * Enter class description
 */
class CalendarDataCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var CalendarData
	 */
	protected $m_calendar_data;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_calendar_data = new CalendarData ( $ID );
		
		$this->ReadNameStrings ( 'classes/Calendar/langs/', 'calendar_data.xml' );
	}
	/**
	 * Return sql statement to create table 
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$str = "CREATE TABLE `calendar_data` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `calendarID` tinyint(3) unsigned NOT NULL,
  `dataTypeID` tinyint(3) unsigned NOT NULL,
  `dataID` int(11) NOT NULL,
  `startDate` date NOT NULL,
  `lastDate` date DEFAULT NULL,
  PRIMARY KEY (`ID`)
)DEFAULT CHARSET=utf8;";
		return $str;
	}
	
	/**
	 * All actions must be done hear
	 *
	 * @return bool
	 */
	function DoAction()
	{
		$ar = null;
		
		switch ($this->GetFormAction ())
		{
			case 'changeCalendarID' :
				$value = DB::POST ( 'CalendarID' );
				$ar = $this->m_calendar_data->SetCalendarID ( $value );
				break;
			case 'changeDataTypeID' :
				$value = DB::POST ( 'DataTypeID' );
				$ar = $this->m_calendar_data->SetDataTypeID ( $value );
				break;
			case 'changeCurrDate' :
				$value = DB::POST ( 'CurrDate' );
				$ar = $this->m_calendar_data->SetCurrDate ( $value );
				break;
			case 'changeLastDate' :
				$value = DB::POST ( 'LastDate' );
				$ar = $this->m_calendar_data->SetLastDate ( $value );
				break;
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
	
	}
	
	/**
	 * Module features must be displayed hear
	 *
	 */
	function DisplayFeatures()
	{
		if (! $this->m_calendar_data->GetID ())
		{return false;}
		$calendar_dataID = $this->m_calendar_data->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		$optionsS = $this->GetNameString ( 'options' );
		
		$CalendarIDS = 'Tab CalendarID'; //$this->GetNameString('CalendarID');
		$DataTypeIDS = 'Tab DataTypeID'; //$this->GetNameString('DataTypeID');
		$CurrDateS = 'Tab CurrDate'; //$this->GetNameString('CurrDate');
		$LastDateS = 'Tab LastDate'; //$this->GetNameString('LastDate');
		

		// values
		$CalendarIDV = $this->m_calendar_data->GetCalendarID ();
		$DataTypeIDV = $this->m_calendar_data->GetDataTypeID ();
		$CurrDateV = $this->m_calendar_data->GetCurrDate ();
		$LastDateV = $this->m_calendar_data->GetLastDate ();
		
		//Display title
		$name = $this->m_calendar_data->GetName ();
		$this->DisplayTitle ( $name, null, false );
		//////////////////////////////////////
		$panelIndex = 0;
		
		print ("<div class='someGTitleBox'>$optionsS</div>") ;
		print ('<div class="someGBox">') ;
		
		//Forms
		

		$reading = $this->IsRecReadable ( $this->m_calendar_data, 'calendarID' );
		$updating = $this->IsRecUpdatable ( $this->m_calendar_data, 'calendarID' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $CalendarIDS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeCalendarID' );
				$this->DisplayHidden ( 'calendar_dataID', $calendar_dataID );
				print ("<input type='text' value='$CalendarIDV' name='CalendarID' id='CalendarID' size='40' maxlength='32' />\n") ;
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ($CalendarIDV) ;
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		$reading = $this->IsRecReadable ( $this->m_calendar_data, 'dataTypeID' );
		$updating = $this->IsRecUpdatable ( $this->m_calendar_data, 'dataTypeID' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $DataTypeIDS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeDataTypeID' );
				$this->DisplayHidden ( 'calendar_dataID', $calendar_dataID );
				print ("<input type='text' value='$DataTypeIDV' name='DataTypeID' id='DataTypeID' size='40' maxlength='32' />\n") ;
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ($DataTypeIDV) ;
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		$reading = $this->IsRecReadable ( $this->m_calendar_data, 'currDate' );
		$updating = $this->IsRecUpdatable ( $this->m_calendar_data, 'currDate' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $CurrDateS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeCurrDate' );
				$this->DisplayHidden ( 'calendar_dataID', $calendar_dataID );
				print ("<input type='text' value='$CurrDateV' name='CurrDate' id='CurrDate' size='40' maxlength='32' />\n") ;
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ($CurrDateV) ;
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		$reading = $this->IsRecReadable ( $this->m_calendar_data, 'lastDate' );
		$updating = $this->IsRecUpdatable ( $this->m_calendar_data, 'lastDate' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $LastDateS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeLastDate' );
				$this->DisplayHidden ( 'calendar_dataID', $calendar_dataID );
				print ("<input type='text' value='$LastDateV' name='LastDate' id='LastDate' size='40' maxlength='32' />\n") ;
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ($LastDateV) ;
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		print ('</div>') ;
		
		// display javascript
		print ('<script type="text/javascript"><!--') ;
		
		for($index = 0; $index <= $panelIndex; $index ++)
			print ("var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n") ;
		print ('//--></script>') ;
		
		return true;
	}
}
?>