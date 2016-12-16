<?php

include_once 'Menu/MenuList.class.inc';

global $lang, $menuViewSite;

$styleleft [1] = array ("divclass" => "siteMapMenu1", "linkclass" => null );
$styleleft [2] = array ("divclass" => "siteMapMenu2", "linkclass" => null );
$styleleft [3] = array ("divclass" => "siteMapMenu3", "linkclass" => null );
$styleleft [4] = array ("divclass" => "siteMapMenu4", "linkclass" => null );
$styleleft [5] = array ("divclass" => "siteMapMenu5", "linkclass" => null );

// get all menus
$menuList = new MenuList ( );
$struc = $menuList->GetStructure ();

foreach ( $struc as $menu )
{
	if ($menu ['ID'] > 1)
	{
		$menuViewSite->DisplayVertical ( $menu ['ID'], true, $styleleft );
	}
}
?>