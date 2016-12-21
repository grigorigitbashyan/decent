<?php
Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );
//TODO corrent the include
Site::IncludeFile ( 'classes/Gallery/Pattern/GalleryPattern.class.inc' );

/**
 * Enter class description
 */
class GalleryPatternCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var GalleryPattern
	 */
	protected $m_gallery_pattern;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_gallery_pattern = new GalleryPattern ( $ID );
		
		$this->ReadNameStrings ( 'classes/Gallery/langs/', 'gallery_pattern.xml' );
	}
	/**
	 * Return sql statement to create table 
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$langs = $this->m_site->GetLanguages ();
		$fields = '';
		
		foreach ( $langs as $lang )
		{
			$fields .= "`name_{$lang}` varchar(255) NOT NULL,
						`header_{$lang}` text,
  						`footer_{$lang}` text,";
		}
		
		$str = "CREATE TABLE `gallery_pattern` (
				  `ID` smallint(6) NOT NULL auto_increment,
				  $fields
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
			case 'changeHeader' :
				$value = DB::POST ( 'Header' );
				$ar = $this->m_gallery_pattern->SetHeader ( $value, $this->m_displayLang );
				break;
			case 'changePattern' :
				$value = DB::POST ( 'Pattern' );
				$ar = $this->m_gallery_pattern->SetPattern( $value, $this->m_displayLang );
				break;
			case 'changeFooter' :
				$value = DB::POST ( 'Footer' );
				$ar = $this->m_gallery_pattern->SetFooter ( $value, $this->m_displayLang );
				break;
			case 'changeName' :
				$value = DB::POST ( 'Name' );
				$ar = $this->m_gallery_pattern->SetName ( $value, $this->m_displayLang );
				break;
			case 'delete' :
				$this->m_gallery_pattern->Delete ();
				break;
		}
		
		if($ar)
		{
			$this->m_gallery_pattern = new GalleryPattern($this->m_gallery_pattern->GetID());
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
		if (! $this->m_gallery_pattern->GetID ())
		{
			return false;
		}
		$gallery_patternID = $this->m_gallery_pattern->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		$optionsS = $this->GetNameString ( 'options' );
		
		$HeaderS = 'Tab Header'; //$this->GetNameString('Header');
		$FooterS = 'Tab Footer'; //$this->GetNameString('Footer');
		$NameS = 'Tab Name'; //$this->GetNameString('Name');
		$PatternS = 'Tab Pattern'; //$this->GetNameString('Pattern');

		// values
		$HeaderV = $this->m_gallery_pattern->GetHeader ( $this->m_displayLang );
		$FooterV = $this->m_gallery_pattern->GetFooter ( $this->m_displayLang );
		$NameV = $this->m_gallery_pattern->GetName ( $this->m_displayLang );
		$PatternV = $this->m_gallery_pattern->GetPattern( $this->m_displayLang );
		
		//Display title
		$name = $this->m_gallery_pattern->GetName ( $this->m_displayLang );
		$form = null;
		if ($this->IsDeletable ( $this->m_gallery_pattern->GetTableName () ))
		{
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$deleteS = $this->GetNameString ( 'delete' );
			$actionName = $this->GetFormActionName ();
			$form = "<div class='titleColR'>
					 <form method='post' class='clearPD'> 
					 <input type='hidden' name='$actionName' value='delete' />
					 <input type='hidden' name='calendarID' value='{$this->m_gallery_pattern->GetID()}' />
					 <input type='submit' onclick='return confirm(\"$sureDeleteS\");' value='$deleteS' />
					 </form>
					 </div>";
		}
		$this->DisplayTitle ( $name, $form, false );
		//////////////////////////////////////
		$panelIndex = 0;
		
		print ( "<div class='someGTitleBox'>$optionsS</div>" );
		print ( '<div class="someGBox">' );
		
		//Forms
		$reading = $this->IsRecReadable ( $this->m_gallery_pattern, 'name' );
		$updating = $this->IsRecUpdatable ( $this->m_gallery_pattern, 'name' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $NameS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeName' );
				$this->DisplayHidden ( 'gallery_patternID', $gallery_patternID );
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
		
		$reading = $this->IsRecReadable ( $this->m_gallery_pattern, 'header' );
		$updating = $this->IsRecUpdatable ( $this->m_gallery_pattern, 'header' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $HeaderS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeHeader' );
				$this->DisplayHidden ( 'gallery_patternID', $gallery_patternID );
				print ( "<textarea name='Header' id='Header' cols='80' rows='7'>$HeaderV</textarea><br />\n" );
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ( $HeaderV );
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		
		$reading = $this->IsReadable ( 'pattern' );
		$updating = $this->IsUpdatable ( 'pattern' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $PatternS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changePattern' );
				$this->DisplayHidden ( 'gallery_patternID', $gallery_patternID );
				print ( "<textarea name='Pattern' id='Pattern' cols='80' rows='7'>$PatternV</textarea><br />\n" );
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ( $HeaderV );
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		$reading = $this->IsRecReadable ( $this->m_gallery_pattern, 'footer' );
		$updating = $this->IsRecUpdatable ( $this->m_gallery_pattern, 'footer' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $FooterS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeFooter' );
				$this->DisplayHidden ( 'gallery_patternID', $gallery_patternID );
				print ( "<textarea name='Footer' id='Footer' cols='80' rows='7'>$FooterV</textarea><br />\n" );
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ( $FooterV );
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
		
		return true;
	}
}
?>