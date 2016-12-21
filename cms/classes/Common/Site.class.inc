<?php
/** 
 * @version 1.0
 * This class content all proprties of corrent site,
 * 1. languages
 * 2. create database
 * 3. print CopyRigth by language
 * @package Main
 */
class Site
{
	/**
	 *
	 * @protected array
	 */
	protected $m_languages;
	
	/**
	 *
	 * @protected string
	 */
	protected $m_languageDefault;
	
	/**
	 *
	 * @protected string
	 */
	protected $m_URL;
	
	/**
	 * secuirity-File root
	 *
	 * @protected string
	 */
	protected $m_sfRoot;
	
	/**
	 * site name, or title
	 *
	 * @var string
	 */
	protected $m_siteName;
	
	/**
	 * array of boolean, it shows existing of boxis in left and right
	 *
	 * @var array
	 */
	protected $m_boxes;
	
	/**
	 *
	 * @var string
	 */
	protected $m_prodImagRoot;
	
	/**
	 *
	 * @var string
	 */
	protected $m_galleryImageRoot;
	
	/**
	 *
	 * @var string
	 */
	protected static $m_struToFileRoot;
	
	/**
	 *
	 * @var string
	 */
	protected static $m_dbName;
	
	/**
	 *
	 * @var string
	 */
	protected static $m_recycleBinPath;
	
	/**
	 * Full path of index directory
	 *
	 * @var sintrg
	 */
	protected static $m_indexRoot;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_maxImageWidth;
	
	/**
	 * Date formats
	 *
	 * @var array
	 */
	protected static $m_dateFormat;
	
	/**
	 *
	 * @var array
	 */
	protected $m_templates;
	
	/**
	 *
	 * @return Site
	 */
	function __construct()
	{
//		 $this->LoadConfiguration();
	}
	
	/**
	 * This function return all languages that current site is support
	 *
	 * en - English
	 * ru - Russion
	 * am - Armenian
	 *
	 * @return array
	 */
	function GetLanguages() // static function
	{
		return array_keys ( $this->m_languages );
	}
	
	/**
	 * This function return all language keys and names that corrent site is support
	 *
	 * en - English
	 * ru - Russion
	 * am - Armenian
	 *
	 * @return array
	 */
	function GetLanguagesName()
	{
		return $this->m_languages;
	}
	
	/**
	 * Get Site URL
	 *
	 * @return string
	 */
	function GetSiteURL()
	{
		return $this->m_URL;
	}
	
