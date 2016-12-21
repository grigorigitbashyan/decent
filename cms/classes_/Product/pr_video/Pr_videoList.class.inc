<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter class description
 * @package Item
 */
class Pr_videoList extends TreeView
{
	/*
	 * Enter description ...
	 */
	function __construct($productID, $group=-1)
	{
		$dlang = View::GetDisplayLang ();
		
		if ( $group>-1 )
		{
			$query = "SELECT `ID`, `link`, `desc_$dlang` as `desc`  
				  FROM `pr_video` 
				  WHERE `productID` = $productID and `group` = $group ORDER BY `ID`";
		}
		else 
		{
			$query = "SELECT `ID`, `link`, `desc_$dlang` as `desc`, `group` 
				  FROM `pr_video` 
				  WHERE `productID` = $productID ORDER BY `ID`";
		}
		
		parent::__construct ( $query, 'pr_videoID', 'link' );
	}
}
?>