<?php
// 	includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter description here...
 *
 * @package Box
 */
class BoxList extends TreeView
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		$query = "SELECT * FROM `box`";
		parent::__construct ( $query, 'boxID');
	}
}
?>