<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * Enter class description
 */
class VideogalleryList extends TreeView
{
	/**
	 * Enter description here...
	*/
	function __construct()
	{
		parent::__construct ( null, 'videogalleryID');

		$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name` FROM `videogallery`";
		$this->SetQueryString($sql);
	
	}
}
?>