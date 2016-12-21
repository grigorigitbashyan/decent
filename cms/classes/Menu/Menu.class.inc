<?php
/** 
 * This class has more features for working with menus (structur).
 * the features are`
 * 1. create DB for menu
 * 2. create menu and set menu options (depth, show type)
 * 3. delete menu and all its referencies ` structurs and contents
 *
 * @package Menu
 */
class Menu extends NamedRecord
{
	/**
	 * this is a menu structure, it contnets menu IDs and their names (all languages)...
	 *
	 * @private array
	 */
	protected $m_structure;
	
	/**
	 *maximum depth of menu
	 *
	 * @private int
	 */
	protected $m_depth;
	
	/**
	 * thsi proprtie for draw (vertical, horizontal, ...)
	 *
	 * @private int
	 */
	protected $m_show_type;
	
	/**
	 * Constructuor
	 *
	 * @param array $menuID
	 * @param bool $bStructure
	 * @param string $lang
	 * @param bool $showHiddens
	 * @param bool $showSecur
	 * @return Menu
	 */
	function __construct($ID = null, $bStructure = true, $lang = 'en', $showHiddens = false, $showSecur = false) // set $bStructure false for queqly get menu information
	{
		parent::__construct ( $ID, 'menu' );
		
		$res = $this->Read ();
		
		// get structure, and sets in the structure property
		if ($res && $bStructure)
		{
			$this->m_structure = array ();
			$this->GetStructureFromDB ( $this->m_structure, 1, $lang, $showHiddens, $showSecur );
		}
	
	}
	
	/**
	 * create new menw and return ID of new creation
	 *
	 * @param string $name
	 * @param int $depth
	 * @param int $showType
	 * @return int
	 * 
	 */
	function Insert($name, $depth) // static function
	{
		$properties ['name'] = "$name";
		$properties ['depth'] = "$depth";
		
		// retun ID of last added tem
		return $this->InsertRecord ( $properties );
	}
	
	/**
	 * delete menu and all its referencies ` structurs and contents
	 *
	 * @return bool
	 */
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		// delete from structure
		$sql = "SELECT count(*) FROM `structure` WHERE `menuID`=>$this->m_ID";
		
		if (DB::QueryToValue( $sql ) > 0)
		{
			// return error code
			return false;
		}

		return $this->DeleteRecord ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $categoryis
	 * @param int $root
	 * @param string $lang
	 * @param bool $allVisible
	 * @param int $currentDeep
	 * @return array
	 */
	function GetStructureFromDB(&$categoryis, $root, $lang, $allVisible = true, $showSecur = false, $currentDeep = 1)
	{
		if ($currentDeep > $this->m_depth)
			return NULL;
		
		$name = "name_$lang"; /// select langucage
		

		$showH = ($allVisible) ? '' : ' AND `visible` = 1 ';
		$showS = ($showSecur) ? '' : ' AND `security` = 0 ';
		
		$query = "SELECT `ID`, `$name` as `name`, `ispathcache`, `path` FROM `structure` 
        		  WHERE `parentID` = $root AND menuID = $this->m_ID $showH $showS 
        		  ORDER BY `position`";
		
		$result = DB::QueryToArray ( $query );
		
		if ($result)
		{
			$array = array ();
			
			foreach ( $result as $row )
			{
				////////
				$arrayTemp = array ("ID" => $row ['ID'], "name" => $row ['name'], "ispathcache" => $row ['ispathcache'], "path" => $row ['path'], "ref" => NULL );
				$this->GetStructureFromDB ( $arrayTemp ['ref'], $row ['ID'], $lang, $allVisible, $showSecur, $currentDeep + 1 );
				
				array_push ( $array, $arrayTemp );
			}
			
			if (count ( $array ) == 0)
				return null;
			
			$categoryis = $array;
			//return $array;
		}
		
		return NULL;
	}
	
	/**
	 * Return depth of menu
	 *
	 * @return int
	 */
	function GetDepth()
	{
		return $this->m_depth;
	}
	/**
	 * Return structure
	 *
	 * @return Array
	 */
	function GetStructure()
	{
		return $this->m_structure;
	}
	
	/**
	 * Return sub menu, root is $subID;
	 * if category hasn`t sub menu return null
	 *
	 * @example $men = new Menu(2);
	 *             $sub = array();
	 *            $men->GetSubMenu(5, $sub, $men->structure);
	 * 
	 * @param int $subID
	 * @param array $subMenu
	 * @param array $structure
	 * 
	 * @return array
	 */
	function GetSubMenu($subID, &$subMenu, $structure = null, $withParent = false)
	{
		if ($structure == null)
			$structure = $this->m_structure;
		
		if($structure)
		{
			foreach ( $structure as $category )
			{
				if ($category ['ID'] == $subID)
				{
					if ($withParent)
					{
						$subMenu [0] = $category;
					}
					else
					{
						$subMenu = $category ['ref'];
					}
					
					return true;
				}
				
				if ($category ['ref'] != null)
				{
					$res = $this->GetSubMenu ( $subID, $subMenu, $category ['ref'] );
					if ($res == true)
						return true;
				}
			}
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $depth
	 * @return bool
	 */
	function SetDepth($depth)
	{
		return $this->UpdateValue('depth', $depth);	
	}
}
?>