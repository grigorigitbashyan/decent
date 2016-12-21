<?php
// inculdes
Site::IncludeFile ( 'classes/ItemOptions/Options.class.inc' );
Site::IncludeFile ( 'classes/ItemOptions/OptionsCMS.class.inc' );
Site::IncludeFile ( 'classes/ItemOptions/OptionsList.class.inc' );
Site::IncludeFile ( 'classes/ItemOptions/Optionsdata.class.inc' );
Site::IncludeFile ( 'classes/ItemOptions/OptionsdataList.class.inc' );

/**
 * Enter description here...
 *
 * @package ItemOptions
 */
class OptionsListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var OptionsList
	 */
	protected $m_optionsList;
	
	protected $m_editMode;
	
	function __construct($editMode)
	{
		parent::__construct ();
		
		$this->m_editMode = $editMode;
		
		$this->Update ();
		
		$this->ReadNameStrings ( 'classes/ItemOptions/langs/', 'items.xml' );
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
		
		// call DoAction of OptionsCMS
		

		$overLink = $this->m_optionsList->GetLinkIDName ();
		if (($owerLinkID = DB::POST ( $overLink )))
		{
			$optionsCMS = new OptionsCMS ( $owerLinkID );
			$ar = $optionsCMS->DoAction ();
		}
		
		// do actions for Options list
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				if (($name = DB::POST ( 'name' )))
				{
					if (strlen ( trim ( $name ) ) > 0)
					{
						// insert new options with name
						$newOptions = new Options ( );
						$params = array ('name' => $name );
						$ar = $newOptions->Insert ( $params );
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
		$name = $this->GetNameString ( 'moduleName' );
		// // insert is denited
		if ($this->m_editMode)
		{
			$currID = $this->m_optionsList->GetCurrentID ();
			
			if ($currID)
			{
				$linkName = $this->m_optionsList->GetLinkIDName ();
				print ( "<div class='someWTitleBox'><a href='?$linkName='>$name</a></div>" );
			}
			else
			{
				print ( '<div class="someWTitleBox">' . $name . '</div>' );
			}
		}
		else
		{
			print ( '<div class="someWTitleBox">' . $name . '</div>' );
		}
		
		print ( '<div class="someWBox">' );
		
		// get oprions data selected item
		$optionsdataList = new OptionsdataList ( $this->m_optionsList->GetCurrentID () );
		$coropdataID = $optionsdataList->GetCurrentID ();
		$cOptions = new Optionsdata ( $coropdataID );
		$coropdataID = $cOptions->GetID ();
		
		if ($coropdataID)
		{
			$this->m_optionsList->DisplayList ( 'directoryMenu1', 'selLink', 'selectedLink' );
		}
		else
		{
			$this->m_optionsList->DisplayList ( 'directoryMenu1', 'selLink' );
		}
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		$currentOptionsID = $this->m_optionsList->GetCurrentID ();
		
		// get cooretn options
		$options = new Options ( $currentOptionsID );
		
		$currentOptionsID = $options->GetID ();
		// if is selected options, then diplay options features
		

		if ($currentOptionsID)
		{
			$optionsCMS = new OptionsCMS ( $currentOptionsID, $this->m_editMode );
			$optionsCMS->DisplayFeatures ();
		
		} // diplay new options form
		else
		{
			// get names
			$createOptionsS = $this->GetNameString ( 'createOptions' );
			$doneS = $this->GetNameString ( 'done' );
			$noEmptyS = $this->GetNameString ( 'noempty' );
			
			// display insert form
			$insertJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
			$this->DisplaySingleTextForm ( 'insert', $createOptionsS, 'name', null, null, $doneS, $insertJS );
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	function GetCurrentOptions()
	{
		return $this->m_optionsList->GetCurrentID ();
	}
	
	function Update()
	{
		$this->m_optionsList = new OptionsList ( );
	}
	
	function CreateDB()
	{
		$opt = new OptionsCMS ( );
		return $opt->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$opt = new OptionsCMS ( );
		return $opt->SelfTest ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function GetObjectTables()
	{
		$opt = new OptionsCMS ( );
		return $opt->GetObjectTables ();
	}
}
?>