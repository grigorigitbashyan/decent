<?php
Site::IncludeFile ( 'classes/Folder/FolderXP.class.inc' );
/**
 * Enter class description
 */
class GalleryItem extends NamedRecord
{
	/**
	 * Description
	 *
	 * @var int
	 */
	protected $m_galleryID;
	
	/**
	 * Description
	 *
	 * @var string
	 */
	protected $m_desc;
	
	/**
	 * Description
	 *
	 * @var char
	 */
	protected $m_link;
	
	/**
	 * Description
	 *
	 * @var char
	 */
	protected $m_path;
	
	/**
	 * Description
	 *
	 * @var int
	 */
	protected $m_position;
	
	/**
	 * @var bool
	 */
	protected $m_isuploaded;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ( $ID, 'gallery_item' );
		
		$this->Read ();
	}
	/**
	 * Insert GalleryItem 
	 * @return bool
	 */
	function Insert($properties)
	{
		// copy name to all languages
		$site = View::GetSite ();
		$langs = $site->GetLanguages ();
		
		foreach ( $langs as $language )
		{
			$properties ["name_$language"] = ' ';
		}
		
		// delete from content DB
		return $this->InsertRecord ( $properties );
	}
	
	/**
	 * Delete record from database
	 *
	 * @return bool
	 */
	function Delete($writeToDB = true, $deleteFile = false)
	{
		$site = View::GetSite ();
		
		// get file path
		if ($this->m_isuploaded)
		{
			$pathFull = $this->m_path;
			
			$prFolStr = $site->GetGalleryImageRoot ( true, true );
			$prFolStrL = strlen ( $prFolStr );
			
			$pos = strpos ( $pathFull, $prFolStr );
			
			if ($pos > 0)
			{
				$pathFull = substr ( $pathFull, $pos + $prFolStrL );
				$prRoot = '../' . $site->GetGalleryImageRoot ( true, true );
				
				$pathFull = $prRoot . $pathFull;
			}
			
			$thumbNile = dirname ( $pathFull ) . '/thumbnail_' . basename ( $pathFull );
			// remofe file
			if (file_exists ( $pathFull ))
			{
				if ($deleteFile)
				{
					FolderXP::Unlink ( $pathFull );
					FolderXP::Unlink ( $thumbNile );
				}
				else
				{
					// get file name;
					$pos = strrpos ( $pathFull, '/' );
					$fileName = substr ( $pathFull, $pos + 1 );
					$recycleRoot = Site::GetRecycleBinPath ();
					
					$newFilePath = $recycleRoot . 'gallery-' . $this->GetID () . '-' . $fileName;
					$newFilePathThumb = $recycleRoot . 'gallery-' . $this->GetID () . '-thumbneil_' . $fileName;
					rename ( $pathFull, $newFilePath );
					rename ( $thumbNile, $newFilePathThumb );
				}
			}
		}
		
		// delete record
		return $this->DeleteRecord ( $writeToDB );
	}
	
	/**
	 * Set value of m_galleryID
	 *
	 * @param int $galleryID
	 * @param bool $dbupdate
	 * @return int
	 */
	function SetGalleryID($galleryID, $dbupdate = true)
	{
		return $this->UpdateValue ( 'galleryID', $galleryID, $dbupdate );
	}
	
	/**
	 * Set value of m_desc
	 *
	 * @param string $desc
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return string
	 */
	function SetDesc($desc, $lang, $dbupdate = true)
	{
		$this->UpdateValue ( "desc_{$lang}", $desc, $dbupdate );
	}
	
	/**
	 * Set value of m_link
	 *
	 * @param char $link
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return char
	 */
	function SetLink($link, $lang, $dbupdate = true)
	{
		return $this->UpdateValue ( "link_{$lang}", "$link", $dbupdate );
	}
	
	/**
	 * Set value of m_path
	 *
	 * @param char $path
	 * @param bool $dbupdate
	 * @return char
	 */
	function SetPath($path, $dbupdate = true)
	{
		return $this->UpdateValue ( 'path', $path, $dbupdate );
	}
	
	/**
	 * Set value of m_position
	 *
	 * @param int $position
	 * @param bool $dbupdate
	 * @return int
	 */
	function SetPosition($position, $dbupdate = true)
	{
		return $this->UpdateValue ( 'position', $position, $dbupdate );
	}
	
	/**
	 * Return value of m_galleryID
	 *
	 * @return int
	 */
	function GetGalleryID()
	{
		return $this->m_galleryID;
	}
	
	/**
	 * Return value of m_desc
	 *
	 * @param string $lang
	 * @return string
	 */
	function GetDesc($lang)
	{
		return $this->m_desc [$lang];
	}
	
	/**
	 * Return value of m_link
	 *
	 * @param string $lang
	 * @return char
	 */
	function GetLink($lang)
	{
		return $this->m_link [$lang];
	}
	
	/**
	 * Return value of m_path
	 *
	 * @return char
	 */
	function GetPath()
	{
		return $this->m_path;
	}
	
	/**
	 * Return value of m_position
	 *
	 * @return int
	 */
	function GetPosition()
	{
		return $this->m_position;
	}

}
?>