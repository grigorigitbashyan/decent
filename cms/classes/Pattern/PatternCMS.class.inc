<?php
Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );

Site::IncludeFile ( 'classes/Pattern/Pattern.class.inc' );

/**
 * Enter class description
 */
class PatternCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var Pattern
	 */
	protected $m_pattern;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_pattern = new Pattern ( $ID );
		
		$this->ReadNameStrings ( 'classes/Pattern/langs/', 'pattern.xml' );
	}
	/**
	 * Return sql statement to create table 
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$str = "CREATE TABLE `pattern` (
		  `ID` varchar(32) character set latin1 collate latin1_bin NOT NULL,
		  `lang` varchar(2) character set latin1 collate latin1_bin NOT NULL default '0',
		  `value` text NOT NULL,
		  UNIQUE KEY `ID` (`ID`,`lang`)
		) ENGINE=MyISAM DEFAULT CHARSET=utf8";
		return $str;
	}
	
	/**
	 * All actions must be done hear
	 *
	 * @param string $lang
	 * @return bool
	 */
	function DoAction($lang = null)
	{
		$ar = null;
		$patternID = $this->m_pattern->GetID();
		
		switch ($this->GetFormAction ())
		{
			case 'changePattern' :
				$value = DB::POST ( "Pattern_{$patternID}" );
				if($value)
				{
					$ar = $this->m_pattern->SetPattern ( $value, $lang );
				}
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
		if (! $this->m_pattern->GetID ())
		{
			return false;
		}
		
		$patternID = $this->m_pattern->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		
		$PatternV = $this->m_pattern->GetPattern ($this->m_displayLang);
		
		$reading = $this->IsReadable ( 'pattern', 'value' );
		$updating = $this->IsUpdatable ( 'pattern',  'value' );
		
		if ($reading || $updating)
		{
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changePattern' );
				$this->DisplayHidden ( 'patternID', $patternID );
				print ( "<textarea name='Pattern_{$patternID}' id='Pattern_{$patternID}' class='mceNoEditor' cols='80' rows='7'>$PatternV</textarea>\n" );
				print('<br />');
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ( $PatternV );
			}
		}
		
		return true;
	}
}
?>