<?php
/**
 * Product filed lists monipulation
 *  
 * @package Item
 */
class Pr_ListMan
{
	function __construct()
	{
		
	}
	
	
	/**
	 * Enter description here...
	 *
	 * @param int $productID
	 * @param int $fieldID
	 * @param int $productType
	 * @return bool
	 */
	function DeleteInts($productID = null, $fieldID = null, $productType = null)
	{		
		$this->DeleteFromDB('pr_int', $productID, $fieldID, $productType);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $productID
	 * @param int $fieldID
	 * @param int $productType
	 * @return bool
	 */
	function DeleteDoubles($productID = null, $fieldID = null, $productType = null)
	{		
		return $this->DeleteFromDB('pr_double', $productID, $fieldID, $productType);
	}

	/**
	 * Enter description here...
	 *
	 * @param int $productID
	 * @param int $fieldID
	 * @param int $productType
	 * @return bool
	 */
	function DeleteStrings($productID = null, $fieldID = null, $productType = null)
	{		
		return $this->DeleteFromDB('pr_char', $productID, $fieldID, $productType);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $productID
	 * @param int $fieldID
	 * @param int $productType
	 * @return bool
	 */
	function DeleteOptions($productID = null, $fieldID = null, $productType = null, $group=0)
	{		
		return $this->DeleteFromDB('pr_options', $productID, $fieldID, $productType, $group);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $tableName
	 * @param int $productID
	 * @param int $fieldID
	 * @param int $productType
	 * @return bool
	 */
	protected function DeleteFromDB($tableName, $productID = null, $fieldID = null, $productType = null, $group=0)
	{
		$sql = '';
		
		if($productType > 0)
		{
			
		}
		else 
		{
			if($productID > 0 || $fieldID > 0)
			{
				if($productID > 0 && $fieldID > 0)
				{
					$sql = "FROM `$tableName` WHERE `productID` = $productID AND `fieldID` = $fieldID";
				}
				elseif ($productID > 0)
				{
					$sql = "FROM `$tableName` WHERE `productID` = $productID";
				}
				else 
				{
					$sql = "FROM `$tableName` WHERE `fieldID` = $fieldID";
				}
				
				$sql .= " and `group` = $group";
			}
		}
		
		// stor statement
		$selSQL = "SELECT * $sql";
		
		$storeResult = array();
		
		$storeResult = DB::QueryToArray($selSQL);
		
		if($storeResult)
		{
			$keys = array_keys($storeResult);
			
			foreach ($keys as $key)
			{
				$storeResult[$key]['cms_table_name'] = $tableName;	
			}
		}
		
		// delete statement
		$delSQL = "DELETE $sql";
		$res = DB::Query($delSQL);
		
		if(!$res)
		{
			print QueryResluts::GetError();
			return false;
		}
		
		return $storeResult;
	}
	
	/**
	 * Delete all related records from all databasees 
	 *
	 * @param int $productID
	 */
	function DeleteProductInformation($productID)
	{
		$storInt = $this->DeleteInts($productID);  
		$storDouble = $this->DeleteDoubles($productID);  
		$storString = $this->DeleteStrings($productID);  
		$storOptions = $this->DeleteOptions($productID);
	
		$resultArray = array();
		
		if($storInt)
		 array_push($resultArray, $storInt);
		if($storDouble)
		 array_push($resultArray, $storDouble);
		if($storString)
		 array_push($resultArray, $storString);
		if($storOptions)
		 array_push($resultArray, $storOptions);
		 
		 return $resultArray;
	}
	
	/**
	 * delete all infromtion from talbes according field
	 *
	 */
	function DeleteFieldInformation($fieldID)
	{
		$this->DeleteFromDB('pr_int', null, $fieldID);
		$this->DeleteFromDB('pr_double', null, $fieldID);
		$this->DeleteFromDB('pr_char', null, $fieldID);
		$this->DeleteFromDB('pr_options', null, $fieldID);
	}
}
?>