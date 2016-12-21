<?php
// inculdes
Site::IncludeFile ( 'classes/ItemOptions/Options.class.inc' );

Site::IncludeFile ( 'classes/Menu/Menu.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuView.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuList.class.inc' );

/**
 * Enter description here...
 *
 * @package ItemOptions
 */
class OptionsCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Options
	 */
	protected $m_options;
	
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	protected $m_editMode;
	
	/**
	 * Enter description here...
	 *
	 * @param int $currentOptionsID
	 * @param bool $editMode
	 */
	function __construct($currentOptionsID = null, $editMode = false)
	{
		parent::__construct ();
		
		$this->m_editMode = $editMode;
		
		$ID = $currentOptionsID;
		if ($currentOptionsID == null)
		{
			$ID = DB::GET_POST('optionsID');
		}
		
		$this->m_options = new Options ( $ID );
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
			case 'rename': // rename Options
				if ( ($newName=DB::POST('name')) )
				{
					$newName = trim($newName);
					if ( strlen($newName)>0 )
					{
						// rename
						$ar = $this->m_options->SetName($newName);
					}
				}
				break;
			case 'delete': // delete Options
				$ar = $this->m_options->Delete();
				break;
			case 'set_category':
				$category_id = DB::POST('category_id');
				$ar = $this->m_options->SetCategoryID($category_id);
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
		$ID = $this->m_options->GetID();
		$objName = $this->m_options->GetName();
		$objCatID = $this->m_options->GetCategoryID();
		
		$overLink = 'optionsID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		
		$delForm = null;
		if ($this->m_editMode)
		{
			// delete form javascript
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
			
			$delForm = "<div  class='titleColR'>
    					<form method='POST' $dJS>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='optionsID' value='$ID'>
    					<input type='submit' value='Delete'>
    					</form>
    					</div>";
		}
		
		// display form title
		$this->DisplayTitle ( $objName, $delForm );
		
		// diplay rename form
		if ($this->m_editMode)
		{
			$renameS = $this->GetNameString ( 'rename' );
			$doneS = $this->GetNameString ( 'done' );
			$noEmptyS = $this->GetNameString ( 'noempty' );
			
			$insertJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
			$this->DisplaySingleTextForm ( 'rename', $renameS, 'name', $objName, array ($overLink => $ID ), $doneS, $insertJS );
		}
		print ( '<br />' );
		
		?>
		<form method='post' name='' id='' class='clearPD' action='' onsubmit='return SingleForm("name", "The field cannot be empty!");'>
		<input type='hidden' name='optionscmsAction' value='set_category'>
		<input type='hidden' name='optionsID' value='<?php echo $ID;?>' />
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
				<tr>
					<td>
						<div class="titleColL"><label><strong>Category</strong></label>
							<select name="category_id" id="category_id">
								<option value="0">---</option>
								<?php
								$cMenu = new Menu(2, true, $this->m_displayLang);
								$struct = $cMenu->GetStructure();
								for ( $i=0; $i<count($struct); $i++ )
								{
									?>
									<option value="<?php echo $struct[$i]["ID"];?>"<?php echo ($struct[$i]["ID"]==$objCatID ? " selected": "");?>><?php echo $struct[$i]["name"];?></option>
									<?php
								}
								?>
							</select>
						</div>
						<div class="titleColR"><input name="" type="submit" value="Done" /></div>
						<br class="clearFloat" />
					</td>
				</tr>
			</table>
		</form>
		<br />
		<?php
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function Delete()
	{
		return $this->m_options->Delete ();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = 'CREATE TABLE `options` (
				  `ID` smallint(5) unsigned NOT NULL auto_increment,
				  `name` varchar(64) NOT NULL,
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;';
		
		return $sql;
	}
}
?>