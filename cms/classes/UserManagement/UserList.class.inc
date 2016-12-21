<?php

/**
 * Enter description here...
 *
 * @package CMSUser
 */
class UserList extends TreeView 
{
	function __construct($privilegy = 2)
	{
		$sql = "SELECT `ID`, `username`, `name`, `last_log` FROM `user` WHERE `usertypeID` = $privilegy";
		parent::__construct($sql, 'userID' , 'name');
	}
}
?>