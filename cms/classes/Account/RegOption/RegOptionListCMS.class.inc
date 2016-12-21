<?php
Site::IncludeFile ( 'classes/Common/CMSListObject.class.inc' );
Site::IncludeFile ( 'classes/Account/RegOption/RegOptionCMS.class.inc' );
Site::IncludeFile ( 'classes/Account/RegOption/RegOptionList.class.inc' );

/**
 * Enter description here...
 *
 * @package Account
 */
class RegOptionListCMS extends CMSListObject
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ( new RegOptionList ( ), 'RegOption' );
		
		$this->ReadNameStrings ( 'classes/Account/langs/', 'siteuser.xml' );
	}
	
	/**
	 * @see CMSListObject::DisplayMenu()
	 *
	 */
	function DisplayMenu()
	{
		// get strucutre and add names
		$structure = $this->m_list->GetStructure ();
		if ($structure)
		{
			$keys = array_keys ( $structure );
			foreach ( $keys as $key )
			{
				// get name
				$keyDisp = $key + 1;
				$nameS = $this->GetNameString ( "regName{$keyDisp}" );
				$structure [$key] ['name'] = $nameS;
			}
			$this->m_list->SetStructure ( $structure );
		}
		// display list
		$regOptionS = $this->GetNameString ( 'regOption' );
		parent::DisplayMenu ( $regOptionS, false );
	}
	
	/**
	 * @see CMSInterface::DoAction()
	 *
	 */
	function DoAction()
	{
		// action result
		$ar = parent::DoAction ();
		
		return $ar;
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = 'CREATE TABLE `registration_option` (
				  `ID` tinyint(3) unsigned NOT NULL auto_increment,
				  `value` smallint(5) unsigned NOT NULL,
				  PRIMARY KEY  (`ID`)
				);';
		
		return $sql;
	}
}
?>