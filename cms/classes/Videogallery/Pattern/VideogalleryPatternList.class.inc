<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * Enter class description
 */
class VideogalleryPatternList extends TreeView
{
	/**
	 * Enter description here...
	*/
	function __construct()
	{
		parent::__construct ( null, 'videogallery_patternID');

		$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name` FROM `videogallery_pattern`";
		$this->SetQueryString($sql);
	
	}
}
?>