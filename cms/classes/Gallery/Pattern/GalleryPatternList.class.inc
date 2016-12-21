<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * Enter class description
 */
class GalleryPatternList extends TreeView
{
	/**
	 * Enter description here...
	*/
	function __construct()
	{
		parent::__construct ( null, 'gallery_patternID');

		$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name` FROM `gallery_pattern`";
		$this->SetQueryString($sql);
	
	}
}
?>