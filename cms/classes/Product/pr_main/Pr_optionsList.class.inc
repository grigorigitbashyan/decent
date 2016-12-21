<?php
Site::IncludeFile ( 'classes/Common/TreeViewNavigation.class.inc' );

class Pr_optionsList extends TreeViewNavigation
{
	function __construct($productID, $fieldID)
	{
		// call parent construct
		parent::__construct ( null, 'optID');

		// sql query string
		$sql = "SELECT * FROM `pr_options` WHERE `productID` = $productID AND `fieldID` = $fieldID";	
		
		$this->SetQuery($sql);
	}
}
?>