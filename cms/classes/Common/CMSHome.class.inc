<?php
/**
 * Enter description here...
 * @package Main
 *
 */
class CMSHome extends CMSObject
{
	/**
	 * 
	 *@return CMSObject 
	 */
	function CMSObject()
	{
		parent::__construct ();
	
	}
	
	/**
	 * 
	 * @see CMSInterface::DisplayFeatures()
	 */
	function DisplayFeatures()
	{
		$siteName = $this->m_site->GetSiteName ();
		print ( "<h1>Welcome to $siteName website CMS.</h1>" );
	}
	
	/**
	 * 
	 * @see CMSInterface::DisplayMenu()
	 */
	function DisplayMenu()
	{
		print ( "&nbsp;" );
	}
	
	/**
	 * 
	 * @see CMSInterface::DoAction()
	 */
	function DoAction()
	{
	
	}
	
	function SelfTest()
	{
		return true;
	}
}

?>