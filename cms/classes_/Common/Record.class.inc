<?php
// includes
Site::IncludeFile ( 'classes/Common/ActionResult.class.inc' );
Site::IncludeFile ( 'classes/Common/Table.class.inc' );
Site::IncludeFile ( 'classes/RecycleBin/RecycleBin.class.inc' );

/**
 * This class is base class for all clases, tath works with database. 
 * It has error property where are all error messsages. 
 *
 * @package Main
 */
abstract class Record extends Table
{
	/**
	 * ID of element
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
	 * @param int $ID
	 * @param string $tableName
	 */
	function __construct($ID, $tableName, $IDFiled = 'ID')
	{
		parent::__construct ( $tableName );
		
		$this->m_ID = $ID;
		$this->m_IDFiled = $IDFiled;
	}
	
	/**
	 * Return ID of element
	 *
	 * @return int
	 */
	public final function GetID()
	{
		return $this->m_ID;
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
		if (! $this->IsDeletable ( $this ))
		{
			return false;
		}
		$IDName = "m_".$this->m_IDFiled;
		$forRec = null;
		// get record ///////////////////////////////////////////////////////////
		$sql = "SELECT * FROM `$this->m_tableName` WHERE `$this->m_IDFiled` = '{$this->$IDName}' LIMIT 1";
		$allFields = DB::QueryToRow ( $sql );
		
		if ($allFields)
		{
			$allFields ['cms_table_name'] = $this->m_tableName;
			$forRec = $allFields;
		}
		else 
		{
			return false;
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
		$sql = "DELETE FROM `$this->m_tableName` WHERE `$this->m_IDFiled` = '{$this->$IDName}' LIMIT 1";
		$res = DB::Query ( $sql );

		if ($res)
		{
			$this->m_ID = null;
			
			// 
			if ($addToRecycle)
			{
				$nameRef = 'm_name';
				
				$names = null;
				if (isset ( $this->$nameRef ))
				{
					$names = $this->$nameRef;
				}
				
				$rb = new RecycleBin ( );
				return $rb->Insert ( $forRec, $names );
			}
			
			return $forRec;
		}
		
		return false;
	}
	
	/**
	 * Read from database
	 *
	 * @return bool
	 */
	protected final function Read()
	{
		// if is set controller then chake the control
		if (! $this->IsReadable ( $this ))
		{
			return false;
		}
		
		// get all properties of element, then get only names
		$class_vars = get_object_vars ( $this );
		$properies = array_keys ( $class_vars );
		
		// if Id is null then set all properties to null
		if ($this->m_ID == null)
		{
			foreach ( $properies as $property )
			{
				// set property value = null
				$propName = "m_$property";
				$this->$propName = null;
			}
			
			return false;
		}
		else
		{
			// get object from table
			$IDValue = is_numeric ( $this->m_ID ) ? $this->m_ID : "'$this->m_ID'";
			$sql = "SELECT * FROM `$this->m_tableName` WHERE `{$this->m_IDFiled}` = $IDValue";
			$row = DB::QueryToRow ( $sql );
			
			if ($row)
			{
				// get site object, it will be used for languages information
				$site = View::GetSite ();
				$languages = $site->GetLanguages ();
				
				// get ojbect
				foreach ( $properies as $property )
				{
					// delete first 2 symbols
					$prp = substr ( $property, 2 );
					
					$prpName = "m_$prp";
					
					// properye without language
					if (isset ( $row [$prp] ))
					{
						$this->$prpName = $row [$prp];
					}
					// properye with language
					else
					{
						foreach ( $languages as $langis )
						{
							if ((isset ( $row ["{$prp}_{$langis}"] )))
							{
								$this->$prpName = array ();
								$arrayPtr = &$this->$prpName;
								
								foreach ( $languages as $lang )
								{
									if (isset ( $row ["{$prp}_{$lang}"] ))
									{
										$arrayPtr [$lang] = $row ["{$prp}_{$lang}"];
									}
								}
								break;
							}
						}
					}
				}
				
				return true;
			}
			else
			{
				$this->m_ID = null;
				return false;
			}
		}
	}
	
	/**
	 * Update database
	 *
	 * @param string $property
	 * @param string $value
	 * @param bool $updateProperty
	 * @return ActionResult
	 */
	protected final function UpdateValue($property, $value, $updateProperty = true)
	{
		// if is set controller then chake the control
		if (! $this->IsUpdatable ( $this, $property ))
		{
			return false;
		}
		
		/////////////////////////////////////////////////////////
		if (! $this->m_ID)
		{
			return false;
		}
		
		// generate action result
		//$ar = $this->GetActionResult ();
		

		//$ar->SetFieldID ( $property );
		//$ar->SetNewValue ( $value );
		//$ar->SetActionTypeID ( 2 );
		
		$IDName = "m_".$this->m_IDFiled;
		// get old value
		$sql = "SELECT `$property` FROM `$this->m_tableName` WHERE `$this->m_tableName`.`$this->m_IDFiled` = '{$this->$IDName}' LIMIT 1;";
		
		//		$propValue = DB::QueryToValue( $sql );
		DB::QueryToValue ( $sql );
		
		if (DB::GetError ())
		{
			die ( 'Invalid query: ' . DB::GetError () . " <br /> SQL - $sql" );
		}
		
		//		$ar->SetOldValue ( $propValue );
		

		// update value
		
		
		$sql = "UPDATE `$this->m_tableName` SET `$property` = '$value' WHERE `$this->m_tableName`.`$this->m_IDFiled` = '{$this->$IDName}' LIMIT 1;";
		$res = DB::Query ( $sql );
		
		// get value
		
		$sql = "SELECT `$property` FROM `$this->m_tableName` WHERE `$this->m_tableName`.`$this->m_IDFiled` = '{$this->$IDName}' LIMIT 1;";
		$value = DB::QueryToValue ( $sql );
		
		if ($res && $updateProperty)
		{
			$property = "m_$property";
			$class_vars = get_object_vars ( $this );
			
			$res = array_key_exists ( $property, $class_vars );
			
			if ($res)
			{
				$this->$property = $value;
				//				$ar->SetStatus ();
			}
			else
			{
				$proplen = strlen ( $property );
				$subprop = substr ( $property, 0, $proplen - 3 );
				
				$res = array_key_exists ( $subprop, $class_vars );
				
				if ($res)
				{
					$subext = substr ( $property, $proplen - 2, 2 );
					$prpname = "$subprop" . "[" . "\"$subext\"" . "]";
					$this->$prpname = $value;
					//					$ar->SetStatus ();
				}
			}
		}
		else
		{
			//			$ar->SetStatus ( 1 );
		}
		
		return true;
		//		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $properties
	 * @return bool
	 */
	protected final function UpdateValues($properties)
	{
		$str = '';
		foreach ( $properties as $key => $val )
		{
			if (strlen ( $str ) > 0)
			{
				$str .= ', ';
			}
			
			if (! get_magic_quotes_gpc ())
			{
				$val = addslashes ( $val );
			}
			
			$str .= "`$key` = '$val'";
		}
		
		$sql = "UPDATE `$this->m_tableName` SET $str WHERE `$this->m_tableName`.`$this->m_IDFiled` = $this->m_ID LIMIT 1;";
		return DB::Query ( $sql, null, true );
	}
	
	/**
	 * Insert new record to database
	 * @param array $properties
	 * @return bool
	 */
	protected final function InsertRecord($properties = null)
	{
		// if is set controller then chake the control
		if (! $this->IsInsertable ( $this ))
		{
			return false;
		}
		
		$sql = '';
		
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
		
		$res = DB::Query ( $sql, null, true );
		
		if ($res)
		{
			$this->m_ID = (isset ( $properties ["{$this->m_IDFiled}"] )) ? $properties ["{$this->m_IDFiled}"] : DB::GetLastID ();
			$this->Read ();
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function GetActionResult()
	{
		$ar = new ActionResult ( );
		
		$ar->SetRecordID ( $this->m_ID );
		$ar->SetModuleID ( $this->m_tableName );
		
		return $ar;
	}
	
	/**
	 * return ID field name
	 *
	 * @return string
	 */
	function GetIDFiled()
	{
		return $this->m_IDFiled;
	}
}
?>