<?php
define ( 'STATUS_READ', 1 );
define ( 'STATUS_INSERT', 2 );
define ( 'STATUS_UPDATE', 4 );
define ( 'STATUS_DELETE', 8 );
define ( 'STATUS_OWNER_READ', 16 );
define ( 'STATUS_OWNER_UPDATE', 32 );
define ( 'STATUS_OWNER_DELETE', 64 );
/**
 * This clas can be used to controll the action. It has been created by abstract type
 * for different manipulations. For example, it can be different sub classes for CMS users and 
 * site uses, ect. 
 *
 */
abstract class ActionController
{
	function __construct()
	{
	
	}
	/**
	 * This function return status of  filed for current user, which means 
	 * what can user do with coresspondin filed of table. If the 
	 * $table is 'Record' object then call GetTableName function to get table name. 
	 * 
	 * @param string|Record $table 	table name or instance of Record class
	 * @param string $field		field name
	 * @return int 
	 */
	abstract function GetStatus($table, $field = 0, $userTypeID = null);
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param string $field		field name
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	function IsReadable($table, $field = 0, $userTypeID = null)
	{
		// first try to read table
		if ($field)
		{
			if (! $this->IsReadable ( $table, 0, $userTypeID ))
			{
				return false;
			}
		}
		// get status
		$status = $this->GetStatus ( $table, $field, $userTypeID );
		
		// if there is a information about status get information abourt filed can be readed or not.
		// In other case return true (to be readed)
		return ($status != -1) ? STATUS_READ & $status : true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param string $field		field name
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	function IsReadableByOwner($table, $field = 0, $userTypeID = null)
	{
		// first try to read table
		if ($field)
		{
			if (! $this->IsReadableByOwner ( $table, 0, $userTypeID ))
			{
				return false;
			}
		}
		// get status
		$status = $this->GetStatus ( $table, $field, $userTypeID );
		
		// if there is a information about status get information abourt filed can be readed or not.
		// In other case return true (to be readed)
		return ($status != -1) ? STATUS_OWNER_READ & $status : true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param string $field		field name
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	function IsUpdatable($table, $field = 0, $userTypeID = null)
	{
		// firt try to read table
		if ($field)
		{
			if (! $this->IsUpdatable ( $table, 0, $userTypeID ))
			{
				return false;
			}
		}
		
		// get status
		$status = $this->GetStatus ( $table, $field, $userTypeID );
		
		// if there is a information about status get information abourt filed can be readed or not.
		// In other case return true (to be readed)
		return ($status != -1) ? STATUS_UPDATE & $status : true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param string $field		field name
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	function IsUpdatableByOwner($table, $field = 0, $userTypeID = null)
	{
		// firt try to read table
		if ($field)
		{
			if (! $this->IsUpdatableByOwner ( $table, 0, $userTypeID ))
			{
				return false;
			}
		}
		
		// get status
		$status = $this->GetStatus ( $table, $field, $userTypeID );
		
		// if there is a information about status get information abourt filed can be readed or not.
		// In other case return true (to be readed)
		return ($status != -1) ? STATUS_OWNER_UPDATE & $status : true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	function IsDeletable($table, $userTypeID = null)
	{
		// get status
		$status = $this->GetStatus ( $table, 0, $userTypeID );
		
		// if there is a information about status get information abourt filed can be readed or not.
		// In other case return true (to be readed)
		return ($status != -1) ? STATUS_DELETE & $status : true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	function IsDeletableByOwner($table, $userTypeID)
	{
		// get status
		$status = $this->GetStatus ( $table, 0, $userTypeID );
		
		// if there is a information about status get information abourt filed can be readed or not.
		// In other case return true (to be readed)
		return ($status != -1) ? STATUS_OWNER_DELETE & $status : true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	function IsInsertable($table, $userTypeID = null)
	{
		// get status
		$status = $this->GetStatus ( $table, 0, $userTypeID );
		
		// if there is a information about status get information abourt filed can be readed or not.
		// In other case return true (to be readed)
		return ($status != -1) ? STATUS_INSERT & $status : true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $table
	 * @param string $userField
	 * @return bool
	 */
	static function HasTableUserField($table, $userField = 'userID')
	{
		$fields = DBxp::GetFieldNames($table);
		return in_array($userField, $fields);
	}
}
?>