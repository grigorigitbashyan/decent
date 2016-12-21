<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_char/Pr_char.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_charCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_char
	 */
	protected $m_pr_char;
	
	function __construct()
	{
		parent::__construct ();
		
		$ID = DB::REQUEST ( 'pr_charID' );
		
		$this->m_pr_char = new Pr_char ( $ID );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function DoAction()
	{
		$ar = null;
		
		switch ($this->GetFormAction ())
		{
			case 'rename' : // rename Pr_char
				if (($newName = DB::POST ( 'name' )))
				{
					$newName = trim ( $newName );
					$ar = $this->m_pr_char->SetName ( $newName );
				}
				break;
			case 'delete' : // delete Pr_char
				$ar = $this->m_pr_char->Delete ();
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
	
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// get names
		$renameS = $this->GetNameString ( 'rename' );
		$doneS = $this->GetNameString ( 'done' );
		$deleteS = $this->GetNameString ( 'delete' );
		
		//
		$ID = $this->m_pr_char->GetID ();
		$objName = $this->m_pr_char->GetName ();
		
		$overLink = 'pr_charID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		$delForm = "<div  class='titleColR'>
    					<form method='POST'>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='pr_charID' value='$ID'>
    					<input type='submit' value='$deleteS'>
    					</form>
    					</div>";
		
		// display form title
		$this->DisplayTitle ( $objName, $delForm );
		
		// diplay rename form
		$this->DisplaySingleTextForm ( 'rename', $renameS, 'name', $objName, array ($overLink => $ID ), $doneS );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function Delete()
	{
		return $this->m_pr_char->Delete ();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `pr_char` (
				  `ID` int(10) unsigned NOT NULL auto_increment,
				  `productID` int(10) unsigned NOT NULL,
				  `fieldID` smallint(5) unsigned NOT NULL,
				  `subID` varchar(2) NOT NULL default '0',
				  `value` varchar(255) default NULL,
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>