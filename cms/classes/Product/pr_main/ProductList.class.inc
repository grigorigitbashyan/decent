<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeViewNavigation.class.inc' );

/**
 * Enter class description
 * @package Item
 */
class ProductList extends TreeViewNavigation
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_categoryID;
	
	/**
	 * Enter description here...
	 *
	 * @param int $productType
	 * @param int $limits
	 * @param array $externalParmas
	 */
	function __construct($categoryID = null, $limit = 5, $externalParmas = null)
	{
		parent::__construct ( null, 'pr_mainID', $limit);
		
		// sort external perimeters
		if (empty($externalParmas))
		{
			$externalParmas = array ();
			$externalParmas ['sortby'] = 'date';
			$externalParmas ['sorttype'] = 'DESC';
		}
		
		$this->SetExternalParams($externalParmas);
		
		if ( $categoryID )
		{
			$this->m_categoryID = $categoryID;
			$currCat = new Category($categoryID);
			
			$query = "select `ID` from `options` where `category_id` = ".$categoryID;
			$optionID = DB::QueryToValue($query);
			if ( $optionID>0 )
			{
				if ( $externalParmas[0] )
				{
					$sql = "
						SELECT PM.`ID`, PM.`typeID`, PM.`name_{$this->m_displayLang}` as `name`, PM.`subtitle_{$this->m_displayLang}` as `subtitle`, PM.`desc_{$this->m_displayLang}` as `desc`, PM.`short_desc_{$this->m_displayLang}` as `short_desc`, PM.`picture`, PM.`cover`, PM.`date` as `sdate`, PM.`date`, PM.`random`, PM.`viewCount` 
						FROM `pr_main` PM 
						JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
						JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID` and OD.`key` = '".$externalParmas[0]."' and OD.`optionsID` = $optionID) 
						WHERE PM.`status` = 1 
						GROUP BY PM.`ID`";
				}
				else 
				{
					$sql = "
						SELECT PM.`ID`, PM.`typeID`, PM.`name_{$this->m_displayLang}` as `name`, PM.`subtitle_{$this->m_displayLang}` as `subtitle`, PM.`desc_{$this->m_displayLang}` as `desc`, PM.`short_desc_{$this->m_displayLang}` as `short_desc`, PM.`picture`, PM.`cover`, PM.`date` as `sdate`, PM.`date`, PM.`random`, PM.`viewCount` 
						FROM `pr_main` PM 
						JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
						JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID` and OD.`optionsID` = $optionID) 
						WHERE PM.`status` = 1 
						GROUP BY PM.`ID`";
				}
			}
			else 
			{
                $moreFields = "";
                $langs = $this->m_site->GetLanguages();
                foreach ( $langs as $lang )
                {
                    $moreFields .= "
                        PM.`name_$lang` as `name_$lang`, 
						PM.`desc_$lang` as `desc_$lang`, 
						PM.`short_desc_$lang` as `short_desc_$lang`, 
						PM.`lang_$lang` as `lang_$lang`, 
                    ";
                }

				$sql = "
					SELECT 
						PM.`ID`, 
						PM.`typeID`, 
						$moreFields
						PM.`name_{$this->m_displayLang}` as `name`, 
						PM.`subtitle_{$this->m_displayLang}` as `subtitle`, 
						PM.`desc_{$this->m_displayLang}` as `desc`, 
						PM.`short_desc_{$this->m_displayLang}` as `short_desc`, 
						PM.`picture`, 
						PM.`cover`, 
						PM.`date`, 
						PM.`random`, 
						PM.`viewCount`, 
						PM.`language` 
					  FROM 
					  	`pr_main` PM 
					  JOIN 
					  	`pr_type` PT 
					  WHERE 
					  	(
					  		(
					  			PM.`ID` in (select PO.`productID` from `pr_options` PO join `pr_field` PF on (PO.`fieldID` = PF.`ID`) where (PF.`type` = 2 or PF.`type` = 8) and PO.`relatedID` = $categoryID)
					  		) or (PT.`structureID` = $categoryID and PM.`typeID` = PT.`ID`)
					  	) and PM.`status` = 1
					  GROUP BY 
					  	PM.`ID`";
			}
			
			$this->SetQuery($sql);
		}
	}
	
	function GetAllProducts($typeID=null, $limit=100, $orderBy="`name`", $orderType="desc", $status=1)
	{
		$catStr = "";
		if ( $typeID!==null )
		{
			$catStr = "and `typeID` = ".$typeID;
		}
		
		$statusStr = "";
		if ( $status!="all" )
		{
			$statusStr = " and `status` = ".$status;
		}
		
		$sql = "
			select 
				`ID`, `typeID`, 
				`name_{$this->m_displayLang}` as `name`, 
				`subtitle_{$this->m_displayLang}` as `subtitle`, 
				`desc_{$this->m_displayLang}` as `desc`, 
				`short_desc_{$this->m_displayLang}` as `short_desc`, 
				`picture`, 
				`cover`, 
				`date`, 
				`random`, 
				`viewCount`
			from 
				`pr_main` 
			where 
				`ID` > 0 $statusStr 
				$catStr 
			order by 
				$orderBy $orderType, `ID` desc 
			limit 
				$limit
		";
		$retArr = DB::QueryToArray($sql);
		
		return $retArr;
	}
	
	function GetProductTypes($orderBy="`name`", $orderType="desc")
	{
		$sql = "select `ID`, `name` from `pr_type` order by ".$orderBy." ".$orderType;
		$retArr = DB::QueryToArray($sql);
		
		return $retArr;
	}
}
?>