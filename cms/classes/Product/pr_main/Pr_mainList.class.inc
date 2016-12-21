<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeViewNavigation.class.inc' );

/**
 * Enter class description
 * @package Item
 */
class Pr_mainList extends TreeViewNavigation
{
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_productType;
	
	/**
	 * Enter description here...
	 *
	 * @param int $productType
	 * @param int $limits
	 */
	function __construct($productType = null, $limit = 20, $sortBy = 'date', $status = null, $field = null)
	{
		parent::__construct ( null, 'pr_mainID', $limit, null, 'name', null, array('sortby' => "$sortBy", 'sorttype' => ' DESC') );
		
		if ($productType)
		{
			switch ( $status )
			{
				case '0': $statusStr = " and PM.`status` = 0"; break;
				case '1': $statusStr = " and PM.`status` = 1"; break;
				case '2': $statusStr = " and PM.`status` = 2"; break;
				case '3': $statusStr = " and PM.`status` = 3"; break;
				case '4': $statusStr = " and PM.`status` = 4"; break;
				default: $statusStr = "";
			}
			
			$fieldStr = "";
			$fieldJoinStr = "";
			if ( $field!="all" && $field!=null )
			{
				$fieldJoinStr = 'JOIN `pr_char` PC on (PC.`productID` = PM.`ID`) ';
				if ( $field==0 )
				{
					$fieldStr = ' and PC.`fieldID` = 18 and PC.`value` >= '.date("Y-m-d");
				}
				else 
				{
					$fieldStr = ' and PC.`fieldID` = 18 and PC.`value` < '.date("Y-m-d");
				}
			}
			
			$userStr = "";
			if ( CMS::GetUser()->GetUserTypeID()>10 )
			{
				$userStr = " and PM.`userID` = ".CMS::GetUser()->GetID();
			}
			
			$this->m_productType = $productType;
			// red from xml data formt
			$sql = "
				SELECT PM.`ID`, PM.`name_{$this->m_displayLang}` as `name`, PM.`short_desc_{$this->m_displayLang}` as `desc`, PM.`picture`, PM.`date` 
				FROM `pr_main` PM 
				$fieldJoinStr
				WHERE PM.`typeID` = $this->m_productType".$statusStr.$userStr.$fieldStr;
					  
			$this->SetQuery($sql, $limit);
		}
	}
}
?>