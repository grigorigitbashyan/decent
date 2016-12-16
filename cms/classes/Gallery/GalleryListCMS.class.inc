<?php
Site::IncludeFile('classes/Common/CMSListObject.class.inc');
//TODO corrent the include
Site::IncludeFile('classes/Gallery/GalleryCMS.class.inc');
Site::IncludeFile('classes/Gallery/GalleryList.class.inc');

/**
 * Enter class description
 */
class GalleryListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct (new GalleryList(), 'Gallery');
		$this->ReadNameStrings ( 'classes/Gallery/langs/', 'gallery.xml' );
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
		$moduleNameS = 'Gallery';//$this->GetNameString ( 'moduleName' );
		parent::DisplayMenu($moduleNameS);
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		// display Gallery
		if(!parent::DisplayFeatures())
		{
			// get language strings
			$doneS = $this->GetNameString('done');

			// display title
			$this->DisplayTitle('New Gallery');

			// display for for new Gallery
			$this->DisplaySingleTextForm('insert', null, 'name', null, null, $doneS);
		}
	}
}
?>