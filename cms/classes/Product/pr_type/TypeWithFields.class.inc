<?php
Site::IncludeFile ( 'classes/Common/AddExtension.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_type/Pr_typeListCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_typefield/Pr_typefieldListCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class TypeWithFields extends AddExtension
{
	function __construct()
	{
		parent::__construct ();
		
		$productTypes = new Pr_typeListCMS ( );
		$this->AddMainModule ( $productTypes );
		
		$typeID = $productTypes->GetCurrentID ();
		$this->AddModuleExtension ( new Pr_typefieldListCMS ( $typeID ) );
	}
}
?>