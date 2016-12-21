<?php
/**
 * Mearging 2 or more modules
 *
 * @package Main
 */
class MergingModules extends CMSObject
{
	
	/**
	 * Array of CMSObjects
	 *
	 * @var array
	 */
	protected $m_modules;
	
	/**
	 * marge name for GET overloading
	 *
	 * @var string
	 */
	protected $m_mergeName;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_subSelect;
	
	/**
	 * @param string $mergeName
	 *@return CMSObject 
	 */
	function __construct($mergeName = 'submenu')
	{
		parent::__construct ();
		$this->m_modules = array ();
		
		// set marge name for GET overloading
		$this->m_mergeName = $mergeName;
		
		// get sub menu select
		$this->m_subSelect = $this->m_site->GetValue ( $this->m_mergeName );
		
		if ($this->m_subSelect === null)
		{
			$this->m_subSelect = 1;
		}
	}
	
	/**
	 * 
	 * @see CMSInterfase::DisplayFeatures()
	 */
	function DisplayFeatures()
	{
		if (isset ( $this->m_modules [$this->m_subSelect - 1] ))
		{
			$module = $this->m_modules [$this->m_subSelect - 1] ['module'];
			
			if ($module instanceof CMSObject)
			{
				$module->DisplayFeatures ();
			}
		}
	}
	
	/**
	 * 
	 * @see CMSInterfase::DisplayMenu()
	 */
	function DisplayMenu()
	{
		foreach ( $this->m_modules as $key => $moduleStruct )
		{
			$name = $moduleStruct ['name'];
			$module = $this->m_modules [$key] ['module'];
			
			if (($key + 1) == $this->m_subSelect)
			{
				if ($module instanceof CMSObject)
				{
					$module->DisplayMenu ();
				}
			}
			else
			{
				if ($module instanceof CMSObject)
				{
					$keyPlus = $key + 1;
					print ( "<div class='someWTitleBox'><a href='?{$this->m_mergeName}=$keyPlus'>$name</a></div>" );
				}
			}
			print ( '<br />' );
		}
	}
	
	/**
	 * 
	 * @see CMSInterfase::DoAction()
	 */
	function DoAction()
	{
		if (isset ( $this->m_modules [$this->m_subSelect - 1] ))
		{
			$module = $this->m_modules [$this->m_subSelect - 1] ['module'];
			
			if ($module instanceof CMSObject)
			{
				$module->DoAction ();
			}
		}
	}
	
	/**
	 * 
	 */
	function __destruct()
	{
	
	}
	
	/**
	 * Enter description here...
	 *
	 * @param CMSObject $module
	 * @param string $displayName
	 */
	public function AddModule($module, $displayName)
	{
		array_push ( $this->m_modules, array ('module' => $module, 'name' => $displayName ) );
	}
	
	/**
	 * 
	 * @see CMSInterfase::CreateDB()
	 */
	function CreateDB()
	{
		$keys = array_keys ( $this->m_modules );
		
		foreach ( $keys as $key )
		{
			$module = $this->m_modules [$key] ['module'];
			
			if ($module instanceof CMSObject)
			{
				$module->CreateDB ();
			}
		}
	}
	
	/**
	 * 
	 * return bool
	 */
	function SelfTest()
	{
		$res = true;
		
		$keys = array_keys ( $this->m_modules );
		
		foreach ( $keys as $key )
		{
			$module = $this->m_modules [$key] ['module'];
			
			if (method_exists ( $module, 'SelfTest' ))
			{
				$res &= $module->SelfTest ();
			}
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		$res = null;
		
		$keys = array_keys ( $this->m_modules );
		
		foreach ( $keys as $key )
		{
			$module = $this->m_modules [$key] ['module'];
			
			if (method_exists ( $module, 'GetObjectTables' ))
			{
				$currTables = $module->GetObjectTables();
				if($currTables)
				{
					if(!$res)
					{
						$res = $currTables;
					}		
					else 
					{
						$res = array_merge($res, $currTables);
					}
				}
			}
		}
		
		if($res)
		{
			return array_unique($res);
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
		if (isset ( $this->m_modules [$this->m_subSelect - 1] ))
		{
			$module = $this->m_modules [$this->m_subSelect - 1] ['module'];
			
			if ($module instanceof CMSObject)
			{
				$res = $module->GetAjax ( $arg );
				
				if ($res)
				{
					return $res;
				}
			}
		}
		
		return null;
	}
}

?>