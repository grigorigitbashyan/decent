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
	function __construct($productID, $gallery=-1, $limit=1000)
	{
		$dlang = View::GetDisplayLang ();
		
		if ( $gallery==0 || $gallery==1 )
		{
			$query = "SELECT `ID`, `path`, `iconPath`, `position`, `desc_$dlang` as `desc` 
				  FROM `pr_picture` 
				  WHERE `productID` = $productID and `gallery` = $gallery ORDER BY `position` LIMIT $limit";
		}
		else 
		{
			$query = "SELECT `ID`, `path`, `iconPath`, `position`, `desc_$dlang` as `desc` 
				  FROM `pr_picture` 
				  WHERE `productID` = $productID ORDER BY `position` LIMIT $limit";
		}
		
		parent::__construct ( $query, 'pr_pictureID', 'path' );
	}
}
?>