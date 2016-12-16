<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_field/Pr_field.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_field/Pr_fieldCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_field/Pr_fieldList.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_fieldListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_fieldList
	 */
	protected $m_pr_fieldList;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->Update ();
		
		$this->ReadNameStrings ( 'classes/Product/langs/', 'field.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function DoAction()
	{
		// action result
		$ar = null;
		
		// call DoAction of Pr_fieldCMS
		$overLink = $this->m_pr_fieldList->GetLinkIDName ();
		if (($overLinkID = DB::POST ( $overLink )))
		{
			$pr_fieldCMS = new Pr_fieldCMS ( $overLinkID );
			$ar = $pr_fieldCMS->DoAction ();
		}
		
		// do actions for Pr_field list
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				if (($name = DB::POST ('insertName') ))
				{
					if (strlen ( trim ( $name ) ) > 0)
					{
						// insert new pr_field with name
						$newPr_field = new Pr_field ( );
						$params = array ('name' => $name );
						$ar = $newPr_field->Insert ( $params );
					}
				}
				break;
		}
		
		// if there is a any action then update object  
		if ($ar)
		{
			$this->Update ();
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
		$currID = $this->m_pr_fieldList->GetCurrentID ();
		
		if ($currID)
		{
			$linkName = $this->m_pr_fieldList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>Field</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">Field</div>' );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_pr_fieldList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		$currentPr_fieldID = $this->m_pr_fieldList->GetCurrentID ();
		// get cooretn pr_field
		$pr_field = new Pr_field ( $currentPr_fieldID );
		
		$currentPr_fieldID = $pr_field->GetID ();
		// if is selected pr_field, then diplay pr_field features
		if ($currentPr_fieldID)
		{
			$pr_fieldCMS = new Pr_fieldCMS ( $currentPr_fieldID );
			$pr_fieldCMS->DisplayFeatures ();
		
		} // diplay new pr_field form
		else
		{
			$doneS = $this->GetNameString ( 'done' );
			// create filed string
			$creFieS = $this->GetNameString ( 'createFiled' );
			$noEmptyS = $this->GetNameString ( 'noempty' );
			
			// insert js
			$iJS = "onsubmit='return SingleForm(\"insertName\", \"$noEmptyS\");'";
			// display insert form
			$this->DisplaySingleTextForm ( 'insert', $creFieS, 'insertName', null, null, $doneS, $iJS );
			// end new pr_field form
		}
	}
	
	function Update()
	{
		$this->m_pr_fieldList = new Pr_fieldList ( );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$prField = new Pr_fieldCMS ( );
		return $prField->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$prField = new Pr_fieldCMS ( );
		return $prField->SelfTest ();
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		$prField = new Pr_fieldCMS ( );
		return $prField->GetObjectTables ();
	}
}
?>