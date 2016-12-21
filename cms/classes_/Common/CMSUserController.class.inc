<?php
Site::IncludeFile ( 'classes/Common/ActionController.class.inc' );

/**
 * Enter description here...
 *
 */
class CMSUserController extends ActionController
{
	/**
	 * Current user ID
	 *
	 * @var int
	 */
	protected $m_userID;
	/**
	 * Current user type ID
	 *
	 * @var int
	 */
	protected $m_userTypeID;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$user = CMS::GetUser ();
		
		$this->m_userID = $user->GetID ();
		$this->m_userTypeID = $user->GetUserTypeID ();
	}
	
	/**
	 * This function return status of  filed for current user, which means 
	 * what can user do with coresspondin filed of table. If the 
	 * $table is 'Record' object then call GetTableName function to get table name. 
	 * 
	 * @param string|Record $table 	table name or instance of Record class
	 * @param string $field		field name
	 * @param int $userTypeID		user type ID, default is current user
	 * @return int 
	 */
	function GetStatus($table, $field = null, $userTypeID = null)
	{
		// get table name
		if (is_string ( $table ))
		{
			$tableName = $table;
		}
		elseif ($table instanceof Table)
		{
			$tableName = $table->GetTableName ();
		}
		
		// get user information
		if (! $userTypeID)
		{
			$userTypeID = $this->m_userTypeID;
		}
		
		// looging for filed access type
		if ($field)
		{
			// get site languages
			$langs = View::GetSite ()->GetLanguages ();
			$length = strlen ( $field );
			
			// trim lagnuage information from field end
			foreach ( $langs as $lang )
			{
				if (substr ( $field, $length - 3 ) == "_$lang")
				{
					$field = substr ( $field, 0, $lang - 3 );
					break;
				}
			}
			
			$sql = "SELECT * FROM `table_access` WHERE `userTypeID` = $userTypeID AND `table` = '$tableName' AND `field` = '$field'";
		}
		else
		{
			$sql = "SELECT * FROM `table_access` WHERE `userTypeID` = $userTypeID AND `table` = '$tableName' AND `field` = '0'";
		}
		
		$res = DB::QueryToRow ( $sql );
		
		if (isset ( $res ['status'] ))
		{
			return ( int ) $res ['status'];
		}
		
		return - 1;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $table
	 * @param string $field
	 * @param int $ID
	 * @param string $IDFieldName
	 * @param string $ownerField
	 */
	function IsReadableCase($table, $field = 0, $ID = 0, $IDFieldName = 'ID', $ownerField = 'userID')
	{
		// is readible for current user
		if (parent::IsReadable ( $table, $field ))
		{
			return true;
		}
		// is readible if current user is owner record
		else
		{
			if (parent::IsReadableByOwner ( $table, $field ))
			{
				$owner = DB::QueryToValue ( "SELECT `$ownerField` FROM `$table` WHERE `$IDFieldName` = $ID" );
				
				if ($owner && $owner == $this->m_userID)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param Record|RecordFlexible $record
	 * @param string $field
	 * @param string $ownerField
	 * @return bool
	 */
	function IsRecReadable($record, $field = 0, $ownerField = 'userID')
	{
		try
		{
			if ($record instanceof Record || $record instanceof RecordFlexible)
			{
				$ID = $record->GetID ();
				$table = $record->GetTableName ();
				$IDFieldName = $record->GetIDFiled ();
				
				return $this->IsReadableCase ( $table, $field, $ID, $IDFieldName, $ownerField );
			}
			
			throw new Exception ( 'The object must be instance of Record and RecordFlexible!' );
		}
		catch ( Exception $e )
		{
			print $e->getMessage ();
		}
		
		return false;
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
	function IsUpdatableCase($table, $field = 0, $ID = 0, $IDFieldName = 'ID', $ownerField = 'userID')
	{
		// is readible for current user
		if (parent::IsUpdatable ( $table, $field ))
		{
			return true;
		}
		// is readible if current user is owner record
		else
		{
			if (parent::IsUpdatableByOwner ( $table, $field ))
			{
				$owner = DB::QueryToValue ( "SELECT `$ownerField` FROM `$table` WHERE `$IDFieldName` = $ID" );
				if ($owner && $owner == $this->m_userID)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param Record|RecordFlexible $record
	 * @param string $field
	 * @param string $ownerField
	 * @return bool
	 */
	function IsRecUpdatable($record, $field = 0, $ownerField = 'userID')
	{
		try
		{
			if ($record instanceof Record || $record instanceof RecordFlexible)
			{
				$ID = $record->GetID ();
				$table = $record->GetTableName ();
				$IDFieldName = $record->GetIDFiled ();
				
				return $this->IsUpdatableCase ( $table, $field, $ID, $IDFieldName, $ownerField );
			}
			
			throw new Exception ( 'The object must be instance of Record or RecordFlexible' );
		}
		catch ( Exception $e )
		{
			print $e->getMessage ();
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $table
	 * @param string $field
	 * @param int $ID
	 * @param string $IDFieldName
	 * @param string $ownerField
	 */
	function IsDeletableCase($table, $ID = 0, $IDFieldName = 'ID', $ownerField = 'userID')
	{
		// is readible for current user
		if (parent::IsDeletable ( $table ))
		{
			return true;
		}
		// is readible if current user is owner record
		else
		{
			if (parent::IsDeletableByOwner ( $table ))
			{
				$owner = DB::QueryToValue ( "SELECT `$ownerField` FROM `$table` WHERE `$IDFieldName` = $ID" );
				
				if ($owner && $owner == $this->m_userID)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param Record $record
	 * @param string $field
	 * @param string $ownerField
	 * @return bool
	 */
	function IsRecDeletable(Record $record, $ownerField = 'userID')
	{
		$ID = $record->GetID ();
		$table = $record->GetTableName ();
		$IDFieldName = $record->GetIDFiled ();
		
		return $this->IsDeletableCase ( $table, $ID, $IDFieldName, $ownerField );
	}
}
?>