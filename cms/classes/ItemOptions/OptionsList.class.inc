<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter class description
 * 
 * @package ItemOptions
 */
class OptionsList extends TreeView
{
	
	/*
	 * Enter description ...
	 */
	function __construct()
	{
		$query = "SELECT * FROM `options`";
		parent::__construct ( $query, 'optionsID', 'name' );
	
	}

}
?>