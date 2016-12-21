<?php
Site::IncludeFile('classes/Common/CMSListObject.class.inc');
//TODO corrent the include
Site::IncludeFile('classes/Videogallery/VideogalleryCMS.class.inc');
Site::IncludeFile('classes/Videogallery/VideogalleryList.class.inc');

/**
 * Enter class description
 */
class VideogalleryListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct (new VideogalleryList(), 'Videogallery');
		$this->ReadNameStrings ( 'classes/Videogallery/langs/', 'videogallery.xml' );
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
		$moduleNameS = 'Videogallery';//$this->GetNameString ( 'moduleName' );
		parent::DisplayMenu($moduleNameS);
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		// display Videogallery
		if(!parent::DisplayFeatures())
		{
			// get language strings
			$doneS = $this->GetNameString('done');

			// display title
			$this->DisplayTitle('New Videogallery');

			// display for for new Videogallery
			$this->DisplaySingleTextForm('insert', null, 'name', null, null, $doneS);
		}
	}
}
?>