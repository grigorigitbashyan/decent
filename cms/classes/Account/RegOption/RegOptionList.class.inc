<?php
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter description here...
 *
 * @package Account
 */
class RegOptionList extends TreeView
{
	function __construct()
	{
		$sql = 'SELECT * FROM `registration_option` ORDER BY `ID`';
		
		parent::__construct ( $sql, 'regOptID' );
	}
}
?>