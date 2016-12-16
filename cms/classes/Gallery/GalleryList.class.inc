<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * Enter class description
 */
class GalleryList extends TreeView
{
	/**
	 * Enter description here...
	*/
	function __construct()
	{
		parent::__construct ( null, 'galleryID');

		$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name` FROM `gallery`";
		$this->SetQueryString($sql);
	
	}
}
?>