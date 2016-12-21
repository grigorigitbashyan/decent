<?php
/**
 * Base class for all CMS List objects
 * 
 * @package Main
 * @author Vazgen Manukyan
 * @package Main
 */
class CMSListObject extends CMSObject
{
	/**
	 * Enter description here...
	 *
	 * @var TreeView
	 */
	protected $m_list;
	
	/**
	 * Record class name (object name)
	 *
	 * @var string
	 */
	protected $m_recClassName;
	
	/**
	 * Object CMS class name
	 *
	 * @var string
	 */
	protected $m_cmsClassName;
	
	protected $m_object;
	/**
	 * Constructor
	 *
	 * @param TreeView $list
	 * @param string $recClassName
	 * @param stirng $cmsClassName
	 */
	function __construct($list, $recClassName, $cmsClassName = null)
	{
		parent::__construct ();
		
		// set list
		$this->m_list = $list;
		
		// set object class name
		$this->m_recClassName = $recClassName;
		
		// set object CMS class name
		$this->m_cmsClassName = ($cmsClassName) ? $cmsClassName : $recClassName . 'CMS';
		
		// set null current object
		$this->m_object = null;
		
		$this->MakeObject ();
	}
	
	/**
	 * Display menu. 
	 *
	 * @param string $name
	 * @param bool $showMenuLink
	 */
	function DisplayMenu($name, $showMenuLink = true)
	{
		$correntID = $this->m_list->GetCurrentID ();
		$path = $this->m_list->GetAdditionalPath();

		if ($showMenuLink && $correntID !== 'new')
		{
			$overLink = $this->m_list->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$overLink=new{$path}'>$name</a></div>" );
		}
		else
		{
			print ( "<div class='someWTitleBox'>$name</div>" );
		}
		
		print ( '<div class="someWBox">' );
		
		$this->m_list->DisplayList ( 'directoryMenu1',  'selLink');
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function DisplayFeatures()
	{
		// parent function
		parent::DisplayFeatures ();
		
		// getting cutrrnet ID
		$currentID = $this->m_list->GetCurrentID ();
		
		// get cooretn box
		$recordObject = new $this->m_recClassName ( $currentID );
		
		$currentID = $recordObject->GetID ();
		// if is selected box, then diplay box features
		
		$this->MakeObject ( $currentID );
		$res = $this->m_object->DisplayFeatures ();
		
		return $res;
	}
	
	/**
	 * Do action for selected object
	 * 
	 * @param bool $insertAction
	 * @return bool|null
	 */
	function DoAction($doWhenNull = false)
	{
		// action result
		$ar = null;
		
		// call DoAction of BoxCMS
		$overLink = $this->m_list->GetLinkIDName ();
		$someID = DB::POST ( $overLink );
		if ($doWhenNull || $someID)
		{
			if (! $someID)
			{
				$someID = DB::GET ( $overLink );
			}
			
			$this->MakeObject ( $someID );
			$ar = $this->m_object->DoAction ();
		}
		
		// do actions for Box list
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				// insert action
				$this->MakeObject();
				
				$recClass = new $this->m_recClassName ( );
				$tableName = $recClass->GetTableName();
				
				// insert control
				$insertAction = $this->IsInsertable($tableName);
				
				if ($insertAction)
				{
					$name = DB::POST ( 'name' );
					if ($name)
					{
						if (strlen ( trim ( $name ) ) > 0)
						{
							// insert new box with name
							$params = array ('name' => $name );
							$ar = $recClass->Insert ( $params );
							$ar = true;
						}
					}
				}
				break;
		}
		
		if (true)
		{
			$this->Update ();
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Update()
	{
		$this->m_list->Update ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$objCMS = new $this->m_cmsClassName ( );
		return $objCMS->SelfTest ();
	}
	
	/**
	 * This function returns object tables.
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		$objCMS = new $this->m_cmsClassName ( );
		return $objCMS->GetObjectTables ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$objCMS = new $this->m_cmsClassName ( );
		return $objCMS->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $someID
	 */
	function MakeObject($someID = null)
	{
		if (! $someID)
		{
			$someID = $this->m_list->GetCurrentID ();
		}

		if (! $this->m_object)
		{
			$this->m_object = new $this->m_cmsClassName ( $someID );
		}
	}
}
?>