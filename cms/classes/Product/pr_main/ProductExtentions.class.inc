<?php
$includes = array ('classes/Common/AddExtension.class.inc', 'classes/Product/pr_main/Pr_mainCMS.class.inc', 'classes/Product/pr_picture/Pr_pictureListCMS.class.inc' );

Site::IncludeFiles ( $includes );

/**
 * Enter description here...
 *
 * @package Item
 */
class ProductExtentions extends AddExtension implements CMSInterface
{
	function __construct($currentPr_mainID)
	{
		parent::__construct ();
		
		$this->AddMainModule ( new Pr_mainCMS ( $currentPr_mainID ) );
		$this->AddModuleExtension ( new Pr_pictureListCMS ( $currentPr_mainID ) );
	}
}
?>