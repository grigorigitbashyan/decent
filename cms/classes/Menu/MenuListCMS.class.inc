<?php
Site::IncludeFile ( 'classes/Common/CMSListObject.class.inc' );

Site::IncludeFile ( 'classes/Menu/MenuCMS.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuList.class.inc' );

/**
 * Enter description here...
 *
 * @package Menu
 */
class MenuListCMS extends CMSListObject
{
	function __construct()
	{
		parent::__construct ( new MenuList ( ), 'Menu' );
	}
	
	function DoAction()
	{
		parent::DoAction(true, true);
	}
}
?>