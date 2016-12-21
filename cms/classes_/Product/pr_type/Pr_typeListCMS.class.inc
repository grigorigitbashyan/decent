<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_type/Pr_type.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_type/Pr_typeCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_type/Pr_typeList.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_typeListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_typeList
	 */
	protected $m_pr_typeList;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->Update ();
		
		$this->ReadNameStrings ( 'classes/Product/langs/', 'type.xml' );
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
		
		// call DoAction of Pr_typeCMS
		$overLink = $this->m_pr_typeList->GetLinkIDName ();
		if (($overLinkID = DB::POST ( $overLink )))
		{
			$pr_typeCMS = new Pr_typeCMS ( $overLinkID );
			$ar = $pr_typeCMS->DoAction ();
		}
		
		// do actions for Pr_type list
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				if (($name = DB::POST('name')))
				{
					if (strlen ( trim ( $name ) ) > 0)
					{
						// insert new pr_type with name
						$newPr_type = new Pr_type ( );
						$params = array ('name' => $name );
						$ar = $newPr_type->Insert ( $params );
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
		$currID = $this->m_pr_typeList->GetCurrentID ();
		
		if ($currID)
		{
			$linkName = $this->m_pr_typeList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>Product type</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">Product type</div>' );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_pr_typeList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		$currentPr_typeID = $this->m_pr_typeList->GetCurrentID ();
		
		// get cooretn pr_type
		$pr_type = new Pr_type ( $currentPr_typeID );
		
		$currentPr_typeID = $pr_type->GetID ();
		// if is selected pr_type, then diplay pr_type features
		if ($currentPr_typeID)
		{
			$pr_typeCMS = new Pr_typeCMS ( $currentPr_typeID );
			$pr_typeCMS->DisplayFeatures ();
		
		} // diplay new pr_type form
		else
		{
			// read language strings
			$doneS = $this->GetNameString ( 'done' );
			$noEmptyS = $this->GetNameString ( 'noempty' );
			// create new product type
			$cNT = $this->GetNameString ( 'createNewType' );
			
			// rename js /////////////////
			$rJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
			// display insert form
			$this->DisplaySingleTextForm ( 'insert', $cNT, 'name', null, null, $doneS, $rJS );
		}
	}
	
	function Update()
	{
		$this->m_pr_typeList = new Pr_typeList ( );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	function GetCurrentID()
	{
		return $this->m_pr_typeList->GetCurrentID ();
	}
	
	function CreateDB()
	{
		$prType = new Pr_typeCMS ( );
		return $prType->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$prType = new Pr_typeCMS ( );
		return $prType->SelfTest ();
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		$prType = new Pr_typeCMS ( );
		return $prType->GetObjectTables ();
	}
}
?>