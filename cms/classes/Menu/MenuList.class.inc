<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * Enter description here...
 *
 * @package Menu
 */
class MenuList extends TreeView
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		$sql = 'SELECT * FROM `menu`';
		parent::__construct ( $sql, 'menuID', 'name' );
	}
	
	/**
	 * Return all menus (with out 0 properties)
	 *
	 * @return array
	 */
	function GetMenus()
	{
		return DB::QueryToArray ( 'SELECT * FROM `menu`' );
	}
}
?>