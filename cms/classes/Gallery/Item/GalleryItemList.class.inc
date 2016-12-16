<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');
/**
 * Enter class description
 */
class GalleryItemList extends TreeView
{
	/**
	 * Enter description here...
	*/
	function __construct($galleryID)
	{
		parent::__construct ( null, 'gallery_itemID');

		$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name`, `link_{$this->m_displayLang}` as `link`, `path`, `desc_{$this->m_displayLang}` as `desc` FROM `gallery_item` WHERE `galleryID` = $galleryID ORDER BY `position`";
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