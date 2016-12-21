<?php
Site::IncludeFile('classes/Common/CMSListObject.class.inc');
//TODO corrent the include
Site::IncludeFile('classes/ModuleAccess/ModuleAccessCMS.class.inc');
Site::IncludeFile('classes/ModuleAccess/ModuleAccessList.class.inc');

/**
 * Enter class description
 */
class ModuleAccessListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct (new ModuleAccessList(), 'ModuleAccess');
		$this->ReadNameStrings ( 'classes/ModuleAccess/langs/', 'ModuleAccess.xml' );
	}
	/**
	* All actions must be done hear
	*
	* @return bool
	*/
	function DoAction()
	{
		return parent::DoAction(true, true);
	}

	/**
	* Display menu
	*
	*/
	function DisplayMenu()
	{
		$moduleNameS = 'ModuleAccess';//$this->GetNameString ( 'moduleName' );
		parent::DisplayMenu($moduleNameS);
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		// display ModuleAccess
		if(!parent::DisplayFeatures())
		{
			// get language strings
			$doneS = $this->GetNameString('done');

			// display title
			$this->DisplayTitle('New ModuleAccess');

			// display for for new ModuleAccess
			$this->DisplaySingleTextForm('insert', null, 'name', null, null, $doneS);
		}
	}
}
?>