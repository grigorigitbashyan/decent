<?php
Site::IncludeFile ( 'classes/Common/CMSListObject.class.inc' );
//TODO corrent the include
Site::IncludeFile ( 'classes/Calendar/CalendarCMS.class.inc' );
Site::IncludeFile ( 'classes/Calendar/CalendarList.class.inc' );

/**
 * Enter class description
 */
class CalendarListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct ( new CalendarList ( ), 'Calendar' );
		$this->ReadNameStrings ( 'classes/Calendar/langs/', 'calendar.xml' );
	}
	/**
	 * All actions must be done hear
	 *
	 * @return bool
	 */
	function DoAction()
	{
		return parent::DoAction ( true, true );
	}
	
	/**
	 * Display menu
	 *
	 */
	function DisplayMenu()
	{
		$moduleNameS = 'Calendar'; //$this->GetNameString ( 'moduleName' );
		parent::DisplayMenu ( $moduleNameS );
		
		// display selected calendar
		$calendarID = $this->m_list->GetCurrentID ();
		$currDate = DB::REQUEST ( 'currDate' );
		if (! $currDate)
		{
			$currDate = DB::REQUEST ( 'subcurrDate' );
			if (! $currDate)
			{
				$currDate = date ( 'Y-m-d' );
			}
		}
		
		if ($calendarID)
		{
			$name = 'Event';
			print ("<br /><div class='someWTitleBox'>$name</div>") ;
			print ('<div class="someWBox">') ;
			print ('<div id="cont"></div>') ;
			?>
<script type="text/javascript">//<![CDATA[
      function updateFields(cal) {
              var date = cal.selection.get();
              date = Calendar.intToDate(date);
              dateS = Calendar.printDate(date, "%Y-%m-%d");
              
              window.location.href = "?subcalendarID=<?php echo $calendarID?>&currDate=" + dateS ;
      };
      Calendar.setup({
              cont         : "cont",
              onSelect     : updateFields,
              onTimeChange : updateFields,
              date		   : "<?php echo $currDate?>"
      });
    //]]></script>

<?php
			print ('</div>') ;
		}
	}
	
	/**
	 * Display features
	 *
	 */
	function DisplayFeatures()
	{
		// display Calendar
		if (! parent::DisplayFeatures ())
		{
			// get language strings
			$doneS = $this->GetNameString ( 'done' );
			
			// display title
			$this->DisplayTitle ( 'New Calendar' );
			
			$noemptyS = $this->GetNameString('noempty');
			$insertJS = "onsubmit='return SingleForm(\"name\", \"$noemptyS\")'";
			
			// display for for new Calendar
			$this->DisplaySingleTextForm ( 'insert', null, 'name', null, null, $doneS, $insertJS );
		}
	}
}
?>