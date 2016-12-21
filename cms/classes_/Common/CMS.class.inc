<?php

include_once 'classes/FileChecker/ExceptionMissFile.class.inc';
include_once 'classes/FileChecker/FileUpdater.class.inc';

include_once 'classes/Common/Site.class.inc';

Site::LoadStatics ();

Site::IncludeFile ( 'classes/Common/HTML.class.inc' );
Site::IncludeFile ( 'classes/Common/ControllerBase.class.inc' );
Site::IncludeFile ( 'classes/Common/View.class.inc' );
Site::IncludeFile ( 'classes/Common/CMSInterface.interface.inc' );
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );
Site::IncludeFile ( 'classes/Common/NamedRecord.class.inc' );
Site::IncludeFile ( 'classes/Common/CMSUserController.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserManagement.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/Access/UsertypeAccessCMS.class.inc' );
Site::IncludeFile ( 'classes/Common/CmsXMLParser.class.inc' );

Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );
Site::IncludeFile ( 'classes/Common/CMSHome.class.inc' );

Site::IncludeFile ( 'classes/Common/DB.class.inc' );
Site::IncludeFile ( 'classes/Common/DBxp.class.inc' );

// Error handler class
Site::IncludeFile ( 'classes/Common/ErrorHandler.class.inc' );
Site::IncludeFile ( 'classes/Common/Value.class.inc' );

Site::IncludeFile ( 'classes/Folder/FolderXP.class.inc' );
/**
 * This class provide all functionality for working  with CMS. It has functions for: 
 * 1) including files of modules
 * 2) getting parameters from GET and POST requests
 * 3) displaying menu of modules (menu of structure )
 * 4) calling necessary actions for modules
 * 5) displaying left side features for each module (display items of structure , display categories of content, etc.)
 * 6) displaying main features for each module
 * 
 * @package Main
 */
class CMS
{
	/**
	 * Array of structure, it content all modules that need CMS
	 *
	 * @protected array
	 */
	protected $m_structure;
	
	/**
	 * Enter description here...
	 *
	 * @protected unknown_type
	 */
	protected $m_currentStructure;
	
	/**
	 * Enter description here...
	 *
	 * @protected Site
	 */
	protected $m_site;
	
	/**
	 * Enter description here...
	 *
	 * @protected CMSObject
	 */
	protected $m_object;
	
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	protected $m_displayLang;
	
	/**
	 * User , who was entered to system
	 *
	 * @private UserManagement
	 */
	protected static $m_user;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected static $m_cmsLang;
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected static $m_cmsLangs;
	
	protected $m_lastStep;
	
	protected static $m_ftpLink;
	protected static $m_ftpConnect;
	protected static $m_ftpRoot;
	
	/**
	 * Enter description here...
	 *
	 * @return UserManagement
	 */
	static public function GetUser()
	{
		return CMS::$m_user;
	}
	/**
	 * Calculate configuration, whitch modules must be included.
	 * Includs baseic files (Base, Site classes and also database files).
	 * Create nuecessery objects (site object).
	 * Initialization protectediables;
	 * 
	 * @return CMS
	 */
	function __construct()
	{
		///////////////////////////////////////////////////////////////
		// database include
		require_once ('files/db.inc');
		
		Site::IncludeFile ( 'classes/Common/View.class.inc' );
		Site::IncludeFile ( 'classes/Common/functions.inc' );
		
		////////////////////////////////////////////////////
		$this->m_site = new Site ();
		$this->m_site->LoadConfiguration ();
		
		$this->m_displayLang = $this->m_site->GetValue ( 'cmsSitelang' );
		
		if (! $this->m_displayLang)
			$this->m_displayLang = $this->m_site->GetDefaultLanguage ();
		
		if (empty ( $this->m_displayLang ))
		{
			$this->m_displayLang = 'en';
		}
		
		// load information abo
		// load statics
		View::SetDisplayLang ( $this->m_displayLang );
		
		// geting current structure
		$this->m_currentStructure = $this->m_site->GetValue ( 'structure' );
		
		// initialisations
		$this->Init ();
		
		// read cms language
		$this->ReadCMSLang ();
		// read global strings
		CMSObject::ReadGlobalNameStrings ();
		CMSObject::SetCMS ( $this );
		
		$this->SaveHistory ();
		//		$typeID = self::GetUser()->GetUserTypeID();
		if (false)
		{
			$old_error_handler = set_error_handler ( "ErrorHandler" );
		}
		
		$this->ReadFtpSettings ();
	}
	
