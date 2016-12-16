<?php
Site::IncludeFile ( 'classes/Common/ContentCaching.class.inc' );
// includes
Site::IncludeFile ( 'classes/Category/Category.class.inc' );
Site::IncludeFile ( 'classes/Content/Content.class.inc' );

Site::IncludeFile ( 'classes/UserManagement/UserManagementCMS.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserStatisticsCMS.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserTypeListCMS.class.inc' );

Site::IncludeFile('classes/Common/ContentCaching.class.inc');
Site::IncludeFile('classes/Common/ValueCMS.class.inc');
Site::IncludeFile('classes/Folder/FolderXP.class.inc');
/**
 * Enter description here...
 *
 * @package Main
 */
class Statistics extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_subPanel;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_userStatus;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		// geting current sub category of control panel
		$this->m_subPanel = $this->m_site->GetValue ( 'subpanel' );
		
		// get current user
		$user = UserManagement::GetCurrentUser ();
		
		if ($user)
		{
			$this->m_userStatus = $user->GetUserTypeID ();
		}
		
		// read name strings
		$this->ReadNameStrings ( 'classes/Common/langs/', 'controlPanel.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return unknown
	 */
	function DoAction()
	{
		$userCMS = new UserStatisticsCMS();
		$res = $userCMS->DoAction ();
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
		// user box
		if (DB::GET_POST ( 'userID' ) || DB::GET_POST ( 'usertypestat' ))
		{
			$this->m_subPanel = '';
		}
		
		// read names
		$userStatsS = $this->GetNameString ( 'userStats' );
		
		// user stats
		print ( "<div class='someWTitleBox'>$userStatsS</div>" );
		print ( '<div class="someWBox">' );
		$userStatsCMS = new UserStatisticsCMS ( );
		$userStatsCMS->DisplayMenu ();
		print ( '</div>' );
	}
	
	/**
	 * This function displays left box links.
	 * 
	 * @param string $name
	 * @param string $link
	 */
	private function printMenuItem($name, $link)
	{
		print ( '<div class="directoryMenu0">' );
		if ($link == $this->m_subPanel)
		{
			print ( $name );
		}
		else
		{
			print ( "<a href='?subpanel=$link'>$name</a>" );
		}
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// user stats
		$userCMS = new UserStatisticsCMS( );
		$userCMS->DisplayFeatures ();
		
		return;
	}
}
?>