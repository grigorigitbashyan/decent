<?php
/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_optionsCMS extends CMSObject 
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = 'CREATE TABLE `pr_options` (
				  `ID` int(10) unsigned NOT NULL auto_increment,
				  `productID` int(10) unsigned NOT NULL,
				  `fieldID` smallint(5) NOT NULL,
				  `relatedID` int(11) default NULL,
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;';
		
		return $sql;
	}
}
?>