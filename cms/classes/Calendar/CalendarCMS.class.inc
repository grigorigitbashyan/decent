<?php
Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );
//TODO corrent the include
Site::IncludeFile ( 'classes/Calendar/Calendar.class.inc' );
Site::IncludeFile ( 'classes/Calendar/Extantions/CalendarExtantionCMS.class.inc' );

Site::IncludeFile ( 'classes/Calendar/CalendarDataCMS.class.inc' );
/**
 * Enter class description
 */
class CalendarCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var Calendar
	 */
	protected $m_calendar;
	
	//
	protected $m_extantions;
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_calendar = new Calendar ( $ID );
		
		$this->ReadNameStrings ( 'classes/Calendar/langs/', 'calendar.xml' );
		
		// read extantions
		

		$xml = simplexml_load_file ( 'config/calendar.xml' );
		
		/* Search for <a><b><c> */
		$results = $xml->xpath ( 'objClasses/objClass' );
		
		$this->m_extantions = array ();
		foreach ( $results as $item )
		{
			
			// create object
			$dirPath = ( string ) $item ['dirPath'];
			$className = ( string ) $item;
			$className .= 'CMS';
			
			Site::IncludeFile ( "classes/Calendar/{$dirPath}{$className}.class.inc" );
			
			$obj = new $className ( $ID );
			array_push ( $this->m_extantions, $obj );
		}
	}
	/**
	 * Return sql statement to create table 
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$str = "CREATE TABLE `calendar` (
  `ID` smallint(6) NOT NULL auto_increment,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8";
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
			case 'changeName' :
				$value = DB::POST ( 'Name' );
				$ar = $this->m_calendar->SetName ( $value );
				break;
			case 'delete':
				$this->m_calendar->Delete();
				break;
		}
		
		// extentaion actions
		foreach ( $this->m_extantions as $CMSItem )
		{
			$CMSItem->DoAction ();
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
		if (! $this->m_calendar->GetID ())
		{return false;}
		
		// if current data has bees set then display slected data features
		if (DB::REQUEST ( 'currDate' ) || DB::REQUEST ( 'subcurrDate' ))
		{
			foreach ( $this->m_extantions as $CMSItem )
			{
				$CMSItem->DisplayFeatures ();
			}
			return true;
		}
		$calendarID = $this->m_calendar->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		$deleteS = $this->GetNameString ( 'delete' );
		$optionsS = $this->GetNameString ( 'options' );
		
		$NameS = $this->GetNameString('rename');
		

		// values
		$NameV = $this->m_calendar->GetName ();
		
		//Display title
		$name = $this->m_calendar->GetName ();
		
		$form = null;
		if($this->IsDeletable($this->m_calendar->GetTableName()))
		{
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$actionName =$this->GetFormActionName();
			$form = "<div  class='titleColR'>
					 <form method='post' class='clearPD'> 
					 <input type='hidden' name='$actionName' value='delete' />
					 <input type='hidden' name='calendarID' value='{$this->m_calendar->GetID()}' />
					 <input type='submit' onclick='return confirm(\"$sureDeleteS\");' value='$deleteS' />
					 </form>
					 </div>";
		}	
		$this->DisplayTitle ( $name, $form, false );
		//////////////////////////////////////
		$panelIndex = 0;
		
		print ("<div class='someGTitleBox'>$optionsS</div>") ;
		print ('<div class="someGBox">') ;
		
		//Forms
		$reading = $this->IsRecReadable ( $this->m_calendar, 'name' );
		$updating = $this->IsRecUpdatable ( $this->m_calendar, 'name' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $NameS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeName' );
				$this->DisplayHidden ( 'calendarID', $calendarID );
				print ("<input type='text' value='$NameV' name='Name' id='Name' size='40' maxlength='32' />\n") ;
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ($NameV) ;
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
	
	function SelfTest()
	{
		$res = parent::SelfTest ();
		
		$calData = new CalendarDataCMS ( );
		$res &= $calData->SelfTest ();
		
		return $res;
	}
}
?>