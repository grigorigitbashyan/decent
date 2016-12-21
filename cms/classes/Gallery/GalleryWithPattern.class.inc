<?php

Site::IncludeFile('classes/Common/MergingModules.class.inc');
Site::IncludeFile('classes/Gallery/GalleryListCMS.class.inc');
Site::IncludeFile('classes/Gallery/Pattern/GalleryPatternListCMS.class.inc');

class GalleryWithPattern extends MergingModules 
{
	
	function __construct()
	{
		parent::__construct ();
	
		// add Gallery (main) module
		$this->AddModule(new GalleryListCMS(), 'Gallery');
		
		// add pattern (extantion) module
		$this->AddModule(new GalleryPatternListCMS(), 'Pattern');
	}
	
	
}

?>