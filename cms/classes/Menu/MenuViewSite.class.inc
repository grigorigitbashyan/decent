<?php
// includes
Site::IncludeFile ( 'classes/Menu/Menu.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuView.class.inc' );

/**
 * Enter description here...
 * @package Menu
 * @author Vazgen Manukyan
 * @example a.php
 */
class MenuViewSite extends View
{
	/**
	 * Enter description here...
	 *
	 * @var Category
	 */
	protected $m_category;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_securCode;
	
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_manus;
	
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_params;
	/**
	 * Enter description here...
	 *
	 * @param Category $category
	 * @param string $lang
	 * @param string $securCode
	 */
	function __construct($category, $lang, $params = null, $securCode = null)
	{
		parent::__construct ();
		
		$this->m_category = $category;
		$this->m_params = $params;
		$this->m_securCode = $securCode;
		$this->m_displayLang = $lang;
		$this->m_manus = array ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param string $spliter
	 * @param string|array $spanStyle
	 * @param string|array $spanStyleSelect
	 * @param string|array $selectedButLink
	 * @param bool $currentToLink
	 */
	function DisplayHorizontal($menuID, $spliter = ' | ', $spanStyle = null, $spanStyleSelect = null, $selectedButLink = null, $currentToLink = false, $tagName = 'span')
	{
		// getting menu structure
		$structure = $this->GetMenuStructure ( $menuID );
		
		// createing MneuView object
		$menuView = new MenuView ( $structure );
		
		$selectAreNot = ($currentToLink && ! empty ( $this->m_params [0] ));
		
		// display horizontal menu
		$menuView->DisplayHorizontal ( $this->m_category, $spliter, $spanStyle, $spanStyleSelect, $selectedButLink, $selectAreNot, $tagName );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param string $spliter
	 * @param string|array $spanStyle
	 * @param string|array $spanStyleSelect
	 * @param string|array $selectedButLink
	 * @param bool $currentToLink
	 */
	function DisplayHorizontalWithSubs($menuID, $spliter = ' | ', $spanStyle = null, $spanStyleSelect = null, $selectedButLink = null, $currentToLink = false, $tagName = 'span', $startDepth = 1, $innerTag1 = null, $innerTag1Class = null, $innerTag2 = null, $innerTag2Class = null, $innerTagMain = null, $innerTagMainClass = null, $innerTagMainActiveClass = null, $showTop = 0)
	{
		// getting menu structure
		$structure = $this->GetMenuStructure ( $menuID );
		
		// createing MenuView object
		$menuView = new MenuView ( $structure );
		
		$selectAreNot = ($currentToLink && ! empty ( $this->m_params [0] ));
		
		// display horizontal menu
		$menuView->DisplayHorizontalWithSubs ( $this->m_category, $spliter, $spanStyle, $spanStyleSelect, $selectedButLink, $selectAreNot, $tagName, $startDepth, $innerTag1, $innerTag1Class, $innerTag2, $innerTag2Class, $innerTagMain, $innerTagMainClass, $innerTagMainActiveClass, $showTop );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param string $spliter
	 * @param string|array $spanStyle
	 * @param string|array $spanStyleSelect
	 * @param string|array $selectedButLink
	 * @param bool $currentToLink
	 */
	function DisplayHorizontalWithSubsNew($menuID, $spliter = ' | ', $spanStyle = null, $spanStyleSelect = null, $selectedButLink = null, $currentToLink = false, $tagName = 'span', $startDepth = 1, $innerTag1 = null, $innerTag1Class = null, $innerTag2 = null, $innerTag2Class = null, $innerTagMain = null, $innerTagMainClass = null, $innerTagMainActiveClass = null, $showTop = 0)
	{
		// getting menu structure
		$structure = $this->GetMenuStructure ( $menuID );
		
		// createing MenuView object
		$menuView = new MenuView ( $structure );
		
		$selectAreNot = ($currentToLink && ! empty ( $this->m_params [0] ));
		
		// display horizontal menu
		$menuView->DisplayHorizontalWithSubsNew ( $this->m_category, $spliter, $spanStyle, $spanStyleSelect, $selectedButLink, $selectAreNot, $tagName, $startDepth, $innerTag1, $innerTag1Class, $innerTag2, $innerTag2Class, $innerTagMain, $innerTagMainClass, $innerTagMainActiveClass, $showTop );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param bool $allOpen
	 * @param array $styles
	 * @param int $startDepth
	 * @param int $depth
	 * @param string|array $selectedButLink
	 * @param bool $currentToLink
	 * 
	 * @return bool
	 */
	function DisplaySubHorizontal($menuID, $spliter = ' | ', $spanStyle = null, $spanStyleSelect = null, $startDepth = 1, $selectedButLink = null, $currentToLink = false, $tagName = 'span')
	{
		// display left menu
		$menu = $this->GetMenu ( $menuID );
		
		$subIDs = array ();
		$this->m_category->GetPathCategories ( $subIDs );
		
		if (isset ( $subIDs [$startDepth - 1] ))
		{
			$subID = $subIDs [$startDepth - 1] ['ID'];
		}
		else
		{
			return false;
		}
		
		$subStructure = array ();
		$structure = $menu->GetStructure ();
		$menu->GetSubMenu ( $subID, $subStructure, $structure );
		
		if (count ( $subStructure ) == 0)
		{
			return false;
		}
		//
		

		$selectAreNot = ($currentToLink && ! empty ( $this->m_params [0] ));
		$menuView = new MenuView ( $subStructure );
		$menuView->DisplayHorizontal ( $this->m_category, $spliter, $spanStyle, $spanStyleSelect, $selectedButLink, $selectAreNot, $tagName );
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param bool $allOpen
	 * @param array $styles
	 * @param int $depth
	 * @param bool $currentToLink
	 */
	function DisplayVertical($menuID, $allOpen = true, $styles = null, $depth = null, $currentToLink = false, $tagName = 'div', $displayIcon = false)
	{
		// display left menu
		$structure = $this->GetMenuStructure ( $menuID );
		$menuView = new MenuView ( $structure );
		
		$displayStyle = ($allOpen) ? 1 : 3;
		
		$selectAreNot = ($currentToLink && ! empty ( $this->m_params [0] ));
		$menuView->Display ( $this->m_category, $displayStyle, $styles, $depth, $selectAreNot , $tagName, $displayIcon );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param bool $allOpen
	 * @param array $styles
	 * @param int $startDepth
	 * @param int $depth
	 * @param bool $currentToLink
	 * 
	 * @return bool
	 */
	function DisplaySubVertical($menuID, $allOpen = true, $styles = null, $startDepth = 1, $depth = null, $currentToLink = false, $tag = 'div', $displayIcon = false, $subTagMain = '', $subTag = '', $toSend=null)
	{
		// display left menu
		$menu = $this->GetMenu ( $menuID );
		
		$subIDs = array ();
		$this->m_category->GetPathCategories ( $subIDs );
		
		if (isset ( $subIDs [$startDepth - 1] ))
		{
			$subID = $subIDs [$startDepth - 1] ['ID'];
		}
		else
		{
			return false;
		}
		
		$subStructure = array ();
		$structure = $menu->GetStructure ();
		$menu->GetSubMenu ( $subID, $subStructure, $structure );
		
		if (count ( $subStructure ) == 0)
		{
			return false;
		}
		//
		$menuView = new MenuView ( $subStructure );
		
		$displayStyle = ($allOpen) ? 1 : 3;
		$selectAreNot = ($currentToLink && ! empty ( $this->m_params [0] ));
		
		$menuView->Display ( $this->m_category, $displayStyle, $styles, $depth, $selectAreNot, $tag, $displayIcon, $subTagMain, $subTag, $toSend );
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param int $startDepth
	 * @return bool
	 */
	function HasSubDisplay($menuID, $startDepth = 1)
	{
		// display left menu
		$menu = $this->GetMenu ( $menuID );
		
		$subIDs = array ();
		$this->m_category->GetPathCategories ( $subIDs );
		
		if (isset ( $subIDs [$startDepth - 1] ))
		{
			$subID = $subIDs [$startDepth - 1] ['ID'];
		}
		else
		{
			return false;
		}
		
		$subStructure = array ();
		$structure = $menu->GetStructure ();
		$menu->GetSubMenu ( $subID, $subStructure, $structure );
		
		if (count ( $subStructure ) > 0)
		{
			return $subID;
		}
		
		return 0;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param bool $showHiddens
	 * @return array
	 */
	protected function GetMenuStructure($menuID, $showHiddens = false)
	{
		// if menu has already seted then retun from array
		if (isset ( $this->m_manus [$menuID] ))
		{
			return $this->m_manus [$menuID]->GetStructure ();
		}
		
		// get menu from db and set it to array
		$menu = new Menu ( $menuID, true, $this->m_displayLang, $showHiddens, $this->m_securCode );
		$this->m_manus [$menuID] = $menu;
		
		// return menu structure
		return $this->m_manus [$menuID]->GetStructure ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 * @param bool $showHiddens
	 * @return Menu
	 */
	function GetMenu($menuID, $showHiddens = false)
	{
		// if menu has already seted then retun from array
		if (isset ( $this->m_manus [$menuID] ))
		{
			return $this->m_manus [$menuID];
		}
		
		// get menu from db and set it to array
		$menu = new Menu ( $menuID, true, $this->m_displayLang, $showHiddens, $this->m_securCode );
		$this->m_manus [$menuID] = $menu;
		
		// return menu structure
		return $this->m_manus [$menuID];
	}
}
?>