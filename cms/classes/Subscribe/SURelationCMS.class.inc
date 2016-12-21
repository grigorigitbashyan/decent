<?php
class SURelationCMS extends CMSObject
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
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = 'CREATE TABLE `subscribe_user` (
			  `subscribeID` int(11) NOT NULL,
			  `userID` int(11) NOT NULL,
			  UNIQUE KEY `subscribeID` (`subscribeID`,`userID`)
			) DEFAULT CHARSET=utf8;';
		
		return $sql;
	}
}
?>