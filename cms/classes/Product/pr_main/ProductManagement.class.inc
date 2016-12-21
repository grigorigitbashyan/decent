<?php
Site::IncludeFile('classes/Common/MergingModules.class.inc');
Site::IncludeFile('classes/ItemOptions/OprionsToOptionsdataCMS.class.inc');

Site::IncludeFile('classes/Product/pr_type/TypeWithFields.class.inc');
Site::IncludeFile('classes/Product/pr_field/Pr_fieldListCMS.class.inc');
Site::IncludeFile('classes/Product/pr_main/ProductCMS.class.inc');


/**
 * Enter description here...
 *
 * @package Item
 */
class ProductManagement extends MergingModules
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct();
		
		$currentUser = UserManagement::GetCurrentUser();
		if($currentUser)
		{
			$userType = $currentUser->GetUserTypeID();
			$editMode = ($userType == 1);
		}
		else 
		{
			$editMode = true;
		}
		 
		$this->AddModule(new OprionsToOptionsdataCMS($editMode), 'Options');
		
		if($editMode)
		{
			$this->AddModule(new Pr_fieldListCMS(), 'Field');
			$this->AddModule(new TypeWithFields(), 'Article Type');
		}
		
		$this->AddModule(new ProductCMS(), 'Articles');
	}
}
?>