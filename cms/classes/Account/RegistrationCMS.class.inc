<?php
Site::IncludeFile ( 'classes/Common/MergingModules.class.inc' );

Site::IncludeFile ( 'classes/Account/SiteUser/SiteUserListCMS.class.inc' );
Site::IncludeFile ( 'classes/Account/RegOption/RegOptionListCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package Account
 */
class RegistrationCMS extends MergingModules
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ( 'regmodule' );
		
		$this->ReadNameStrings ( 'classes/Account/langs/', 'siteuser.xml' );
		
		$siteUseS = $this->GetNameString ( 'siteUser' );
		$regOptS = $this->GetNameString ( 'regOption' );
		
		$this->AddModule ( new SiteUserListCMS ( ), $siteUseS );
		
		$this->AddModule ( new RegOptionListCMS ( ), $regOptS );
	}
}
?>