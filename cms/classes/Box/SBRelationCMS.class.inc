<?php
class SBRelationCMS extends CMSObject
{
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
		$sql = "CREATE TABLE `structure_boxcollection_box` (
				  `structureID` smallint(5) unsigned NOT NULL default '0',
				  `boxcollectionID` smallint(5) unsigned NOT NULL default '0',
				  `align` tinyint(3) unsigned NOT NULL default '0',
				  UNIQUE KEY `structureID` (`structureID`,`boxcollectionID`,`align`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>