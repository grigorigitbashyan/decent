<?php
Site::IncludeFile('classes/Common/CMSListObject.class.inc');
//TODO corrent the include
Site::IncludeFile('classes/UserManagement/Access/UsertypeAccessCMS.class.inc');
Site::IncludeFile('classes/UserManagement/UserTypeList.class.inc');

/**
 * Enter class description
 */
class UsertypeAccessListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct (new UserTypeList(CMS::GetUser()->GetUserTypeID()), 'UserType', 'UserTypeAccessCMS');
		$this->ReadNameStrings ( 'classes/UserManagement/langs/', 'user.xml' );
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
		$moduleNameS = 'Usertype';//$this->GetNameString ( 'moduleName' );
		
		$styles = array();
		$styles[1] = 'directoryMenu1';
		$styles[2] = 'directoryMenu2';
		$styles[3] = 'directoryMenu3';
		$styles[4] = 'directoryMenu4';
		
		print ( "<div class='someWTitleBox'>$moduleNameS</div>" );
		print ( '<div class="someWBox">' );
		$this->m_list->DisplayTree($styles, null);
		print ( '</div>' );
		print ( '</div>' );
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		$currentID = $this->m_list->GetCurrentID();
		
		// after delete $current ID still not null, so there is a need to check it aggin
		$userType = new UserType($currentID); 
		if($userType->GetID() && !$this->m_list->FindPathInTree($currentID))
		{
			print('Access is denited!');
			return;
		}
		// display Usertype
		if(!parent::DisplayFeatures())
		{
			// get language strings
			$doneS = $this->GetNameString('done');

			// display title
			$this->DisplayTitle('New Usertype');

			// display for for new Usertype
			$this->DisplaySingleTextForm('insert', null, 'name', null, null, $doneS);
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param mixed $arg
	 * @return string
	 */
	function GetAjax($arg)
	{
		$res = $this->m_object->GetAjax($arg);
		return $res;
	}
}
?>