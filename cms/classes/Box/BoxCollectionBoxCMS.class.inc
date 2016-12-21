<?php
class BoxCollectionBoxCMS extends CMSObject
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
		$sql = "CREATE TABLE `boxcollection_box` (
				  `ID` smallint(6) NOT NULL auto_increment,
				  `boxcollectionID` int(11) NOT NULL default '0',
				  `boxID` int(11) NOT NULL default '0',
				  `position` int(11) NOT NULL default '0',
				  PRIMARY KEY  (`ID`),
				  UNIQUE KEY `boxcollectionID` (`boxcollectionID`,`boxID`,`position`)
				)";
		
		return $sql;
	}
}
?>