<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_int/Pr_int.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_intCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_int
	 */
	protected $m_pr_int;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$ID = DB::REQUEST ( 'pr_intID' );
		$this->m_pr_int = new Pr_int ( $ID );
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
			case 'rename' : // rename Pr_int
				if (($newName = DB::POST ( 'name' )))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_pr_int->SetName ( $newName );
					}
				}
				break;
			case 'delete' : // delete Pr_int
				$ar = $this->m_pr_int->Delete ();
				break;
		}
		
		// if there is a any action then update Pr_int  
		//				if ($ar)
		//				{
		//					// save
		//				}
		

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
		$ID = $this->m_pr_int->GetID ();
		$objName = $this->m_pr_int->GetName ();
		
		$overLink = 'pr_intID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		$delForm = "<div  class='titleColR'>
    					<form method='POST'>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='pr_intID' value='$ID'>
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
		return $this->m_pr_int->Delete ();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `pr_int` (
				  `ID` int(10) unsigned NOT NULL auto_increment,
				  `productID` int(10) unsigned NOT NULL,
				  `fieldID` smallint(5) unsigned NOT NULL,
				  `value` int(11) NOT NULL,
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		return array ('pr_int' );
	}
}
?>