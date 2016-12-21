<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * This class is used to get access incfo for yser type.
 */
class ModuleAccessList extends TreeView
{
	/**
	 * construct of class
	*/
	function __construct($userTypeID)
	{
		parent::__construct ( null, 'module_accessID');

		$sql = "SELECT * FROM `module_access` WHERE `userTypeID` = $userTypeID";
		$this->SetQueryString($sql);
	}
}
?>