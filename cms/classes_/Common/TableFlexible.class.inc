<?php

/**
 * This class is sub class of Table. 
 * It provites abibility to add, delete or change columns, 
 * also it provides information about fields.
 *
 */
class TableFlexible extends Table
{
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_fieldNamesOutLang;
	
	/**
	 * construct
	 *
	 * @param string $table table name
	 */
	function __construct($table)
	{
		parent::__construct ( $table );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $field
	 * @param string $type
	 * @param int $length
	 * @param array $langs
	 * @param bool $NULL
	 * @param bool $unsigned
	 * @return bool
	 */
	function AddField($field, $type, $length = null, $langs = null, $NULL = false, $unsigned = false)
	{
		$res = true;
		
		// add field for multiple language
		if ($langs)
		{
			foreach ( $langs as $lang )
			{
				$res &= DBxp::AddField ( $this->m_tableName, "{$field}_{$lang}", $type, $length, $NULL );
			}
		}
		// add single field
		else
		{
			$res = DBxp::AddField ( $this->m_tableName, $field, $type, $length, $NULL, $unsigned );
		}
		
		return $res;
	}
	
	/**
	 * Delete field(s) from table
	 *
	 * @param string $field
	 * @param array $langs
	 * @return bool
	 */
	function DeleteField($field, $langs = null)
	{
		$res = true;
		
		// add field for multiple language
		if ($langs)
		{
			foreach ( $langs as $lang )
			{
				$res &= DBxp::DeleteField ( $this->m_tableName, "{$field}_{$lang}" );
			}
		}
		// add single field
		else
		{
			$res = DBxp::DeleteField ( $this->m_tableName, $field );
		}
		
		return $res;
	}
	
	/**
	 * return all fields
	 *
	 * @return array
	 */
	function GetFields()
	{
		return DBxp::GetFields ( $this->m_tableName );
	}
	
	/**
	 * return all field names included all languages
	 *
	 * @return array
	 */
	function GetFieldNames()
	{
		return DBxp::GetFieldNames ( $this->m_tableName );
	}
	
	/**
	 * return all field names, but only one name for multilang (name for name_ru and name_en)
	 *
	 * @return array
	 */
	function GetFieldNamesOutLang()
	{
		if (! $this->m_fieldNamesOutLang)
		{
			$this->m_fieldNamesOutLang = DBxp::GetFieldNamesOutLang ( $this->m_tableName );
		}
		
		return $this->m_fieldNamesOutLang;
	}
	
	/**
	 * This function returns the fields information without multiple languages.
	 *
	 * @return array
	 */
	function GetFieldsOutLangs()
	{
		return DBxp::GetFieldsOutLangs ( $this->m_tableName );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $field
	 * @return bool
	 */
	function IsFieldMultiple($field)
	{
		return in_array("{$field}+", $this->GetFieldNamesOutLang());
	}
}
?>