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
	
	protected $m_company;

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
	* @param int $ID
	* @return Site_user
	*/
	function __construct($ID = null)
	{
		 parent::__construct($ID, 'site_user');

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
	function Insert($fullName, $password, $email, $company, $status = 1)
	{
		// get delay days from configuration //////////////////
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
		
		if(!file_exists($filePath))
		{
			die("The configuration file ($filePath) does not exist!");
		}
		
		$xml = simplexml_load_file($filePath); 
		$activationDelay = (int) ($xml->activationWaitTerm) ? $xml->activationWaitTerm : 1;
		 
		$this->DeleteOlds($activationDelay);
		///////////////////////////////////////////////////////
		$fullName = trim($fullName);
		
		$password = trim($password);
		$email = trim($email);
		
		// get user name
		$sql = "SELECT count(*) FROM `site_user` WHERE `email` = '$email'";
		$res = DB::QueryToValue($sql);
		
		if($res)
		{
			return null;
		}
		
		////////////////////////////
		$properties['name'] = $fullName;
		$properties['company'] = $company;
		
		$properties['password'] = md5($password);
		$properties['email'] = $email;
		$properties['activation_code'] = GetRandCode(32);
		$properties['date'] = date('Y-m-d');
		$properties['status'] = $status;
		$properties['logcode'] = GetRandCode(16);
		
		return $this->InsertRecord($properties);
	}

	/**
	 * Delete all registrrated users, that has not activated
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
	* Return value of m_username
	*
	* @return string
	*/
	function GetLastName()
	{
		// split name and return second element
		$flName = split('&nbsp;', $this->m_name);
		return $flName[1];
	}

	/**
	* Return value of m_username
	*
	* @return string
	*/
	function GetFirstName()
	{
		// split name and return first element
//		$flName = split('&nbsp;', $this->m_name);
//		return $flName[0];

		return $this->m_name;
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
	
	// Set functions ////////////////////////////////////////////////////


	/**
	* Set value to m_username  
	*
	* @param string $username
	* @param bool $dbupdate
	* @return bool
	*/
	function SetFirstName($username, $dbupdate = true)
	{
		return $this->UpdateValue('name', $username, $dbupdate);
	}

	function SetLastName($username, $dbupdate = true)
	{
		return $this->UpdateValue('lname', $username, $dbupdate);
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
    
    function GetCompany()
    {
    	return $this->m_company;
    }
    
    function SetCompany($company, $dbupdate = true)
    {
    	$this->UpdateValue('company', $company, $dbupdate);
    }
}
?>