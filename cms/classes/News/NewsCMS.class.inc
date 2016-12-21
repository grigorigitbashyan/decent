<?php
Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );
//TODO corrent the include
Site::IncludeFile ( 'classes/News/News.class.inc' );

/**
 * Enter class description
 */
class NewsCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var News
	 */
	protected $m_news;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_news = new News ( $ID );
		
		$this->ReadNameStrings ( 'classes/News/langs/', 'news.xml' );
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
				$ar = $this->m_news->SetName ( $value );
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
		if (! $this->m_news->GetID ())
		{
			return;
		}
		$newsID = $this->m_news->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		$optionsS = $this->GetNameString ( 'options' );
		
		$NameS = 'Tab Name'; //$this->GetNameString('Name');
		

		// values
		$NameV = $this->m_news->GetName ();
		
		//Display title
		$name = $this->m_news->GetName ();
		$this->DisplayTitle ( $name, null, false );
		//////////////////////////////////////
		$panelIndex = 0;
		
		print ( "<div class='someGTitleBox'>$optionsS</div>" );
		print ( '<div class="someGBox">' );
		
		//Forms
		$updating = true;
		$reading = false;
		
		if (CMSObject::$controller)
		{
			$reading = CMSObject::$controller->IsRecReadable ( $this->m_news, 'name' );
			$updating = CMSObject::$controller->IsRecUpdatable ( $this->m_news, 'name' );
		}
		
		if ($reading || $updating)
		{
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $NameS );
			
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeName' );
				$this->DisplayHidden ( 'newsID', $newsID );
				print ( "<input type='text' value='$NameV' name='Name' id='Name' size='40' maxlength='32' />\n" );
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ( $NameV );
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		print ( '</div>' );
		
		// display javascript
		print ( '<script type="text/javascript"><!--' );
		
		for($index = 0; $index <= $panelIndex; $index ++)
			print ( "var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n" );
		print ( '//--></script>' );
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = 'CREATE TABLE `news` (
				  `ID` int(11) NOT NULL auto_increment,
				  `name` varchar(32) NOT NULL,
				  PRIMARY KEY  (`ID`)
				);';
		return $sql;
	}
}
?>