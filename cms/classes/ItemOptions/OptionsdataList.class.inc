<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter class description
 * 
 * @package ItemOptions
 */
class OptionsdataList extends TreeView
{
	
	/**
	 * Enter description here...
	 *
	 * @param int $optionsID use positive to gate names and use negative to get keys
	 */
	function __construct($optionsID = null, $order = 'name', $orderType = 'asc')
	{
		parent::__construct ( null, 'optionsdataID', 'name' );
		
		if ($optionsID != null)
		{
			// get vlaues
			if($optionsID > 0)
			{
				$query = "SELECT `ID`, `optionsID`, `name_{$this->m_displayLang}` as `name`, `key` FROM `optionsdata` WHERE `optionsID` = $optionsID";
			}
			// get keys
			else 
			{
				$optionsID *= -1;
				$query = "SELECT `ID`, `optionsID`, `key` as `name` FROM `optionsdata` WHERE `optionsID` = $optionsID";
			}
			
			$query = $query." order by ".$order." ".$orderType;
			
			$this->SetQueryString($query);
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $IDs
	 * @return array
	 */
	function GetOptionsValues($IDs)
	{
		$sql = "SELECT `name_{$this->m_displayLang}` FROM `optionsdata` WHERE `ID` IN (".implode(', ', $IDs).")";
		return DB::QueryToVector($sql);
	}
}
?>