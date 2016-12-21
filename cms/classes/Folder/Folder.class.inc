<?php
// includes
Site::IncludeFile ( 'classes/Folder/FolderStructure.class.inc' );
Site::IncludeFile ( 'classes/Common/Image.class.inc' );
Site::IncludeFile ( 'classes/Folder/FolderXP.class.inc' );

/**
 * Enter class description here...
 *
 * @package Folder
 */
class Folder extends NamedRecord
{
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_parentID;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_isDelete;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_filetype;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_fileOrFolder;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @return Folders
	 */
	function __construct($ID = null)
	{
		parent::__construct ( $ID, 'folder' );

		$this->Read ();
	}
	
	/**
	 * Delete record from database
	 *
	 * @param string $toRecycleBin
	 * @return bool
	 */
	function Delete($toRecycleBin = true)
	{
		$root = $this->GetRoot ();
		
		if ($this->m_ID == null)
		{
			return false;
		}
		
		// cann`t be deleted (image, sounds, flashes and other files)
		if ($this->m_isDelete == 0)
		{
			return false;
		}
		
		// get sub files or folders, if it is not 0 then return false
		$path = $this->GetFolderPath ();
		
		$res = false;
		
		if ($this->m_fileOrFolder)
		{
			if (file_exists ( $root . $path ))
			{
				$oldPath = $root . $path;
				
				if ($toRecycleBin)
				{
					$parentID = $this->GetParentID ();
					$resPath = Site::GetRecycleBinPath ();
					
					$newPath = $resPath . $parentID . '-' . $this->GetName ();
					
					$res = FolderXP::Rename($oldPath, $newPath);
				}
				else
				{
					$res = FolderXP::Unlink($oldPath);
				}
			
			}
			else
			{
				$res = true;
			}
		}
		else
		{
			// get sub folders of  corrent folder
			$res = DB::QueryToArray ( "select * from `folder`  where `parentID` = $this->m_ID" );
			
			// folder has file or folder
			if ($res)
				return false;
				
			// delete folder
			if (file_exists ( $root . $path ))
			{
				$res = FolderXP::Rmdir( $root . $path );
			}
			else
			{
				$res = true;
			}
		}
		
		if ($res)
		{
			$filetype = $this->m_filetype;
			$res = $this->DeleteRecord ( $this->m_fileOrFolder );
			$this->m_filetype = $filetype;
		}
		
		return $res;
	}
	/**
	 * Update record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Update($properties)
	{
		return $this->UpdateValues ( $properties );
	}
	/**
	 * Insert new record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Insert($name = null, $type = null)
	{
		$name = trim ( $name );
		
		$root = $this->GetRoot ();
		
		// if name is empaty
		if (strlen ( $name ) < 1)
			return false;
			
		/////////////////////////////////////////////////////

		$folderPath = $this->GetFolderPath ();
		$fullName = "{$root}$folderPath/$name/";
		
		$res = FolderXP::Mkdir($fullName, 0753 );
		
		if ($res != false)
		{
			$properties ['name'] = $name;
			$properties ['parentID'] = $this->m_ID;
			$properties ['filetype'] = ($type) ? $type : $this->m_filetype;
			
			return $this->InsertRecord ( $properties );
		}
		
		return false;
	}
	
	/**
	 * Return folder path
	 *
	 * @param int $folderID
	 * @param int $deep
	 * @return string
	 */
	function GetFolderPath($folderID = null, $deep = 0)
	{
		// inshuranse
		$fs = new FolderStructure ( );
		
		if ($deep > $fs->GetMaxDepth ())
			return false;
			////////////////////////////////////////////
		if ($folderID == null)
		{
			$folderID = $this->m_ID;
		}
		if ($folderID == 1)
		{
			return $fs->GetRoot ();
		}
		
		if ($folderID == - 1)
		{
			$secRoot = $fs->GetSecurRoot ();
			
			if(strpos($secRoot, '/') !== false)
			{
				$secRoot = substr($secRoot, 0, strlen($secRoot) - 1);
			}
			// return only name
			return $secRoot;
		}
		
		$corrFolder = new Folder ( $folderID ); // get corrent folder
		$name = $corrFolder->m_name; // get folder name

		$parentPath = $this->GetFolderPath ( $corrFolder->m_parentID, $deep + 1 ); // get path for parent folder

		return "$parentPath/$name";
	
	}
	
	/**
	 * Return global root of folder (1 for media, -1 for security folder root)
	 *
	 * @param int $folderID
	 * @param int $deep
	 * @return int
	 */
	function GetFolderRootID($folderID = null, $deep = 0)
	{
		// inshuranse
		$fs = new FolderStructure ( );
		
		if ($deep > $fs->GetMaxDepth ())
			return false;
			////////////////////////////////////////////
		if ($folderID == null)
		{
			$folderID = $this->m_ID;
		}
		
		///////////////
		if ($folderID == 1)
		{
			return $folderID;
		}
		if ($folderID == - 1)
		{
			return $folderID;
		}
		
		$corrFolder = new Folder ( $folderID ); // get corrent folder
		

		$parentID = $this->GetFolderRootID ( $corrFolder->m_parentID, $deep + 1 ); // get path for parent folder
		

		return $parentID;
	}
	
