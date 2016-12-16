<?php
class RecordFlexible extends TableFlexible
{
	/**
	 * Enter description here...
	 *
	 * @var ActionController
	 */
	protected static $controller;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_ID;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_IDFiled;
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_row;
	/**
	 * Enter description here...
	 *
	 * @param string $table
	 * @param int $ID
	 * @param string $IDName
	 */
	function __construct($table, $ID = null, $IDName = 'ID')
	{
		parent::__construct ( $table );
		
		$this->m_IDFiled = $IDName;
		$this->m_ID = $ID;
		
		$this->Read ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @param string $IDName
	 * @return bool
	 */
	function Read()
	{
		// if is set controller then chake the control
		if (RecordFlexible::$controller)
		{
			if (! RecordFlexible::$controller->IsReadable ( $this ))
			{
				return false;
			}
		}
		
		$IDValue = is_numeric($this->m_ID) ? $this->m_ID : "'$this->m_ID'";
		$sql = "SELECT * FROM `$this->m_tableName` WHERE `$this->m_IDFiled` = $IDValue";
		
		$this->m_row = DB::QueryToRow ( $sql );
		
		if (! $this->m_row)
		{
			$this->m_ID = null;
			return false;
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return unknown
	 */
	function GetID()
	{
		return $this->m_ID;
	}
	
	/**
	 * Chack if there is a $field valu return it, other case chake $field_$lang field to return.
	 * return NULL if filed does not found.
	 *
	 * @param string $field
	 * @param string $lang
	 * 
	 * @return mixed
	 */
	function GetFieldValue($field, $lang = null)
	{
		if (isset ( $this->m_row ["$field"] ))
		{
			return $this->m_row ["$field"];
		}
		
		if (isset ( $this->m_row ["{$field}_{$lang}"] ))
		{
			return $this->m_row ["{$field}_{$lang}"];
		}
		
		return null;
	}
	
	/**
	 * Return array of multiple valus for the field.
	 * This is for languages. 
	 *
	 * @param string $field
	 * 
	 * @return array
	 */
	function GetFieldMultValue($field)
	{
		$array = array ();
		
		$langs = View::GetSite ()->GetLanguages ();
		
		foreach ( $langs as $lang )
		{
			if (isset ( $this->m_row ["{$field}_{$lang}"] ))
			{
				$array["$lang"] = $this->m_row ["{$field}_{$lang}"];
			}
		}
		
		return $array;
	}
	
	/**
	 * Update database
	 *
	 * @param string $property
	 * @param string $value
	 * @param bool $updateProperty
	 * @return ActionResult
	 */
	protected final function UpdateValue($property, $value, $lang = null)
	{
		// if is set controller then chake the control
		if (RecordFlexible::$controller)
		{
			if (! RecordFlexible::$controller->IsUpdatable ( $this, $property ))
			{
				return false;
			}
		}
		/////////////////////////////////////////////////////////
		if (! $this->m_ID)
		{
			return false;
		}
		
		if (! get_magic_quotes_gpc ())
		{
			$value = addslashes ( $value );
		}
		
		if (! array_key_exists($property, $this->m_row))
		{
			if (! array_key_exists("{$property}_{$lang}", $this->m_row))
			{
				return false;
			}
			else
			{
				$property = "{$property}_{$lang}";
			}
		}
		// update value
		$sql = "UPDATE `$this->m_tableName` SET `$property` = '$value' WHERE `$this->m_tableName`.`$this->m_IDFiled` = $this->m_ID LIMIT 1;";
		
		if (DB::Query ( $sql ))
		{
			return $this->Read ();
		}
		
		return true;
	}
	
	/**
	 * Insert new record to database
	 * @param array $properties
	 * @return bool
	 */
	protected final function InsertRecord($properties = null)
	{
		// if is set controller then chake the control
		if (RecordFlexible::$controller)
		{
			if (! RecordFlexible::$controller->IsInsertable ( $this ))
			{
				return false;
			}
		}
		
		// inserting with properies
		if ($properties)
		{
			$sqlProp = '';
			$sqlValu = '';
			
			foreach ( $properties as $key => $val )
			{
				if (strlen ( $sqlProp ) > 0)
				{
					$sqlProp .= ', ';
					$sqlValu .= ', ';
				}
				
				$sqlProp .= "`$key`";
				$sqlValu .= "'$val'";
			}
			
			$sql = "INSERT INTO `$this->m_tableName` ($sqlProp) VALUES ($sqlValu);";
		}
		// inserting empty record
		else
		{
			$sql = "INSERT INTO `$this->m_tableName` (`$this->m_IDFiled` ) VALUES (NULL);";
		}
		
		$res = DB::Query ( $sql, true );
		
		if ($res)
		{
			$this->m_ID = DB::GetLastID ();
			$IDField = $this->m_IDFiled;
			$this->m_IDFiled = 'ID';
			$this->Read ();
			$this->m_IDFiled = $IDField;
		}
		
		return $res;
	}
	
	/**
	 * Delete from datababse and return result
	 *
	 * @param bool $addToRecycle
	 * @param string $additionStore
	 * @return bool|array
	 */
	public final function DeleteRecord($addToRecycle = true, $additionStore = null)
	{
		// if is set controller then chake the control
		if (RecordFlexible::$controller)
		{
			if (! RecordFlexible::$controller->IsDeletable ( $this ))
			{
				return false;
			}
		}
		
		$forRec = null;
		// get record ///////////////////////////////////////////////////////////
		$sql = "SELECT * FROM `$this->m_tableName` WHERE `$this->m_IDFiled` = $this->m_ID LIMIT 1";
		$allFields = DB::QueryToRow ( $sql );
		
		if ($allFields)
		{
			$allFields ['cms_table_name'] = $this->m_tableName;
			$forRec = $allFields;
		}
		
		// add adition stor infromation
		if ($additionStore)
		{
			foreach ( $additionStore as $item )
			{
				array_push ( $forRec, $item );
			}
		}
		
		/////////////////////////////////////////////////////////////////////////
		$sql = "DELETE FROM `$this->m_tableName` WHERE `$this->m_IDFiled` = $this->m_ID LIMIT 1";
		$res = DB::Query ( $sql );
		
		if ($res)
		{
			$this->m_ID = null;
			
			// 
			if ($addToRecycle)
			{
				// get names
				$names = $this->GetFieldMultValue('name');
				
				// create rysivleben
				$rb = new RecycleBin ( );
				return $rb->Insert ( $forRec, $names );
			}
			
			return $forRec;
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetIDFiled()
	{
		return $this->m_IDFiled;
	}
}
?>