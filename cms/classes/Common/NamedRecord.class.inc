<?php
Site::IncludeFile ( 'classes/Common/Record.class.inc' );

/**
 * Enter description here...
 *
 * @package Main
 */
class NamedRecord extends Record
{
	/**
	 * Enter description here...
	 *
	 * @var array|string
	 */
	protected $m_name;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @param string $tableName
	 * @param string $IDFiled - if ID filed is defferent from `ID`
	 */
	function __construct($ID, $tableName, $IDFiled = 'ID')
	{
		parent::__construct ( $ID, $tableName, $IDFiled );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $lang
	 * @return string
	 */
	final function GetName($lang = null, $stripSlashes = true)
	{
		$name = null;
		
		if (is_string ( $this->m_name ))
		{
			$name = $this->m_name;
		}
		elseif (is_array ( $this->m_name ) && $lang && isset ( $this->m_name [$lang] ))
		{
			$name = $this->m_name [$lang];
		}
		
		// strip slahses
		if($name && $stripSlashes)
		{
			$name = htmlspecialchars($name);
		}
		
		return $name;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $newName
	 * @param string $lang
	 * @param bool $updateDB
	 * @return bool
	 */
	final function SetName($newName, $lang = null, $updateDB = true)
	{
		$name = ($lang) ? "name_{$lang}" : 'name';
		return $this->UpdateValue ( $name, trim ( $newName ), $updateDB );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $addToRecycle
	 * @param string $additionalStore
	 * @return bool|array
	 */
	function Delete($addToRecycle = false, $additionalStore = null)
	{
		return $this->DeleteRecord ( $addToRecycle, $additionalStore );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 */
	function Insert($params)
	{
		return $this->InsertRecord($params);
	}
}
?>