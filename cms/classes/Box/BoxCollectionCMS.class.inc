<?php
// inculdes
Site::IncludeFile ( 'classes/Box/BoxCollection.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxCollectionList.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxCollectionBox.class.inc' );
Site::IncludeFile ( 'classes/Box/SBRelation.class.inc' );

/**
 * Enter description here...
 * @package Box
 */
class BoxCollectionCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var BoxCollectionList
	 */
	protected $m_boxCollection;
	
	function __construct()
	{
		parent::__construct ();
		
		$this->m_boxCollection = new BoxCollection( );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function DoAction()
	{
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
		
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `boxcollection` (
				  `ID` int(11) NOT NULL auto_increment,
				  `name` varchar(255) NOT NULL default '',
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>