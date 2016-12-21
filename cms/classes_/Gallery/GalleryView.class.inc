<?php
Site::IncludeFile('classes/Gallery/Gallery.class.inc');
Site::IncludeFile('classes/Gallery/Item/GalleryItemList.class.inc');

class GalleryView extends View 
{
	protected $m_galleres;
	protected $m_catID;
	
	function __construct($catID)
	{
		parent::__construct();
		
		$this->m_catID = $catID;	
		$this->m_galleres = array();
	}
	
	/**
	 * This function displays gallery title
	 *
	 * @param int $galleryID
	 */
	function GetGalleryName($galleryID)
	{
		$gallery = $this->GetGallery($galleryID);
		return $gallery->GetName($this->m_displayLang);
	}
	
	/**
	 * This function displays gallery title
	 *
	 * @param int $galleryID
	 */
	function GetGalleryThumbnail($galleryID)
	{
		$gallery = $this->GetGallery($galleryID);
		return $gallery->GetThumbnail();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $galleryID
	 * @param string $pattern
	 * @param int $groupNumber
	 * @param string|array $groupStyle
	 * @param string $groupTag
	 * @param string $append
	 * @return string
	 */
	function GetGalleryItemsInPattern($galleryID, $pattern, $groupNumber = 0, $groupStyle = null, $groupTag = 'div', $append = null)
	{
		$gallery = $this->GetGallery($galleryID);
		$itemsList = $gallery->GetItemList();
		
		return $itemsList->GetPatternDesign($pattern, null, null, null, $groupNumber, $groupStyle, $groupTag, $append);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $galleryID
	 * @return GalleryItemList
	 */
	function GetGalleryItemList($galleryID)
	{
		$gallery = $this->GetGallery($galleryID);
		return $gallery->GetItemList();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $galleryID
	 * @return int
	 */
	function GetGalleryItemsCount($galleryID)
	{
		$gallery = $this->GetGallery($galleryID);
		$itemsList = $gallery->GetItemList();
		
		return $itemsList->GetItemCount();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $galleryID
	 * @return Gallery
	 */
	function GetGallery($galleryID)
	{
		if(isset($this->m_galleres[$galleryID]))
		{
			return $this->m_galleres[$galleryID];
		}
		else 
		{
			$this->m_galleres[$galleryID] = new Gallery($galleryID);
		}
		
		return $this->m_galleres[$galleryID];
	}
}
?>