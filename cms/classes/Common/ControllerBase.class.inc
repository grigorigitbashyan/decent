<?php
abstract class ControllerBase
{
	/**
	 * Enter description here...
	 *
	 * @var ActionController
	 */
	private static $controller;
	
	/**
	 * @return ControllerBase
	 */
	function __construct()
	{
	
	}
	
	/**
	 * Enter description here...
	 *
	 * @param ActionController $controller
	 */
	static public function SetController($controller)
	{
		self::$controller = $controller;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param Record|RecordFlexible $record
	 * @param string $field
	 * @param string $ownerField
	 * @return bool
	 */
	final function IsRecReadable($record, $field = 0, $ownerField = 'userID')
	{
		if (self::$controller)
		{
			return self::$controller->IsRecReadable ( $record, $field, $ownerField );
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $table
	 * @param string $field
	 * @param int $ID
	 * @param string $IDFieldName
	 * @param string $ownerField
	 * @return bool
	 */
	final function IsReadableCase($table, $field = 0, $ID = 0, $IDFieldName = 'ID', $ownerField = 'userID')
	{
		if (self::$controller)
		{
			return self::$controller->IsReadableCase($table, $field, $ID, $IDFieldName, $ownerField);
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param Record $record
	 * @param string $field
	 * @param string $ownerField
	 * @return bool
	 */
	final function IsRecUpdatable($record, $field = 0, $ownerField = 'userID')
	{
		if (self::$controller)
		{
			return self::$controller->IsRecUpdatable ( $record, $field, $ownerField );
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	final function IsDeletable($table, $userTypeID = null)
	{
		if (self::$controller)
		{
			return self::$controller->IsDeletable ( $table, $userTypeID );
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	final function IsInsertable($table, $userTypeID = null)
	{
		if (self::$controller)
		{
			return self::$controller->IsInsertable ( $table, $userTypeID );
		}
		
		return true;
	}
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table
	 * @param string $field
	 * @param int $ID
	 * @param string $IDFieldName
	 * @param string $ownerField
	 */
	final function IsUpdatableCase($table, $field = 0, $ID = 0, $IDFieldName = 'ID', $ownerField = 'userID')
	{
		if (self::$controller)
		{
			return self::$controller->IsUpdatableCase ( $table, $field, $ID, $IDFieldName, $ownerField );
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param string $field		field name
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	final function IsReadable($table, $field = 0, $userTypeID = null)
	{
		if (self::$controller)
		{
			return self::$controller->IsReadable ( $table, $field, $userTypeID );
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string|Record $table 	table name or instance of Record class
	 * @param string $field		field name
	 * @param int $userTypeID		user type ID, default is current user
	 * @return bool
	 */
	final function IsUpdatable($table, $field = 0, $userTypeID = null)
	{
		if (self::$controller)
		{
			return self::$controller->IsUpdatable ( $table, $field, $userTypeID );
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	final static function HasController()
	{
		return (self::$controller != null);
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
		if (self::$controller)
		{
			return self::$controller->IsReadableByOwner ( $table, $field, $userTypeID );
		}
		
		return true;
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
		if (self::$controller)
		{
			return self::$controller->IsDeletableByOwner($table, $userTypeID);
		}
		
		return true;
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
		if (self::$controller)
		{
			return self::$controller->IsUpdatableByOwner($table, $field, $userTypeID);
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $table
	 * @param string $userField
	 * @return bool
	 */
	function HasTableUserField($table, $userField = 'userID')
	{
		return ActionController::HasTableUserField($table, $userField);
	}
	
	/**
	 * Enter description here...
	 *
	 * @return CMSUserController
	 */
	static function GetController()
	{
		return self::$controller;
	}
}
?>