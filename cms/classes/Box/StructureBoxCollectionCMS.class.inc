<?php
Site::IncludeFile ( 'classes/Box/BoxCollectionList.class.inc' );
Site::IncludeFile ( 'classes/Box/SBRelation.class.inc' );
Site::IncludeFile ( 'classes/Box/SBRelationCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package Box
 */
class StructureBoxCollectionCMS extends CMSObject implements CMSInterface
{
	function __construct()
	{
		parent::__construct ();
		
		$this->ReadNameStrings ( 'classes/Box/langs/', 'box.xml' );
	}
	
	function DoAction()
	{
		switch ($this->GetFormAction ())
		{
			case 'addRelation' :
				$catID = DB::POST ( 'catID' );
				$sbRel = new SBRelation ( $catID );
				
				$collTypes = $this->m_site->GetBoxCollectionTypes ();
				
				if ($collTypes)
				{
					$collTypesKeys = array_keys ( $collTypes );
					
					foreach ( $collTypesKeys as $key )
					{
						$selectedID = DB::POST ( "sitebox$key" );
						
						if ($selectedID > 0)
						{
							$sbRel->Add ( $selectedID, $key );
						}
						else
						{
							$sbRel->Delete ( $key );
						}
					}
				}
				break;
		}
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
	 * @param int $catID
	 */
	function DisplayFeatures($catID = null)
	{
		if ($catID < 1)
		{
			return;
		}
		
		$doneS = $this->GetNameString ( 'done' );
		$catRelColl = $this->GetNameString ( 'catRelColl' );
		
		$sbRel = new SBRelation ( $catID );
		
		print ( "<div class='someGTitleBox'>$catRelColl</div>" );
		print ( '<div class="someGBox">' );
		
		$boxCollection = new BoxCollectionList ( );
		
		$this->DisplayFormHeadr ( 'addRelation' );
		
		print ( "<input type='hidden' name='catID' value='$catID'>" );
		
		// if site has legt box
		$collTypes = $this->m_site->GetBoxCollectionTypes ();
		
		if ($collTypes)
		{
			print ( '<table border="0" style="padding:5px">' );
			foreach ( $collTypes as $key => $title )
			{
				print ( '<tr>' );
				
				print ( "<td align='right'>$title : </td>" );
				
				print ( '<td>' );
				print ( "<select name='sitebox$key'>" );
				$leftID = $sbRel->GetCatRelationID ( $key );
				$boxCollection->DisplaySelect ( '- - -', $leftID );
				print ( '</select>' );
				print ( '</td>' );
				print ( '<tr>' );
			}
			print ( '</table>' );
		}
		
		print ( "<input type='submit' value='$doneS' />" );
		print ( '</form>' );
		
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function CreateDB()
	{
		$sbRelation = new SBRelationCMS ( );
		$sbRelation->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$sbRelation = new SBRelationCMS ( );
		$res = $sbRelation->SelfTest ();
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		$sbRelation = new SBRelationCMS ( );
		return $sbRelation->GetObjectTables ();
	}
}
?>