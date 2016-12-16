<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter class description
 * @package Item
 */
class Pr_pictureList extends TreeView
{
	/*
	 * Enter description ...
	 */
	function __construct($productID, $group=-1)
	{
		$dlang = View::GetDisplayLang ();
		
		if ( $group>-1 )
		{
			$query = "SELECT `ID`, `path`, `iconPath`, `position`, `desc_$dlang` as `desc`, `source_$dlang` as `source`, `url` 
				  FROM `pr_picture` 
				  WHERE `productID` = $productID and `group` = $group ORDER BY `position`";
		}
		else 
		{
			$query = "SELECT `ID`, `path`, `iconPath`, `position`, `desc_$dlang` as `desc`, `source_$dlang` as `source`, `url` 
				  FROM `pr_picture` 
				  WHERE `productID` = $productID ORDER BY `position`";
		}
		
		parent::__construct ( $query, 'pr_pictureID', 'path' );
	}
}
?>