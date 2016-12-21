<?php
// includes
Site::IncludeFile ( "classes/Folder/Folder.class.inc" );
Site::IncludeFile ( "classes/Folder/FolderCMS.class.inc" );
Site::IncludeFile ( "classes/Folder/FileList.class.inc" );
Site::IncludeFile ( "classes/Folder/FolderTree.class.inc" );

/**
 * Enter description here...
 *
 * @package Folder
 */
class FolderTreeCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var FolderTree
	 */
	protected $m_folderTree;
	
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	protected $m_security;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct($security = false)
	{
		parent::__construct ();
		
		$this->m_security = $security;
		
		$this->Update ();
		
		$this->ReadNameStrings ( 'classes/Folder/langs/', 'media.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DoAction()
	{
		// if exist the content ID, then call DoAction for content
		if (($folderID = DB::POST ('folderID') ))
		{
			$folderCMS = new FolderCMS ( $folderID);
			$res = $folderCMS->DoAction ();
			
			if ($res)
			{
				$this->Update ();
			}
		}
		
		$ar = false;
		switch ($this->GetFormAction ())
		{
			case 'deleteFiles' :
				if (($IDs = DB::POST('fileIDs')))
				{
					foreach ( $IDs as $ID )
					{
						$file = new Folder ( $ID );
						$file->Delete ();
					}
					
					$fCMS = new FolderCMS ($file->GetParentID());
					$fCMS->SaveToFile ();
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
		if ($this->m_security)
		{
			$sectionName = $this->GetNameString ( 'securDirectories' );
		}
		else
		{
			$sectionName = $this->GetNameString ( 'directories' );
		}
		
		print ( "<div class='someWTitleBox'>$sectionName</div>" );
		print ( '<div class="someWBox">' );
		
		$styles = array (1, 'directoryMenu1', 'directoryMenu2', 'directoryMenu3', 'directoryMenu4', 'directoryMenu5', 'directoryMenu6' );
		
		$this->m_folderTree->DisplayTree ( $styles, null, 6 );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// get current ID of folder
		$currID = $this->m_folderTree->GetCurrentID ();
		
		// display folder features
		if ($currID)
		{
			$folderCMS = new FolderCMS ( $currID );
			$folderCMS->DisplayFeatures ();
		}
		else
		{
			print ( "&nbsp;" );
			return;
		}
		
		/// files
		$folder = new Folder ( $currID );
		
		$fileList = new FileList ( $currID );
		
		$filesCount = $fileList->GetItemCount ();
		
		if ($filesCount > 0)
		{
			$paterm = null;
			
			$siteURL = $this->m_site->GetSiteURL ();
			$path = $folder->GetFolderPath ();
			
			if ($this->m_security == false)
			{
				$path = $siteURL . $path;
			}
			else
			{
				$folderRoot = $folder->GetRoot ();
				$path = '../' . $folderRoot . $path;
			}
			
			$currentUser = UserManagement::GetCurrentUser ();
			$uID = $currentUser->GetCookie ();
			
			$target = ($this->m_security == - 1) ? "files/FileMeaker.php?p={$path}/@@name@@&uid=$uID" : "$path/@@name@@";
			
			if ($folder->GetFiletype () == 1)
			{
				$paterm = '<div class="mediaImgBox">
			                    <div class="mediaImgUnit">
			                    <a href="' . $target . '" target="_blank">
			                    <img src="files/ImageMeaker.php?w=100&h=80&p=' . $path . '/@@name@@" border="0" />
			                    </a></div>
			                    <div class="mediaImgChackbox">
			                    <input name="fileIDs[]" id="fileIDs[]" type="checkbox" value="@@ID@@" />
			                    </div> 
			                    <div class="mediaImgTitle"><span>@@name@@</span></div>
			                </div>';
			}
			else
			{
				$fileList->AddIcons ();
				
				$paterm = '<div class="mediaImgBox">
		                    <div class="mediaImgUnit">
		                    <a href="' . $target . '" target="_blank">
		                    <img src="../media/@@icon@@" width="80" height="80" border="0" /></a></div>
		                    <div class="mediaImgChackbox">
		                    <input name="fileIDs[]" id="fileIDs[]" type="checkbox" value="@@ID@@" />
		                    </div> 
		                    <div class="mediaImgTitle"><span>@@name@@</span></div>
		                </div>';
			}
			$allS = $this->GetNameString ( 'all' );
			$noneS = $this->GetNameString ( 'none' );
			$selectS = $this->GetNameString ( 'select' );
			$deleteS = $this->GetNameString ( 'delete' );
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$noSelectedS = $this->GetNameString ( 'noselected' );
			print ( '<br />' );
			
			$this->DisplayFormHeadr ( 'deleteFiles', 'files' );
			?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
	<tr>
		<td>
			<div class="titleColR"><span style="padding-right: 20px"> <strong><?php echo $selectS?></strong>
			<a href="#" onclick="chekeBoxes('files', 'fileIDs[]', true); return false"><?php echo $allS?></a>,
			<a href="#" onclick="chekeBoxes('files', 'fileIDs[]', false); return false"><?php echo $noneS?></a>
			</span> <input name="" type="submit" value="<?php echo $deleteS?>" onclick="return ChecksFormDelete('files', 'fileIDs[]', '<?php echo $noSelectedS?>', '<?php echo $sureDeleteS?>');" /></div>
			<br class="clearFloat" />
		</td>
	</tr>
</table>
<table width="100%" border="0" cellspacing="0" cellpadding="5" class="mediaFileAreArea">
	<tr>
		<td>
			<?php
			$fileList->DisplayListDiv ( $paterm );
			?>
		</td>
	</tr>
</table>
</form>
<br class="clearFloat" />
<?php
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$folderCMS = new FolderCMS ( );
		return $folderCMS->CreateDB ( $this->m_security );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	protected function Update()
	{
		$parentID = ($this->m_security) ? - 1 : 1;
		$this->m_folderTree = new FolderTree ( $parentID );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$folderCMS = new FolderCMS ( );
		return $folderCMS->SelfTest ( $this->m_security );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function GetObjectTables()
	{
		$folderCMS = new FolderCMS ( );
		return $folderCMS->GetObjectTables ( );
	}
}
?>