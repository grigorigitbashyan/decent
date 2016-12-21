<?php
Site::IncludeFile('classes/Common/CMSListObject.class.inc');
//TODO corrent the include
Site::IncludeFile('classes/Videogallery/Pattern/VideogalleryPatternCMS.class.inc');
Site::IncludeFile('classes/Videogallery/Pattern/VideogalleryPatternList.class.inc');

/**
 * Enter class description
 */
class VideogalleryPatternListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct (new VideogalleryPatternList(), 'VideogalleryPattern');
		$this->ReadNameStrings ( 'classes/Videogallery/langs/', 'videogallery_pattern.xml' );
	}
	/**
	* All actions must be done hear
	*
	* @return bool
	*/
	function DoAction()
	{
		return parent::DoAction(true, true);
	}

	/**
	* Display menu
	*
	*/
	function DisplayMenu()
	{
		$moduleNameS = 'VideogalleryPattern';//$this->GetNameString ( 'moduleName' );
		parent::DisplayMenu($moduleNameS);
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		// display VideogalleryPattern
		if(!parent::DisplayFeatures())
		{
			// get language strings
			$doneS = $this->GetNameString('done');

			// display title
			$this->DisplayTitle('New VideogalleryPattern');

			// display for for new VideogalleryPattern
			$this->DisplaySingleTextForm('insert', null, 'name', null, null, $doneS);
		}
	}
}
?>