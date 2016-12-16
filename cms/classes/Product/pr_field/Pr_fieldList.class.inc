<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter class description
 * @package Item
 */
class Pr_fieldList extends TreeView
{
	
	/*
	 * Enter description ...
	 */
	function __construct()
	{
		$query = "SELECT * FROM `pr_field`";
		parent::__construct ( $query, 'pr_fieldID', 'name' );
	
	}

}
?>