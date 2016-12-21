<?php
/**
 * This class is additional class for woring with database.
 *
 * @package Main
 */
class DBxp extends DB
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
	
	}
	
	/**
	 * This function make repositions for set of element by some cretery
	 *
	 * @param string $tableName
	 * @param array|int $toMoveIDs
	 * @param int $position /after position, or set 0 to set first
	 * @param string $conditions
	 * @param string $posField
	 */
	static function Reposition($tableName, $toMoveIDs, $position, $conditions = null, $IDField = 'ID', $posField = 'position')
	{
		// incrimend position
		$position ++;
		
		// if the move IDs is int value , then create array for loop
		if (is_int ( $toMoveIDs ))
		{
			$intValue = $toMoveIDs;
			$toMoveIDs = array ();
			array_push ( $toMoveIDs, $intValue );
		}
		
		// if there are conditions then add string wor SQL statement
		if ($conditions)
		{
			$conditionsS = "AND $conditions";
		}
		
		// get number of all elements
		$sqlAll = '';
		if ($conditions)
		{
			$sqlAll = "SELECT count(*) FROM `$tableName` WHERE  $conditions";
		}
		else
		{
			$sqlAll = "SELECT count(*) FROM `$tableName`";
		}
		
		$numberAll = DB::QueryToValue ( $sqlAll );
		
		// get number of elements which must be moved
		$numberMove = count ( $toMoveIDs );
		
		// get all elements without selected
		$selIDs = join ( ', ', $toMoveIDs );
		$sqlNoSelected = "SELECT `$IDField` FROM `$tableName` WHERE `$IDField` NOT IN ($selIDs) $conditionsS ORDER BY `$posField`";
		$noSelectedIDs = DB::QueryToVector ( $sqlNoSelected );
		
		// calculate position if 
		if ($position > $numberAll - $numberMove + 1)
		{
			$position = $numberAll - $numberMove + 1;
		}
		
		// loop for all elements
		$toMoveIndex = 0;
		$noMoveIndex = 0;
		for($pos = 1; $pos <= $numberAll; $pos ++)
		{
			$elementID = 0;
			// if current position is not selected position then set for outside element
			if ($pos >= $position && $pos < $position + $numberMove)
			{
				$elementID = $toMoveIDs [$toMoveIndex ++];
			}
			else
			{
				$elementID = $noSelectedIDs [$noMoveIndex ++];
			}
			
			// change position
			$sql = "UPDATE `$tableName` SET `$posField` = $pos WHERE `$IDField` = $elementID";
			DB::Query ( $sql );
		}
	}
	
	/**
	 * This function get sql statement tables. 
	 *
	 * @param string $sql
	 * @return array|null
	 */
	static function GetSQLQueryTables($sql)
	{
		$sql = trim ( $sql );
		
		$posFROM = stripos ( $sql, ' from ' );
		
		$subSQL = null;
		
		if ($posFROM)
		{
			$subSQL = trim ( substr ( $sql, $posFROM + 6 ) );
			
			// split where
			$posWHERE = stripos ( $subSQL, ' where ' );
			
			if ($posWHERE)
			{
				$subSQL = trim ( substr ( $subSQL, 0, $posWHERE ) );
			}
			
			// split limit
			$posLIMIT = stripos ( $subSQL, ' limit ' );
			
			if ($posLIMIT)
			{
				$subSQL = trim ( substr ( $subSQL, 0, $posLIMIT ) );
			}
		}
		
		//
		

		$tablesTemp = split ( ' ', $subSQL );
		
		$tables = array ();
		
		foreach ( $tablesTemp as $table )
		{
			$tableName = str_replace ( '`', '', $table );
			array_push ( $tables, $tableName );
		}
		
		return $tables;
	}
	
	/**
	 * Return all databases which are available
	 *
	 * @return array
	 */
	static function GetDatabases()
	{
		return DB::QueryToVector ( 'SHOW DATABASES' );
	}
	
	/**
	 * Return selected database
	 *
	 * @return string
	 */
	static function GetDatabase()
	{
		return DB::QueryToValue ( 'SELECT DATABASE();' );
	}
	
	/**
	 * Return all 
	 *
	 * @param string $db database name
	 * @return array table names
	 */
	static function GetTables($db = null)
	{
		$sql = ($db) ? "SHOW TABLES FROM $db" : "SHOW TABLES";
		return DB::QueryToVector ( $sql );
	}
	
	/**
	 * Return array of filed
	 *
	 * @param string $table table name
	 * @param string $type null|FULL information type 
	 * @param string $db database name
	 * @return array
	 */
	static function GetFields($table, $type = null, $db = null)
	{
		if ($db)
		{
			$sql = "SHOW $type COLUMNS FROM `$table` FROM `$db`";
		}
		else
		{
			$sql = "SHOW $type COLUMNS FROM `$table`";
		}
		
		return DB::QueryToArray ( $sql );
	}
	
	/**
	 * Return array of filed names
	 *
	 * @param string $table table name
	 * @param string $db database name
	 * @return array
	 */
	static function GetFieldNames($table, $db = null)
	{
		$fields = DBxp::GetFields ( $table, null, $db );
		$nems = array ();
		
		if($fields)
		{
			foreach ( $fields as $field )
			{
				array_push ( $nems, $field ['Field'] );
			}
		}
		
		return $nems;
	}
	
	/**
	 * Return array of filed names with out langiaged.
	 * This function is particular for Siteius database architecture.
	 * This cuntion adds + sign to the end of multiple field name
	 * 
	 * @param string $table table name
	 * @param string $db database name
	 * @return array
	 */
	static function GetFieldNamesOutLang($table, $db = null)
	{
		$fields = DBxp::GetFieldNames ( $table, $db );
		
		$names = array ();
		
		foreach ( $fields as $field )
		{
			$fLen = strlen ( $field );
			
			if (substr ( $field, $fLen - 3, 1 ) == "_")
			{
				$field = substr ( $field, 0, $fLen - 3 );
				// + menas multiple language
				$field .= '+';
			}
			
			if (in_array ( $field, $names ))
			{
				continue;
			}
			
			array_push ( $names, $field );
		}
		
		return $names;
	}
	
	/**
	 * This function returns the fields information without multiple languages.
	 * array strucutre is:
	 * array['Field'] = "field name";
	 * array['Type'] = enum(bool, int, float, char, string);
	 * array['Multiple'] = true|false; //true if it is multiple language field, this is only for chars and strings
	 *
	 * @param string $table
	 * @param string $db
	 * @return array
	 */
	static function GetFieldsOutLangs($table, $db = null)
	{
		$retFields = array ();
		
		// get filed names with out lagnuages
		$fieldNames = DBxp::GetFieldNamesOutLang ( $table, $db );
		$fields = DBxp::GetFields ( $table, $db );
		// ca
		foreach ( $fieldNames as $fieldName )
		{
			// calc multiple
			$mult = false;
			if (strpos ( $fieldName, '+' ))
			{
				$mult = true;
				$fieldName = substr ( $fieldName, 0, strlen ( $fieldName ) - 1 );
			}
			
			if ($fieldName == 'ID')
				continue;
				
			// calculate filed
			$field = null;
			foreach ( $fields as $cfield )
			{
				$fName = $cfield ['Field'];
				
				// for not multiple case
				if ($fName == $fieldName)
				{
					$field = $cfield;
					break;
				}
				// for multiple case
				elseif (substr ( $fName, strlen ( $fName ) - 3, 1 ) == '_' && substr ( $fName, 0, strlen ( $fName ) - 3 ) == $fieldName)
				{
					$cfield ['Field'] = substr ( $cfield ['Field'], 0, strlen ( $cfield ['Field'] ) - 3 );
					$field = $cfield;
				}
			}
			
			$fieldC = array ('Field' => $field ['Field'], 'Type' => $field ['Type'], 'Multiple' => $mult );
			
			// to to array
			array_push ( $retFields, $fieldC );
		}
		
		return $retFields;
	}
	
	/**
	 * Add column to table, return result
	 *
	 * @param string $table
	 * @param string $field
	 * @param string $type
	 * @param int $length
	 * @param bool $NULL
	 * @param bool $unsigned
	 * @return bool
	 */
	static public function AddField($table, $field, $type, $length = null, $NULL = false, $unsigned = false)
	{
		$nullS = ($NULL) ? 'NULL' : 'NOT NULL';
		$unsignedS = ($unsigned) ? 'UNSIGNED' : '';
		$typeS = ($length) ? "$type ( $length )" : $type;
		
		$sql = "ALTER TABLE `$table` ADD `$field` $typeS $unsignedS $nullS ;";
		
		return DB::Query ( $sql, true );
	}
	
	/**
	 * Delete field from table
	 *
	 * @param string $table
	 * @param string $field
	 * @return bool
	 */
	static public function DeleteField($table, $field)
	{
		
		$columens = self::GetFields ( $table );
		
		if (in_array ( $field, $columens ))
		{
			$sql = "ALTER TABLE `$table` DROP `$field`";
			return DB::Query ( $sql, true );
		}
		
		return true;
	}
	
	/**
	 * Get table fileds and chack them with given fields
	 *
	 * @param string $table
	 * @param array $fields
	 * @param array $langKeys keys of all languages am, ru, en, ...
	 * @return bool
	 */
	static public function TestTable($table, $fields, $langKeys = null, $displayMessage = true)
	{
		// chack table
		$tables = DBxp::GetTables ();
		
		if (! in_array ( $table, $tables ))
		{
			if ($displayMessage)
			{
				print ( "<b>Warning:</b> The table <b>'$table'</b> does not exist!<br />" );
			}
			
			return false;
		}
		
		// result
		$res = true;
		
		// get table fileds
		$tableFieldNames = DBxp::GetFieldNames ( $table );
		
		// loop all fields
		foreach ( $fields as $field )
		{
			$lastChar = substr ( $field, strlen ( $field ) - 1 );
			// multiple
			if ($lastChar == '+')
			{
				// get name with out last + sign
				$field = substr ( $field, 0, strlen ( $field ) - 1 );
				
				foreach ( $langKeys as $langKey )
				{
					$fnName = "{$field}_{$langKey}";
					
					if (! in_array ( $fnName, $tableFieldNames ))
					{
						print ( "<b>Warning:</b> table <b>'$table'</b> - missing field: <b>$fnName</b><br />" );
						$res = false;
					}
				}
			}
			// single
			else
			{
				if (! in_array ( $field, $tableFieldNames ))
				{
					print ( "<b>Warning:</b> table <b>'$table'</b> - missing field: <b>$field</b><br />" );
					$res = false;
				}
			}
		}
		
		return $res;
	}
	
	/**
	 * This function get SQL of create table, compear with existing table and add missing fileds.
	 * Table and field names mut be closed to `` signs.
	 *
	 * @param string $createTableSQL create talbe sql statement
	 */
	static function AddMissingFieldsToTable($createTableSQL)
	{
		// get table name
		$tableName = DBxp::GetTableNameFromCreateSQL ( $createTableSQL );
		// get all fileds from creat statement
		$toBeFields = DBxp::GetTableFieldsFromCreateSQL ( $createTableSQL );
		
		// get all fields from exist table
		$fields = DBxp::GetFieldNames ( $tableName );
		
		// loop oll fileds
		foreach ( $toBeFields as $key => $toBeField )
		{
			// add missing fileds
			if (! in_array ( $key, $fields ))
			{
				$sql = "ALTER TABLE  `$tableName` ADD  $toBeField;";
				DB::Query ( $sql );
			}
		}
	}
	
	/**
	 * Retrive table name from create table sql statement.
	 *
	 * @param string $sql
	 * @return string|null
	 */
	static function GetTableNameFromCreateSQL($createTableSQL)
	{
		// get table name
		$tabReg = '/CREATE[\s]*TABLE[\s]*(?:IF[\s]*NOT[\s]*EXISTS)?[\s]*`?([\w]*)`?[\s]*/';
		$tables = array ();
		preg_match ( $tabReg, $createTableSQL, $tables );
		if (count ( $tables ) > 0)
		{
			return $tables [1];
		}
		
		return null;
	}
	
	/**
	 * Retrive table fileds from create table sql statement.
	 *
	 * @param string $sql
	 * @return array
	 */
	static function GetTableFieldsFromCreateSQL($createTableSQL)
	{
		$start = strpos ( $createTableSQL, '(' );
		$end = strrpos ( $createTableSQL, ')' );
		$fieldsSQL = substr ( $createTableSQL, $start + 1, $end - $start - 1 );
		
		// remove primary and uniq part from sql //////////////////////////////
		if(stripos($fieldsSQL, 'UNIQUE'))
		{
			$patternUniq = '/[\s]*,?[\s]*(UNIQUE)[\s]*[^(]*\([^(]*\)?/';
			$fieldsSQL = preg_replace($patternUniq, '', $fieldsSQL);
		}
		
		if(stripos($fieldsSQL, 'PRIMARY'))
		{
			$patternPrim = '/[\s]*,?[\s]*(PRIMARY[\s*]KEY)[\s]*[^(]*\([^(]*\)?/i';
			$fieldsSQL = preg_replace($patternPrim, '', $fieldsSQL);
		}
		
		// end - remove primary and uniq part from sql ////////////////////////
		$fieldsSepSQL = explode ( ',', $fieldsSQL );
		
		$toBeFields = array ();
		
		foreach ( $fieldsSepSQL as $fieldSQL )
		{
			// get field name
			$fldReg = '/[\s]*`?([\w]*)`?[\s]*/';
			$filed = array ();
			preg_match ( $fldReg, $fieldSQL, $filed );
			
			if (count ( $filed ) > 0)
			{
				if(strlen(trim($fieldSQL)) > 0 && strlen(trim($filed[1])) > 0)
				{
					$toBeFields ["{$filed[1]}"] = trim ( $fieldSQL );
				}
			}
		}
		
		return $toBeFields;
	}
	
	/**
	 * Get create table SQL from existing table.
	 *
	 * @param string $table
	 * @return string
	 */
	static function GetCreateTableSQL($table)
	{
		$sql = "SHOW CREATE TABLE `$table`;";
		return DB::QueryToValue($sql);
	}
	
	/**
	 * Drop given table
	 *
	 * @param string $table
	 * @return bool
	 */
	static function Drop($table)
	{
		$sql = "DROM TABLE `$table`";
		return DB::Query($sql);
	}
	
	/**
	 * empty table
	 *
	 * @param string $table
	 * @return bool
	 */
	static function EmptyTable($table)
	{
		$sql = "TRUNCATE TABLE `$table`";
		return DB::Query($sql);
	}
}
?>