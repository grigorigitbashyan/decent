<?php
Site::IncludeFile('classes/Videogallery/Videogallery.class.inc');
Site::IncludeFile('classes/Videogallery/Item/VideogalleryItemList.class.inc');

class VideogalleryViewSite extends View 
{
	protected $m_galleres;
	
	function __construct()
	{
		parent::__construct();
		
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
	 * @return string
	 */
	function GetVideogalleryItemsInPattern($videogalleryID, $pattern)
	{
		$videogallery = $this->GetVideogallery($videogalleryID);
		$itemsList = $videogallery->GetItemList();
		
		return $itemsList->GetPatternDesign($pattern);
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
	
	function GetLastVideos($videogalleryID=14, $limit=3, $expID=0)
	{
		$query = "
					select `ID`, `videoID`, `videoCode`, `name_".$this->m_displayLang."` as `name`, `desc_".$this->m_displayLang."` as `desc` 
					from `videogallery_item` 
					where `videogalleryID` = $videogalleryID and `hide` = 0 and `videoCode` <> '' and `ID` <> ".intval($expID)." 
					order by `position` asc, `ID` desc 
					limit 0, $limit
		";
		
		$retArr = DB::QueryToArray($query);
		
		return $retArr;
	}
	
	function GetVideo( $videogallery=14, $videoID )
	{
		$query = "
					select `ID`, `videoID`, `videoCode`, `name_".$this->m_displayLang."` as `name`, `desc_".$this->m_displayLang."` as `desc` 
					from `videogallery_item` 
					where `videogalleryID` = $videogallery and `videoID` = '".$videoID."' 
		";
		
		$retArr = DB::QueryToArray($query);
		
		return $retArr;
	}
}
?>