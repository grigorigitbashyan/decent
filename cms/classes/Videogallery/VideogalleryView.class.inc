<?php
Site::IncludeFile('classes/Videogallery/Videogallery.class.inc');
Site::IncludeFile('classes/Videogallery/Item/VideogalleryItemList.class.inc');

class VideogalleryView extends View 
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
	 * This function displays videogallery title
	 *
	 * @param int $videogalleryID
	 */
	function GetVideogalleryName($videogalleryID)
	{
		$videogallery = $this->GetVideogallery($videogalleryID);
		return $videogallery->GetName($this->m_displayLang);
	}
	
	/**
	 * This function displays videogallery title
	 *
	 * @param int $videogalleryID
	 */
	function GetVideogalleryThumbnail($videogalleryID)
	{
		$videogallery = $this->GetVideogallery($videogalleryID);
		return $videogallery->GetThumbnail();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $videogalleryID
	 * @param string $pattern
	 * @param int $groupNumber
	 * @param string|array $groupStyle
	 * @param string $groupTag
	 * @param string $append
	 * @return string
	 */
	function GetVideogalleryItemsInPattern($videogalleryID, $pattern, $groupNumber = 0, $groupStyle = null, $groupTag = 'div', $append = null)
	{
		$videogallery = $this->GetVideogallery($videogalleryID);
		$itemsList = $videogallery->GetItemList();
		
		return $itemsList->GetPatternDesign($pattern, null, null, null, $groupNumber, $groupStyle, $groupTag, $append);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $videogalleryID
	 * @return VideogalleryItemList
	 */
	function GetVideogalleryItemList($videogalleryID)
	{
		$videogallery = $this->GetVideogallery($videogalleryID);
		return $videogallery->GetItemList();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $videogalleryID
	 * @return int
	 */
	function GetVideogalleryItemsCount($videogalleryID)
	{
		$videogallery = $this->GetVideogallery($videogalleryID);
		$itemsList = $videogallery->GetItemList();
		
		return $itemsList->GetItemCount();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $videogalleryID
	 * @return Videogallery
	 */
	function GetVideogallery($videogalleryID)
	{
		if(isset($this->m_galleres[$videogalleryID]))
		{
			return $this->m_galleres[$videogalleryID];
		}
		else 
		{
			$this->m_galleres[$videogalleryID] = new Videogallery($videogalleryID);
		}
		
		return $this->m_galleres[$videogalleryID];
	}
}
?>