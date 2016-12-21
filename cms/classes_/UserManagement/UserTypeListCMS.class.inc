<?php
// includes
Site::IncludeFile ( 'classes/UserManagement/UserTypeCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package CMSUser
 */
class UserTypeListCMS extends CMSObject
{
	function __construct()
	{
		parent::__construct ();
	}
	
	function CreateDB()
	{
		$userType = new UserTypeCMS ( );
		return $userType->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$userType = new UserTypeCMS ( );
		return $userType->SelfTest();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function GetObjectTables()
	{
		$userType = new UserTypeCMS ( );
		return $userType->GetObjectTables();
	}
}
?>