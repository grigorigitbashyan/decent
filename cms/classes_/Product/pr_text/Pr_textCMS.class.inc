<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_text/Pr_text.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_textCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_text
	 */
	protected $m_pr_text;
	
	function __construct()
	{
		parent::__construct ();
		
		$ID = DB::REQUEST ( 'pr_textID' );
		$this->m_pr_text = new Pr_text ( $ID );
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
			case 'rename' : // rename Pr_text
				if (($newName = DB::POST ( 'name' )))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_pr_text->SetName ( $newName );
					}
				}
				break;
			case 'delete' : // delete Pr_text
				$ar = $this->m_pr_text->Delete ();
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
		$ID = $this->m_pr_text->GetID ();
		$objName = $this->m_pr_text->GetName ();
		
		$overLink = 'pr_textID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		$delForm = "<div  class='titleColR'>
    					<form method='POST'>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='pr_textID' value='$ID'>
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
		return $this->m_pr_text->Delete ();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `pr_text` (
				  `ID` int(10) unsigned NOT NULL auto_increment,
				  `productID` int(10) unsigned NOT NULL,
				  `fieldID` smallint(5) unsigned NOT NULL,
				  `subID` varchar(2) NOT NULL default '0',
				  `value` text default NULL,
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>