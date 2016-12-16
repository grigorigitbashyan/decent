<?php
//TODO corrent the include
Site::IncludeFile ( 'classes/Common/Value.class.inc' );

/**
 * Enter class description
 */
class ValueCMS extends CMSObject
{
	
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct()
	{
		parent::__construct ();
	}
	/**
	 * Return sql statement to create table 
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$str = "CREATE TABLE `value` (
  `ID` varchar(32) NOT NULL,
  `value` varchar(255) NOT NULL,
  UNIQUE KEY `ID` (`ID`)
) DEFAULT CHARSET=utf8;";
		return $str;
	}
	
	/**
	 * All actions must be done hear
	 *
	 * @return bool
	 */
	function DoAction()
	{
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
	
	}
	
	/**
	 * Module features must be displayed hear
	 *
	 */
	function DisplayFeatures()
	{
		return true;
	}
}
?>