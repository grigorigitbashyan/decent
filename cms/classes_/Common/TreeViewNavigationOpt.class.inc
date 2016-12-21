<?php
Site::IncludeFile ( 'classes/Common/TreeViewNavigation.class.inc' );
Site::IncludeFile ( 'classes/ItemOptions/OptionsdataList.class.inc' );

/**
 * This class is used to manipulate trees with options.
 * It use 'optionsdata' table to join to the current talbe. 
 *
 */
class TreeViewNavigationOpt extends TreeViewNavigation
{
	/**
	 * Enter description here...
	 *
	 * @param string|array $structure
	 * @param string $linkIDName
	 * @param int $limit
	 * @param int $page
	 * @param string $linkName
	 * @param int $currID
	 * @param string $currLang
	 * @param array $exParmas
	 * @param array $tableNames
	 */
	function __construct($structure = null, $linkIDName = null, $limit = null, $page = null, $linkName = 'name', $currID = null, $exParmas = null, $tableNames = null)
	{
		parent::__construct ( $structure, $linkIDName, $limit, $page, $linkName, $currID, $exParmas, $tableNames );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $structure
	 * @param array $optionFields
	 * @param int $limit
	 */
	function SetQuery($structure, $optionFields, $limit = null)
	{
		if ($limit)
		{
			$this->m_limit = $limit;
		}
		// parse string
		$fromStart = stripos ( $structure, 'FROM ' );
		
		$this->m_allResultesquery = "SELECT count(*) as `count` " . substr ( $structure, $fromStart );
		
		if ($this->m_sortBy)
		{
			if (in_array ( $this->m_sortBy, $optionFields )) // by options
			{
				$structure .= " ORDER BY `optionsdata`.`name_{$this->m_displayLang}` {$this->m_sortType} ";
			}
			else
			{
				$structure .= " ORDER BY `{$this->m_sortBy}` {$this->m_sortType} ";
			}
		}
		
		// main query with limit
		if ($this->m_limit)
		{
			$start = ($this->m_page - 1) * $this->m_limit;
			$structure .= " LIMIT $start, {$this->m_limit}";
		}
		
		$this->SetQueryString ( $structure );
	}
	/**
	 * Enter description here...
	 *
	 * @param array $optionFields
	 */
	protected function UpdateOptionsData($optionFields)
	{
		// get all data for used options
		$optionData = array ();
		foreach ( $optionFields as $key => $option )
		{
			//  $key < 0 fileds have not been replaced
			$optList = new OptionsdataList ( $option );
			$struc = $optList->GetStructure ();
			
			foreach ( $struc as $value )
			{
				$optionData [$option] [$value ['ID']] = $value ['name'];
			}
		}
		
		if ($this->m_struct)
		{
			foreach ( $this->m_struct as $key => $value )
			{
				foreach ( $optionFields as $keyF => $option )
				{
					if (isset ( $optionData [$option] [$value [$keyF]] ))
					{
						$value [$keyF] = $optionData [$option] [$value [$keyF]];
					}
				}
				
				$this->m_struct [$key] = $value;
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $filter
	 * @param array $displayfields
	 * @param string $table
	 * @param string $joindTable
	 */
	function Search($filters, $displayfields, $table, $joindTable = null)
	{
		if (! $joindTable)
		{
			$joindTable = $table;
		}
		// create field sql statment //////////////////////////////
		$fieldsS = '';
		if (strpos ( implode ( '', $displayfields ), '`', 1 ) !== null)
		{
			$fieldsS = implode ( ', ', $displayfields );
		}
		else
		{
			$fieldsS = '`' . implode ( '`, `', $displayfields ) . '`';
		}
		
		///////////////////////////////////////////////////////////
		$filterS = '';
		$limit = null;
		foreach ( $filters as $key => $filter )
		{
			switch ($filter ['type'])
			{
				case 1 :
				case 2 :
				case 3 :
					if (! empty ( $filter ['_min'] ) || ! empty ( $filter ['_max'] ))
					{
						if (! empty ( $filter ['_min'] ))
						{
							$filterS .= " AND `$table`.`$key` >= '{$filter['_min']}'";
						}
						
						if (! empty ( $filter ['_max'] ))
						{
							$filterS .= " AND `$table`.`$key` <= '{$filter['_max']}'";
						}
					}
					elseif (! empty ( $filter ['value'] ))
					{
						$filterS .= " AND `$table`.`$key` = '{$filter['value']}'";
					}
					break;
				case 7 :
					if (! empty ( $filter ['value'] ))
					{
						// array
						if (is_array ( $filter ['value'] ) && count ( $filter ['value'] ) > 0)
						{
							$valusS = "'" . implode ( "', '", $filter ['value'] ) . "'";
							$filterS .= " AND `$table`.`$key` IN ($valusS)";
						}
						else
						{
							$filterS .= " AND `$table`.`$key` = '{$filter ['value']}'";
						}
					}
					break;
				default :
					// array
					if ($filter ['type'] == 'reportQuantity')
					{
						$limit = $filter ['value'];
					}
					elseif (($filter ['type'] == - 1) || ! empty ( $filter ['value'] ))
					{
						if (is_array ( $filter ['value'] ) && count ( $filter ['value'] ) > 0)
						{
							$valusS = "'" . implode ( "', '", $filter ['value'] ) . "'";
							$filterS .= " AND `$table`.`$key` IN ($valusS)";
						}
						else
						{
							$filterS .= " AND `$table`.`$key` = '{$filter ['value']}'";
						}
					}
					break;
			}
		}
		
		// remove first 'AND' string
		if (strlen ( $filterS ) > 0)
		{
			$filterS = substr ( $filterS, 4 );
		}
		
		// add first and last ` sybol if not 
		if (substr ( $joindTable, 0, 1 ) != '`')
		{
			$joindTable = '`' . $joindTable;
		}
		if (substr ( $joindTable, - 1, 1 ) != '`')
		{
			$joindTable = $joindTable . '`';
		}
		////////////////////////////////////
		

		// query
		$sql = "SELECT $fieldsS FROM $joindTable WHERE $filterS";
		parent::SetQuery ( $sql, $limit );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $fld
	 * @param bool $IDorValue
	 * @param int $userTypeID
	 * @param strong $localLang
	 * @return string
	 */
	function GetFieldCatSQL($fld, $IDorValue = true, $userTypeID = 0, $localLang = null)
	{
		$res = '';
		
		// if $fld is a more then one filed seperated by '/'
		//TODO correct
		if (strpos ( $fld, '/' ) !== false)
		{
			// get all sub fileds
			$subfields = explode ( '/', $fld );
			$strCat = 'CONCAT_WS("/",';
			
			foreach ( $subfields as $key => $subfield )
			{
				$subfield = trim ( $subfield );
				$subfields ["$key"] = $subfield;
				
				$subfieldReName = ($localLang) ? "{$subfield}_{$localLang}" : $subfield;
				// replase with nulls if the visibility status is for only owner
				$isRead = $this->IsReadable ( 'estate', $subfield, $userTypeID );
				$isReadOwner = $this->IsReadableByOwner ( 'estate', $subfield, $userTypeID );
				
				if ($IDorValue)
				{
					if (! $isRead && $isReadOwner)
					{
						$strCat .= "IF(`estate`.`userID` = $userTypeID, `estate`.`$subfieldReName`, null) as `$subfield`";
					}
					else
					{
						$strCat .= "`estate`.`$subfieldReName`";
					}
				}
				else
				{
					if ($subfield == 'zone')
					{
						$strCat .= "`zone`.`name`";
					}
					elseif ($subfield == 'street')
					{
						$strCat .= "`street`.`name_{$this->m_displayLang}`";
					}
					else
					{
						$strCat .= "`estate`.`$subfield`";
					}
				}
				
				$strCat .= ', ';
			}
			
			// create new name
			$strCat = substr ( $strCat, 0, - 2 );
			$strCat .= ")";
			
			$res = "$strCat as `$fld`";
		}
		// single field 
		else
		{
			
			// replase with nulls if the visibility status is for only owner
			$isRead = $this->IsReadable ( 'estate', $fld, $userTypeID );
			$isReadOwner = $this->IsReadableByOwner ( 'estate', $fld, $userTypeID );
			
			$subfieldReName = ($localLang) ? "{$fld}_{$localLang}" : $fld;
			
			if ($IDorValue)
			{
				if (! $isRead && $isReadOwner)
				{
					$res = "IF(`estate`.`userID` = $userTypeID, `estate`.`$subfieldReName`, null) as `$fld`";
				}
				else
				{
					$res = "`estate`.`$subfieldReName` as `$fld`";
				}
			}
			else
			{
				if ($fld == 'zone')
				{
					$res = "`zone`.`name` as `$fld`";
				}
				elseif ($fld == 'street')
				{
					$res = "`street`.`name_{$this->m_displayLang}` as `$fld`";
				}
				else
				{
					if (! $isRead && $isReadOwner)
					{
						$res = "IF(`estate`.`userID` = $userTypeID, `estate`.`$fld`, null) as `$fld`";
					}
					else
					{
						$res = "`estate`.`$subfieldReName` as `$fld`";
					}
				}
			}
		}
		
		return $res;
	}
}
?>