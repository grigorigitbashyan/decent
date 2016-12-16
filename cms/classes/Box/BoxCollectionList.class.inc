<?php
// 	includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter description here...
 *
 * @package Box
 */
class BoxCollectionList extends TreeView {
	function __construct()
	{
		$query = "SELECT * FROM `boxcollection`";
		parent::__construct ( $query, 'boxCollectionID');
	}
}
?>