	/**
	 * Return default title
	 *
	 * @return string
	 */
	function GetDefaultTitle($lang = 'en')
	{
		// read from database
		$rootCat = new Category ( 1 );
		return $rootCat->GetTitle ( $lang );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetDefaultLanguage()
	{
		return $this->m_languageDefault;
	}
	
	/**
	 * Return default keywords
	 *
	 * @return string
	 */
	function GetDefaultKeywords($lang = 'en')
	{
		//	read from database
		$rootCat = new Category ( 1 );
		return $rootCat->GetKeywords ( $lang );
	}
	
	/**
	 * return default description
	 *
	 * @return string
	 */
	function GetDefaultDescription($lang = 'en')
	{
		//	read from database
		$rootCat = new Category ( 1 );
		return $rootCat->GetDescription ( $lang );
	}
	
	/**
	 * return default picture
	 *
	 * @return string
	 */
	function GetDefaultPicture()
	{
		//	read from database
		$rootCat = new Category ( 1 );
		return $rootCat->GetPicture();
	}
	
	/**
	 * Return secuirity-file root
	 *
	 * @return string
	 */
	function GetSFRoot()
	{
		return $this->m_sfRoot;
	}
	
	/**
	 * Save key value to the cookie or session, (befor saveing trim value)
	 *
	 * @param string $key
	 * @param string $value
	 * @param bool $toCookie
	 */
	function SaveValue($key, $value, $toCookie = true)
	{
		if ($toCookie)
		{
			$value = trim ( $value );
			setcookie ( $key, $value, NULL, "/" );
		}
	}
	
	/**
	 * Return value from cookie
	 *
	 * @param string $key
	 * @param bool $toCookie
	 * @return string
	 */
	function GetValue($key, $toCookie = true)
	{
		if ($v = DB::GET ( $key ))
		{
			$this->SaveValue ( $key, $v );
			return $v;
		}
		
		if ($v = DB::POST ( $key ))
		{
			$this->SaveValue ( $key, $v );
			return $v;
		}
		
		if ($toCookie)
		{
			return DB::COOKIE ( $key );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $toMail
	 * @param string $subjact
	 * @param string $message
	 * @param string $fromMail
	 * @return bool
	 */
	function Mail($toMail, $subjact, $message, $fromMail = null, $fromName = null)
	{
		//TODO add site email
		if (! $fromMail)
		{
			$fromMail = "info@undl.com";
		}
		
		// if the from email name is not set, then read site name and set as a from name
		if (! $fromName)
		{
			$fromName = $this->GetSiteName ();
		}
		
		$headers = 'MIME-Version: 1.0' . "\r\n";
		$headers .= 'Content-type: text/html; charset=utf-8' . "\r\n";
		
		// Additional headers
		$headers .= "From:$fromName<" . $fromMail . ">\r\n";
		
		@mail ( $toMail, $subjact, $message, $headers );
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $lang
	 * @param Category $category
	 * @param int $lastID
	 */
	function GetRequestParams(&$lang, &$categoryID, &$lastIDs)
	{
		// language	
		if (DB::GET ( 'lang' ))
		{
			// get luangage request from URL
			$lang = strip_tags ( DB::GET ( 'lang' ) );
			
			// if langage is from values
			if (! (array_key_exists ( $lang, $this->m_languages )))
			{
				$lang = $this->m_languageDefault;
			}
			
			$this->SaveValue ( "lang", $lang );
		}
		else
		{
			// get cookie language value
			$lang = $this->GetValue ( "lang" );
			
			if (strlen ( $lang ) < 2)
			{
				$lang = $this->m_languageDefault;
				$this->SaveValue ( "lang", $lang );
			}
		}
		
		// category
		$catIDs = array ();
		for ( $index=1; $index<10; $index++ )
		{
			$v = DB::GET ( "p$index" );
			if ( $v )
			{
				array_push($catIDs, HTML::RemoveXSS($v));
			}
		}
		
		$count = count ( $catIDs );
		$path = "";
		$truePath = "";
		
		if ($count > 0)
		{
			for ($index = 0; $index < $count; $index ++)
			{
				if (strlen ( $catIDs [$index] ) < 1)
				{
					break;
				}
				
				$path .= $catIDs [$index] . "/";
				
				$category = new Category ( null, $path );
				
				if ($category->GetID ())
				{
					$truePath = $path;
				}
				else
				{
					if ( strpos($catIDs[$index], "page")===false )
					{
						$tempPr = new Pr_main($catIDs[$index]);
						if ( !$tempPr->GetID() )
						{
							$query = "select * from `optionsdata` where `key` = '".$catIDs[$index]."'";
							$retArr = DB::QueryToRow($query);
							if ( is_array($retArr) || $catIDs[$index]=="other" )
							{
								$truePath = $path;
								$truePath = substr($truePath, 0, -1);
								$pos = strrpos($truePath, "/");
								$truePath = substr($truePath, 0, $pos)."/";
							}
							else 
							{
								header('HTTP/1.0 404 Not Found');
								$errorPage = file_get_contents($this->GetSiteURL()."404.html");
								print $errorPage;
								exit();
							}
						}
						else 
						{
							$truePath = $tempPr->GetCategoryPath();
						}
					}
					
					$lastIDs = array();
					
					for($subIndex = $index; $subIndex < $count; $subIndex ++)
					{
						array_push ( $lastIDs, $catIDs [$subIndex] );
					}
					
					break;
				}
			}
			
			$category = new Category ( null, $truePath );
			$categoryID = $category->GetID ();
		}
		
		if ($categoryID < 2)
		{
			$categoryID = 2;
		}
	}
	
	/**
	 * Include file by $fileName address
	 *
	 * @param string $fileName
	 */
	static public function IncludeFile($fileName)
	{
		try
		{
			// if file does not exist, then an exception would be generated
			if (! file_exists ( Site::$m_indexRoot . $fileName ))
			{
				// if class ExceptionMissFile exist the generate it's instance
				if (class_exists ( 'ExceptionMissFile' ))
				{
					throw new ExceptionMissFile ( $fileName, $fileName );
				}
				// generates regular exception
				else
				{
					throw new Exception ( $fileName );
				}
			}
			
			return include_once Site::$m_indexRoot . $fileName;
		}
		catch ( Exception $e )
		{
			print ("The missed file: " . $e->getMessage () . "<br />\n") ;
		}
		
		return false;
	}
	
	/**
	 * Include file by $fileName address
	 *
	 * @param string $fileName
	 */
	static public function IncludeFiles(&$fileNames)
	{
		foreach ( $fileNames as $fileName )
		{
			include_once Site::$m_indexRoot . $fileName;
		}
	}
	
	/**
	 * return site name
	 *
	 * @return string
	 */
	function GetSiteName()
	{
		Site::IncludeFile ( 'classes/Content/Content.class.inc' );
		Site::IncludeFile ( 'classes/Category/Category.class.inc' );
		
		if (! $this->m_siteName)
		{
			$category = new Category ( 1 );
			$lang = View::GetDisplayLang ();
			$this->m_siteName = $category->GetName ( $lang );
		}
		
		return $this->m_siteName;
	}
	
	/**
	 * Return true if there is a box collection in $align
	 *
	 * @return array
	 */
	function GetBoxCollectionTypes()
	{
		if (isset ( $this->m_boxes ) && count ( $this->m_boxes ) > 0)
		{
			return $this->m_boxes;
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetProImageRoot($local = true, $addDocRoot = true)
	{
		if ($local)
		{
			$addLink = Site::GetAdditionFileLink ();
			if ($addDocRoot)
			{
				return $_SERVER ['DOCUMENT_ROOT'] . '/' . $addLink . $this->m_prodImagRoot;
			}
			else
			{
				return $this->m_prodImagRoot;
			}
		}
		else
		{
			return $this->m_URL . $this->m_prodImagRoot;
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetGalleryImageRoot($local = true, $RelURL = false)
	{
		if ($RelURL)
		{
			return $this->m_galleryImageRoot;
		}
		
		if ($local)
		{
			$addLink = Site::GetAdditionFileLink ();
			if ($addLink)
			{
				return $_SERVER ['DOCUMENT_ROOT'] . '/' . $addLink . $this->m_galleryImageRoot;
			}
			else
			{
				return $_SERVER ['DOCUMENT_ROOT'] . '/' . $this->m_galleryImageRoot;
			}
		}
		else
		{
			return $this->m_URL . $this->m_galleryImageRoot;
		}
	}
	
	function GetVideogalleryImageRoot($local = true, $RelURL = false)
	{
		$RetArr["demo"] = $_SERVER ['DOCUMENT_ROOT']."/files/demovideo/";
		
		$videoGall = $_SERVER ['DOCUMENT_ROOT']."/";
		$videoGall = str_replace("public_html", "videos", $videoGall);
		
		$RetArr["other"] = $videoGall;
		return $RetArr;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	static function GetStructureToFileRoot()
	{
		return Site::$m_struToFileRoot;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	static function GetRecycleBinPath()
	{
		return Site::$m_recycleBinPath;
	}
	
	static function SetStructureToFileRoot($root)
	{
		Site::$m_struToFileRoot = $root;
	}
	
	/**
	 * Load site configuration from XML 
	 *
	 * @return bool
	 */
	function LoadConfiguration()
	{
		$cinfFilePath = Site::GetConfigFilePath ();
		$config = simplexml_load_file ( $cinfFilePath );
		
		// load languages
		if (isset ( $config->languages ))
		{
			$langs = ( array ) $config->languages;
			$langsA = $langs ['language'];
			
			if (! is_array ( $langsA ))
			{
				
				$linkID = ( string ) $langsA->linkID;
				$linkName = ( string ) $langsA->linkName;
				$this->m_languages [$linkID] = $linkName;
			}
			else
			{
				foreach ( $langsA as $lang )
				{
					$linkID = ( string ) $lang->linkID;
					$linkName = ( string ) $lang->linkName;
					
					$this->m_languages [$linkID] = $linkName;
				}
			}
			
			$langKes = array_keys ( $this->m_languages );
			$this->m_languageDefault = $langKes [0];
		}
		
		// load templates
		if (isset ( $config->templates ))
		{
			$templates = ( array ) $config->templates;
			$langsA = $templates ['template'];
			
			if (! is_array ( $langsA ))
			{
				$this->m_templates = null;
			}
			else
			{
				$this->m_templates = array ();
				
				foreach ( $langsA as $lang )
				{
					$fileName = ( string ) $lang;
					array_push ( $this->m_templates, $fileName );
				}
			}
		}
		
		// load siteURL
		if (isset ( $config->url ))
		{
			$this->m_URL = ( string ) $config->url;
			$this->m_URL = trim ( $this->m_URL );
		}
		
		// load siteURL
		if (isset ( $config->maxImageWidth ))
		{
			$this->m_maxImageWidth = ( int ) $config->maxImageWidth;
		}
		
		// secuirity-file root
		if (isset ( $config->securFolder ))
		{
			$this->m_sfRoot = ( string ) $config->securFolder;
			$this->m_sfRoot = trim ( $this->m_sfRoot );
		}
		
		// boxes
		if (isset ( $config->boxes ))
		{
			$boxes = ( array ) $config->boxes;
			$boxes = $boxes ['box'];
			if (! is_array ( $boxes ))
			{
				$this->m_boxes = array ($boxes );
			}
			else
			{
				$this->m_boxes = $boxes;
			}
		}
		
		// product image root
		if (isset ( $config->productImageRoot ))
		{
			$str = ( string ) $config->productImageRoot;
			$this->m_prodImagRoot = trim ( $str );
		}
		
		// product image root
		if (isset ( $config->galleryRoot ))
		{
			$str = ( string ) $config->galleryRoot;
			$this->m_galleryImageRoot = trim ( $str );
		}
		
		return true;
	}
	
	static public function LoadStatics()
	{
		$cinfFilePath = Site::GetConfigFilePath ();
		
		$config = simplexml_load_file ( $cinfFilePath );
		// load file root
		if (isset ( $config->addFileRoot ))
		{
			$addRoot = ( string ) $config->addFileRoot;
			Site::SetStructureToFileRoot ( $addRoot . 'files/' );
		}
		else
		{
			Site::SetStructureToFileRoot ( 'files/' );
		}
		
		if (isset ( $config->recycleBinPath ))
		{
			Site::$m_recycleBinPath = $config->recycleBinPath;
		}
		
		// calculate root 
		$docRoot = $_SERVER ['DOCUMENT_ROOT']."";
		
		$struToFile = Site::GetStructureToFileRoot ();
		
		// on server
		if ($struToFile == 'files/')
		{
			Site::$m_indexRoot = $docRoot . '/cms/';
		}
		else
		{
			$struToFile = str_replace ( '/files/', '/', $struToFile );
			Site::$m_indexRoot = $docRoot . '/' . $struToFile . 'cms/';
		}
		
		// load date formats
		if (isset ( $config->dateformats ))
		{
			Site::$m_dateFormat = array ();
			
			foreach ( $config->dateformats->dateformat as $format )
			{
				$langID = ( string ) $format ['id'];
				$froamt = ( string ) $format;
				
				Site::$m_dateFormat [$langID] = $froamt;
			}
		}
		
		Site::IncludeFile ( 'classes/Common/View.class.inc' );
		// load static for view
		View::LoadSite ();
	}
	
	static public function GetAdditionFileLink()
	{
		$root = Site::GetStructureToFileRoot ();
		
		if ($root == 'files/')
		{
			return null;
		}
		
		return substr ( $root, 0, strlen ( $root ) - 6 );
	}
	
	static function ChackOffline()
	{
		$content = new Content ( 1 );
		$isOnline = $content->GetTitleShow ();
		
		if ( !$isOnline && DB::COOKIE("beta_login")!="ok" )
		{
			include_once 'offline.php';
			exit ();
		}
	}
	
	static public function GetConfigFilePath()
	{
		// getting file path
		$configFileName = 'siteconfig.xml';
		// might paths //  on server
		$path = null;
		
		if (file_exists ( $path ))
		{
			$path = $_SERVER ['DOCUMENT_ROOT'] . '/cms/config/' . $configFileName;
		}
		elseif (file_exists ( 'config/' . $configFileName ))
		{
			$path = 'config/' . $configFileName;
		}
		elseif (file_exists ( '../config/' . $configFileName ))
		{
			$path = '../config/' . $configFileName;
		}
		elseif (file_exists ( 'cms/config/' . $configFileName ))
		{
			$path = 'cms/config/' . $configFileName;
		}
		elseif (file_exists ( '../cms/config/' . $configFileName ))
		{
			$path = '../cms/config/' . $configFileName;
		}
		else
		{
			return false;
		}
		
		return $path;
	}
	
	function GetMaxImageWidth()
	{
		return $this->m_maxImageWidth;
	}
	
	function GetDateTransformation($date, $lang, $pattern = null)
	{
		// get patter from site configuration
		if ($pattern == null)
		{
			$pattern = Site::$m_dateFormat [$lang];
		}
		
		// replase
		$da = explode ( '-', $date );
		$formDate = mktime ( 0, 0, 0, $da [1], $da [2], $da [0] );
		
		// transformation of date
		$tDate = date ( $pattern, $formDate );
		
		// replase string, if it is not english version
		if ($lang != 'en')
		{
			$schars = array ('F', 'M', 'D', 'l', 'S' );
			
			foreach ( $schars as $schar )
			{
				if (strpos ( $pattern, $schar ) !== false)
				{
					$tDate = $this->GetDateTransValue ( $schar, $lang, $tDate );
				}
			}
		}
		
		return $tDate;
	}
	
	protected function GetDateTransValue($index, $lang, $tDate)
	{
		// load english and selected language data trnasformation files
		$langN = simplexml_load_file ( "langs/en/date.xml" );
		$langTrans = simplexml_load_file ( "langs/$lang/date.xml" );
		
		$trFrom = ( array ) $langN->$index;
		$trFrom = ( array ) $trFrom ['item'];
		
		$trTo = ( array ) $langTrans->$index;
		$trTo = ( array ) $trTo ['item'];
		
		foreach ( $trFrom as $key => $value )
		{
			if (strpos ( $tDate, $value ) !== false)
			{
				return str_replace ( $value, $trTo [$key], $tDate );
			}
		}
		
		return $tDate;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return array|null
	 */
	function GetTemplates()
	{
		return $this->m_templates;
	}
}
?>