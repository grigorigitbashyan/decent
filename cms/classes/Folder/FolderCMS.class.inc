<?php
// includes
Site::IncludeFile ( "classes/Folder/Folder.class.inc" );
Site::IncludeFile ( "classes/Folder/FolderStructure.class.inc" );

/**
 * Enter description here...
 *
 * @package Folder
 */
class FolderCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Folder
	 */
	protected $m_folder;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_folder = new Folder ( $ID );
		
		$this->ReadNameStrings ( 'classes/Folder/langs/', 'media.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DoAction()
	{
		// if exist the content ID, then call DoAction for content
		if (! $this->m_folder->GetID ()) return false;
		
		$ar = false;
		
		switch ($this->GetFormAction ())
		{
			case 'add' :
				$name = DB::POST ( 'name' );
				$ar = $this->m_folder->Insert ( $name );
				break;
			case 'delete' :
				$ar = $this->m_folder->Delete ( true );
				
				if ($ar) // file uploaded
				{
					$this->SaveToFile ();
				}
				break;
			case "upload" :
				$maxW = $this->m_site->GetMaxImageWidth();
				$ar = $this->m_folder->UploadFile ($maxW);
				
				if ($ar) // file uploaded
				{
					$this->SaveToFile ();
				}
				
				break;
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
		// tree feature
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// /////////////////////////////
		$currID = $this->m_folder->GetID ();
		
		if (! $currID) return;
		
		$title = $this->m_folder->GetName ();
		
		$isDel = $this->m_folder->GetIsDelete ();
		
		// display folder name with delete button
		$deleteS = $this->GetNameString ( 'delete' );
		$doneS = $this->GetNameString ( 'done' );
		
		if ($isDel)
		{
			// delete form javascript
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
			
			$this->DisplayFormHeadr ( 'delete', 'delForm', 'post', $dJS );
			$this->DisplayHidden ( 'folderID', $currID );
			?>
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL">
		<h1><?php echo $title?></h1>
		</div>
		<div class="titleColR"><input name="" type="submit"
			value="<?php echo $deleteS?>" /></div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
</form>
<br class="clearFloat" />
<?php
		}
		else
		{
			?>
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL">
		<h1><?php echo $title?></h1>
		</div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
<br class="clearFloat" />
<?php
		}
		// dispaly form of inserting new folder
		$createDirS = $this->GetNameString ( 'createDirectory' );
		$noemptyS = $this->GetNameString ( 'noempty' );
		
		$insertJS = "onsubmit='return SingleForm(\"name\", \"$noemptyS\")'";
		$this->DisplaySingleTextForm ( 'add', $createDirS, 'name', null, array ('folderID' => $currID ), $doneS, $insertJS );
		
		// dispaly file upload form
		print ('<br />') ;
		$this->DisplayUploadForm ();
		print ('<br />') ;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayUploadForm()
	{
		$currID = $this->m_folder->GetID ();
		$maxFileSize = ini_get ( 'upload_max_filesize' );
		
		$fileType = $this->m_folder->GetFiletype ();
		
		$fs = new FolderStructure ( );
		$types = $fs->GetTypes ( $fileType );
		$typeStr = join ( ', ', $types );
		
		$actionName = $this->GetFormActionName ();
		$uploadS = $this->GetNameString ( 'upload' );
		$fileUploadS = $this->GetNameString ( 'fileUpload' );
		?>
<form action="" method="POST" class="clearPD"
	enctype='multipart/form-data'><input type="hidden"
	name="<?php echo $actionName?>" value="upload"> <input type="hidden"
	name="folderID" value="<?php echo $currID?>" />
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL"><label><strong><?php echo $fileUploadS?></strong> </label>
		<input type="file" name="gl2_img" size="48" /> <span>&nbsp;[ Max = <?php echo $maxFileSize?> ] <?php echo $typeStr?></span>
		</div>
		<div class="titleColR"><input name="" type="submit"
			value="<?php echo $uploadS?>" /></div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
</form>
<?php
	}
	/**
	 * Enter description here...
	 *
	 */
	function SaveToFile()
	{
		$fs = new FolderStructure ( );
		switch ($this->m_folder->GetFiletype ())
		{
			case 1 :
				// images
				$fs->WriteToCMSImage ();
				break;
			case 4 :
			case 3 :
				// documents
				$ID = DB::QueryToValue ( "SELECT `ID` FROM `folder` WHERE `parentID` = 1 AND `fileType` = 4" );
				if ($ID)
				{
					$fs->WriteDbToCMSLists ( $ID, "js/lists/documentList.js", "tinyMCELinkList" );
				}
				
				$ID = DB::QueryToValue ( "SELECT `ID` FROM `folder` WHERE `parentID` = -1 AND `fileType` = 4" );
				if ($ID)
				{
					$fs->WriteDbToCMSLists ( $ID, "js/lists/documentList.js", "tinyMCELinkListSecur", false, false );
				}
				break;
			case 2 :
				// vides
				$fs->WriteDbToCMSLists ( 4, "js/lists/media_list.js", "tinyMCEMediaList" );
				break;
		}
	}
	
	/**
	 * @static 
	 * Create folder database 
	 *
	 * @return bool
	 */
	function CreateDB($createTable = true)
	{
		if ($createTable)
		{
			$res = parent::CreateDB ();
			
			// insert HOME
			if ($res)
			{
				// if media folder does not exist then create it
				if (! file_exists ( '../media/' ))
				{
					$res = FolderXP::Mkdir('../media/', 0753 );
				}
				
				if ($res)
				{
					DB::Query ( "INSERT INTO `folder` VALUES (1, 'ROOT', '0', 0, 0, 0);" );

					// insert main directories
					$rootFolder = new Folder ( 1 );
					$rootFolder->Insert('images', 1);
					
					$rootFolder = new Folder ( 1 );
					$rootFolder->Insert('videos', 2);
					
					$rootFolder = new Folder ( 1 );
					$rootFolder->Insert('audios', 3);
					
					$rootFolder = new Folder ( 1 );
					$rootFolder->Insert('documents', 4);
					return true;
				}
				return false;
			}
			
			return $res;
		}
		
		return true;
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `folder` 
					(`ID` int(6) NOT NULL auto_increment,
					  `name` varchar(128) character set utf8 collate utf8_bin NOT NULL default '',
					  `parentID` varchar(6) character set utf8 collate utf8_bin NOT NULL default '1',
					  `isDelete` int(1) NOT NULL default '1',
					  `filetype` int(1) NOT NULL default '0',
					  `fileOrFolder` int(3) NOT NULL default '0',
					  PRIMARY KEY  (`ID`)
					) DEFAULT CHARSET=utf8;";
		return $sql;
	}
}
?>