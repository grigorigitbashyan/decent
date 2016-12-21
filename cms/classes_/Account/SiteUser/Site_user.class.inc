<?php
/**
* Enter class description here...
*
* @package Account
*/
class Site_user extends NamedRecord
{
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_random;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_name;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_lname;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_url;

	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_password;

	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_email;

	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_activation_code;

	/**
	* Enter description here...
	*
	* @protected date
	*/
	protected $m_date;

	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_status;

	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_logcode;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_image;
	
	/**
	 * Enter description here...
	 *
	 * @var boolean
	 */
	protected $m_notifications;
	
	/**
	 * Enter description here...
	 *
	 * @var boolean
	 */
	protected $m_follow;
	
	/**
	 * Enter description here...
	 *
	 * @var boolean
	 */
	protected $m_mention;
	
	/**
	 * Enter description here...
	 *
	 * @var boolean
	 */
	protected $m_recommend;
	
	/**
	 * Enter description here...
	 *
	 * @var boolean
	 */
	protected $m_share;
	
	/**
	* Enter description here...
	*
	* @param int $ID
	* @return Site_user
	*/
	function __construct($ID = null, $field = "ID")
	{
		parent::__construct($ID, 'site_user', $field);
		
		$this->Read();
	}

	/**
	* Delete record from database
	*
	* @return bool
	*/
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		 return $this->DeleteRecord(false);
	}
	
	/**
	* Update record
	*
	* @param array $properties
	* @return bool
	*/
	function Update($properties)
	{
		 return $this->UpdateValues($properties);
	}
	
	/**
	* Insert new record
	*
	* @param array $properties
	* @return bool
	*/
	function Insert($fullname, $url, $email, $password, $status = 1)
	{
		// get delay days from configuration
		$fileRoot = Site::GetStructureToFileRoot();
		
		if($fileRoot == 'files/')
		{
			$fileRoot = '';
		}
		else 
		{
			$fileRoot = substr($fileRoot, 0, strlen($fileRoot) - 6);
		}

		$filePath = $_SERVER['DOCUMENT_ROOT'].'/'.$fileRoot.'cms/config/userConfig.xml';
		
		if ( !file_exists($filePath) )
		{
			die("The configuration file ($filePath) does not exist!");
		}
		
		$xml = simplexml_load_file($filePath); 
		$activationDelay = (int) ($xml->activationWaitTerm) ? $xml->activationWaitTerm : 1;
		 
		$this->DeleteOlds($activationDelay);
		
		$password = trim($password);
		$email = trim($email);
		
		// get user name
		$sql = "SELECT count(*) FROM `site_user` WHERE `email` = '$email'";
		$res = DB::QueryToValue($sql);
		
		if ( $res )
		{
			return null;
		}
		
		$properties['name'] = $fullname;
		$properties['url'] = $url;
		$properties['password'] = md5($password);
		$properties['email'] = $email;
		$properties['activation_code'] = GetRandCode(32);
		$properties['date'] = date('Y-m-d');
		$properties['status'] = $status;
		$properties['logcode'] = GetRandCode(16);
		$properties['random'] = GetRandCode(6);
		
		return $this->InsertRecord($properties);
	}
	
	/**
	 * Delete all registrated users that has not activated
	 *
	 */
	function DeleteOlds($day = 1)
	{
		// delete old registrated
		$yerstaday  = mktime(0, 0, 0, date("m")  , date("d")-$day, date("Y"));
		$yerstaday = date('Y-m-d', $yerstaday);
		
		$sql = "DELETE FROM `site_user` WHERE `status` = 1 AND `date` < '$yerstaday'";
		DB::Query($sql);
	}

	/**
	* Return value of m_random
	*
	* @return string
	*/
	function GetRandom()
	{
		return $this->m_random;
	}
	
	/**
	* Return value of m_name
	*
	* @return string
	*/
	function GetFullName()
	{
		return $this->m_name;
	}
	/**
	* Return value of m_url
	*
	* @return string
	*/
	function GetURL()
	{
		return $this->m_url;
	}
	
	/**
	* Return value of m_password
	*
	* @return string
	*/
	function GetPassword()
	{
		return $this->m_password;
	}
	
	/**
	* Return value of m_email
	*
	* @return string
	*/
	function GetEmail()
	{
		return $this->m_email;
	}
	
	/**
	* Return value of m_activation_code
	*
	* @return string
	*/
	function GetActivation_code()
	{
		return $this->m_activation_code;
	}

	/**
	* Return value of m_date
	*
	* @return date
	*/
	function GetDate()
	{
		return $this->m_date;
	}

	/**
	* Return value of m_status
	*
	* @return int
	*/
	function GetStatus()
	{
		return $this->m_status;
	}

	/**
	* Return value of m_logcode
	*
	* @return int
	*/
	function GetLogcode()
	{
		return $this->m_logcode;
	}
	
	/**
	* Return value of m_image
	*
	* @return string
	*/
	function GetImage()
	{
		return $this->m_image;
	}
	
	/**
	* Return value of m_notifications
	*
	* @return boolean
	*/
	function GetNotificationsSetting()
	{
		return $this->m_notifications;
	}
	
	/**
	* Return value of m_follow
	*
	* @return boolean
	*/
	function GetFollowSetting()
	{
		return $this->m_follow;
	}
	
	/**
	* Return value of m_mention
	*
	* @return boolean
	*/
	function GetMentionSetting()
	{
		return $this->m_mention;
	}
	
	/**
	* Return value of m_recommend
	*
	* @return boolean
	*/
	function GetRecommendSetting()
	{
		return $this->m_recommend;
	}
	
	/**
	* Return value of m_share
	*
	* @return boolean
	*/
	function GetShareSetting()
	{
		return $this->m_share;
	}
	
	// Set functions ////////////////////////////////////////////////////
	/**
	* Set value to m_name
	*
	* @param string $fullname
	* @param bool $dbupdate
	* @return bool
	*/
	function SetFullName($fullname, $dbupdate = true)
	{
		return $this->UpdateValue('name', $fullname, $dbupdate);
	}
	
	/**
	* Set value to m_url
	*
	* @param string $url
	* @param bool $dbupdate
	* @return bool
	*/
	function SetURL($url, $dbupdate = true)
	{
		return $this->UpdateValue('url', $url, $dbupdate);
	}
	
	/**
	* Set value to m_password  
	*
	* @param string $password
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPassword($password, $dbupdate = true)
	{
		$password = md5($password);
		
		return $this->UpdateValue('password', $password, $dbupdate);
	}

	/**
	* Set value to m_email  
	*
	* @param string $email
	* @param bool $dbupdate
	* @return bool
	*/
	function SetEmail($email, $dbupdate = true)
	{
		return $this->UpdateValue('email', $email, $dbupdate);
	}

	/**
	* Set value to m_activation_code  
	*
	* @param string $activation_code
	* @param bool $dbupdate
	* @return bool
	*/
	function SetActivation_code($activation_code, $dbupdate = true)
	{
		return $this->UpdateValue('activation_code', $activation_code, $dbupdate);
	}

	/**
	* Set value to m_date  
	*
	* @param date $date
	* @param bool $dbupdate
	* @return bool
	*/
	function SetDate($date, $dbupdate = true)
	{
		return $this->UpdateValue('date', $date, $dbupdate);
	}

	/**
	* Set value to m_status  
	*
	* @param int $status
	* @param bool $dbupdate
	* @return bool
	*/
	function SetStatus($status, $dbupdate = true)
	{
		$this->DeleteOlds();
		
		return $this->UpdateValue('status', $status, $dbupdate);
	}

	/**
	* Set value to m_status  
	*
	* @param int $status
	* @param bool $dbupdate
	* @return bool
	*/
	function SetLogCode($logcode, $dbupdate = true)
	{
		return $this->UpdateValue('logcode', $logcode, $dbupdate);
	}
	
	/**
	* Set value to m_image
	*
	* @param int $image
	* @param bool $dbupdate
	* @return bool
	*/
	function SetImage($image, $dbupdate = true)
	{
		return $this->UpdateValue('image', $image, $dbupdate);
	}
	
	/**
	* Set value to m_notifications
	*
	* @param int $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetNotificationsSetting($value, $dbupdate = true)
	{
		return $this->UpdateValue('notifications', $value, $dbupdate);
	}
	
	/**
	* Set value to m_follow
	*
	* @param int $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetFollowSetting($value, $dbupdate = true)
	{
		return $this->UpdateValue('follow', $value, $dbupdate);
	}
	
	/**
	* Set value to m_mention
	*
	* @param int $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetMentionSetting($value, $dbupdate = true)
	{
		return $this->UpdateValue('mention', $value, $dbupdate);
	}
	
	/**
	* Set value to m_recommend
	*
	* @param int $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetRecommendSetting($value, $dbupdate = true)
	{
		return $this->UpdateValue('recommend', $value, $dbupdate);
	}
	
	/**
	* Set value to m_share
	*
	* @param int $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetShareSetting($value, $dbupdate = true)
	{
		return $this->UpdateValue('share', $value, $dbupdate);
	}
	
	/**
     * Enter description here...
     *
     * @param string $logId
     * @return bool
     */
    static function IsUserLogin($logID)
    {
    	$sql = "SELECT `ID` FROM `site_user` WHERE `logcode` = '$logID'";
    	return DB::QueryToValue($sql);
    }
    
    function CheckEmail($email)
    {
    	$sql = "SELECT * FROM `site_user` WHERE `email` = '$email'";
		$res = DB::QueryToArray($sql);
		if ( is_array($res) )
		{
			return $res;
		}
		else 
		{
			return false;
		}
    }
    
    function GetMyArticles($status=10, $private=10, $limit=null)
    {
    	$statusStr = "";
    	if ( $status==0 || $status==1 )
    	{
    		$statusStr = " and `status` = ".$status;
    	}
    	
    	$privateStr = "";
    	if ( $private==0 || $private==1 )
    	{
    		$privateStr = " and `private` = ".$private;
    	}
    	
    	$limitStr = "";
    	if ( $limit!=null )
    	{
    		$limitStr = " limit ".$limit;
    	}
    	
    	$sql = "SELECT * FROM `pr_main` WHERE `userID` = ".$this->GetID().$statusStr.$privateStr." and `typeID` = 1 order by `date` DESC, `ID` DESC".$limitStr;
		$res = DB::QueryToArray($sql);
		
		return $res;
    }
    
    function GetMyFollowers()
    {
    	$query = "select * from `user_tags` where `tagID` = ".$this->m_ID." and `tagType` = 'author' and `follow` = 1 and `userID` in (select `ID` from `site_user`) order by `count` DESC";
    	$retArr = DB::QueryToArray($query);
    	
    	return $retArr;
    }
    
    function Signin($email, $password)
    {
    	$password = md5($password);
    	
    	$sql = "SELECT * FROM `site_user` WHERE `email` = '$email' and `password` = '$password'";
		$res = DB::QueryToRow($sql);
		if ( is_array($res) )
		{
			return $res;
		}
		else 
		{
			return false;
		}
    }
}
?>