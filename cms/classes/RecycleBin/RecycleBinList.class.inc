<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeViewNavigation.class.inc' );

/**
 * Enter class description
 * 
 * @package RecycleBin
 */
class RecycleBinList extends TreeViewNavigation
{
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_categoryID;
	
	/**
	 * Enter description here...
	 *
	 * @param int $productType
	 * @param int $limits
	 */
	function __construct($categoryID = null, $limit = 20)
	{
		parent::__construct ( null, 'recycleID', $limit);

		$this->m_categoryID = $categoryID;
		$query = "SELECT `ID`, `type`, `name_{$this->m_displayLang}` as `name` FROM `recycle_bin`";
		
		$this->SetQuery($query);
	}
}
?>