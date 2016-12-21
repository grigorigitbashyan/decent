<?php
Site::IncludeFile('classes/Folder/FolderXP.class.inc');
/**
 * Enter class description here...
 *
 * @package Item
 */
class Pr_picture extends Record
{
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_productID;
	
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	protected $m_path;
	
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	protected $m_iconPath;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_position;
	
	/**
	 * Enter description here...
	 *
	 * @protected array
	 */
	protected $m_title;
	
	/**
	 * Enter description here...
	 *
	 * @protected array
	 */
	protected $m_desc;
	
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	protected $m_isuploaded;
	
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	protected $m_gallery;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @return Pr_picture
	 */
	function __construct($ID = null)
	{
		parent::__construct ( $ID, 'pr_picture' );
		
		$this->Read ();
	}
	
	/**
	 * Delete record from database
	 *
	 * @return bool
	 */
	function Delete($writeToDB = true, $deleteFile = false)
	{
		// get file path
		if( $this->m_isuploaded )
		{
			$pathFull = $this->m_path;
			$iconPathFull = $this->m_iconPath;
			
			$prFolStr = '/media/images/products/';
			$prFolStrL = strlen($prFolStr);
			
			$pos = strpos($pathFull, $prFolStr);
			if( $pos > 0 )
			{
				$pathFull = substr($pathFull, $pos+$prFolStrL);
				$iconPathFull = substr($iconPathFull, $pos+$prFolStrL);
				
				$site = View::GetSite();
				$prRoot = '../'.$site->GetProImageRoot(true, false);
				$pathFull = $prRoot . $pathFull;
				$iconPathFull = $prRoot . $iconPathFull;
			}
			 
			// remove file
			if(file_exists($pathFull))
			{
				if($deleteFile)
				{
					$res = FolderXP::Unlink($pathFull);
					$res = FolderXP::Unlink($iconPathFull);
				}
				else 
				{
					// get file name;
					$pos = strrpos($pathFull, '/');
					$fileName = substr($pathFull, $pos + 1);
					$recycleRoot = Site::GetRecycleBinPath();
					
					$newFilePath = $recycleRoot . 'product-'. $this->GetID(). '-' . $fileName;
					$res = FolderXP::Rename($pathFull, $newFilePath);
				}
			}
		}
		
		$product = new Pr_main($this->m_productID);
		$defPic = $product->GetPicture();
		
		if($defPic == $this->m_path)
		{
			$product->SetDefPicNull();
		}
		// delete record
		return $this->DeleteRecord ($writeToDB);
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
	function Insert($properties = null)
	{
		return $this->InsertRecord ( $properties );
	}
	
	// Get functions ///////////////////////////////////////////////////
	
	/**
	 * Return value of m_productID
	 *
	 * @return int
	 */
	function GetProductID()
	{
		return $this->m_productID;
	}
	
	/**
	 * Return value of m_path
	 *
	 * @return string
	 */
	function GetPath()
	{
		return $this->m_path;
	}
	
	/**
	 * Return value of m_iconPath
	 *
	 * @return string
	 */
	function GetIconPath()
	{
		return $this->m_iconPath;
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
	
	/**
	 * Return value of m_title
	 *
	 * @param string $lang
	 * @return string
	 */
	function GetTitle($lang = 'en')
	{
		return $this->m_title[$lang];
	}
	
	/**
	 * Return value of m_desc
	 *
	 * @param string $lang
	 * @return string
	 */
	function GetDesc($lang = 'en')
	{
		return $this->m_desc [$lang];
	}
	
	/**
	 * Return value of m_gallery
	 *
	 * @return int
	 */
	function GetGallery()
	{
		return $this->m_gallery;
	}
	
	// Set functions ////////////////////////////////////////////////////
	/**
	 * Set value to m_productID  
	 *
	 * @param int $productID
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetProductID($productID, $dbupdate = true)
	{
		return $this->UpdateValue ( 'productID', $productID, $dbupdate );
	}
	
	/**
	 * Set value to m_path  
	 *
	 * @param string $path
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetPath($path, $dbupdate = true)
	{
		return $this->UpdateValue ( 'path', $path, $dbupdate );
	}
	
	/**
	 * Set value to m_iconPath  
	 *
	 * @param string $path
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetIconPath($path, $dbupdate = true)
	{
		return $this->UpdateValue ( 'iconPath', $path, $dbupdate );
	}
	
	/**
	 * Set value to m_position  
	 *
	 * @param int $position
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetPosition($position, $dbupdate = true)
	{
		return $this->UpdateValue ( 'position', $position, $dbupdate );
	}
	
	/**
	 * Set value to m_title
	 *
	 * @param string $title
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetTitle($title, $lang, $dbupdate = true)
	{
		return $this->UpdateValue ( "title_$lang", $title, $dbupdate );
	}
	
	/**
	 * Set value to m_desc  
	 *
	 * @param string $desc
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetDesc($desc, $lang, $dbupdate = true)
	{
		return $this->UpdateValue ( "desc_$lang", $desc, $dbupdate );
	}
	
	/**
	 * Set value to m_gallery
	 *
	 * @param int $gallery
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetGallery($gallery, $dbupdate = true)
	{
		return $this->UpdateValue ( 'gallery', $gallery, $dbupdate );
	}
}
?>