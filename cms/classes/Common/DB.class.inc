<?php
/**
 * This class is main class for working with query. 
 * It has features to do query and return results in array.
 * It also has function for viewing reports.
 *
 * @package Main
 */
class DB
{
	/**
	 * Link to database
	 *
	 * @var resource
	 */
	protected static $dbLink;
	
	protected static $dbLinkArray;
	
	protected static $dbArray;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
	
	}
	/**
	 * Do query and return results to array, or null
	 *
	 * @param string $sql
	 * @return array
	 */
	static function QueryToArray($sql, $conn = null, $db = null, $dispError = false, $fieldName=null)
	{
		// set current db
		if ($db)
		{
			DB::SetDBLink ( $db );
		}
		
		// set db connection
		if ($conn)
		{
			DB::SetDBLink($conn);
		}
		$res = mysql_query ( $sql, DB::$dbLink );
		// if there is a error
		if (mysql_error ( DB::$dbLink ))
		{
			if ($dispError)
			{
				print ("Invalid query: " . mysql_error ( DB::$dbLink )) ;
			}
			
			return null;
		}
		
		$array = array ();
		
		if ($res)
		{
			$isEmpty = true;
			
			while ( ($row = mysql_fetch_assoc ( $res )) )
			{
				$isEmpty = false;
				if ( $fieldName )
				{
					array_push ( $array, $row["$fieldName"] );
				}
				else 
				{
					array_push ( $array, $row );
				}
			}
			
			return ($isEmpty) ? NULL : $array;
		}
		
		// return result
		return NULL;
	}
	
	/**
	 * View report for query
	 *
	 * @param string $sql
	 */
	static function Report($sql)
	{
		$results = DB::QueryToArray ( $sql );
		
		if (count ( $results ) > 0)
		{
			foreach ( $results as $result )
			{
				print_r ( $result );
				print ("<br>\n") ;
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $tableName
	 * @param string $relCollName
	 * @param string $cols
	 * @param int $rootElem
	 * @param bool $withRoot
	 * @param int $currentDepth
	 * @param int $maxdDepth
	 * @return array
	 */
	static function QueryToTree($tableName, $relCollName, $conditions = null, $cols = null, $rootElem = 1, $withRoot = false, $currentDepth = 1, $maxdDepth = null)
	{
		//m_depth ?
		if ($maxdDepth && $currentDepth > $maxdDepth)
			return NULL;
			
		// generate query
		$cols = ($cols) ? $cols : " * ";
		
		if ($withRoot && $currentDepth == 1)
		{
			$query = "SELECT $cols FROM `$tableName` WHERE `ID` = $rootElem";
		}
		else
		{
			$query = "SELECT $cols FROM `$tableName` WHERE `$relCollName` = $rootElem $conditions";
		}
		
		$result = DB::QueryToArray ( $query );
		
		if ($result)
		{
			foreach ( $result as $key => $value )
			{
				$result [$key] ['ref'] = DB::QueryToTree ( $tableName, $relCollName, $conditions, $cols, $value ['ID'], $withRoot, $currentDepth + 1, $maxdDepth );
			}
		}
		
		return $result;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $sql
	 * @return bool
	 */
	static function Query($sql, $dbLink = null, $displayError = false)
	{
		// set current db
		if ($dbLink)
		{
			DB::SetDBLink ( $dbLink );
		}
		
		$res = mysql_query ( $sql, DB::$dbLink );
		
		if ($displayError && ! $res)
		{
			print DB::GetError ();
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return null|string
	 */
	static function GetError($dbLink = NULL)
	{
		// set current db
		if ($dbLink)
		{
			DB::SetDBLink ( $dbLink );
		}
		
		return mysql_error ( DB::$dbLink );
	}
	
	/**
	 * Return single value from query
	 *
	 * @param string $sql
	 * @return mixed
	 */
	static function QueryToValue($sql, $conn = null)
	{
		// set current db
		if ($conn)
		{
			DB::SetDBLink ( $conn );
		}
		
		$res = mysql_query ( $sql, DB::$dbLink );
		
		if ($res)
		{
			$row = mysql_fetch_row ( $res );
			return $row [0];
		}
		
		return null;
	}
	
	/**
	 * Return single row (array)
	 *
	 * @param string $sql
	 * @return array|null
	 */
	static function QueryToRow($sql, $db = null)
	{
		// set current db
		if ($db)
		{
			DB::SetDBLink ( $db );
		}
		
		$res = DB::Query ( $sql );
		
		if ($res)
		{
			return mysql_fetch_array ( $res, MYSQL_ASSOC );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $sql
	 * @return array|null
	 */
	static function QueryToVector($sql, $db = null)
	{
		// set current db
		if ($db)
		{
			DB::SetDBLink ( $db );
		}
		
		$res = mysql_query ( $sql, DB::$dbLink );
		
		// add IDs into the array
		if ($res)
		{
			$vector = array ();
			while ( ($cat = mysql_fetch_row ( $res )) )
			{
				array_push ( $vector, $cat [0] );
			}
			
			return $vector;
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	static function GetLastID($db = null)
	{
		// set current db
		if ($db)
		{
			DB::SetDBLink ( $db );
		}
		
		return mysql_insert_id ( DB::$dbLink );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $dbName
	 * @return array|null
	 */
	static function GetDBTables($dbName, $db = null)
	{
		// set current db
		if ($db)
		{
			DB::SetDBLink ( $db );
		}
		
		$result = mysql_list_tables ( "$dbName", DB::$dbLink );
		$num_rows = mysql_num_rows ( $result );
		
		if ($num_rows > 0)
		{
			$dbTables = array ();
			
			for($i = 0; $i < $num_rows; $i ++)
			{
				$tableName = mysql_tablename ( $result, $i );
				array_push ( $dbTables, $tableName );
			}
			
			return $dbTables;
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $host
	 * @param string $username
	 * @param string $password
	 * @param string $dbName
	 * @param string $connectionIndex
	 */
	static function Connect($host, $username, $password, $dbName = null, $connectionIndex = null)
	{
		if (! DB::$dbLinkArray)
		{
			DB::$dbLinkArray = array ();
			DB::$dbArray = array ();
		}
		
		DB::$dbLink = mysql_connect ( $host, $username, $password );
		
		if (! DB::$dbLink)
		{
			die ( "The connection to server $host is feild!" );
		}
		
		// add current connection link to array
		if ($connectionIndex)
		{
			DB::$dbLinkArray ["$connectionIndex"] = DB::$dbLink;
		}
		else
		{
			DB::$dbLinkArray [] = DB::$dbLink;
		}
		
		if (DB::$dbLink && $dbName)
		{
			
			mysql_query("SET NAMES 'utf8'");
			mysql_query("SET collation_connection = 'UTF-8_general_ci'");
			mysql_query("SET collation_server = 'UTF-8_general_ci'");
			mysql_query("SET character_set_client = 'UTF-8'");
			mysql_query("SET character_set_connection = 'UTF-8'");
			mysql_query("SET character_set_results = 'UTF-8'");
			mysql_query("SET character_set_server = 'UTF-8'");
			
			
			mysql_select_db ( $dbName, DB::$dbLink );
			
			DB::$dbArray [] = $dbName;
			
			$error = DB::GetError ();
			
			if ($error)
			{
				print ($error) ;
				return null;
			}
		
		}
		
		return DB::$dbLink;
	}
	
	/**
	 * 
	 * @param string $index
	 */
	static function GetDBLink($index = null)
	{
		// get db link by index
		if ($index && isset ( DB::$dbLinkArray ["$index"] ))
		{
			return DB::$dbLinkArray ["$index"];
		}
		
		// return current db link
		return DB::$dbLink;
	}
	
	/**
	 * Sets new database link and returns the old one
	 *
	 * @param resurse $dbLink
	 * @return resurse
	 */
	static function SetDBLink($dbLink, $dbName = null)
	{
		$oldDBLink = DB::$dbLink;
		
		if (is_string ( $dbLink ))
		{
			$dbLink = DB::GetDBLink ( $dbLink );
		}
		
		DB::$dbLink = $dbLink;
		
		if ($dbName)
		{
			mysql_query("SET NAMES 'utf8'");
			mysql_query("SET collation_connection = 'UTF-8_general_ci'");
			mysql_query("SET collation_server = 'UTF-8_general_ci'");
			mysql_query("SET character_set_client = 'UTF-8'");
			mysql_query("SET character_set_connection = 'UTF-8'");
			mysql_query("SET character_set_results = 'UTF-8'");
			mysql_query("SET character_set_server = 'UTF-8'");
			
			mysql_select_db ( $dbName, $dbLink );
		}
		
		return $oldDBLink;
	}
	/**
	 * Enter description here...
	 *
	 * @param string $value
	 * @return string
	 */
	static function AddSlashes($value)
	{
		if (! get_magic_quotes_gpc ())
		{
			return addslashes ( $value );
		}
		
		return $value;
	}
	
	static function StripSlashes($value)
	{
		return stripcslashes ( $value );
	}
	
	static function EscapeInjections($value)
	{
		if (! get_magic_quotes_gpc ())
		{
			return mysql_real_escape_string ( $value );
		}
		
		return $value;
	}
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @return mixed
	 */
	static function GET($key)
	{
		if (isset ( $_GET ["$key"] ))
		{
			$value = $_GET ["$key"];
			
		if (is_array ( $value ))
			{
				$res = array();
				foreach ($value as $val){
					array_push($res, DB::EscapeInjections($val));
				}
				return $res;
			}
			
			return DB::EscapeInjections ( $value );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @return mixed
	 */
	static function POST($key)
	{
		if (isset ( $_POST ["$key"] ))
		{
			$value = $_POST ["$key"];
			
			if (is_array ( $value ))
			{
				$res = array();
				foreach ($value as $val){
					array_push($res, DB::EscapeInjections($val));
				}
				return $res;
			}
			
			$res = DB::EscapeInjections ( $value );
			return $res;
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @return mixed
	 */
	static function COOKIE($key)
	{
		if (isset ( $_COOKIE ["$key"] ))
		{
			$value = $_COOKIE ["$key"];
			
		if (is_array ( $value ))
			{
				$res = array();
				foreach ($value as $val){
					array_push($res, DB::EscapeInjections($val));
				}
				return $res;
			}
			
			$res = DB::EscapeInjections ( $value );
			return $res;
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param mixed $key
	 * @return mixed
	 */
	static function GET_POST($key)
	{
		if (($value = DB::GET ( $key )))
		{
			return $value;
		}
		
		return DB::POST ( $key );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param mixed $key
	 * @return mixed
	 */
	static function POST_GET($key)
	{
		if (($value = DB::GET ( $key )))
		{
			return $value;
		}
		
		return DB::POST ( $key );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param mixed $key
	 * @return mixed
	 */
	static function REQUEST($key)
	{
		if (isset ( $_REQUEST ["$key"] ))
		{
			if (is_array ( $_REQUEST ["$key"] ))
			{
				$res = array();
				foreach ($_REQUEST ["$key"] as $val){
					array_push($res, DB::EscapeInjections($val));
				}
				return $res;
			}
			
			return DB::EscapeInjections ( $_REQUEST ["$key"] );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $index
	 * @return bool
	 */
	static function ChangeConnection($index)
	{
		if (isset ( DB::$dbLinkArray [$index], DB::$dbArray [$index] ))
		{
			DB::$dbLink = DB::$dbLinkArray [$index];
			mysql_query("SET NAMES 'utf8'");
			mysql_query("SET collation_connection = 'UTF-8_general_ci'");
			mysql_query("SET collation_server = 'UTF-8_general_ci'");
			mysql_query("SET character_set_client = 'UTF-8'");
			mysql_query("SET character_set_connection = 'UTF-8'");
			mysql_query("SET character_set_results = 'UTF-8'");
			mysql_query("SET character_set_server = 'UTF-8'");
			mysql_select_db ( DB::$dbArray [$index], DB::$dbLink );
			
			return true;
		}
		
		return false;
	}
}
?>