<?php
class SFRelationCMS extends CMSObject
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
		// create structure-content table
		$sql = "CREATE TABLE `structure_file` (
				  `structureID` smallint(5) unsigned NOT NULL default '0',
				  `filename` varchar(255) NOT NULL default '',
				  `show_type` tinyint(1) NOT NULL default '0',
				  UNIQUE KEY `structureID` (`structureID`,`show_type`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>