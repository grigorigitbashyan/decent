<?php
/**
 * Enter class description here...
 *
 * @package Item
 */
class Analitics extends NamedRecord
{
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	protected $m_title;
	
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	protected $m_url;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_place;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @return Analitics
	 */
	function __construct($ID = null)
	{
		parent::__construct ( $ID, 'analitics' );
		$this->Read ();
	}
	
	/**
	 * Delete record from database
	 *
	 * @return bool
	 */
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		return $this->DeleteRecord ( true, $toStroe );
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
	
	/**
	 * Return value of m_title
	 *
	 * @return string
	 */
	function GetTitle()
	{
		return $this->m_title;
	}
	
	/**
	 * Return value of m_code
	 *
	 * @return string
	 */
	function GetUrl()
	{
		return $this->m_url;
	}
	
	/**
	 * Return value of m_place
	 *
	 * @return int
	 */
	function GetPlace()
	{
		return $this->m_place;
	}
	
	/**
	 * Set value to m_title
	 *
	 * @param string $title
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetTitle($title, $dbupdate = true)
	{
		return $this->UpdateValue( "title", $title, $dbupdate );
	}
	
	/**
	 * Set value to m_url
	 *
	 * @param string $url
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetUrl($url, $dbupdate = true)
	{
		return $this->UpdateValue( "url", $url, $dbupdate );
	}
	
	/**
	 * Set value to m_place
	 *
	 * @param string $place
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetPlace($place, $dbupdate = true)
	{
		return $this->UpdateValue( "place", $place, $dbupdate );
	}
}
?>