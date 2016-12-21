<?php

class Session extends NamedRecord
{
	/**
	 * Session ID
	 * @protected string
	 */
	protected $m_ID;
	
	/**
	 * Session data
	 * @protected string $m_session_data
	 */
	protected $m_session_data;
	
	/**
	 * Session Expired date
	 * @protected int $m_session_expire
	 */
	protected $m_session_expire;
	
	/**
	 * Registration key kept in session
	 * @protected string $m_rkey
	 */
	protected $m_rkey;
	
	/**
	 * Session start
	 * @protected bool
	 */
	protected $m_started;
	
	/**
	 * Session initialised
	 * @protected bool
	 
    protected $m_initialised;            // the initalised var
    
    
	protected $m_rg; 
	protected $m_old;	
	protected $m_underline;*/
	
	protected $m_session;
	
	function __construct($ID = null)
	{		
		parent::__construct ( $ID, 'session_data' );
		
		if (!$this->SessionIDExists())
		{
			$properties['ID'] = $ID;
			
			if (!empty($_SESSION))
			{
				foreach ($_SESSION as $key => $value)
				{
					$session_data[$key] = $value;
				}
				$properties['session_data'] = serialize($session_data);
			}
			
			$properties['session_expire'] = time() + 3600;
			$properties['rkey'] = GetRandCode( 6 );
			
			$this->Insert($properties);
		}
		
		//TODO: if there is no session start at all I need to start it here and Insert values (if ($ID = null))
		$this->m_name = session_name();
		$this->m_started = ($ID) ? 1 : 0;
				
		$this->Read ();		
	}
	
	/**
	 * Delete record from database
	 *
	 * @return bool
	 */
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		return $this->DeleteRecord ( false );
	}
	
	/**
	 * Update record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Update($properties)
	{
		return $this->UpdateValues ( $properties );
	}
	
	/**
	 * Insert new record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Insert($properties = null)
	{
		// generate rkey code
		$properties['rkey'] = GetRandCode(6);
		return $this->InsertRecord ( $properties );
	}
	
	/**
	 * Return value of m_session_data
	 *
	 * @return string
	 */
	function GetSessionData()
	{
		return $this->m_session_data;
	}
	
	/**
	 * Return value of m_session_expire
	 *
	 * @return int timestamp
	 */
	function GetSessionExpire()
	{
		return $this->m_session_expire;
	}
	
	/**
	 * Return value of m_rkey
	 *
	 * @return string
	 */
	function GetRkey()
	{
		return $this->m_rkey;
	}
	
	/**
	 * Set value to m_session_data
	 * 
	 * @param string $sessionData
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetSessionData($sessionData, $dbupdate = true)
	{
		return $this->UpdateValue ( 'session_data', $sessionData, $dbupdate );
	}
	
	/**
	 * Set value to m_session_expire
	 * 
	 * @param int $sessionExpire
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetSessionExpire($sessionExpire, $dbupdate = true)
	{
		return $this->UpdateValue ( 'session_expire', $sessionExpire, $dbupdate );
	}
	
	/**
	 * Set value to m_rkey
	 * 
	 * @param string $rkey
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetRkey($rkey, $dbupdate = true)
	{
		return $this->UpdateValue( 'rkey', $rkey, $dbupdate );
	}
	
	/**
	 * Check if session id exists
	 * 
	 *  @return bool
	 */
	function SessionIDExists ()
	{
		$sql = "SELECT `ID`
					FROM `session_data`
					WHERE `ID` = '{$this->m_ID}'";
		
		return DB::QueryToValue($sql);
	}	

	/**
	 * Delete Old sessions' records from database by compareing with session_expire date
	 *
	 * @return 
	 */
	function DeleteOldSessions()
	{
		$date = time();
		$sql = "DELETE FROM `{$this->m_tableName}` WHERE `session_expire` < {$date}";
		$res = DB::Query ( $sql );
	}
	
	/**
	 * Get session data from database and return it as array 
	 *
	 * @return array
	 */
	function GetSessionDataArray()
	{
		return unserialize($this->GetSessionData());
	}
	
	function UpdateSessionDataArray($session_data)
	{
		$sessionDataFromDb = unserialize($this->GetSessionData());
		
		if (empty($sessionDataFromDb))
		{
			$sessionDataFromDb = array();
		}
		
		if (!empty ($session_data))
		{
			foreach ($session_data as $key => $value)
			{
				// Doesn't matter this:if (array_key_exists($key, $sessionDataFromDb))
				$sessionDataFromDb[$key] = $value;
			}
		}
		$sessionDataToDb = serialize($sessionDataFromDb);
		$this->SetSessionData($sessionDataToDb);
	}
	
	/**
	 * Destroy session with its all data and delete record about this session from database
	 *
	 * @return bool
	 */
	function DestroySession()
	{
		session_destroy();
		return $this->DeleteRecord ( false );
	}
	
	/*
	function Initialize()
	{
		// Set var so we know we initialised
		$this->m_initialised = true;
	}	
	
    function SessionStart ()
    {
    	$session_name = 'ID';
    	if (session_name() != $session_name)
    	{
    		session_name($session_name);
    	}
    	
    	if (!$this->m_started)
    	{
    		session_start();
    		$this->m_started = true;
		}
		
    }
    
    // Setting a session var (like session_register()) 
    function SetVar()
    {
    	$var = func_get_args();
    	
    	if (is_array($var[0]))
    	{
    		foreach($var[0] as $k => $value)
    		{
    			if($this->CheckVarSyntax($k)) $this->SetRegVars($k, $value);
    		}
    	}
    	else
    	{
    		if (count($var) % 2)
    		{
    			$this->errors[] = "Variable '" . $var[count($var) - 1] . "' ignored";
    			array_pop($var);
    		}
    		
    		for($i = 0; $i < count($var); $i += 2)
    		{
    			if($this->CheckVarSyntax($var[$i])) $this->SetRegVars($var[$i], $var[$i+1]);
    		}
    	}
    } 
    
    function CheckVarSyntax ($buff)
    {
    	if (preg_match("!^[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*$!i", $buff))
    	{
    		return true;
    	}
    	else
    	{
    		$this->errors[] = "Variable '<i>" . $buff . "</i>' ignored,<br>wrong syntax";
    		return false;
    	}
    }
    
    function SetRegVars($n, $v)
    {
    	if($this->m_rg)
    	{
    		$GLOBALS[$n] = $v;
    		session_register("$n");
    	}
    	else
    	{
    		session_register("$n");
    		if ($this->m_old)
    		{
    			$GLOBALS['HTTP_SESSION_VARS'][$n] = $v;
    		}
    		else
    		{
    			$GLOBALS['_SESSION'][$n] = $v; 
    		}
    	}
    }
      
    // Delete a session var
    function UnsetSessionVar($var)
    { 
        return session_unregister($var); 
    } 

    // check if the var exists 
    function CheckSessionVar($var)
    {
    	return(session_is_registered($var));
    } 

    // Getting the var
    function GetSessionVar($var)
    {
    	if ($this->CheckSessionVar($var))
    	{
    		$this->$var=$_SESSION[$var];
    	}
		else
		{
			$this->$var='Guest';
		}
		
		return($this->$var);
	} 

    // Getting the current session id 
    function GetSessionID()
    { 
		$this->m_ID = session_id();
		return (session_id());
	} 

    // fun, who am i ?
    function Me()
    { 
        return($_SERVER['PHP_SELF']); 
    } 

    // Drop everything 
    function DestroySession()
    {
    	$this->Delete();
    }*/
} 

?>