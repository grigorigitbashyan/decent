<?php
Site::IncludeFile ( 'classes/Common/ControllerBase.class.inc' );
/**
 * This class is parent class for all View classes
 *
 * @package Main
 */
class View extends ControllerBase
{
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_nameStrings;
	
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	protected $m_displayLang;
	
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	private static $m_displayLangStatic;
	
	/**
	 * Enter description here...
	 *
	 * @var Site
	 */
	protected $m_site;
	
	private static $m_siteStatic;
	/**
	 * Enter description here...
	 *
	 * @param string $displayLang
	 * @return View
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->m_site = View::$m_siteStatic;
		$this->m_displayLang = View::$m_displayLangStatic;
		
		// it is first time and language is not set , then set language
		if (! $this->m_displayLang)
		{
			$this->m_displayLang = $this->m_site->GetValue ( 'lang' );
			
			if (! $this->m_displayLang)
				$this->m_displayLang = $this->m_site->GetDefaultLanguage ();
			
			View::SetDisplayLang ( $this->m_displayLang );
		}
	}
	
	static final public function LoadSite()
	{
		$site = new Site ();
		$site->LoadConfiguration ();
		
		View::$m_siteStatic = $site;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return Site
	 */
	static final public function GetSite()
	{
		return View::$m_siteStatic;
	}
	
	/**
	 * return display language
	 *
	 * @return string
	 */
	static function GetDisplayLang()
	{
		return View::$m_displayLangStatic;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $lang
	 */
	static final function SetDisplayLang($lang)
	{
		View::$m_displayLangStatic = $lang;
	}
	
	/**
	 * Read names from XML file, root start from Site root 
	 *
	 * @param string $filePath
	 */
	public function ReadNameStrings($filePath, $fileName, $currentLang = null, $readByDOM = true)
	{
		if (! $currentLang)
		{
			$currentLang = $this->m_displayLang;
		}
		
		// get file full name
		$docRoot = $_SERVER ['DOCUMENT_ROOT'] . '/';
		$sfRoot = Site::GetStructureToFileRoot ();
		
		if ($sfRoot != 'files/')
		{
			$lenght = strlen ( $sfRoot );
			$docRoot .= substr ( $sfRoot, 0, $lenght - 6 );
		}
		
		///////////////////////////////////////////////////////////////
		
		// abs path
		$fullPath = $docRoot . $filePath . "$currentLang/" . $fileName;
		
		if (! file_exists ( $fullPath ))
		{
			$fullPath1 = $docRoot . 'cms/' . $filePath . "$currentLang/" . $fileName;
			if (file_exists ( $fullPath1 ))
			{
				$fullPath = $fullPath1;
			}
		}
		
		//
		try
		{
			// if file does not exist, then an exception would be generated
			if (! file_exists ( $fullPath ))
			{
				// relative path
				$relPath = $filePath . "$currentLang/" . $fileName;
				
				// if class ExceptionMissFile exist the generate it's instance
				if (class_exists ( 'ExceptionMissFile' ))
				{
					throw new ExceptionMissFile ( $relPath, $relPath );
				}
				// generates regular exception
				else
				{
					throw new Exception ( $relPath );
				}
			}
		}
		catch ( Exception $e )
		{
			print ("The missed file: " . $e->getMessage () . "<br />\n") ;
			return false;
		}
		
		if ($readByDOM)
		{
			$this->m_nameStrings = View::ReadNamesDOM ( $fullPath );
		}
		else
		{
			$this->m_nameStrings = simplexml_load_file ( $fullPath );
		}
	
	}
	
	static final protected function ReadNamesDOM($filePath)
	{
		$names = array ();
		
		// create DOM document
		$dom = new DOMDocument ();
		$dom->load ( $filePath );
		
		$root = $dom->documentElement;
		
		if ($root->hasChildNodes ())
		{
			$children = $root->childNodes;
			
			foreach ( $children as $node )
			{
				if ($node->hasAttributes ())
				{
					$attributes = $node->attributes;
					
					if ($node->nodeName == 'name')
					{
						$attr = $attributes->getNamedItem ( "id" );
						
						if ($attr)
						{
							$arValue = ( string ) $attr->nodeValue;
							$names [$arValue] = ( string ) $node->nodeValue;
						}
					}
				}
			}
		}
		
		return $names;
	}
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @return string
	 */
	function GetNameString($key)
	{
		if (isset ( $this->m_nameStrings [$key] ))
		{
			return $this->m_nameStrings [$key];
		}
		
		return null;
	}
}
?>