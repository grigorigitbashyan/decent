<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_field/Pr_field.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_field/Pr_fieldList.class.inc' );

Site::IncludeFile ( 'classes/Menu/Menu.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuView.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuList.class.inc' );

Site::IncludeFile ( 'classes/Category/Category.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_int/Pr_ListMan.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_fieldCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_field
	 */
	protected $m_pr_field;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$ID = DB::REQUEST ( 'pr_fieldID' );
		$this->m_pr_field = new Pr_field ( $ID );
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
			case 'rename' : // rename Pr_field
				if (($newName = DB::POST ( 'newName' )))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_pr_field->SetName ( $newName );
					}
				}
				break;
			case 'delete' : // delete Pr_field
				$fieldID = $this->m_pr_field->GetID ();
				
				// delete field
				$ar = $this->m_pr_field->Delete ();
				
				if ($ar)
				{
					// delete field relations with options tables
					$opList = new Pr_ListMan ( );
					$opList->DeleteFieldInformation ( $fieldID );
					
					// delete field relations with product types
					$pr_typeList = new Pr_typefieldListCMS ( );
					$pr_typeList->DeleteFieldFromAllTypes ( $fieldID );
				}
				
				break;
			case 'selecttype' :
				if (($type = DB::POST ( 'type' )))
				{
					$this->m_pr_field->SetType ( $type );
					
					// set multy language true
					if ($type == 6 && $type == 7)
					{
						$this->m_pr_field->SetMultilang ( true );
					}
				}
				break;
			case 'selecttypeoptions' :
				if (($options = DB::POST ( 'options' )))
				{
					$this->m_pr_field->SetSubID ( $options );
				}
				
				if (($multOpt = DB::POST ( 'multOpt' )))
				{
					$this->m_pr_field->SetMultilang ( $multOpt );
				}
				break;
			case 'editor':
				$editor = DB::POST ( 'editor' );
				$this->m_pr_field->SetEditor ( $editor );
				break;
			case 'subtype':
				$subtype = DB::POST ( 'subtype' );
				$this->m_pr_field->SetSubtype ( $subtype );
				break;
			case 'selecttypefields':
				$fields = DB::POST('fields');
				$pattern = $_POST['pattern'];
				if ( is_array($fields) )
				{
					$fields = implode(",", $fields);
				}
				$this->m_pr_field->SetFields($fields);
				$this->m_pr_field->SetFieldPattern($pattern);
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
		$ID = $this->m_pr_field->GetID ();
		$objName = $this->m_pr_field->GetName ();
		$fieldType = $this->m_pr_field->GetType ();
		$overLink = 'pr_fieldID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		
		// delete form javascript
		$sureDeleteS = $this->GetNameString ( 'sureDelete' );
		$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
		
		$delForm = "<div  class='titleColR'>
    					<form method='POST' $dJS>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='pr_fieldID' value='$ID'>
    					<input type='submit' value='Delete'>
    					</form>
    					</div>";
		
		// display form title
		
		$this->DisplayTitle ( $objName, $delForm );
		
		// diplay rename form
		$renameS = $this->GetNameString ( 'rename' );
		$doneS = $this->GetNameString ( 'done' );
		$noEmptyS = $this->GetNameString ( 'noempty' );
		
		// rename js
		$rJS = "onsubmit='return SingleForm(\"newName\", \"$noEmptyS\");'";
		$this->DisplaySingleTextForm ( 'rename', $renameS, 'newName', $objName, array ($overLink => $ID ), $doneS, $rJS );
		
		// display field type form
		if ($fieldType)
		{
			print ( '<br />' );
			
			switch ($fieldType)
			{
				// options type
				case 1 :
					print ( '<h2>Related to Options</h2><br />' );
					$subID = $this->m_pr_field->GetSubID ();
					$opTypeList = new OptionsList ( );
					
					print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          <tr>
			            <td>
			                <div class="titleColL">' );
					$this->DisplayFormHeadr ( 'selecttypeoptions' );
					$this->DisplayHidden ( $overLink, $ID );
					
					print ( '<select name="options">' );
					$opTypeList->DisplaySelect ( ' -- Select Options -- ', $subID );
					print ( '</select>' );
					
					$sm = $this->m_pr_field->GetMultilang ();
					
					$checkS = ($sm == 1) ? 'checked="checked"' : '';
					$checkM = ($sm == 2) ? 'checked="checked"' : '';
					
					print ( '<input name="multOpt" id="multOpt" type="radio" value="1" ' . $checkS . '/> Single ' );
					print ( '<input name="multOpt" id="multOpt" type="radio" value="2" ' . $checkM . ' /> Multiple' );
					
					print ( '<br />' );
					print ( '</div>
			                <div class="titleColR">' );
					$this->DisplayFormFooter ();
					print ( '</div>
		             <br class="clearFloat" />
		             
			        	</td>
			          </tr>
			        </table>
			        </form>' );
					
					break;
				// structure
				case 2 :
					print ( '<h2>Related to Structure</h2><br />' );
					$subID = $this->m_pr_field->GetSubID ();
					
					print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          <tr>
			            <td>
			                <div class="titleColL">' );
					
					if ($subID > 0)
					{
						$cat = new Category ( $subID );
						$title = $cat->GetName ( $this->m_displayLang );
						
						$scMenuID = $cat->GetMenuID ();
						$csMenu = new Menu ( $scMenuID, false );
						$menuName = $csMenu->GetName ();
						
						print ( "<h3>Selected structrue is '$title' from '$menuName' menu</h3>" );
					}
					elseif ($subID < 0)
					{
						$csMenu = new Menu ( - $subID, false );
						$menuName = $csMenu->GetName ();
						
						print ( "<h3>Selected menu is '$menuName'</h3>" );
					}
					
					$menuList = new MenuList ( );
					
					$menus = $menuList->GetMenus ();
					
					foreach ( $menus as $menu )
					{
						$cMenu = new Menu ( $menu ['ID'], true, $this->m_displayLang );
						$struct = $cMenu->GetStructure ();
						
						if (count ( $struct ) > 0)
						{
							print ( "<h3>menu '{$menu['name']}'</h3>" );
							
							$this->DisplayFormHeadr ( 'selecttypeoptions' );
							$this->DisplayHidden ( $overLink, $ID );
							
							print ( "<select name='options'>\n" );
							print ( "<option value='m{$menu['ID']}'> -- {$menu['name']} -- </option>\n" );
							
							$drMenu = new MenuView ( $struct, $this->m_displayLang );
							
							$drMenu->Display ( - 1, 5 );
							
							print ( "</select>\n" );
							
							$this->DisplayFormFooter ();
						}
					}
					
					print ( '<br />' );
					print ( '</div>
			                <div class="titleColR">' );
					
					print ( '</div>
		             <br class="clearFloat" />
		             
			        	</td>
			          </tr>
			        </table>
			        </form>' );
					break;
				case 3 :
					print ( '<h2>Intager Number Field</h2>' );
					break;
				case 4 :
					print ( '<h2>Double Number Field</h2>' );
					break;
				case 5 :
					print ( '<h2>Short Text Field (255 chars)</h2>' );
					
					$subtype = $this->m_pr_field->GetSubtype();
					print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          <tr>
			            <td>
			                <div class="titleColL">' );
					$this->DisplayFormHeadr ( 'subtype' );
					$this->DisplayHidden ( $overLink, $ID );
					?>
					<select name="subtype">
						<option value="">---</option>
						<option value="url"<?php echo ($subtype=="url" ? " selected": "");?>>URL</option>
						<option value="datefield"<?php echo ($subtype=="datefield" ? " selected": "");?>>Datefield</option>
					</select>
					<?php
					print ( '<br />' );
					print ( '</div>
			                <div class="titleColR">' );
					$this->DisplayFormFooter ();
					print ( '</div>
		             <br class="clearFloat" />
		             
			        	</td>
			          </tr>
			        </table>
			        </form>' );
					break;
				case 6 :
					print ( '<h2>Short Text Field (255 chars multy languages)</h2>' );
					break;
				case 7 :
					$editor = $this->m_pr_field->GetEditor();
					print ( '<h2>Text Field</h2>' );
					
					print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          <tr>
			            <td>
			                <div class="titleColL">' );
					$this->DisplayFormHeadr ( 'editor' );
					$this->DisplayHidden ( $overLink, $ID );
					if ( $editor )
					{
						print ( "Display with Text Editor: <input type='radio' name='editor' value='1' checked> yes <input type='radio' name='editor' value='0'> no\n" );
					}
					else 
					{
						print ( "Display with Text Editor: <input type='radio' name='editor' value='1'> yes <input type='radio' name='editor' value='0' checked> no\n" );
					}
					
					print ( '<br />' );
					print ( '</div>
			                <div class="titleColR">' );
					$this->DisplayFormFooter ();
					print ( '</div>
		             <br class="clearFloat" />
		             
			        	</td>
			          </tr>
			        </table>
			        </form>' );
					break;
				// structure multi-choice
				case 8 :
					print ( '<h2>Related to Structure</h2><br />' );
					$subID = $this->m_pr_field->GetSubID ();
					
					print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          <tr>
			            <td>
			                <div class="titleColL">' );
					
					if ($subID > 0)
					{
						$cat = new Category ( $subID );
						$title = $cat->GetName ( $this->m_displayLang );
						
						$scMenuID = $cat->GetMenuID ();
						$csMenu = new Menu ( $scMenuID, false );
						$menuName = $csMenu->GetName ();
						
						print ( "<h3>Selected structrue is '$title' from '$menuName' menu</h3>" );
					}
					elseif ($subID < 0)
					{
						$csMenu = new Menu ( - $subID, false );
						$menuName = $csMenu->GetName ();
						
						print ( "<h3>Selected menu is '$menuName'</h3>" );
					}
					
					$menuList = new MenuList ( );
					
					$menus = $menuList->GetMenus ();
					
					foreach ( $menus as $menu )
					{
						$cMenu = new Menu ( $menu ['ID'], true, $this->m_displayLang );
						$struct = $cMenu->GetStructure ();
						
						if (count ( $struct ) > 0)
						{
							print ( "<h3>menu '{$menu['name']}'</h3>" );
							
							$this->DisplayFormHeadr ( 'selecttypeoptions' );
							$this->DisplayHidden ( $overLink, $ID );
							
							print ( "<select name='options'>\n" );
							print ( "<option value='m{$menu['ID']}'> -- {$menu['name']} -- </option>\n" );//It was "m{$menu['ID']}"
							
							$drMenu = new MenuView ( $struct, $this->m_displayLang );
							
							$drMenu->Display ( - 1, 5 );
							
							print ( "</select>\n" );
							
							$this->DisplayFormFooter ();
						}
					}
					
					print ( '<br />' );
					print ( '</div>
			                <div class="titleColR">' );
					
					print ( '</div>
		             <br class="clearFloat" />
		             
			        	</td>
			          </tr>
			        </table>
			        </form>' );
					break;
				case 9:
					print ( '<h2>Select fields & set Pattern</h2><br />' );
					$fields = $this->m_pr_field->GetFields();
					$pattern = $this->m_pr_field->GetFieldPattern();
					if ( $fields )
					{
						$fields = explode(",", $fields);
					}
					$fieldList = new Pr_fieldList ( );
					$fieldListArr = $fieldList->GetStructure();
					
					print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          			<tr>
			            			<td>
			                			<div class="titleColL">' );
											$this->DisplayFormHeadr ( 'selecttypefields' );
											$this->DisplayHidden ( $overLink, $ID );
											?>
											<select name="fields[]" size="10" multiple>
												<?php
												for ( $i=0; $i<count($fieldListArr); $i++ )
												{
													if ( $fieldListArr[$i]["ID"]!=$this->m_pr_field->GetID() && $fieldListArr[$i]["type"]!=2 && $fieldListArr[$i]["type"]!=8 )
													{
														?>
														<option value="<?php echo $fieldListArr[$i]["ID"];?>"<?php echo (in_array($fieldListArr[$i]["ID"], $fields) ? " selected": "");?>><?php echo $fieldListArr[$i]["name"];?></option>
														<?php
													}
												}
												?>
											</select>
											<br />
											<?php
										print ( '</div><div class="titleColL"><textarea name="pattern" style="width:500px;height:200px;" class="mceNoEditor">'.$pattern.'</textarea></div><div class="titleColR">' );
										$this->DisplayFormFooter ();
										print ( '</div>
		             					<br class="clearFloat" />
						        	</td>
			          			</tr>
			        		</table>
			        	</form>' );
					break;
			}
		}
		else
		{
			print ( '<br /><table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          <tr>
			            <td>
			                <div class="titleColL">' );
			$this->DisplayFormHeadr ( 'selecttype' );
			$this->DisplayHidden ( $overLink, $ID );
			
			$labels [1] = 'Options';
			$labels [2] = 'Structure';
			$labels [3] = 'Number (intager)';
			$labels [4] = 'Number (point)';
			$labels [5] = 'String (255 chars)';
			$labels [6] = 'String (255 chars in multi language)';
			$labels [7] = 'Text (multi language)';
			$labels [8] = 'Structure (multi choice)'; // Adriada; added
			$labels [9] = 'Group of fields'; // Grigor; added
			
			for($index = 1; $index < 10; $index ++) //Adriada; changed; from 8 to 9
			{
				if ($index == $fieldType)
				{
					print ( "<input name='type' type='radio' value='$index' checked='checked'/> $labels[$index]" );
				}
				else
				{
					print ( "<input name='type' type='radio' value='$index' /> $labels[$index]" );
				}
				print ( '<br />' );
			}
			
			print ( '<br />' );
			print ( '</div>
			                <div class="titleColR">' );
			$this->DisplayFormFooter ();
			print ( '</div>
		             <br class="clearFloat" />
			        	</td>
			          </tr>
			        </table>
			        </form>' );
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function Delete()
	{
		return $this->m_pr_field->Delete ();
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `pr_field` (
				  `ID` smallint(5) unsigned NOT NULL auto_increment,
				  `type` tinyint(3) unsigned default NULL,
				  `name` varchar(128) NOT NULL,
				  `subID` tinyint(3) unsigned default NULL,
				  `multilang` tinyint(1) default NULL,
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		return array ('pr_field' );
	}
}
?>