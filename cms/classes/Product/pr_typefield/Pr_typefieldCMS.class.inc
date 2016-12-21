<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_typefield/Pr_typefield.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_typefieldCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_typefield
	 */
	protected $m_pr_typefield;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		if (! $ID)
		{
			$ID = DB::REQUEST ( 'pr_typefieldID' );
		}
		
		$this->m_pr_typefield = new Pr_typefield ( $ID );
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
			case 'rename' : // rename Pr_typefield
				if (($newName = DB::POST ( 'name' )))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_pr_typefield->SetName ( $newName );
					}
				}
				break;
			case 'delete' : // delete Pr_typefield
				$ar = $this->m_pr_typefield->Delete ();
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
		$ID = $this->m_pr_typefield->GetID ();
		$objName = $this->m_pr_typefield->GetName ();
		
		$overLink = 'pr_typefieldID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		$delForm = "<div  class='titleColR'>
    					<form method='POST'>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='pr_typefieldID' value='$ID'>
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
		return $this->m_pr_typefield->Delete ();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `pr_typefield` (
				  `ID` smallint(5) unsigned NOT NULL auto_increment,
				  `producttypeID` smallint(5) unsigned NOT NULL,
				  `fieldID` smallint(5) unsigned NOT NULL,
				  `single` tinyint(1) NOT NULL default '1',
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>