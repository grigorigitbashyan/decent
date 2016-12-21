<?php
// includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );
/**
 * Enter description here...
 *
 * @package CMSUser
 */
class UserTypeList extends TreeView
{
	/**
	 * Enter description here...
	 *
	 * @param int $privilegy
	 */
	function __construct($parentID = 0, $link = "usertype")
	{
		parent::__construct ( null, $link, 'name' );
		$this->SetQueryStringTree("usertype", 'parentID', null, null, $parentID, true);
	}
}
?>