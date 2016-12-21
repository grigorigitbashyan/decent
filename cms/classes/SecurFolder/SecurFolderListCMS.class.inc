<?php
Site::IncludeFile ( 'classes/Common/MergingModules.class.inc' );
Site::IncludeFile ( 'classes/Folder/FolderTreeCMS.class.inc' );
/**
 * Enter description here...
 *
 */
class SecurFolderListCMS extends MergingModules
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ( 'exfolder' );
		
		$this->ReadNameStrings ( 'classes/Folder/langs/', 'media.xml' );
		
		$directoriesS = $this->GetNameString ( 'directories' );
		$securDirectoriesS = $this->GetNameString ( 'securDirectories' );
		
		$this->AddModule ( new FolderTreeCMS ( false ), $directoriesS );
		$this->AddModule ( new FolderTreeCMS ( true ), $securDirectoriesS );
	}
}
?>