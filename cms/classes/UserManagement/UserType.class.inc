<?php
/**
 * Enter description here...
 *
 * @package CMSUser
 */
class UserType extends NamedRecord
{
	protected $m_parentID;
	/**
	 * @desc Constructure
	 * @param int $ID
	 * 
	 * @return UserType
	 */
	function __construct($ID = null)
	{
		parent::__construct ( $ID, 'usertype' );
		$this->Read ();
	}
	
	/**
	 * Add new user type
	 *
	 * @param string $userType /name for new type
	 * @return bool|int
	 */
	function AddUserType($userType)
	{
		$properties ['name'] = trim ( $userType );
		$res = $this->InsertRecord ( $properties );
		
		return $res;
	}
	
	/**
	 * Insert Usertype 
	 * @return bool
	 */
	function Insert($properties)
	{
		// delete from content DB
		return $this->InsertRecord ( $properties );
	}
	
	/**
	 * Delete Usertype 
	 * @return bool
	 */
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		$res = true;
		// get all sub user types
		$sql = "SELECT `ID` FROM `usertype` WHERE `parentID` = {$this->m_ID}";
		$subUsers = DB::QueryToVector($sql);
		
		if($subUsers)
		{
			foreach ($subUsers as $subUser)
			{
				$subUser = new UserType($subUser);
				$res &= $subUser->Delete();
			}
		}

		// delete from content DB
		return $this->DeleteRecord ();
	}
	
	/**
	 * Return parent ID
	 *
	 * @return int
	 */
	function GetParentID()
	{
		return $this->m_parentID;
	}
}
?>