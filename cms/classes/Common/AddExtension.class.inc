<?php
/**
 * Enter description here...
 *
 * @package Main
 */
class AddExtension extends CMSObject implements CMSInterface
{
	
	/**
	 * Enter description here...
	 *
	 * @var CMSObject
	 */
	protected $m_mainModule;
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_extensions;
	
	/**
	 * 
	 *@return AddExtension 
	 */
	function __construct()
	{
		parent::__construct ();
		$this->m_extensions = array ();
	}
	
	/**
	 * 
	 * @see CMSInterface::DisplayFeatures()
	 */
	function DisplayFeatures()
	{
		//call main module DisplayFeatures then all extansion's
		$currentID = $this->m_mainModule->DisplayFeatures ();
		
		// get all keys of extantions
		$keys = array_keys ( $this->m_extensions );
		
		foreach ( $keys as $key )
		{
			$this->m_extensions [$key]->DisplayFeatures ( $currentID );
		}
	}
	
	/**
	 * Display main CMSObject menu
	 * @see CMSInterface::DisplayMenu()
	 */
	function DisplayMenu()
	{
		//call main module DisplayMenu then all extansion's
		$this->m_mainModule->DisplayMenu ();
	}
	
	/**
	 * Call main CMSObject DoAction and then 
	 * do the same for all extentions
	 *  
	 * @see CMSInterface::DoAction()
	 */
	function DoAction()
	{
		//call main module DoAction then all extansion's
		$this->m_mainModule->DoAction ();
		
		// get all keys of extantions
		$keys = array_keys ( $this->m_extensions );
		
		foreach ( $keys as $key )
		{
			$this->m_extensions [$key]->DoAction ();
		}
	}
	
	/**
	 * Adding extension to array
	 *
	 * @param CMSObject $CMSObject
	 */
	function AddModuleExtension($CMSObject)
	{
		// if $CMSObject is sub class of CMSObject then add it to array
		if ($CMSObject instanceof CMSObject)
		{
			array_push ( $this->m_extensions, $CMSObject );
		}
	}
	
	/**
	 * Put main CMSObject
	 *
	 * @param CMSObject $CMSObject
	 */
	function AddMainModule($CMSObject)
	{
		// if $CMSObject is sub class of CMSObject then put it as main module
		if ($CMSObject instanceof CMSObject)
		{
			$this->m_mainModule = $CMSObject;
		}
	}
	
	function CreateDB()
	{
		//call main module DoAction then all extansion's
		$this->m_mainModule->CreateDB ();
		
		// get all keys of extantions
		$keys = array_keys ( $this->m_extensions );
		
		foreach ( $keys as $key )
		{
			$this->m_extensions [$key]->CreateDB ();
		}
	}
	
	/**
	 * 
	 */
	function SelfTest()
	{
		//call main module DisplayFeatures then all extansion's
		$res = $this->m_mainModule->SelfTest ();
		
		// get all keys of extantions
		$keys = array_keys ( $this->m_extensions );
		
		foreach ( $keys as $key )
		{
			if ($this->m_extensions [$key] instanceof CMSObject)
			{
				$res &= $this->m_extensions [$key]->SelfTest ();
			}
		}
		
		return $res;
	}
	
	/**
	 * 
	 */
	function GetObjectTables()
	{
		//call main module DisplayFeatures then all extansion's
		$res = $this->m_mainModule->GetObjectTables ();
		
		// get all keys of extantions
		$keys = array_keys ( $this->m_extensions );
		
		foreach ( $keys as $key )
		{
			if ($this->m_extensions [$key] instanceof CMSObject)
			{
				$currTables = $this->m_extensions [$key]->GetObjectTables ();
				if ($currTables)
				{
					if (! $res)
					{
						$res = $currTables;
					}
					else
					{
						$res = array_merge ( $res, $currTables );
					}
				}
			}
		}
		
		if ($res)
		{
			return array_unique ( $res );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param mixed $arg
	 * @return string
	 */
	function GetAjax($arg)
	{
		//call main module DisplayFeatures then all extansion's
		$res = $this->m_mainModule->GetAjax ( $arg );
		
		if ($res)
		{
			return $res;
		}
		
		// get all keys of extantions
		$keys = array_keys ( $this->m_extensions );
		
		foreach ( $keys as $key )
		{
			if ($this->m_extensions [$key] instanceof CMSObject)
			{
				$res = $this->m_mainModule->GetAjax ( $arg );
				
				if ($res)
				{
					return $res;
				}
			}
		}
		
		return null;
	}
	
	/**
	 * This function read extantions from XML file.
	 *
	 * @param string $xmlFileName
	 * @return bool
	 */
	function ReadExtFromXML($xmlFileName)
	{
		try
		{
			$xml = simplexml_load_file ( $xmlFileName );
			if (! $xml)
				throw new Exception ( "The file <b>$xmlFileName</b> does not exist or is not right file!" );
				
			// get extantions
			$extantions = $xml->xpath ( "extantion" );
			
			if (! $extantions)
				throw new Exception ( "The extantions in XML file must be decalred well!" );
				
			foreach ($extantions as $extantion)
			{
				$filePath = (string)$extantion;
				$className = (string)$extantion['className'];
				
				// include files and add new extantions classes
				Site::IncludeFile("classes/" . $filePath);
				$this->AddModuleExtension(new $className);
			}
			
		}
		catch ( Exception $e )
		{
			$message = $e->getMessage ();
			print ( $message );
		}
		
		return true;
	}
}

?>