<?php
Site::IncludeFile('classes/Videogallery/Structure/SGVideoRelation.class.inc');
Site::IncludeFile('classes/Videogallery/Pattern/VideogalleryPattern.class.inc');
Site::IncludeFile('classes/Videogallery/VideogalleryView.class.inc');

Site::IncludeFile('classes/Videogallery/Item/VideogalleryItem.class.inc');

Site::IncludeFile('classes/Common/DB.class.inc');

class VideogallerySite extends VideogalleryView
{
	function __construct($catID)
	{
		parent::__construct($catID);
		
		$this->m_catID = $catID;	
	}
	
	function DisplayContent()
	{
		$sg = new SGVideoRelation($this->m_catID);
		
		$rels = $sg->GetStructure();
		
		if($rels && count($rels) > 0)
		{
			foreach ($rels as $gallInfo)
			{
				// get videogallery
				$videogallery = new Videogallery($gallInfo['videogalleryID']);
				$videogalleryName = $videogallery->GetName($this->m_displayLang);
				
				$pager = DB::GET("p3");
				if ( $pager=="" || $pager=="page1" ) {
					
					$pagerItem = 0;
				} else {
					
					$pagerItem = intval(substr($pager, 4))-1;
				}
				
				$pageItemCount = 10;		//	number of items to show per page
				
				$limitStr = "limit ".($pagerItem*$pageItemCount).", ".$pageItemCount;
				 
				$gli = new VideogalleryItemList($gallInfo['videogalleryID'], 1, $limitStr);		//	getting videos for current page
				$gliAll = new VideogalleryItemList($gallInfo['videogalleryID'], 1);				//	getting all videos
				
				// if number of videogallery items more then 0 then display it
				if($gli->GetItemCount())
				{
					// get pattern
					$patternObj = new VideogalleryPattern($gallInfo['patternID']);
					
					// display pattern header
					$header = $patternObj->GetHeader($this->m_displayLang);
					print str_replace('@@name@@', $videogalleryName, $header);
					
					// display videogallery
					$gli->DisplayListDiv($patternObj->GetPattern($this->m_displayLang));
					
					// display pattern footer
					$footer = $patternObj->GetFooter($this->m_displayLang);
					print str_replace('@@name@@', $videogalleryName, $footer);
				}
				
				$PagerCount = ceil($gliAll->GetItemCount() / $pageItemCount);		//	number of pages
				if ( $PagerCount>1 ) {		//	show pager if number of pages is more then 1
					
					?>
					<div style="text-align:center;">
					<?php
					for ( $i=0; $i<$PagerCount; $i++ ) {
						
						if ( $i==$pagerItem ) {				//	if current page then show without link
							
							echo "<span style='padding:0px 2px 0px 2px;'>".($i+1)."</span>";
						} else {
							
							echo "<span style='padding:0px 2px 0px 2px;'><a href='ru/video-gallery/page".($i+1)."/'>".($i+1)."</a></span>";
						}
					}
					?>
					</div>
					<?php
				}
				
			}
		}
	}
}
?>