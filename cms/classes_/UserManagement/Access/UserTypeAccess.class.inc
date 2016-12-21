<?php
/**
 * This class can be used to modifie user type access to modules and tables.
 *
 */
class UserTypeAccess
{
	/**
	 * User type ID
	 *
	 * @var int
	 */
	protected $m_userTypeID;
	
	/**
	 * Construct
	 *
	 * @param int $userTypeID
	 */
	function __construct($userTypeID)
	{
		$this->m_userTypeID = $userTypeID;
	}
	
	/**
	 * Modify module accesses for selected yser type.
	 *
	 * @param array $modules
	 */
	function ModifyModuleAccesses($modules)
	{
		// delete all accesses
		$sql = "DELETE FROM `module_access` WHERE `userTypeID` = {$this->m_userTypeID}";
		DB::Query ( $sql );
		
		// insert
		if ($modules)
		{
			foreach ( $modules as $module )
			{
				$sql = "INSERT INTO `module_access` (`ID` ,`module` ,`userTypeID`, `access`)
								VALUES (NULL , '$module', '$this->m_userTypeID', '1');";
				DB::Query ( $sql );
			}
		}
		
		// change all accesses for chields
		$this->ChangeChiledsAccesses ( $this->m_userTypeID );
	}
	
	/**
	 * Modify table accesses for selected yser type.
	 *
	 * @param array $modules
	 * @return bool 
	 */
	function ModifyTableAccesses($table, $status)
	{
		// update access
		$sql = "SELECT * FROM `table_access`
							    WHERE `table` = '$table' AND `field` = '0' AND `userTypeID` ={$this->m_userTypeID}";
		
		$resQu = DB::QueryToArray ( $sql, true );
		
		$updateChilds = false;
		if ($resQu)
		{
			// update
			$updateChilds = true;
			$sql = "UPDATE `table_access` SET `status` = '$status' WHERE `table` = '$table' AND `field` = '0' AND `userTypeID` ={$this->m_userTypeID} LIMIT 1 ;";
		}
		else
		{
			// insert
			$sql = "INSERT INTO `table_access` (`ID` ,`table` ,`field` ,`userTypeID` ,`status`)
							VALUES (NULL , '$table', '0', '{$this->m_userTypeID}', '$status');";
		}
		
		$res = DB::Query ( $sql );
		
		// update all fileds in table forcurrent user type access (binnary & with all talbe fields)
		if ($updateChilds)
		{
			$sql = "UPDATE `table_access` SET `status` = `status` & {$status} WHERE `table` = '$table' AND `field` != '0' AND `userTypeID` ={$this->m_userTypeID}";
			$res &= DB::Query ( $sql );
			
			// rekursive change childs status
			$res &= $this->ChangeChiledsAccessesTable ( $this->m_userTypeID, $table );
		}
		
		return $res;
	}
	
	/**
	 * Modify table's fields accesses for selected yser type.
	 *
	 * @param array $modules
	 */
	function ModifyTableFieldAccesses($table, $field, $status)
	{
		// update access
		$sql = "SELECT * FROM `table_access`
							    WHERE `table` = '$table' AND `field` = '$field' AND `userTypeID` ={$this->m_userTypeID}";
		
		$resQu = DB::QueryToArray ( $sql, true );
		if ($resQu)
		{
			// update
			$sql = "UPDATE `table_access` SET `status` = '$status' WHERE `table` = '$table' AND `field` = '$field' AND `userTypeID` ={$this->m_userTypeID} LIMIT 1 ;";
		}
		else
		{
			// insert
			$sql = "INSERT INTO `table_access` (`ID` ,`table` ,`field` ,`userTypeID` ,`status`)
							VALUES (NULL , '$table', '$field' , '{$this->m_userTypeID}', '$status');";
		}
		
		DB::Query ( $sql );
		
		// rekursive change childs status
		$this->ChangeChiledsAccessesTable ( $this->m_userTypeID, $table, $field );
	}
	
