<?php
Site::IncludeFile ( "classes/Common/DB.class.inc" );
/**
 * Enter description here...
 *
 * @package Main
 */
class Table extends ControllerBase
{
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_tableName;
	
	/**
	 * Enter description here...
	 *
	 * @param string $tableName
	 * @return Table
	 */
	function __construct($tableName)
	{
		parent::__construct();
		
		$this->m_tableName = $tableName;
	}
	
	/**
	 * Return table name
	 *
	 * @return string
	 */
	function GetTableName()
	{
		return $this->m_tableName;
	}
	
	/**
	 * This function returns true if uniqu exist in table.
	 * In other clase it return false.
	 * 
	 * @param string $field
	 * @param mixed $value
	 * @return bool
	 */
	function IsUniquExist($field, $value)
	{
		$fieldValue = (is_numeric($value)) ? $value : "'$value'";
		$sql = "SELECT count(*) FROM `{$this->m_tableName}` WHERE `{$this->m_tableName}`.`$field` = $fieldValue";
		return DB::QueryToValue($sql);
	}
}
?>