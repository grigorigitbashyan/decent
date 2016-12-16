<?php
Site::IncludeFile('classes/Common/CMSListObject.class.inc');
//TODO corrent the include
Site::IncludeFile('classes/Gallery/Pattern/GalleryPatternCMS.class.inc');
Site::IncludeFile('classes/Gallery/Pattern/GalleryPatternList.class.inc');

/**
 * Enter class description
 */
class GalleryPatternListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct (new GalleryPatternList(), 'GalleryPattern');
		$this->ReadNameStrings ( 'classes/Gallery/langs/', 'gallery_pattern.xml' );
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
		$moduleNameS = 'GalleryPattern';//$this->GetNameString ( 'moduleName' );
		parent::DisplayMenu($moduleNameS);
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		// display GalleryPattern
		if(!parent::DisplayFeatures())
		{
			// get language strings
			$doneS = $this->GetNameString('done');

			// display title
			$this->DisplayTitle('New GalleryPattern');

			// display for for new GalleryPattern
			$this->DisplaySingleTextForm('insert', null, 'name', null, null, $doneS);
		}
	}
}
?>