<?php
// includes
Site::IncludeFile ( 'classes/UserManagement/UserManagement.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserType.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserTypeList.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserList.class.inc' );

/**
 * This class provide actions with users(delete, add, change password) and drawing (user form) 
 *
 * @package CMSUser
 */
class UserManagementCMS extends CMSObject implements CMSInterface
{
	function __construct()
	{
		parent::__construct ();
		$this->ReadNameStrings ( 'classes/UserManagement/langs/', 'user.xml' );
	}
	/**
	 * This function doing actions with users, add, delete change password
	 *
	 * @return bool
	 */
	function DoAction()
	{
		$username = DB::POST ( 'username' );
		
		$res = false;
		
		switch ($this->GetFormAction ())
		{
			case 'update' :
				// get user
				$user = new UserManagement ( $username, false );
				
				if ($user->GetUserName () == null)
					return false;
				
				$email = DB::POST ( 'email' );
				$name = DB::POST ( 'name' );
				$publish = DB::POST ( 'publish' );
				
				$res = $user->SetEmail ( $email );
				$res = $user->SetName ( $name );
				if ( $publish )
				{
					$res = $user->SetPublish(1);
				}
				else 
				{
					$res = $user->SetPublish(0);
				}
				
				// get passwords
				$password = $_POST ['password'];
				$cpassword = $_POST ['cpassword'];
				
				$password = trim ( $password );
				$cpassword = trim ( $cpassword );
				// copaer password and confirm password
				if (strcmp ( $password, $cpassword ) != 0)
					return false;
				
				$oldPass = $user->GetPassword ();
				
				if (strcmp ( $oldPass, $password ))
				{
					$user->SetPassword ( $password );
				}
				
				break;
			case 'modifyUsers' :
				// get user
				$usernames = DB::POST ( 'usernames' );
				if ($usernames)
				{
					foreach ( $usernames as $username )
					{
						$user = new UserManagement ( $username, false );
						// check privilegis
						$user->Delete ();
					}
				}
				break;
			case 'insert' :
				$username = DB::POST ( 'username' );
				$password = DB::POST ( 'password' );
				$cpassword = DB::POST ( 'cpassword' );
				$priority = DB::POST ( 'usertype' );
				$name = DB::POST ( 'name' );
				$email = DB::POST ( 'email' );
				
				$password = trim ( $password );
				$cpassword = trim ( $cpassword );
				// copaer password and confirm password
				if (strcmp ( $password, $cpassword ) != 0)
					return false;
				
				$um = new UserManagement ( );
				$res = $um->AddUser ( $username, $password, $email, $priority, $name );
				break;
			case 'delete' :
				$username = DB::POST ( 'username' );
				if ( $username )
				{
					$user = new UserManagement ( $username, false );
					// check privilegis
					$user->Delete ();
				}
				break;
		}
		
		return $res;
	}
	
