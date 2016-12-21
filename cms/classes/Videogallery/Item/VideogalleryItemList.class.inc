<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * Enter class description
 */
class VideogalleryItemList extends TreeView
{
	/**
	 * Enter description here...
	*/
	function __construct($videogalleryID, $hide=0, $limit="")
	{
		parent::__construct ( null, 'videogallery_itemID');
		
		if ( $hide==0 ) {
			
			$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name`, `link_{$this->m_displayLang}` as `link`, `path`, `videoID`, `videoCode`, `desc_{$this->m_displayLang}` as `desc` FROM `videogallery_item` WHERE `videogalleryID` = $videogalleryID ORDER BY `position` ".$limit;
		} else {
			
			$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name`, `link_{$this->m_displayLang}` as `link`, `path`, `videoID`, `videoCode`, `desc_{$this->m_displayLang}` as `desc` FROM `videogallery_item` WHERE `videogalleryID` = $videogalleryID AND `hide` = 0 ORDER BY `position` ".$limit;
		}
		$this->SetQueryString($sql);
		
		$this->CalcThumbnails();
	}
	
	function CalcThumbnails()
	{
		if($this->m_struct && count($this->m_struct))
		{
			foreach ($this->m_struct as $key => $item)
			{
				$dirName = dirname($item['path']);
				$baseName = basename($item['path']);
	
				$this->m_struct[$key]['thumbnail'] = $dirName . '/thumbnail_' . $baseName; 
			}
		}
	}
}
?>