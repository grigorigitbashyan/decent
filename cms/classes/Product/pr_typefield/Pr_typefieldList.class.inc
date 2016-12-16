<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter class description
 * 
 * @package Item
 */
class Pr_typefieldList extends TreeView
{
	/**
	 * Enter description here...
	 *
	 * @param int $productTypeID
	 */
	function __construct($productTypeID)
	{
		if ($productTypeID != null)
		{
			$query = "
				SELECT 
					`PTF`.`ID` as `ID`, 
					`PTF`.`fieldID` as `fieldID`, 
					`PTF`.`single` as `single`, 
					`PTF`.`order` as `order`, 
					`PF`.`name` as `name`, 
					`PF`.`type` as `fieldType`, 
					`PF`.`multilang` 
				FROM 
					`pr_typefield` as `PTF` 
				INNER JOIN `pr_field` as `PF` ON `PTF`.`fieldID` = `PF`.`ID`  
				WHERE 
					`PTF`.`producttypeID` = $productTypeID
				ORDER BY
					`order` desc";
			
			parent::__construct ( $query, 'pr_typefieldID', 'name' );
		}
	}
	
	function HasField($productTypeID, $fieldID)
	{
		$query = "select `ID` from `pr_typefield` where `producttypeID` = $productTypeID and `fieldID` = $fieldID";
		$ptFieldID = DB::QueryToValue($query);
		
		return $ptFieldID;
	}
	
	function HasFieldInGroup($productTypeID, $fieldID)
	{
		$query = "
			select PF.`fields` from `pr_field` PF
			join `pr_typefield` PTF on (PF.`ID` = PTF.`fieldID`) 
			where PTF.`producttypeID` = $productTypeID and PF.`type` = 9";
		$fields = DB::QueryToValue($query);
		
		$fields = explode(",", $fields);
		if ( in_array($fieldID, $fields) )
		{
			return true;
		}
		
		return false;
	}
	
	function GetFieldSingle($productTypeID, $fieldID)
	{
		$query = "select `single` from `pr_typefield` where `producttypeID` = $productTypeID and `fieldID` = $fieldID";
		$single = DB::QueryToValue($query);
		
		return $single;
	}
}
?>