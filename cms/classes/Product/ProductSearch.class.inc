<?php
/**
 * Enter description here...
 *
 * @package Item
 */
class ProductSearch extends TreeViewNavigation
{
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct();
	}

	/**
	 * Enter description here...
	 *
	 * @param string $keyword
	 * @param array $conditions
	 */
	function Search($keyword, $conditions = null)
	{
		$query = $this->GetSearchSQL($keyword, $conditions);
		$this->SetQuery($query, 200);		
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $keyword
	 * @param array $conditions
	 * 
	 * @return string
	 */
	function GetSearchSQL($keyword, $conditions = null)
	{
		// get all languages
		$langs = $this->m_site->GetLanguages();
		
		// make filter conditions
		$filterStr = null;
		
		foreach ($langs as $clang)
		{
			$filterStr .= ($filterStr)? 
							" OR `name_$clang` LIKE '%$keyword%' OR `short_desc_$clang` LIKE '%$keyword%' OR `desc_$clang` LIKE '%$keyword%'" 
							: 
							" 	 `name_$clang` LIKE '%$keyword%' OR `short_desc_$clang` LIKE '%$keyword%' OR `desc_$clang` LIKE '%$keyword%'";
			
		}
		
		$filterStr = "`status` = 1 and (".$filterStr.")";
		
		// create sql
		if($conditions)
		{
			$sql = "SELECT 
					`ID`, `random`, `typeID`, `name_{$this->m_displayLang}` as `name`, `short_desc_{$this->m_displayLang}` as `short_desc`, `picture`, `temp`.`date` FROM `pr_main` as `temp`
				 	WHERE $filterStr";
		}
		else 
		{
			$sql = "SELECT 
					`ID`, `random`, `typeID`, `name_{$this->m_displayLang}` as `name`, `short_desc_{$this->m_displayLang}` as `short_desc`, `picture`, `temp`.`date` FROM `pr_main` as `temp`
				WHERE $filterStr";	
		}
		
		return $sql;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplaySearchResults($path)
	{
		$prTypeView = new pr_typeView ( );

		if($this->m_struct && count($this->m_struct))
		{
			foreach ($this->m_struct as $current) 
			{
				$product = new Pr_main($current['ID']);
				echo $prTypeView->MakeSearchView ( $product, $path);
			}
		}
	}
}
?>