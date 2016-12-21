<?php

Site::IncludeFile('classes/Common/TreeViewNavigation.class.inc');

class ContentToCalendarList extends TreeViewNavigation
{
	/**
	 * 
	 * @param  date $date
	 * @param  int $calendarID
	 * @param  string $linkIDName
	 * @param  int $limit
	 * @param  int $page
	 * @param  string $linkName
	 * @param  int $currID
	 * @param  array $exParmas
	 * @param  array $tableNames
	 */
	public function __construct($date, $calendarID = 1, $linkIDName = null, $limit = null, $page = null, $linkName = 'name', $currID = null, $exParmas = null, $tableNames = null)
	{
		parent::__construct ( null, $linkIDName, $limit, $page, $linkName, $currID, $exParmas, $tableNames);
	
		$sql = "SELECT `cd`.`ID` as `ID`, `c`.`ID` as `dataID`, `c`.`name_{$this->m_displayLang}` as `title`, `c`.`purpose`, `cd`.`startDate`, `cd`.`lastDate`
				FROM `content` as `c`  INNER JOIN `calendar_data` as `cd` ON `c`.`ID` = `cd`.`dataID`
				WHERE `cd`.`startDate` <= '$date' AND `cd`.`lastDate` >= '$date' AND `cd`.`dataTypeID` = 1 AND `cd`.`calendarID` = $calendarID";
		
		$this->SetQuery($sql);
	}
}

?>