	/**
	 * Change access to table for all chied types.
	 *
	 * @param int $userTypeID
	 */
	function ChangeChiledsAccessesTable($userTypeID, $table, $field = 0)
	{
		// get all chields
		$sql = "SELECT `ID` FROM `usertype` WHERE `parentID` = $userTypeID";
		$chiledIDs = DB::QueryToVector ( $sql );
		if ($chiledIDs)
		{
			$fieldV = ($field) ? " = '$field' " : ' AND `field` = "0" ';
			
			// get parent and parent status
			$sql = "SELECT `status` FROM `table_access`
							    WHERE `table` = '$table' $fieldV AND `userTypeID` =$userTypeID";
			$parentStatus = ( int ) DB::QueryToValue ( $sql );
			/////////////////
			foreach ( $chiledIDs as $chiledID )
			{
				$sql = "SELECT * FROM `table_access`
							    WHERE `table` = '$table' $fieldV AND `userTypeID` =$chiledID";
				
				// get chil
				$chiled = DB::QueryToArray ( $sql );
				
				// get child status
				$sql = "SELECT `status` FROM `table_access`
							    WHERE `table` = '$table' $fieldV AND `userTypeID` =$chiledID";
				
				$chiledStatus = ( int ) DB::QueryToValue ( $sql );
				
				if ($chiled)
				{
					// root owner do not have any status
					if ($parentStatus != - 1)
					{
						$chiledStatus &= ( int ) $parentStatus;
					}
					
					$sql = "UPDATE `table_access` SET `status` = '$chiledStatus' WHERE `table` = '$table' AND `field` $fieldV AND `userTypeID` =$chiledID LIMIT 1 ;";
					
					// delete all modules which is not in parent
					DB::Query ( $sql );
				}
				
				//update all fileds in table for current user type access (binnary & with all talbe fields)
				if (! $field) // if table
				{
					$sql = "UPDATE `table_access` SET `status` = `status` & {$chiledStatus} WHERE `table` = '$table' AND `field` ='0' AND `userTypeID` ={$chiledID}";
					DB::Query ( $sql );
				}
				
				// calculate for sub chieds
				$this->ChangeChiledsAccesses ( $chiledID, $table, $field );
			}
		}
	}
	
	/**
	 * Change access to module for all chied types.
	 *
	 * @param int $userTypeID
	 */
	function ChangeChiledsAccesses($userTypeID)
	{
		// get all chields
		$sql = "SELECT `ID` FROM `usertype` WHERE `parentID` = $userTypeID";
		$chiledIDs = DB::QueryToVector ( $sql );
		if ($chiledIDs)
		{
			// get parent modules
			$sql = "SELECT `module` FROM `module_access` WHERE `userTypeID` = $userTypeID";
			$parentModules = DB::QueryToVector ( $sql );
			
			if ($parentModules)
			{
				$parentModulesS = '"' . implode ( '", "', $parentModules ) . '"';
				
				foreach ( $chiledIDs as $chiledID )
				{
					
					// delete all modules which is not in parent
					$sql = "DELETE FROM `module_access` 
							WHERE `userTypeID` = $chiledID AND `module` NOT IN($parentModulesS)";
					
					DB::Query ( $sql );
					// calculate for sub chieds
					$this->ChangeChiledsAccesses ( $chiledID );
				}
			}
		}
	}
	
	/**
	 * Return boolean value has hte user type access to given module? 
	 *
	 * @param string $key
	 * @param int $userTypeID
	 * @return bool
	 */
	static function HasModuleAccess($key, $userTypeID = null)
	{
		// for owner all modules are reachible
		if ($userTypeID == 1)
		{
			return true;
		}
		
		$sql = "SELECT `access` FROM `module_access` WHERE `module` = '$key' AND `userTypeID` = {$userTypeID}";
		$res = DB::QueryToValue ( $sql );
		
		return $res;
	}
	
	/**
	 * Delete all accesses (module and table) for given user type with chidrens.
	 *
	 * @param int $userType
	 * @return bool
	 */
	function DeleteUserTypeAllAccess($userType = null)
	{
		if (! $userType)
		{
			$userType = $this->m_userTypeID;
		}
		
		$res = true;
		// get all sub user types
		$sql = "SELECT `ID` FROM `usertype` WHERE `parentID` = $userType";
		$subUsers = DB::QueryToVector ( $sql );
		
		if ($subUsers)
		{
			foreach ( $subUsers as $subUser )
			{
				$res &= $this->DeleteUserTypeAllAccess ( $subUser );
			}
		}
		// delete from module access
		$sql = "DELETE FROM `module_access` WHERE `userTypeID` = {$userType}";
		$res &= DB::Query ( $sql );
		// delete from table and field access
		$sql = "DELETE FROM `table_access` WHERE `userTypeID` = {$userType}";
		$res &= DB::Query ( $sql );
		
		return $res;
	}
}
?>