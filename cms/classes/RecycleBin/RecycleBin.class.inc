<?php
Site::IncludeFile('classes/Folder/FolderXP.class.inc');
/**
 * Enter description here...
 *
 * @package RecycleBin
 */
class RecycleBin
{
	protected $m_ID;
	protected $m_type;
	protected $m_value;
	protected $m_names;
	
	function __construct($ID = null)
	{
		$this->m_ID = $ID;
		
		if ($this->m_ID)
		{
			$this->Read ();
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $value
	 * @param array $name
	 * @return bool
	 */
	function Insert($value, $name = null)
	{
		$serValue = serialize ( $value );
		
		if (! get_magic_quotes_gpc ())
		{
			$serValue = addslashes ( $serValue );
		}
		
		// get languages
		$site = View::GetSite ();
		$langs = $site->GetLanguagesName ();
		$langKeys = array_keys ( $langs );
		
		$nameInDB = '';
		$nameValueIn = '';
		
		foreach ( $langKeys as $lang )
		{
			$nameInDB .= ", `name_$lang`";
			
			if ($name == null)
			{
				$nameValueIn .= ", 'unknowen item'";
			}
			elseif (is_string ( $name ))
			{
				$nameValueIn .= ", '$name'";
			}
			elseif (is_array ( $name ))
			{
				if (isset ( $name [$lang] ))
				{
					$nameValueIn .= ", '{$name[$lang]}'";
				}
			}
			else
			{
				$nameValueIn .= ", 'unknowen type'";
			}
		}
		
		////////////////////////////////////////
		$sql = "INSERT INTO `recycle_bin` 
				(`ID` , `type` , `value` $nameInDB)
				VALUES (NULL , '{$value['cms_table_name']}', '$serValue' $nameValueIn);";
		
		$res = DB::Query ( $sql, null, true );
		
		if (! $res)
		{
			return false;
		}
		
		return DB::GetLastID ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $value
	 * @return bool
	 */
	function Push($value)
	{
		$unSer = unserialize ( $this->m_value );
		
		array_push ( $unSer, $value );
		
		$newValue = serialize ( $unSer );
		
		if (! get_magic_quotes_gpc ())
		{
			$newValue = addslashes ( $newValue );
		}
		// save value
		$sql = "UPDATE `recycle_bin` SET `value` = '$newValue' WHERE `recycle_bin`.`ID` = {$this->m_ID} LIMIT 1 ;";
		$res = DB::Query ( $sql, null, true );
		
		return $res;
	}
	/**
	 * Read from database
	 *
	 */
	function Read()
	{
		$sql = "SELECT * FROM `recycle_bin` WHERE `ID` = {$this->m_ID}";
		$arrat = DB::QueryToRow ( $sql );
		
		if ($arrat)
		{
			$this->m_type = $arrat ['type'];
			$this->m_value = $arrat ['value'];
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function Restore()
	{
		$unSer = unserialize ( $this->m_value );
		// if onli one recode, the out it to array
		if(isset($unSer['cms_table_name']))
		{
			$unSer = array($unSer);
		}
		
		$res = true;
		
		// file case
		$fileOldName = null;
		$fileRecName = null;
		
		if ($unSer)
		{
			foreach ( $unSer as $element )
			{
				$tableName = $element ['cms_table_name'];
				
				// file
				if ($tableName == 'folder')
				{
					$fileOldName = $element ['name'];
					$fileRecName = "{$element['parentID']}-{$element['name']}";
					
					try
					{
						$folder = new Folder ( $element ['parentID'] );
						
						$path = $folder->GetFolderPath ();
						$root = $folder->GetRoot ();
						
						$fullPath = $root . $path . '/' . $fileOldName;
						$resFullPat = Site::GetRecycleBinPath () . $fileRecName;
						
						$res &= rename ( $resFullPat, $fullPath );
					}
					catch ( Exception $e )
					{
						print $e->getMessage ();
					}
					
					reset ( $element );
				}
				elseif ($tableName == 'pr_picture')
				{
					$imageID = $element ['ID'];
					$path = $element ['path'];
					
					// old name
					$lasPos = strrpos ( $path, '/' );
					$fileName = substr ( $path, $lasPos + 1 );
					$oldName = 'product-' . $imageID . '-' . $fileName;
					
					// new name
					$folderPath = '../media/images/products/' . $element ['productID'] . '/';
					
					if (! file_exists ( $folderPath ))
					{
						FolderXP::Mkdir( $folderPath, 0753 );
					}
					
					$newAddress = $folderPath . $fileName;
					$oldAddress = Site::GetRecycleBinPath () . $oldName;
					
					// move file
					rename ( $oldAddress, $newAddress );
				}
				
				// database restore ///////////////////////////
				$keyString = '(';
				$valueString = '(';
				
				foreach ( $element as $key => $value )
				{
					if ($key == 'cms_table_name')
						continue;
					
					if (! get_magic_quotes_gpc ())
					{
						$value = addslashes ( $value );
					}
					
					$repValue = (is_numeric ( $value )) ? $value : "'$value'";
					
					$keyString .= ((strlen ( $keyString ) > 1)) ? ", `$key`" : "`$key`";
					$valueString .= ((strlen ( $valueString ) > 1)) ? ", $repValue" : $repValue;
				}
				
				$keyString .= ')';
				$valueString .= ')';
				
				$sql = "INSERT INTO `$tableName` $keyString VALUES $valueString";
				$res &= DB::Query ( $sql, null, true );
				
				if (! $res)
				{
					break;
				}
			}
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function Delete()
	{
		// file
		$unSer = unserialize ( $this->m_value );
		
		// if onli one recode, the out it to array
		if(isset($unSer['cms_table_name']))
		{
			$unSer = array($unSer);
		}
		
		$res = true;
		
		// file case
		$fileRecName = null;
		
		if ($unSer)
		{
			foreach ( $unSer as $element )
			{
				$tableName = $element ['cms_table_name'];
				
				// file
				if ($tableName == 'folder')
				{
					$fileRecName = "{$element['parentID']}-{$element['name']}";
					
					try
					{
						$resFullPat = Site::GetRecycleBinPath () . $fileRecName;
						$res = FolderXP::Unlink( $resFullPat );
					}
					catch ( Exception $e )
					{
						print $e->getMessage ();
					}
				
				}
				elseif ($tableName == 'pr_picture')
				{
					$imageID = $element ['ID'];
					$path = $element ['path'];
					
					// old name
					$lasPos = strrpos ( $path, '/' );
					$fileName = substr ( $path, $lasPos + 1 );
					$oldName = 'product-' . $imageID . '-' . $fileName;
					
					$oldAddress = Site::GetRecycleBinPath () . $oldName;
					
					try
					{
						$res = FolderXP::Unlink( $oldAddress );
					}
					catch ( Exception $e )
					{
						print $e->getMessage ();
					}
				}
				elseif ($tableName == 'pr_main')
				{
					$productID = $element ['ID'];
					
					$folderAddress = "../media/images/products/$productID/";
					
					try
					{
						$res = FolderXP::Rmdir( $folderAddress );
					}
					catch ( Exception $e )
					{
						print $e->getMessage ();
					}
				}
			}
		}
		
		// database
		$sql = "DELETE FROM `recycle_bin` WHERE `ID` = {$this->m_ID}";
		$res = DB::Query ( $sql, null, true );
		
		return $res;
	}
}
?>