	function DisplayMenu()
	{
		// display profile
		// display users
		// get corrent user privilegy
		$currentUser = UserManagement::GetCurrentUser ();
		
		// get user type list that great then corrent users privilegy
		$usertypeList = new UserTypeList ( $currentUser->GetUserTypeID () );
		
		$styles = array ();
		$styles [1] = 'directoryMenu1';
		$styles [2] = 'directoryMenu2';
		$styles [3] = 'directoryMenu3';
		$styles [4] = 'directoryMenu4';
		
		$usertypeList->DisplayTree ( $styles, null );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		$usertypeID = DB::GET ( 'usertype' );
		
		// create user list
		$userList = new UserList ( $usertypeID );
		$userNumber = $userList->GetItemCount ();
		$currentID = $userList->GetCurrentID ();
		$user = new UserManagement($currentID);
		$currentID = $user->GetID();
		
		// get user type list that great then corrent users privilegy
		$currentUser = UserManagement::GetCurrentUser ();
		$usertypeList = new UserTypeList ( $currentUser->GetUserTypeID () );
		$res = $usertypeList->FindPathInTree ( $usertypeID );
		
		// aloow user info and co cousers
		$allowUser = (count ( $res ) > 0);
		$allowCouser = (count ( $res ) > 1);
		
		// if user type has been selected then display according firature
		if ($usertypeID && $allowUser)
		{
			// display prifile
			if ($usertypeID == 'profile')
			{
				$this->DisplayProfile ();
			}
			// display new user form
			elseif (! $currentID && $allowCouser)
			{
				$this->DisplayUserForm ( null, $usertypeID );
			}
			// display user edit form
			elseif ($allowCouser)
			{
				$user = new UserManagement ( $currentID );
				$this->DisplayUserForm ( $user->GetUserName () );
			}
			else
			{
				// display profile
				$this->DisplayProfile ();
			}
			
			// display users
			if ($allowCouser && $userNumber)
			{
				// if currrent user type global parent of user to be displayed then dispay
				$allS = $this->GetNameString ( 'all' );
				$noneS = $this->GetNameString ( 'none' );
				$deleteS = $this->GetNameString ( 'delete' );
				$sureDeleteS = $this->GetNameString ( 'sureDelete' );
				$noSelectedS = $this->GetNameString ( 'noselected' );
				
				// javascript to alert
				$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
				
				// display form header
				$this->DisplayFormHeadr ( 'modifyUsers', 'usersForm', 'post', $dJS );
				
				print ( '<div class="titleBox">
            <div class="titleColR"><span style="padding-right:20px"><strong>Select:</strong>' );
				print ( " <a onclick=\"chekeBoxes('usersForm', 'usernames[]', true); return false\" href=\"#\">$allS</a>, <a onclick=\"chekeBoxes('usersForm', 'usernames[]', false); return false\" href=\"#\">$noneS</a>" );
				
				print ( '</span><input onclick="return ChecksForm(\'usersForm\', \'usernames[]\', \'' . $noSelectedS . '\');" type="submit" value="' . $deleteS . '" /></div>
            <br class="clearFloat" />
		</div>
        <div class="someTableArea">' );
				
				$structure = $userList->GetStructure ();
				
				$index = 0;
				foreach ( $structure as $item )
				{
					$styale = (++ $index % 2 == 0) ? 'tableEven' : 'tableUneven';
					print ( "<div class=\"$styale\">" );
					
					$pattern = '<div class="tableCHB"><input name="usernames[]" id="usernames[]" type="checkbox" value="%s" /></div>
                <div class="tableT"><a href="?userID=%d&usertype=%d">%s</a></div>
                <div class="tableD">%s</div>
                <br class="clearFloat" />';
					printf ( $pattern, $item ['username'], $item ['ID'], $usertypeID, $item ['username'], $item ['last_log'] );
					
					print ( '</div>' );
					print ( '<br class="clearFloat" />' );
				}
				
				print ( '</div></form>' );
			}
		}
	}
	/**
	 * Enter description here...
	 *
	 */
	protected function DisplayProfile()
	{
		// get current user
		$currentUser = UserManagement::GetCurrentUser ();
		
		// display form
		$this->DisplayUserForm ( $currentUser->GetUserName () );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	public function DisplayUserForm($username = null, $usertypeID = 2)
	{
		$name = "";
		$email = "";
		
		// if it is not new user then get name and email
		if ($username)
		{
			$user = new UserManagement ( $username, false );
			
			$name = $user->GetName ();
			$email = $user->GetEmail ();
			$publish = $user->GetPublish();
			$uType = $user->GetUserTypeID();
			
			// if it is not current user then create delete form
			
			// get current user name
			$curUser = UserManagement::GetCurrentUser ();
			$profileS = $this->GetNameString ( 'profile' );
			
			if ($curUser->GetUserName () == $username)
			{
				$this->DisplayTitle ( "$profileS '$username'" );
			}
			else
			{
				// get language strings
				$deleteS = $this->GetNameString ( 'delete' );
				$sureS = $this->GetNameString ( 'sureDelete' );
				$formActionName = $this->GetFormActionName ();
				
				$deleteForm = "<div  class='titleColR'>
								<form action='' method='POST' onsubmit='return confirm(\"$sureS\");'>
								<input type='hidden' name='$formActionName' value='delete'/>
								<input type='hidden' name='username' value='$username'/>
								<input type='submit' value='$deleteS'>
								</form>
							   </div>";
				
				$this->DisplayTitle ( "$profileS '$username'", $deleteForm );
			}
		}
		else
		{
			$createNewS = $this->GetNameString ( 'createNew' );
			
			$newuserType = new UserType ( $usertypeID );
			$typename = $newuserType->GetName ();
			$this->DisplayTitle ( "$createNewS '$typename'" );
		}
		
		$passwordS = $this->GetNameString ( 'password' );
		$cpasswordS = $this->GetNameString ( 'cpassword' );
		$nameS = $this->GetNameString ( 'name' );
		$emailS = $this->GetNameString ( 'email' );
		
		print ( '<div class="someContent">' );
		
		// display form header
		$formAction = ($username) ? 'update' : 'insert';
		$this->DisplayFormHeadr ( $formAction );
		
		// if ID = 0 the display new user form
		if ($username == null)
		{
			$screenName = $this->GetNameString ( 'screenName' );

			print ( '<div class="userFormLabel"><label><strong>' . $screenName . '</strong></label></div>' );
			print ( '<div style="float:left"><input name="username" type="text" size="36" maxlength="32" style="font-size:14px" value="" /></div>' );
			print ( '<br class="clearFloat" />' );
		}
		else
		{
			print ( "<input type='hidden' name='username' value='$username'/>" );
		}
		
		print ( '<div class="userFormLabel"><label><strong>' . $passwordS . '</strong></label></div>' );
		print ( '<div style="float:left">' );
		print ( "<input name='password' type='password' size='36' maxlength='64' style='font-size:14px' />" );
		print ( '</div>' );
		print ( '<br class="clearFloat" />' );
		
		print ( '<div class="userFormLabel"><label><strong>' . $cpasswordS . '</strong></label></div>' );
		
		print ( '<div style="float:left">' );
		print ( "<input name='cpassword' type='password' size='36' maxlength='64' style='font-size:14px' />" );
		print ( '</div>' );
		print ( '<br class="clearFloat" />' );
		
		print ( '<div class="userFormLabel"><label><strong>' . $nameS . '</strong></label></div>' );
		print ( '<div style="float:left">' );
		print ( "<input name='name' type='text' size='48' maxlength='128' style='font-size:14px' value='$name'/>" );
		print ( '</div>' );
		print ( '<br class="clearFloat" />' );
		
		print ( '<div class="userFormLabel"><label><strong>' . $emailS . '</strong></label></div>' );
		print ( '<div style="float:left">' );
		print ( "<input name='email' type='text' size='48' maxlength='256' style='font-size:14px' value='$email'/>" );
		print ( '</div>' );
		print ( '<br class="clearFloat" />' );
		
		if ( $uType>11 )
		{
			print ( '<div class="userFormLabel"><label><strong>Publish permission</strong></label></div>' );
			print ( '<div style="float:left">' );
			if ( $publish )
			{
				print ( "<input name='publish' type='checkbox' checked/>" );
			}
			else 
			{
				print ( "<input name='publish' type='checkbox'/>" );
			}
			print ( '</div>' );
			print ( '<br class="clearFloat" />' );
		}
		
		// usertype
		if ($username == null)
		{
			print ( "<input type='hidden' name='usertype' value='$usertypeID'>" );
		}
		
		$doneS = $this->GetNameString ( 'done' );
		
		print ( '<div class="userFormLabel"><label><strong>&nbsp;</strong></label></div>' );
		print ( '<div style="float:left"><input name="" type="submit" value="' . $doneS . '" /></div>' );
		print ( '<br class="clearFloat" />' );
		print ( '</form>' );
		print ( '</div>' );
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `user` (
				  `ID` smallint(5) unsigned NOT NULL auto_increment,
				  `name` varchar(32) NOT NULL default '',
				  `username` varchar(32) NOT NULL default '',
				  `password` varchar(64) NOT NULL default '',
				  `email` varchar(64) NOT NULL default '',
				  `cmsLang` varchar(2) default NULL,
				  `usertypeID` tinyint(3) unsigned NOT NULL default '0',
				  `last_log` date NOT NULL default '0000-00-00',
				  `cookie` varchar(64) NOT NULL default '',
				  PRIMARY KEY  (`ID`),
				  UNIQUE KEY `username` (`username`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>