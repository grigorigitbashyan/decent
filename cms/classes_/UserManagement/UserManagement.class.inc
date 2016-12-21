<?php
/**
 * @version 1.0
 * 
 * this class is for login, the fitures of this class are
 * +1 create login databace (user, priority, hesh password, email, last log date)
 * +2 login/logout, chack log
 * +3 add/delete login user
 * +4 send email for forgeting password (create new password, hesh it and send by email)
 * +5 change password
 * +6 get corrent user (see the sessin id, then return user information)
 * 
 * future version 0.2
 * send email, if has atace user and password
 * 
 * @package CMSUser
 */
class UserManagement extends NamedRecord
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_maxSessionTime;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_username;
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_usertypeID;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_email;
	
	/**
	 * Enter description here...
	 *
	 * @var date
	 */
	protected $m_last_log;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_password;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_cookie;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_cmsLang;
	
	/**
	 * Enter description here...
	 *
	 * @var boolean
	 */
	protected $m_publish;
	
	/**
	 * Enter description here...
	 *
	 * @var boolean
	 */
	protected $m_paid;
	
	/**
	 * @desc Construcor
	 * @param string $userName
	 * @param int $time
	 */
	function __construct($userName = null, $allowByID = true, $time = 28800 )
	{
		parent::__construct ( null, 'user' );
		
		$this->m_maxSessionTime = $time;
		
		if ($userName != null)
		{
			if (intval ( $userName ) && $allowByID)
			{
				$sql = "SELECT * FROM `user` WHERE `ID`= $userName";
			}
			else
			{
				$sql = "SELECT * FROM `user` WHERE `username`= '$userName'";
			}
			
			$res = DB::QueryToArray ( $sql );
			if ($res)
			{
				parent::__construct ( $res ['0'] ['ID'], 'user' );
				$this->Read ();
			}
		}
	}
	
	/**
	 * @desc  login
	 * @param  string $username
	 * @param  string $password
	 * @param  int $priorty
	 * 
	 * @return bool
	 */
	function Login($username, $password)
	{
		// if user is olready login then return true
		if ($this->CheckLog ())
			return true;
			
		// try to login, if the user exist then set the session or cookie, 
		// update it in db  and return true
		
		// is empoty return false;
		$username = trim ( $username );
		$password = trim ( $password );
		
		if (strlen ( $username ) < 1 || strlen ( $password ) < 1)
		{
			return false;
		}
		
		// add slashes and do md5 coding for password
		$username = $username;
		$password = md5 ( $password );
		
		// do query
		$sql = "SELECT * FROM `user` WHERE `username`='$username' AND `password`='$password'";
		$user = DB::QueryToRow ( $sql );
		// if user exist
		if ($user)
		{
			$this->m_ID = $user ['ID'];
			
			// update members
			$this->Read ();
			
			$sessionID = GetRandCode ( 64 );
			setcookie ( "u", $sessionID, (time () + $this->m_maxSessionTime), "/" );
			
			// update db
			$res = $this->UpdateValue ( 'cookie', $sessionID );
			
			return $res;
		}
		
		// if login is feald return messsage for wrong username or password
		return false;
	}
	
	/**
	 * @desc  logout
	 * @return bool
	 */
	function Logout()
	{
		// set random session id into the db and apdate data
		$noid = GetRandCode ( 64 );
		
		$date = date ( "Y-m-d" );
		
		$res1 = $this->UpdateValue ( 'last_log', $date );
		$res2 = $this->UpdateValue ( 'cookie', $noid );
		
		// remove cookie
		setcookie ( "u", "1", - 1, "/" );
		
		return ($res1 && $res2);
	}
	
	/**
	 * cheake login
	 *
	 * @param int $priority
	 * @return bool
	 */
	function CheckLog()
	{
		// isn`t set cookoie , return flase
		$u = UserManagement::GetSessionID ();
		
		if ($u == null)
			return false;
			// cookie isn`t equal cookie from db, return false
		$sql = "SELECT * FROM `user` WHERE `cookie`='$u'";
		$user = DB::QueryToRow ( $sql );
		
		if (! $user)
		{
			return false;
		}
		
		// update members
		$this->m_ID = $user ['ID'];
		$this->Read ();
		
		return true;
	}
	
	/**
	 * @desc add user
	 * @param string $username
	 * @param string $password
	 * @param string $email
	 * @param int $userType
	 * 
	 * @return int
	 */
	function AddUser($username, $password, $email, $userType, $name)
	{
		$this->m_usertypeID = intval ( $this->m_usertypeID );
		$userType = intval ( $userType );
		
		$username = trim ( $username );
		
		if (strlen ( $username ) < 1)
		{
			return false;
		}
		
		// allow or not
		$currentUser = UserManagement::GetCurrentUser ();
		$usertypeList = new UserTypeList ( $currentUser->GetUserTypeID () );
		$res = $usertypeList->FindPathInTree ( $userType );
		
		// aloow user info and co cousers
		if ((count ( $res ) < 2))
		{
			// you dont have permition to insert  user of this type
			return false;
		}
		// trim strings, adn coding password by md5
		$username = addslashes ( trim ( $username ) );
		$password = md5 ( trim ( $password ) );
		$email = trim ( $email );
		
		$properties ['username'] = $username;
		$properties ['password'] = $password;
		$properties ['email'] = $email;
		$properties ['usertypeID'] = $userType;
		$properties ['name'] = $name;
		
		// chaking user privileges it must be great then creator
		
		$res = $this->InsertRecord ( $properties );
		
		return $res;
	}
	
	/**
	 * Delete user
	 *
	 * @return bool
	 */
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		$this->m_usertypeID = intval ( $this->m_usertypeID );
		
		$curUser = $this->GetCurrentUser ();
		$userType = intval ( $curUser->GetUserTypeID () );
		
		if ($this->m_usertypeID <= $userType)
		{
			// you dont have permition to delete  user of this type
			return false;
		}
		
		return $this->DeleteRecord ( false );
	}
	
	/**
	 * @desc  Change password
	 * @return bool
	 */
	function SetPassword($newPassword)
	{
		$newPassword = trim ( $newPassword );
		
		if (strlen ( $newPassword ) < 1)
			return false;
		
		$password = md5 ( $newPassword );
		
		return $this->UpdateValue ( 'password', $password );
	}
	
	/**
	 * This function change username 
	 *
	 * @param string $newName
	 * @return bool
	 */
	function SetEmail($email)
	{
		$email = trim ( $email );
		
		if (strlen ( $email ) < 1)
			return false;
		
		return $this->UpdateValue ( 'email', $email );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param strung $lang
	 * @return bool
	 */
	function SetCMSLang($lang)
	{
		return $this->UpdateValue ( 'cmsLang', $lang );
	}
	
	/**
	 * Change password and send email
	 *
	 * @param string $email
	 * @return bool
	 */
	function SetPassSendEmail($email)
	{
		// get user information if exist
		$sql = "SELECT * FROM `user` WHERE `email`='$email'";
		$user = DB::QueryToRow ( $sql );
		
		if (! $user)
		{
			return false;
		}
		
		// generate new password
		$npass = GetRandCode ( 8 );
		
		// update for corrent user
		$userObj = new UserManagement($user['ID']);
        $userObj->SetPassword($npass);
		//$this->SetPassword ( $npass );
		// send mail
		
		$email = $user ['email'];
		
		// preperint e-mail message
		$msg = "Greetings, <br> Someone, possibly you, requested account login information for this e-mail address, which is associated with user account at CMS.\r\n\r\n<br>";
		
		$msg .= "-------------------------------------------\r\n<br>";
		$msg .= "Your Username: " . $user ['username'] . "\r\n<br>";
		$msg .= "Your Password: " . $npass . "\r\n<br>";
		$msg .= "-------------------------------------------\r\n<br>";
		
		$subj = "CMS Login Information.";
		
		// sending e-mail
		$site = new Site ( );
		$site->Mail ( $email, $subj, $msg, $user ['fname'], "Site Administration" );
				
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return UserManagement
	 */
	static public function GetCurrentUser()
	{
		$user = null;
		
		$session = UserManagement::GetSessionID ();
		
		if ($session == NULL)
			return null;
			// is session exist
		if ($session != null)
		{
			$sql = "SELECT * FROM  `user` WHERE `cookie`='$session'";
			$user = DB::QueryToRow ( $sql );

			// if sessin has recorded in db
			if ($user)
			{
				return new UserManagement ( $user ['username'], false );
			}
		}
		
		// return user 
		return null;
	}
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	static function GetSessionID()
	{
		$id = null;
		
		if (isset ( $_COOKIE ['u'] ))
			$id = trim ( $_COOKIE ['u'] );
		
		if (strlen ( $id ) < 1)
			return null;
		
		return $id;
	}
	
	/**
	 * REturn UserName
	 *
	 * @return string
	 */
	function GetUserName()
	{
		return $this->m_username;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetPassword()
	{
		return $this->m_password;
	}
	
	/**
	 * return e-mail address
	 *
	 * @return string
	 */
	function GetEmail()
	{
		return $this->m_email;
	}
	
	/**
	 * Return cms language
	 *
	 * @return strung
	 */
	function GetCMSLang()
	{
		return $this->m_cmsLang;
	}
	
	/**
	 * return  user type ID
	 *
	 * @return int
	 */
	function GetUserTypeID()
	{
		return $this->m_usertypeID;
	}
	
	/**
	 * return user publish permission
	 *
	 * @return int
	 */
	function GetPublish()
	{
		return $this->m_publish;
	}
	
	/**
	 * return user Paid sum
	 *
	 * @return int
	 */
	function GetPaid()
	{
		return $this->m_paid;
	}
	
	/**
	 * return user total sum
	 *
	 * @return int
	 */
	function GetTotal()
	{
		$query = "select SUM(`price`) as `total` from `pr_main` where `userID` = ".$this->GetID();
		$total = DB::QueryToValue($query);
		
		return $total;
	}
	
	/**
	 * return user bonus sum
	 *
	 * @return int
	 */
	function GetBonus()
	{
		$query = "select SUM(`bonus`) as `total` from `pr_main` where `userID` = ".$this->GetID();
		$total = DB::QueryToValue($query);
		
		return $total;
	}
	
	/**
	 * return user payment history
	 *
	 * @return array
	 */
	function GetPaymentHistory()
	{
		$query = "select * from `paymentHistory` where `userID` = ".$this->GetID()." order by `date` desc, `ID` desc";
		$retArr = DB::QueryToArray($query);
		
		return $retArr;
	}
	
	/**
	 * return user statistics
	 *
	 * @return array
	 */
	function GetStatistics()
	{
		//	total articles count
		$query = "select count(*) from `pr_main` where `userID` = ".$this->GetID();
		$retArr["totalArticles"] = DB::QueryToValue($query);
		
		//	approved/published articles count
		$query = "select count(*) from `pr_main` where `userID` = ".$this->GetID()." and (`status` = 1 or `status` = 2)";
		$retArr["publishedArticles"] = DB::QueryToValue($query);
		
		//	rejected articles count
		$query = "select count(*) from `pr_main` where `userID` = ".$this->GetID()." and `rejectedCount` > 0";
		$retArr["rejectedArticles"] = DB::QueryToValue($query);
		
		return $retArr;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $logId
	 * @return bool
	 */
	static function IsUserLogin($logID)
	{
		$sql = "SELECT `ID` FROM `user` WHERE `cookie` = '$logID'";
		return DB::QueryToValue ( $sql );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetCookie()
	{
		return $this->m_cookie;
	}
	
	/**
	 * Set user publishing permission
	 *
	 * @return bool
	 */
	function SetPublish($value)
	{
		return $this->UpdateValue ( 'publish', $value);
	}
	
	/**
	 * Set user paid sum
	 *
	 * @return bool
	 */
	function SetPaid($value)
	{
		$total = $value + $this->GetPaid();
		
		//	add to history
		$query = "insert into `paymentHistory` (`userID`, `sum`, `date`) values (".$this->GetID().", $value, '".date("Y-m-d")."')";
		DB::Query($query);
		
		return $this->UpdateValue ( 'paid', $total);
	}
}
?>