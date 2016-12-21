<?php
/**
 * Enter description here...
 *
 * @package CMSUser
 */
class UserTypeCMS extends CMSObject
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDBSQL()
	{
		$sql = 'CREATE TABLE `usertype` (
                      `ID` int(2) NOT NULL auto_increment,
                      `name` varchar(64) NOT NULL,
                      `parentID` smallint(6) NOT NULL,
                      PRIMARY KEY  (`ID`)
                    ) DEFAULT CHARSET=utf8;';
		
		return $sql;
	}
}
?>