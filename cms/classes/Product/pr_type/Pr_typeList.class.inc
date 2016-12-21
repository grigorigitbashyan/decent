<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter class description
 * 
 * @package Item
 */
class Pr_typeList extends TreeView
{
	
	/*
	 * Enter description ...
	 */
	function __construct()
	{
		$query = "SELECT * FROM `pr_type` order by `order` desc";
		parent::__construct ( $query, 'pr_typeID', 'name' );
	
	}
	
	function GetPrTypeByName($name)
	{
		$query = "select * from `pr_type` where `name` = '".$name."'";
		$retArr = DB::QueryToArray($query);
		
		return $retArr;
	}
}
?>