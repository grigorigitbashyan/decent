<?php
Site::IncludeFile('classes/Gallery/Structure/SGRelation.class.inc');
Site::IncludeFile('classes/Gallery/Pattern/GalleryPattern.class.inc');
Site::IncludeFile('classes/Gallery/GalleryView.class.inc');

class GallerySite extends GalleryView
{
	function __construct($catID)
	{
		parent::__construct($catID);
		
		$this->m_catID = $catID;	
	}
	
	function DisplayContent()
	{
		$sg = new SGRelation($this->m_catID);
		
		$rels = $sg->GetStructure();
		
		if($rels && count($rels) > 0)
		{
			foreach ($rels as $gallInfo)
			{
				// get gallery
				$gallery = new Gallery($gallInfo['galleryID']);
				$galleryName = $gallery->GetName($this->m_displayLang);
				 
				$gli = new GalleryItemList($gallInfo['galleryID']);
				
				// if number of gallery items great then 0 then display it
				if($gli->GetItemCount())
				{
					// get pattern
					$patternObj = new GalleryPattern($gallInfo['patternID']);
					
					// display pattern header
					$header = $patternObj->GetHeader($this->m_displayLang);
					print str_replace('@@name@@', $galleryName, $header);
					
					// display gallery
					$gli->DisplayListDiv($patternObj->GetPattern($this->m_displayLang));
					
					// display pattern footer
					$footer = $patternObj->GetFooter($this->m_displayLang);
					print str_replace('@@name@@', $galleryName, $footer);
				}
			}
		}
	}
}
?>