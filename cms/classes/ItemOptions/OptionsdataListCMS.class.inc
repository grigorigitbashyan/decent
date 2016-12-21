<?php
// inculdes
Site::IncludeFile ( 'classes/ItemOptions/Optionsdata.class.inc' );
Site::IncludeFile ( 'classes/ItemOptions/OptionsdataCMS.class.inc' );
Site::IncludeFile ( 'classes/ItemOptions/OptionsdataList.class.inc' );

Site::IncludeFile ( 'classes/ItemOptions/Options.class.inc' );

/**
 * Enter description here...
 *
 * @package ItemOptions
 */
class OptionsdataListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var OptionsdataList
	 */
	protected $m_optionsdataList;
	
	/**
	 * The ID of options collection
	 *
	 * @var int
	 */
	protected $m_optionsID;
	
	
	/**
	 * Construct
	 *
	 * @param int $opID
	 * @param bool $valueType if true return values(for selected language) else return key
	 */
	function __construct($opID)
	{
		parent::__construct ();
		
		$this->m_optionsID = $opID;
		
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
		$optionsEx = new Options ( $this->m_optionsID );
		$this->m_optionsID = $optionsEx->GetID ();
		// action result
		$ar = null;
		
		// call DoAction of OptionsdataCMS
		$overLink = $this->m_optionsdataList->GetLinkIDName ();
		if (($owerID = DB::POST ( $overLink )))
		{
			$optionsdataCMS = new OptionsdataCMS ( $owerID );
			$ar = $optionsdataCMS->DoAction ();
		}
		
		// do actions for Optionsdata list
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				$name = DB::POST ('insertName');
				$opID = DB::POST ('optionsID');
				if ($name && $opID)
				{
					if (strlen ( trim ( $name ) ) > 0)
					{
						// insert new optionsdata with name
						$newOptionsdata = new Optionsdata ( );
						
						$params = array ('optionsID' => $opID );
						
						$langs = $this->m_site->GetLanguages ();
						
						foreach ( $langs as $clang )
						{
							$params ["name_$clang"] = $name;
						}
						
						$ar = $newOptionsdata->Insert ( $params );
					}
				}
				break;
			case 'delete' :
				if (($IDs = DB::POST ('optdIDs')))
				{
					foreach ( $IDs as $ID )
					{
						$optionsdata = new Optionsdata ( $ID );
						$ar = $optionsdata->Delete ();
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
		$currID = $this->m_optionsdataList->GetCurrentID ();
		
		if ($currID)
		{
			$linkName = $this->m_optionsdataList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>Optionsdata</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">Optionsdata</div>' );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_optionsdataList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		if (! $this->m_optionsID)
		{
			return;
		}
		
		// get cooretn optionsdata
		$currentOptionsdataID = $this->m_optionsdataList->GetCurrentID ();
		$optionsdata = new Optionsdata ( $currentOptionsdataID );
		
		$currentOptionsdataID = $optionsdata->GetID ();
		// if is selected optionsdata, then diplay optionsdata features
		if ($currentOptionsdataID)
		{
			$optionsdataCMS = new OptionsdataCMS ( $currentOptionsdataID );
			$optionsdataCMS->DisplayFeatures ();
		
		} // diplay new optionsdata form
		else
		{
			// get names
			$allS = $this->GetNameString ( 'all' );
			$noneS = $this->GetNameString ( 'none' );
			$selectS = $this->GetNameString ( 'select' );
			$createS = $this->GetNameString ( 'create' );
			$deleteS = $this->GetNameString ( 'delete' );
			$doneS = $this->GetNameString ( 'done' );
			$noEmptyS = $this->GetNameString ( 'noempty' );
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$noSelectedS = $this->GetNameString ( 'noselected' );
			
			$options = new Options ( $this->m_optionsID );
			$title = $options->GetName ();
			
			// display insert form
			$insertJS = "onsubmit='return SingleForm(\"insertName\", \"$noEmptyS\");'";
			$this->DisplaySingleTextForm ( 'insert', "$createS $title: ", 'insertName', null, array ('optionsID' => $this->m_optionsID ), $doneS, $insertJS );
			// end new optionsdata form
			

			$opdatCount = $this->m_optionsdataList->GetItemCount ();
			if ($opdatCount > 0)
			{
				// display form
				$this->DisplayFormHeadr ( 'delete', 'optionsdata' );
				?>
<br />
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColR"><span style="padding-right: 20px"><strong><?php echo $selectS?></strong>
		<a href="#"
			onclick="chekeBoxes('optionsdata', 'optdIDs', true); return false"><?php echo $allS?></a>,
		<a href="#"
			onclick="chekeBoxes('optionsdata', 'optdIDs', false); return false"><?php echo $noneS?></a>
		</span> <input name="" type="submit" value="<?php echo $deleteS?>"
			onclick="return ChecksFormDelete('optionsdata', 'optdIDs', '<?php echo $noSelectedS?>', '<?php echo $sureDeleteS?>');" />
		</div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	style="border-left: 1px solid #777777; border-right: 1px solid #777777">
	<tr>
		<td>
		<?php
				$overName = $this->m_optionsdataList->GetLinkIDName ();
				
				if ( $this->m_optionsID==3 )
				{
					$patern = "<div class='tableCHB'>
		                <input name='optdIDs[]' id='optdIDs' type='checkbox' value='@@ID@@' />
		                </div>
	                    <div class='tableT'>
	                    	<a href='?optionsID={$this->m_optionsID}&{$overName}=@@ID@@'>@@name@@ / @@key@@</a>
	                    </div>
		                <br class='clearFloat' />";
				}
				else 
				{
					$patern = "<div class='tableCHB'>
		                <input name='optdIDs[]' id='optdIDs' type='checkbox' value='@@ID@@' />
		                </div>
	                    <div class='tableT'>
	                    	<a href='?optionsID={$this->m_optionsID}&{$overName}=@@ID@@'>@@name@@</a>
	                    </div>
		                <br class='clearFloat' />";
				}
				
				$this->m_optionsdataList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
				
				print ( '</td></tr></table></form>' );
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Update()
	{
		$this->m_optionsdataList = new OptionsdataList ( $this->m_optionsID );
	}
	
	function CreateDB()
	{
		$opData = new OptionsdataCMS ( );
		$opData->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$opData = new OptionsdataCMS ( );
		return $opData->SelfTest ();
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		$opData = new OptionsdataCMS ( );
		return $opData->GetObjectTables ();
	}
}
?>