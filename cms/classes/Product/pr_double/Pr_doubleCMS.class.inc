<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_double/Pr_double.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_doubleCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_double
	 */
	protected $m_pr_double;
	
	function __construct()
	{
		parent::__construct ();
		
		$ID = DB::REQUEST ( 'pr_doubleID' );
		$this->m_pr_double = new Pr_double ( $ID );
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
			case 'rename' : // rename Pr_double
				if (($newName = DB::POST ( 'name' )))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_pr_double->SetName ( $newName );
					}
				}
				break;
			case 'delete' : // delete Pr_double
				$ar = $this->m_pr_double->Delete ();
				break;
		}
		
		// if there is a any action then update Pr_double  
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
		$ID = $this->m_pr_double->GetID ();
		$objName = $this->m_pr_double->GetName ();
		
		$overLink = 'pr_doubleID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		$delForm = "<div  class='titleColR'>
    					<form method='POST'>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='pr_doubleID' value='$ID'>
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
		return $this->m_pr_double->Delete ();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `pr_double` (
				  `ID` int(10) unsigned NOT NULL auto_increment,
				  `productID` int(10) unsigned NOT NULL,
				  `fieldID` smallint(5) unsigned NOT NULL,
				  `value` double NOT NULL,
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>