	/**
	 * Get sub folders(0) or files(1), default 0 array
	 * ID, parentID, name, sorted by name 
	 *
	 * @param int $parentID
	 * @param int $type
	 * @return array|null
	 */
	function GetSub($type = 0)
	{
		// get sub folders of  corrent folder
		$query = "select `ID`, `parentID`, `name` from `folder` where `parentID` = $this->m_ID AND `fileOrFolder` = $type order by `name`";
		
		return DB::QueryToArray ( $query );
	}
	
	/**
	 * Return last uploaded file
	 * @return Folder
	 */
	function GetLastUploadedFile()
	{
		$query = "select ID from folders  where parentID = $this->m_ID AND fileOrFolder = 1 ORDER BY `ID` DESC LIMIT 1"; // get sub folders of  corrent folder
		$result = DB::QueryToArray ( $query );
		
		if ($result)
		{
			$folder = new Folder ( $result [0] );
			return $folder;
		}
		
		return NULL;
	}
	
	/**
	 * Upload file in corrent category
	 *
	 * @param string $msg
	 * @return bool
	 */
	function UploadFile($maxW = null)
	{
		$name = str_replace ( " ", "", $_FILES ['gl2_img'] ['name'] );
		$fileTempName = str_replace ( " ", "", $_FILES ['gl2_img'] ['tmp_name'] );
		$size = str_replace ( " ", "", $_FILES ['gl2_img'] ['size'] );
		
		if ($size < 1)
		{
			return false;
		}
		
		// get uploaded file extension
		$path_parts = pathinfo ( $_FILES ['gl2_img'] ['name'] );
		$ext = $path_parts ['extension'];
		$ext = strtolower ( $ext );
		
		// replase uper chares to lower
		$pos = strripos ( $name, $ext );
		$name = substr ( $name, 0, $pos ) . $ext;
		
		//////////////////////////////////////////////////////
		$filetype = $this->m_filetype;
		
		if ($filetype > 0) //uplotible file types 
		{
			$fs = new FolderStructure ( );
			$types = $fs->GetTypes ( $filetype );
			$res = array_search ( $ext, $types );
			
			if (! is_int ( $res ))
			{
				print('Type uncoresponding!');
				return false;
			}
		}
		//////////////////////////////////////////////////////
		$root = $this->GetRoot ();
		
		$folderPath = $this->GetFolderPath ();
		
		if(!is_dir($root . $folderPath . "/"))
		{
			print("The directory $root$folderPath does not exist!");
			return false;
		}
		
		$fileAddress = $root . $folderPath . "/" . $name;
		if (file_exists ( $fileAddress ))
		{
			print('The file has olready exsist!');
			return false;
		}
		
		// uploading
		$res = FolderXP::Upload($_FILES ['gl2_img'] ['tmp_name'], $fileAddress);
		
		if(in_array($ext, array('jpg', 'jpeg', 'png', 'gif')))
		{
			$im = new Image();
			$im->ChangeSizeInBox($fileAddress, $maxW);	
		}
		
		if ($res)
		{
			$properties ['name'] = $name;
			$properties ['parentID'] = $this->m_ID;
			$properties ['filetype'] = $this->m_filetype;
			$properties ['fileOrFolder'] = 1;
			
			$res = $this->InsertRecord ( $properties );
		}
		
		////
		return $res;
	}
	
	/**
	 * Return value of m_parentID
	 *
	 * @return string
	 */
	function GetParentID()
	{
		return $this->m_parentID;
	}
	
	/**
	 * Return value of m_isDelete
	 *
	 * @return int
	 */
	function GetIsDelete()
	{
		return $this->m_isDelete;
	}
	
	/**
	 * Return value of m_filetype
	 *
	 * @return int
	 */
	function GetFiletype()
	{
		return $this->m_filetype;
	}
	
	/**
	 * Return value of m_fileOrFolder
	 *
	 * @return int
	 */
	function GetFileOrFolder()
	{
		return $this->m_fileOrFolder;
	}
	
	/**
	 * Set value to m_parentID  
	 *
	 * @param string $parentID
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetParentID($parentID, $dbupdate = true)
	{
		return $this->UpdateValue ( 'parentID', $parentID, $dbupdate );
	}
	
	/**
	 * Set value to m_isDelete  
	 *
	 * @param int $isDelete
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetIsDelete($isDelete, $dbupdate = true)
	{
		return $this->UpdateValue ( 'isDelete', $isDelete, $dbupdate );
	}
	
	/**
	 * Set value to m_filetype  
	 *
	 * @param int $filetype
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetFiletype($filetype, $dbupdate = true)
	{
		return $this->UpdateValue ( 'filetype', $filetype, $dbupdate );
	}
	
	/**
	 * Set value to m_fileOrFolder  
	 *
	 * @param int $fileOrFolder
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetFileOrFolder($fileOrFolder, $dbupdate = true)
	{
		return $this->UpdateValue ( 'fileOrFolder', $fileOrFolder, $dbupdate );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetRoot()
	{
		return '../';
	}
}
?>