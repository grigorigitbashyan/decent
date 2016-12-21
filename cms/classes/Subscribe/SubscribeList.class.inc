<?php
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter description here...
 *
 * @package Notification
 */
class SubscribeList extends TreeView 
{
	function __construct()
	{
		$sql = 'SELECT * FROM `subscribe`';
		parent::__construct($sql, 'SubscribeListID');
	}
}
?>