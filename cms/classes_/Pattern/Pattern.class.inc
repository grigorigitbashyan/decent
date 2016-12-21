<?php
/**
 * This class is used to read and write patterns to database.
 *
 */
class Pattern
{
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected  $m_ID;
	
	/**
	 * The $ID and $langmust be unice (together).
	 *
	 * @param string $ID 
	 * @param string $lang
	 */
	function __construct($ID)
	{
		$this->m_ID = $ID;
	}
	
	/**
	 * Return ID
	 *
	 * @return string
	 */
	function GetID()
	{
		return $this->m_ID;
	}
	/**
	 * Enter description here...
	 *
	 * @param strun $lang
	 * @return unknown
	 */
	function GetPattern($lang = null)
	{
		//  check pattern record exist
		$sql = "SELECT * FROM `pattern` WHERE `ID` = '{$this->m_ID}' AND `lang` = ";
		$sql .= ($lang) ? "'$lang'": '0';
		
		// get pattern from database
		$pattRec = DB::QueryToRow($sql);
		if($pattRec)
		{
			if(strlen($pattRec['value']))
			{
				return $pattRec['value'];
			}	
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $pattern
	 * @param string $lang
	 * @return bool
	 */
	function SetPattern($pattern, $lang = null)
	{
		//  check pattern record exist
		$sql = "SELECT * FROM `pattern` WHERE `ID` = '{$this->m_ID}' AND `lang` = ";
		$sql .= ($lang) ? "'$lang'" : '0';

		$pattRec = DB::QueryToRow($sql);
		$sql = '';
		if($pattRec)
		{
			$sql = "UPDATE `pattern` SET `value` = '{$pattern}' WHERE `ID` = '{$this->m_ID}' AND `lang` = ";
			$sql .= ($lang) ? "'$lang'": '0';
		}
		else 
		{
			$sql = "INSERT INTO `pattern` (`ID`, `lang`, `value`) VALUES ('{$this->m_ID}', '$lang', '{$pattern}')";
		}
		
		return DB::Query($sql);
	}
}
?>