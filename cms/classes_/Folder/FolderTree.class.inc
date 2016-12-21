<?php
/**
 * Enter description here...
 *
 * @package Folder
 */
class FolderTree extends TreeView
{
	/**
	 * Enter description here...
	 *
	 * @param int $securID
	 */
	function __construct($securID = 1)
	{
		$result = DB::QueryToTree ( "folder", "parentID", "  AND `fileOrFolder` = 0 ", null, $securID );
		parent::__construct ( $result, 'folderID' );
	}
}
?>