	/*
	 * Destructor
	 */
	function __destruct()
	{
		if (CMS::$m_ftpLink)
		{
			ftp_close ( CMS::$m_ftpConnect );
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Init()
	{
		$cmsReader = new CmsXmlParser ();
		
		$this->m_structure = $cmsReader->GetStructure ();
		CMS::$m_cmsLangs = $cmsReader->GetLanguages ();
		
		// chack current structure in structure
		if (! array_key_exists ( $this->m_currentStructure, $this->m_structure ))
		{
			$this->m_currentStructure = null;
		}
	}
	
	static function GetCMSLangs()
	{
		return CMS::$m_cmsLangs;
	}
	
	function ReadCMSLang()
	{
		$currUser = UserManagement::GetCurrentUser ();
		
		if ($currUser)
		{
			$cmsLang = DB::POST ( 'cmslanguage' );
			if ($cmsLang)
			{
				$currUser->SetCMSLang ( $cmsLang );
			}
			else
			{
				$cmsLang = $currUser->GetCMSLang ();
				
				if (! $cmsLang)
				{
					$langs = CMS::GetCMSLangs ();
					$keys = array_keys ( $langs );
					
					$cmsLang = $keys [0];
					
					$currUser->SetCMSLang ( $cmsLang );
				}
			}
			
			CMS::$m_cmsLang = $cmsLang;
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	static function GetCMSLang()
	{
		return CMS::$m_cmsLang;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return array
	 */
	function GetStructure()
	{
		return $this->m_structure;
	}
	/**
	 * Enter description here...
	 *
	 */
	function IncludeModuleCMSfile($moduleID = null)
	{
		// include files for coresponding module
		if (! $moduleID)
		{
			$moduleID = $this->m_currentStructure;
		}
		// including  files of corrent module
		if ($moduleID)
		{
			$module = $this->m_structure [$moduleID];
			
			$fileName = 'classes/' . $module ['filePath'];
			
			return Site::IncludeFile ( $fileName );
		}
	}
	
	/**
	 * Chack login status, also doing logout
	 *
	 * @param int|array $status
	 * @return bool
	 */
	function CheckLog($status = 1)
	{
		$um = new UserManagement ();
		
		if (is_array ( $status ))
		{
			$res = false;
			
			foreach ( $status as $value )
			{
				if ($um->CheckLog ( $value ))
				{
					$res = true;
					break;
				}
			}
			
			if (! $res)
			{
				header ( "Location: login.php" );
			}
		}
		else
		{
			if (! $um->CheckLog ( $status ))
			{
				header ( "Location: login.php" );
			}
		}
		
		if (isset ( $_GET ['logout'] ))
		{
			header ( "Location: logout.php" );
		}
		
		CMS::$m_user = $um;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatureMenu()
	{
		
		// getcms language 
		foreach ( $this->m_structure as $key => $module )
		{
			if ($this->HasModuleAccess ( $key ))
			{
				if ($this->m_structure [$key] ['menuID'] == 2)
				{
					// get structure name
					$name = $module ['name'] [CMS::GetCMSLang ()];
					
					// display structure
					if ($this->m_currentStructure == $key)
					{
						print ("<span class='selected'><a href='?structure=$key'>$name</a></span>") ;
					}
					else
					{
						print ("<span><a href='?structure=$key'>$name</a></span>") ;
					}
				}
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayCommonMenu()
	{
		// get current user and display name
		$currUer = UserManagement::GetCurrentUser ();
		$name = null;
		
		if ($currUer)
		{
			$name = $currUer->GetName ();
		}
		
		if (! (DB::GET ( 'usertype' ) == 'profile'))
		{
			$name = "<a href='?structure=controlpanel&usertype=profile'>$name</a>";
		}
		
		$hiS = CMSObject::GetGlobalNameString ( 'hi' );
		
		print ("$hiS, $name | ") ;
		
		// cms home page link
		if ($this->m_object)
		{
			$cmsHomeS = CMSObject::GetGlobalNameString ( 'cmshome' );
			print ("<a href='?structure=home'>$cmsHomeS</a> | ") ;
		}
		
		$url = $this->m_site->GetSiteURL ();
		$launchSite = CMSObject::GetGlobalNameString ( 'launchsite' );
		
		print ("<a href='$url' target='_blank'>$launchSite</a> | ") ;
		
		// display cmenu items 
		foreach ( $this->m_structure as $key => $module )
		{
			// of it is not closed module then display link to it
			if ($this->HasModuleAccess ( $module ['linkname'] ))
			{
				if ($this->m_structure ["$key"] ['menuID'] == 1)
				{
					// get module name
					$name = $module ['name'] [CMS::GetCMSLang ()];
					
					if ($this->m_currentStructure == $key)
					{
						print ("<span class='commMenuS'>$name</span> | ") ;
					}
					else
					{
						print ("<a href='?structure={$module['linkname']}'>$name</a> | ") ;
					}
				}
			}
		}
		
		// display help link
		$helpS = CMSObject::GetGlobalNameString ( 'help' );
		$helpURL = "http://www.siteius.com/en/";
		
		if ($this->m_currentStructure)
		{
			$helpURL .= $this->m_structure [$this->m_currentStructure] ['helpPath'];
		}
		
		print ("<a href='$helpURL' target='_blank'>$helpS</a> | ") ;
		// display logout
		$logoutS = CMSObject::GetGlobalNameString ( 'logout' );
		print ("<a href='login.php?action=logout' class='red'>$logoutS</a>") ;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	static function DisplayCopyRight()
	{
		$currentYear = date ( 'Y' );
		print ("&copy; 2001&#151;$currentYear AUC&reg;") ;
	}
	/**
	 * Enter description here...
	 *
	 */
	function DoAction()
	{
		if ($this->m_currentStructure && $this->HasModuleAccess ( $this->m_currentStructure ))
		{
			$cmsClassName = $this->m_structure [$this->m_currentStructure] ['cmsname'];
			$this->m_object = new $cmsClassName ();
			
			return $this->m_object->DoAction ();
		}
		else
		{
			return true;
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayLeftFeatures()
	{
		
		if ($this->m_object && $this->HasModuleAccess ( $this->m_currentStructure ))
		{
			$this->m_object->DisplayMenu ();
		}
		else
		{
			$CMSHome = new CMSHome ();
			$CMSHome->DisplayMenu ();
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMainFeatures()
	{
		if ($this->m_object && $this->HasModuleAccess ( $this->m_currentStructure ))
		{
			$this->m_object->DisplayFeatures ();
		}
		else
		{
			$CMSHome = new CMSHome ();
			$CMSHome->DisplayFeatures ();
			
			if (CMS::GetUser ()->GetUserTypeID () == 1)
			{
				$this->SelfTest ();
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayLanguages()
	{
		$langs = $this->m_site->GetLanguagesName ();
		
		$langCount = count ( $langs );
		
		if ($langCount < 2)
		{
			print ('&nbsp;') ;
			return;
		}
		
		$link = '';
		
		// overloading parametors
		// for getting true link after changing language
		if (isset ( $_GET ))
		{
			foreach ( $_GET as $prop => $value )
			{
				$value = trim ( $value );
				$prop = trim ( $prop );
				
				$value = DB::EscapeInjections ( $value );
				$value = HTML::RemoveXSS ( $value );
				
				$prop = DB::EscapeInjections ( $prop );
				$prop = HTML::RemoveXSS ( $prop );
				
				if (empty ( $link ))
				{
					$link .= "?$prop=$value";
				}
				else
				{
					$link .= "&$prop=$value";
				}
			}
		}
		/////////////////////////////////////////////////////////////
		$selectlanguageS = CMSObject::GetGlobalNameString ( 'selectlanguage' );
		
		print ("<form method=\"post\" action=\"$link\" style=\"margin:0\">") ;
		print ("<label>{$selectlanguageS}</label>") ;
		print ('<select name="cmsSitelang" id="jumpMenu" onchange="submit();">') ;
		foreach ( $langs as $key => $lang )
		{
			if ($this->m_displayLang == $key)
				print ("<option value='$key' selected>$lang</option>") ;
			else
				print ("<option value='$key'>$lang</option>") ;
		}
		print ('</select>
      	</form>') ;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	public function IncludeJavaScripts()
	{
		// including  files of corrent module
		if ($this->m_currentStructure)
		{
			$key = $this->m_currentStructure;
			$module = $this->m_structure [$key];
			
			$fileNames = $module ['javascripts'];
			
			// some modules can have more then one files
			if ($fileNames)
			{
				foreach ( $fileNames as $filename )
				{
					print ("<script src=\"$filename\" type=\"text/javascript\" ></script>\n") ;
				}
			}
		}
	}
	
	/**
	 * Create all tables
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$moduleLinks = array_keys ( $this->m_structure );
		
		foreach ( $moduleLinks as $moduleLink )
		{
			$this->m_currentStructure = $moduleLink;
			
			if ($this->IncludeModuleCMSfile ())
			{
				$cmsClassName = $this->m_structure [$this->m_currentStructure] ['cmsname'];
				$object = new $cmsClassName ();
				
				$object->CreateDB ();
			}
		}
		
		return true;
	}
	
	/**
	 * Test all modules
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$res = true;
		
		$moduleLinks = array_keys ( $this->m_structure );
		
		foreach ( $moduleLinks as $moduleLink )
		{
			$this->m_currentStructure = $moduleLink;
			
			// count of missing files
			$missFileCount = 0;
			
			if ($this->IncludeModuleCMSfile () && $missFileCount == count ( ExceptionMissFile::GetMissFiles () ))
			{
				$cmsClassName = $this->m_structure [$this->m_currentStructure] ['cmsname'];
				$object = new $cmsClassName ();
				
				$res &= $object->SelfTest ();
			}
			else
			{
				$res = false;
			}
			
			// get miss files count
			$missFileCount == count ( ExceptionMissFile::GetMissFiles () );
		}
		
		if (! $res)
		{
			// correct CMS missed files
			if (DB::POST ( 'correctCMS' ) == Correct)
			{
				$exceps = ExceptionMissFile::GetMissFiles ();
				if (count ( $exceps ) > 0)
				{
					$fu = new FileUpdater ();
					
					foreach ( $exceps as $missFile )
					{
						$fu->PutMissedFile ( $missFile );
					}
				}
			}
			print ('<br /><form method="post"><input type="submit" name="correctCMS" value="Correct"></form>') ;
		}
		
		return $res;
	}
	
	/**
	 * Test all modules
	 *
	 * @return CMSObject
	 */
	function GetMudlueMainClass($module)
	{
		if ($this->HasModuleAccess ( $module ))
		{
			if ($this->IncludeModuleCMSfile ( $module ))
			{
				$cmsClassName = $this->m_structure [$module] ['cmsname'];
				return new $cmsClassName ();
			}
		}
		
		return null;
	}
	/**
	 * This function returns ajax reponse for current module
	 *
	 * @param mixed $arg
	 * @return string
	 */
	function GetAjax($arg)
	{
		if ($this->m_currentStructure && $this->HasModuleAccess ( $this->m_currentStructure ))
		{
			$cmsClassName = $this->m_structure [$this->m_currentStructure] ['cmsname'];
			$this->m_object = new $cmsClassName ();
			
			return $this->m_object->GetAjax ( $arg );
		}
		else
		{
			return true;
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @return bool
	 */
	function HasModuleAccess($key)
	{
		$userID = self::GetUser ()->GetUserTypeID ();
		return UsertypeAccessCMS::HasModuleAccess ( $key, $userID );
	}
	
	/**
	 * @return ErrorHandler
	 */
	static function GetErrorHandler()
	{
		static $errorHandler = null;
		
		if (! $errorHandler)
		{
			$errorHandler = new ErrorHandler ();
		}
		
		return $errorHandler;
	}
	
	/**
	 * Save visit history i cookie
	 *
	 */
	function SaveHistory()
	{
		// get last two history from cookie
		$last1 = $this->m_site->GetValue ( 'last1' );
		$currURL = "structure={$this->m_currentStructure}"; //http_build_query($_GET);
		

		// get structure from first last
		if ($last1)
		{
			$patter = '/structure=([\w]+)/';
			$matches = array ();
			preg_match_all ( $patter, $last1, $matches );
			
			$lastStruc = null;
			if (isset ( $matches [1] [0] ))
			{
				$lastStruc = $matches [1] [0];
			}
			
			if ($lastStruc != $this->m_currentStructure)
			{
				// save last 1 to last 2
				$this->m_lastStep = $last1;
				$this->m_site->SaveValue ( 'last2', $last1 );
			}
			else
			{
				$this->m_lastStep = $this->m_site->GetValue ( 'last2' );
			}
		}
		
		$fromGet = $_GET;
		
		// remove structure is exist
		if (array_key_exists ( 'structure', $fromGet ))
		{
			unset ( $fromGet ['structure'] );
		}
		
		$str = http_build_query ( $fromGet );
		if (strlen ( $str ) > 0)
		{
			$str = '&' . $str;
		}
		$this->m_site->SaveValue ( 'last1', $currURL . $str );
	}
	
	function DisplayModuleBack()
	{
		// get last two history from cookie
		if ($this->m_lastStep && strlen ( $this->m_lastStep ) > 0)
		{
			print ("<a href=\"?{$this->m_lastStep}\"><img src=\"img/switchModule.gif\"></a>") ;
		}
	}
	
	function ReadFtpSettings()
	{
		if (! CMS::$m_ftpLink)
		{
			$valueObj = new Value ( 1 );
			$ftpSett = $valueObj->GetValue ( true );
			
			if (isset ( $ftpSett ['ftpurl'], $ftpSett ['ftpuser'], $ftpSett ['ftppass'] ))
			{
				CMS::$m_ftpConnect = ftp_connect ( $ftpSett ['ftpurl'] );
				CMS::$m_ftpRoot = $ftpSett ['ftproot'];
				
				if (! ftp_login ( CMS::$m_ftpConnect, $ftpSett ['ftpuser'], $ftpSett ['ftppass'] ))
				{
					CMS::$m_ftpConnect = null;
					CMS::$m_ftpRoot = null;
				}
			}
		}
		
		FolderXP::SetFtpConnection ( CMS::$m_ftpConnect );
		FolderXP::SetFtpRoot ( CMS::$m_ftpRoot );
	}
	
	static function GetFtpConnectionLink()
	{
		return CMS::$m_ftpConnect;
	}
	
	static function GetFtpRoot()
	{
		return CMS::$m_ftpRoot;
	}
}
?>