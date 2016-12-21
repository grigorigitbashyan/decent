<?php

Site::IncludeFile ( 'classes/Common/AddExtension.class.inc' );

Site::IncludeFile ( 'classes/ItemOptions/OptionsListCMS.class.inc' );
Site::IncludeFile ( 'classes/ItemOptions/OptionsdataListCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package ItemOptions
 */
class OprionsToOptionsdataCMS extends AddExtension implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @param bool $editMode
	 */
	function __construct($editMode = null)
	{
		parent::__construct ();
		
		$optionsList = new OptionsListCMS ( $editMode );
		$this->AddMainModule ( $optionsList );
		
		$opID = $optionsList->GetCurrentOptions ();
		
		$this->AddModuleExtension ( new OptionsdataListCMS ( $opID ) );
	}
}
?>