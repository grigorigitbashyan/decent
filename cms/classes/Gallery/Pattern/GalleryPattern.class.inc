<?php
Site::IncludeFile('classes/Pattern/Pattern.class.inc');
/**
 * Enter class description
 */
class GalleryPattern extends NamedRecord
{
	/**
	* Description
	*
	* @var string
	*/
	protected $m_header;

	/**
	* Description
	*
	* @var string
	*/
	protected $m_footer;

	/**
	 * Enter description here...
	 *
	 * @var Pattern
	 */
	protected $m_pattern;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	*/
	function __construct($ID = null)
	{
		parent::__construct($ID, 'gallery_pattern');
	
		$this->Read();
		
		if($ID)
		{
			$this->m_pattern = new Pattern("gallery_pattern_{$ID}"); 
		}
	}
	/**
	 * Insert GalleryPattern 
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
		return $this->InsertRecord($properties);	}

	/**
	 * Delete GalleryPattern 
	* @return bool
	*/
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		// delete from content DB
		return $this->DeleteRecord();
	}

	/**
	 * Set value of m_header
	 *
	 * @param string $header
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetHeader($header, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("header_{$lang}", $header, $dbupdate);
	}

	/**
	 * Set value of m_footer
	 *
	 * @param string $footer
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetFooter($footer, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("footer_{$lang}", $footer, $dbupdate);
	}

	/**
	 * Return value of m_header
	 *
	 * @param string $lang
	 * @return string
	*/
	function GetHeader($lang)
	{
		return $this->m_header[$lang];
	}

	/**
	 * Return value of m_footer
	 *
	 * @param string $lang
	 * @return string
	*/
	function GetFooter($lang)
	{
		return $this->m_footer[$lang];
	}

	function GetPattern($lang)
	{
		return $this->m_pattern->GetPattern($lang);	
	}
	
	function SetPattern($pattern, $lang)
	{
		return $this->m_pattern->SetPattern($pattern, $lang);
	}
}
?>