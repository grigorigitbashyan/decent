<?php
/**
 * Enter class description
 */
class Gallery extends NamedRecord
{
	/**
	* Description
	*
	* @var int
	*/
	protected $m_type;

	/**
	 * Enter description here...
	 *
	 * @var GalleryItemList
	 */
	protected $m_itemList;
	
	/**
	* Description
	*
	* @var char
	*/
	protected $m_thumbnail;

	/**
	 * Enter description here...
	 *
	 * @param int $ID
	*/
	function __construct($ID = null)
	{
		parent::__construct($ID, 'gallery');
		
		$this->Read();
		
		$this->m_itemList = new GalleryItemList($this->m_ID);
	}
	
	/**
	 * Insert Gallery 
	* @return bool
	*/
	function Insert($properties)
	{
		// copy name to all languages
		if($properties['name'])
		{
			$name = $properties['name'];
			unset($properties['name']);
			$site = View::GetSite();
			$langs = $site->GetLanguages();

			foreach ($langs as $language)
			{
				$properties["name_$language"] = $name;
			}
		}

		// delete from content DB
		return $this->InsertRecord($properties);
	}

	/**
	 * Delete Gallery 
	* @return bool
	*/
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		// delete from content DB
		return $this->DeleteRecord();	
	}

	/**
	 * Set value of m_type
	 *
	 * @param int $type
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetType($type, $dbupdate = true)
	{
		return $this->UpdateValue('type', $type, $dbupdate);
	}

	/**
	 * Set value of m_thumbnail
	 *
	 * @param char $thumbnail
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetThumbnail($thumbnail, $dbupdate = true)
	{
		return $this->UpdateValue('thumbnail', $thumbnail, $dbupdate);
	}

	/**
	 * Return value of m_type
	 *
	 * @return int
	*/
	function GetType()
	{
		return $this->m_type;
	}

	/**
	 * Return value of m_thumbnail
	 *
	 * @return char
	*/
	function GetThumbnail()
	{
		return $this->m_thumbnail;
	}

	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SetDefPicNull()
	{
		return $this->SetThumbnail( 'img/thumbnail.gif' );
	}
	
	/**
	 * REturn items list object
	 *
	 * @return GalleryItemList
	 */
	function GetItemList()
	{
		return $this->m_itemList;
	}
}
?>