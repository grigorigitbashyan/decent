<?php
Site::IncludeFile('classes/Common/TreeViewNavigation.class.inc');

/**
 * Enter description here...
 *
 * @package News
 */
class NewsList extends TreeViewNavigation 
{
	function __construct()
	{
		// call parent construct
		parent::__construct ( null, 'newsID', 20 );

		// sql query string
		$sql = "SELECT * FROM `news`";	
		
		$this->SetQuery($sql, 20);
	}
}
?>