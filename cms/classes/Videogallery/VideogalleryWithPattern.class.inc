<?php

Site::IncludeFile('classes/Common/MergingModules.class.inc');
Site::IncludeFile('classes/Videogallery/VideogalleryListCMS.class.inc');
Site::IncludeFile('classes/Videogallery/Pattern/VideogalleryPatternListCMS.class.inc');

class VideogalleryWithPattern extends MergingModules 
{
	
	function __construct()
	{
		parent::__construct ();
	
		// add Videogallery (main) module
		$this->AddModule(new VideogalleryListCMS(), 'Videogallery');
		
		// add pattern (extantion) module
		$this->AddModule(new VideogalleryPatternListCMS(), 'Pattern');
	}
	
	
}

?>