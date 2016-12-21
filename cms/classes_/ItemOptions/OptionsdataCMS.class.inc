<?php
// inculdes
Site::IncludeFile ( 'classes/ItemOptions/Optionsdata.class.inc' );

/**
 * Enter description here...
 *
 * @package ItemOptions
 */
class OptionsdataCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Optionsdata
	 */
	protected $m_optionsdata;
	
	function __construct()
	{
		parent::__construct ();
		
		$ID = DB::GET_POST ( 'optionsdataID' );
		
		$this->m_optionsdata = new Optionsdata ( $ID );
		
		$this->ReadNameStrings ( 'classes/ItemOptions/langs/', 'items.xml' );
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
			case 'rename' : // rename Optionsdata
				if (($newName = DB::POST ( 'newName' )))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_optionsdata->SetName ( $newName, $this->m_displayLang );
					}
				}
				break;
			case 'setKey' : // rename Optionsdata
				if (($newName = DB::POST ( 'newKey' )))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_optionsdata->SetKey( $newName );
					}
				}
				break;
			case 'delete' : // delete Optionsdata
				$ar = $this->m_optionsdata->Delete ( false );
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
		$ID = $this->m_optionsdata->GetID ();
		$objName = $this->m_optionsdata->GetName ( $this->m_displayLang );
		$objKey = $this->m_optionsdata->GetKey();
		
		$overLink = 'optionsdataID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		
		// delete form javascript
		$sureDeleteS = $this->GetNameString ( 'sureDelete' );
		$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
		
		$delForm = "<div  class='titleColR'>
    					<form method='POST' $dJS>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='optionsdataID' value='$ID'>
    					<input type='submit' value='Delete'>
    					</form>
    					</div>";
		
		// display form title
		$this->DisplayTitle ( $objName, $delForm );
		
		
		// diplay rename form
		$renameS = $this->GetNameString ( 'rename' );
		$doneS = $this->GetNameString ( 'done' );
		$noEmptyS = $this->GetNameString ( 'noempty' );
		$keyS = $this->GetNameString ( 'key' );
		
		$renameJS = "onsubmit='return SingleForm(\"newName\", \"$noEmptyS\");'";
		$this->DisplaySingleTextForm ( 'rename', $renameS, 'newName', $objName, array ($overLink => $ID ), $doneS, $renameJS );
		
		print('<br />');
		$renameJS = "onsubmit='return SingleForm(\"newKey\", \"$noEmptyS\");'";
		$this->DisplaySingleTextForm ( 'setKey', $keyS, 'newKey', $objKey, array ($overLink => $ID ), $doneS, $renameJS );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function Delete()
	{
		return $this->m_optionsdata->Delete ();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$langs = $this->m_site->GetLanguages ();
		
		$nameS = '';
		
		foreach ( $langs as $lang )
		{
			$nameS .= "`name_$lang` varchar(255) NOT NULL,";
		}
		
		$sql = "CREATE TABLE `optionsdata` (
				  `ID` smallint(5) unsigned NOT NULL auto_increment,
				  `optionsID` smallint(5) unsigned NOT NULL,
				  `key` varchar(32) NULL,
				  $nameS
				  PRIMARY KEY  (`ID`)
				)DEFAULT CHARSET=utf8 ";
		
		return $sql;
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		return array ('optionsdata' );
	}
}
?>