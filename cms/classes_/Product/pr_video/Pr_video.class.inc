<?php
/**
 * Enter class description here...
 *
 * @package Item
 */
class Pr_video extends Record
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
	protected $m_link;
	
	/**
	 * Enter description here...
	 *
	 * @protected array
	 */
	protected $m_desc;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_group;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @return Pr_picture
	 */
	function __construct($ID = null)
	{
		parent::__construct ( $ID, 'pr_video' );
		$this->Read ();
	}
	
	/**
	 * Delete record from database
	 *
	 * @return bool
	 */
	function Delete($writeToDB = true)
	{
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
	function GetLink()
	{
		return $this->m_link;
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
	 * Return value of m_group
	 *
	 * @return int
	 */
	function GetGroup()
	{
		return $this->m_group;
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
	 * Set value to m_link
	 *
	 * @param string $link
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetLink($link, $dbupdate = true)
	{
		return $this->UpdateValue ( 'link', $link, $dbupdate );
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
	 * Set value to m_group
	 *
	 * @param int $group
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetGroup($group, $dbupdate = true)
	{
		return $this->UpdateValue ( 'group', $group, $dbupdate );
	}
}
?>