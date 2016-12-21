<?php
// 	includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter description here...
 *
 * @package Content
 */
class ContentTypeList extends TreeView
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ( null, 'contentTypeID');
		
		$query = "SELECT * FROM `contenttype`";
		$this->SetQueryString($query);
	}
}
?>