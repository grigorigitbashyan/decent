<?php
/**
 * Enter description here...
 *
 * @package Folder
 */
class FolderStructure
{
	protected $m_fileTypes;
	
	/**
	 * maximum deep of folders
	 *
	 * @var int
	 */
	protected $m_maxDeep = 16;
	
	/**
	 * Media folder root
	 *
	 * @var string
	 */
	protected $m_home = "media";
	
	/**
	 * security folder root
	 *
	 * @var string
	 */
	function __construct()
	{
		$this->m_fileTypes = array(
	/* image */				'1' => array ('jpeg', 'jpg', 'gif', 'png' ),
	/* flash and video */	'2' => array ('wav', 'avi', 'swf', 'flv' ),
	/* sound */				'3' => array ('mp3' ),
	/* document */			'4' => array ('doc', 'docx', 'txt', 'rtf', 'zip', 'rar', 'pdf', 'xls', 'xlsx', 'ppt' ) );
	}
	
	function GetTypes($type)
	{
		return (isset ( $this->m_fileTypes [$type] )) ? $this->m_fileTypes [$type] : null;
	}
	/**
	 * return folder structure from database first folder is home and its parent is -1,
	 * the oders hav parent 0 or great by 0, table name is folder and has 3 fields id, name, parentID 
	 *
	 * @param int $folderID
	 * @param int $currentDeep
	 * @return array|null
	 */
	function GetFoltersFromDB($folderID = 1, $currentDeep = 1)
	{
		// Home ID is 1
		if ($currentDeep > $this->m_maxDeep)
			return NULL;
			// get corrent folder
		$correntFolder = new Folder ( $folderID );
		
		// get sub files
		$folder = new Folder ( $folderID );
		$files = $folder->GetSub ( 1 );
		$count = count ( $files ); // count of files in corrent folder
		// get sub folders
		$folders = $folder->GetSub ( 0 );
		
		$tree = array ("ID" => $correntFolder->GetID (), "name" => $correntFolder->GetName (), "ref" => NULL, "fileCount" => $count );
		
		$array = null;
		
		if (count ( $folders ) > 0)
		{
			$array = array ();
			
			foreach ( $folders as $folder )
			{
				$arrayTemp = $this->GetFoltersFromDB ( $folder ['ID'], $currentDeep + 1 );
				array_push ( $array, $arrayTemp );
			}
			
			// im empoty return null
			if (count ( $array ) < 1)
				$array = null;
		
		}
		
		$tree ['ref'] = $array;
		
		return $tree;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetRoot()
	{
		return $this->m_home;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetSecurRoot()
	{
		$site = View::GetSite();
		return $site->GetSFRoot();
	}
	// create image file for cms
	
	/**
	 * Enter description here...
	 *
	 */
	function WriteToCMSImage($pID=0, $pFiles="")
	{
		$inmagFolders = $this->GetFoltersFromDB ( 2, 1 );
		
		$folders = array ();
		$files = array ();
		
		$this->ff ( $inmagFolders, $folders, $files );
		
		$filename = 'js/lists/imagesList.js';
		if ( $pID>0 )
		{
			$filename = 'js/lists/imagesListP.js';
		}
		
		if (is_writable ( $filename ))
		{
			if (! $handle = fopen ( $filename, 'w' ))
			{
				echo "This file can not be open ($filename)";
				exit ();
			}
			
			// write folders
			fwrite ( $handle, "var tinyMCEImageFolderList = new Array(\n" );
			$count = 0;
			foreach ( $folders as $folder )
			{
				if ($count > 0)
					fwrite ( $handle, ",\n" );
				
				fwrite ( $handle, "[\"{$folder['name']}\", {$folder['ID']}]" );
				$count ++;
			}
			if ( $pID>0 )
			{
				fwrite ( $handle, ",\n" );
				fwrite ( $handle, "[\"media/images/products/{$pID}\", {$pID}]" );
				$count ++;
			}
			fwrite ( $handle, "\n);" );
			
			// write files
			fwrite ( $handle, "\n\n\n var tinyMCEImageList = new Array(\n" );
			$count = 0;
			foreach ( $files as $folder )
			{
				if ($count > 0)
					fwrite ( $handle, ",\n" );
				
				$foldSpl = split ( "/", $folder ['name'] );
				$con = count ( $foldSpl );
				
				fwrite ( $handle, "[\"{$foldSpl[$con-1]}\", \"{$folder['name']}\", {$folder['ID']}]" );
				$count ++;
			}
			if ( $pID>0 )
			{
				for ( $i=0; $i<count($pFiles); $i++ )
				{
					fwrite ( $handle, ",\n" );
					fwrite ( $handle, "[\"{$pFiles[$i]}\", \"../media/images/products/{$pID}/{$pFiles[$i]}\", {$pID}]" );
					$count ++;
				}
			}
			fwrite ( $handle, "\n);" );
			
			fclose ( $handle );
		}
		else
		{
			echo "This file is not writable ($filename).";
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	function GetMaxDepth()
	{
		return $this->m_maxDeep;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function WriteDbToCMSLists($folderID, $filename, $varName, $clearFile = true, $nameOrID = true)
	{
		$inmagFolders = $this->GetFoltersFromDB ( $folderID, 1 );
		
		$folders = array ();
		$files = array ();
		
		$this->ff ( $inmagFolders, $folders, $files );
		
		if (is_writable ( $filename ))
		{
			// change file permitions
			$res = chmod($filename, 0755);
			
			if(!$res)
			{
				print('The changeing permitions of file has been unsuccessful!');
				return false;	
			}
			
			$type = ($clearFile) ? 'w' : 'a';
			
			if (! $handle = fopen ( $filename, $type ))
			{
				echo "This file can not be open ($filename)";
				exit ();
			}
			
			// write files
			fwrite ( $handle, "var $varName = new Array(\n" );
			$count = 0;
			
			if(count($files) > 0)
			{
				foreach ( $files as $folder )
				{
					if ($count > 0)
						fwrite ( $handle, ",\n" );
					
					$foldSpl = split ( "/", $folder ['name'] );
					$con = count ( $foldSpl );
					
					// write link
					if($nameOrID)
					{
						fwrite ( $handle, "[\"{$foldSpl[$con-1]}\", \"{$folder['name']}\"]" );
					}
					else 
					{
						fwrite ( $handle, "[\"{$foldSpl[$con-1]}\", \"./**securityfileID**{$folder['fileID']}\"]" );
					}
					
					$count ++;
				}
			}
			
			fwrite ( $handle, "\n);" );
			
			fclose ( $handle );
		}
		else
		{
			echo "This file is not writable ($filename).";
		}
	}
	
	// frite files in to the cms file
	function ff($inmagFolders, &$folders, &$files)
	{
		$fold = new Folder ( $inmagFolders ['ID'] );
		
		$cfiles = $fold->GetSub ( 1 );
		if (count ( $cfiles ) > 0)
		{
			$folderpath = $fold->GetFolderPath ();
			
			array_push ( $folders, array ("ID" => $inmagFolders ['ID'], "name" => $folderpath ) );
			
			foreach ( $cfiles as $file )
				array_push ( $files, array ("ID" => $inmagFolders ['ID'], "name" => "../$folderpath/{$file['name']}", "fileID" => $file['ID'] ) );
		}
		
		if ($inmagFolders ['ref'])
		{
			$sub = $inmagFolders ['ref'];
			
			foreach ( $sub as $folder )
				$this->ff ( $folder, $folders, $files );
		}
	}
}
?>