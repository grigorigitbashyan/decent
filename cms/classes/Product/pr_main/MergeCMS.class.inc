<?php

Site::IncludeFile('classes/Common/MergingModules.class.inc');
Site::IncludeFile('classes/Product/pr_main/ShoppingOptionsCMS.class.inc');
Site::IncludeFile('classes/Product/pr_main/ShoppingCartListCMS.class.inc');

class MergeCMS extends MergingModules 
{
	function __construct()
	{
		parent::__construct();
		
		$this->AddModule(new ShoppingCartListCMS(), 'Shopping Cart Info');
		$this->AddModule(new ShoppingOptionsCMS(), 'Shopping options');
	}	
}
?>