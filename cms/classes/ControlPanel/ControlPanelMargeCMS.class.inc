<?php
Site::IncludeFile ( 'classes/Common/MergingModules.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserManagementCMS.class.inc' );

class ControlPanelMargeCMS extends MergingModules
{
	function __construct()
	{
		parent::__construct ( 'controlmain' );
		
		$this->ReadNameStrings ( 'classes/ControlPanel/langs/', 'controlPanel.xml' );
		$cmsUsersS = $this->GetNameString ( 'cmsUsers' );
//		$siteOptionsS = $this->GetNameString('siteOptions');
		

		$this->AddModule ( new UserManagementCMS ( ), $cmsUsersS );
//		$this->AddModule(new RegOptionListCMS(), $siteOptionsS);
	}
}
?>