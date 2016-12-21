<?php
Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserType.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/Access/UserTypeAccess.class.inc' );

/**
 * Enter class description
 */
class UsertypeAccessCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var Usertype
	 */
	protected $m_usertype;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_usertype = new Usertype ( $ID );
		
		$this->ReadNameStrings ( 'classes/UserManagement/langs/', 'user.xml' );
	}
	
	/**
	 * All actions must be done hear
	 *
	 * @return bool
	 */
	function DoAction()
	{
		$ar = null;
		
		switch ($this->GetFormAction ())
		{
			case 'changeName' :
				$value = DB::POST ( 'Name' );
				$ar = $this->m_usertype->SetName ( $value );
				break;
			case 'insertSub' :
				$params ['name'] = DB::POST ( 'Name' );
				$params ['parentID'] = $this->m_usertype->GetID ();
				
				$newUserType = new UserType ( );
				$ar = $newUserType->Insert ( $params );
				break;
			case 'changeAccesses' :
				$modules = DB::POST ( 'modules' );
				$userTypeID = $this->m_usertype->GetID ();
				
				$userTpAces = new UserTypeAccess ( $userTypeID );
				$userTpAces->ModifyModuleAccesses ( $modules );
				break;
			case 'changeAccessesTable' :
				// get all talbes
				// get module main class
				$selectedModule = DB::REQUEST ( 'selectedModule' );
				$mainModule = CMSObject::$CMS->GetMudlueMainClass ( $selectedModule );
				
				// get CMS object tables
				$tables = $mainModule->GetObjectTables ();
				
				// get current user type ID
				$userTypeID = DB::POST ( 'usertype' );
				
				//TODO check privilegis to updata user type detales
				
				// result
				if ($tables)
				{
					// create user type access object
					$userTpAces = new UserTypeAccess ( $userTypeID );
					
					foreach ( $tables as $table )
					{
						// calc table access
						$isResad = (isset ( $_POST ['status_read'] ) && in_array ( $table, $_POST ['status_read'] )) ? STATUS_READ : 0;
						$isWrite = (isset ( $_POST ['status_write'] ) && in_array ( $table, $_POST ['status_write'] )) ? STATUS_UPDATE : 0;
						$isInsert = (isset ( $_POST ['status_insert'] ) && in_array ( $table, $_POST ['status_insert'] )) ? STATUS_INSERT : 0;
						$isDelete = (isset ( $_POST ['status_delete'] ) && in_array ( $table, $_POST ['status_delete'] )) ? STATUS_DELETE : 0;
						$isOwResad = (isset ( $_POST ['status_owner_read'] ) && in_array ( $table, $_POST ['status_owner_read'] )) ? STATUS_OWNER_READ : 0;
						$isOwWrite = (isset ( $_POST ['status_owner_write'] ) && in_array ( $table, $_POST ['status_owner_write'] )) ? STATUS_OWNER_UPDATE : 0;
						$isOwDelete = (isset ( $_POST ['status_owner_delete'] ) && in_array ( $table, $_POST ['status_owner_delete'] )) ? STATUS_OWNER_DELETE : 0;
						
						// status for current type
						$res = $isResad | $isWrite | $isInsert | $isDelete | $isOwResad | $isOwWrite | $isOwDelete;
						
						// make binnary and (&) with parent, for security purpose
						$parentStatus = self::GetController ()->GetStatus ( $table, null, $this->m_usertype->GetParentID () );
						if ($parentStatus != - 1)
						{
							$res &= $parentStatus;
						}
						
						$userTpAces->ModifyTableAccesses ( $table, $res );
					}
				}
				break;
			case 'changeAccessesField' :
				// get all talbes
				// get module main class
				$table = DB::REQUEST ( 'selectedTable' );
				
				// get CMS object tables
				$fields = DBxp::GetFieldNamesOutLang ( $table );
				
				// get current user type ID
				$userTypeID = DB::POST ( 'usertype' );
				
				//TODO check privilegis to updata user type detales
				
				// result
				if ($fields)
				{
					// create user type access object
					$userTpAces = new UserTypeAccess ( $userTypeID );
					
					foreach ( $fields as $field )
					{
						// calc table access
						$isRead = (isset ( $_POST ['status_read'] ) && in_array ( $field, $_POST ['status_read'] )) ? STATUS_READ : 0;
						$isWrite = (isset ( $_POST ['status_write'] ) && in_array ( $field, $_POST ['status_write'] )) ? STATUS_UPDATE : 0;
						$isOwRead = (isset ( $_POST ['status_owner_read'] ) && in_array ( $field, $_POST ['status_owner_read'] )) ? STATUS_OWNER_READ : 0;
						$isOwWrite = (isset ( $_POST ['status_owner_write'] ) && in_array ( $field, $_POST ['status_owner_write'] )) ? STATUS_OWNER_UPDATE : 0;
						
						// status for current type
						$res = $isRead | $isWrite | $isOwRead | $isOwWrite;
						
						// make binnary and (&) with parent, for security purpose
						$parentStatus = ControllerBase::GetController()->GetStatus ( $table, $field, $this->m_usertype->GetParentID () );
						if ($parentStatus != - 1)
						{
							$res &= $parentStatus;
						}
						
						$userTpAces->ModifyTableFieldAccesses ( $table, $field, $res );
					}
				}
				break;
			case 'delete' :
				// get current user type ID
				$userTypeID = DB::POST ( 'usertype' );
				
				//TODO security issue
				if ($userTypeID)
				{
					// delete access
					$userTypeAccess = new UserTypeAccess ( $userTypeID );
					$userTypeAccess->DeleteUserTypeAllAccess ();
					
					// delete type
					$userType = new UserType ( $userTypeID );
					$userType->Delete ();
				}
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
	 * Module features must be displayed hear
	 *
	 */
	function DisplayFeatures()
	{
		if (! $this->m_usertype->GetID ())
		{
			return false;
		}
		
		$usertypeID = $this->m_usertype->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		$optionsS = $this->GetNameString ( 'options' );
		$deleteS = $this->GetNameString ( 'delete' );
		
		$NameS = 'rename'; //$this->GetNameString('Name');
		$newS = 'Insert new sub'; //$this->GetNameString('Name');
		
		// values
		$NameV = $this->m_usertype->GetName ();
		
		//Display title
		$name = $this->m_usertype->GetName ();
		
		$form = null;
		if ($usertypeID > 1)
		{
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$actionName = $this->GetFormActionName ();
			
			$form = "<div  class='titleColR'>
					 <form method='post' class='clearPD'> 
					 <input type='hidden' name='$actionName' value='delete' />
					 <input type='hidden' name='usertype' value='$usertypeID' />
					 <input type='submit' onclick='return confirm(\"$sureDeleteS\");' value='$deleteS' />
					 </form>
					 </div>";
		}
		$this->DisplayTitle ( $name, $form, false );
		
		$panelIndex = 0;
		
		print ( "<div class='someGTitleBox'>$optionsS</div>" );
		print ( '<div class="someGBox">' );
		
		//Forms
		
		// tab header
		$this->DisplayTabHeader ( ++ $panelIndex, $NameS );
		
		$this->DisplayFormHeadr ( 'changeName' );
		$this->DisplayHidden ( 'usertypeID', $usertypeID );
		print ( "<input type=\"text\" value=\"$NameV\" name=\"Name\" id=\"Name\" size=\"40\" maxlength=\"32\" />\n" );
		$this->DisplayFormFooter ( $doneS );
		// tab footer
		$this->DisplayTabFooter ();
		
		// tab header
		$this->DisplayTabHeader ( ++ $panelIndex, $newS );
		
		$this->DisplayFormHeadr ( 'insertSub' );
		$this->DisplayHidden ( 'usertypeID', $usertypeID );
		print ( "<input type='text' value='' name='Name' id='Name' size='40' maxlength='32' />\n" );
		$this->DisplayFormFooter ( $doneS );
		// tab footer
		$this->DisplayTabFooter ();
		
		print ( '</div>' );
		
		// display javascript
		print ( '<script type="text/javascript"><!--' );
		
		for($index = 0; $index <= $panelIndex; $index ++)
			print ( "var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n" );
		print ( '//--></script>' );
		
		// display modules
		$this->DisplayModuleAccess ();
		
		// display tables
		$this->DisplayTableAccess ();
		
		// display fields
		$this->DisplayFieldAccess ();
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayModuleAccess()
	{
		$userTypeID = $this->m_usertype->GetID ();
		if ($userTypeID == CMS::GetUser ()->GetUserTypeID ())
		{
			return;
		}
		
		$parentID = $this->m_usertype->GetParentID ();
		
		$structure = CMSObject::$CMS->GetStructure ();
		
		$allS = $this->GetNameString ( 'all' );
		$noneS = $this->GetNameString ( 'none' );
		
		print ( '<!--Modules Access-->' );
		print ( '<div class="someGTitleBox">Modules Access</div>' );
		$this->DisplayFormHeadr ( 'changeAccesses', 'modulesForm' );
		print ( '<div class="titleBox">
                <div class="titleColL"></div>
                <div class="titleColR"><span style="padding-right:20px">
                	<strong>Select:</strong> <a onclick="chekeBoxes(\'modulesForm\', \'modules\', true); return false" href="#">' . $allS . '</a>, <a onclick="chekeBoxes(\'modulesForm\', \'modules\', false);" href="#">' . $noneS . '</a></span><input name="" type="submit" value="Done" /></div>
                <br class="clearFloat" />
            </div>' );
		print ( '<div class="someTableArea">' );
		
		$index = 0;
		foreach ( $structure as $key => $module )
		{
			// if parent does not have access than child also could not have access
			if (! self::HasModuleAccess ( $key, $parentID ))
			{
				continue;
			}
			
			// get structure name
			$name = $module ['name'] [CMS::GetCMSLang ()];
			$style = ($index % 2 == 0) ? 'tableUneven' : 'tableEven';
			
			print ( "<div class='$style'>" );
			
			// display structure
			$checked = (self::HasModuleAccess ( $key, $userTypeID )) ? ' checked="checked" ' : '';
			print ( "<div class='tableCHB'><input name='modules[]' id='modules' type='checkbox' value='{$key}'$checked/></div>\n" );
			
			print ( "<div class='tableT'>$name</div>\n" );
			print ( "</div>\n<br class='clearFloat' />\n" );
			
			$index ++;
		}
		print ( '</div></form>    
        <!--//Modules Access-->' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @param int $userTypeID
	 * @return bool
	 */
	static function HasModuleAccess($key, $userTypeID = null)
	{
		return UserTypeAccess::HasModuleAccess ( $key, $userTypeID );
	}
	
	/**
	 * This function displays interfase to change user privilegies on module tables.
	 *
	 */
	function DisplayTableAccess()
	{
		$userTypeID = $this->m_usertype->GetID ();
		if ($userTypeID == CMS::GetUser ()->GetUserTypeID ())
		{
			return;
		}
		
		$selectedModule = DB::REQUEST ( 'selectedModule' );
		
		$structure = CMSObject::$CMS->GetStructure ();
		
		print ( '<!--Tables Accesst-->' );
		print ( '<div class="someGTitleBox">Tables Access</div>' );
		$this->DisplayFormHeadr ( 'changeAccessesTable', 'tableForm' );
		$this->DisplayHidden ( 'usertype', $userTypeID );
		print ( '<div class="titleBox">
                <div class="titleColL">
                	<label>Choose Modul: </label>' );
		
		$ajaxS = " onchange=\"xajax_callAjax('usertype={$userTypeID}&action=getModuleTables&module=' + this.value, 'moduleTablesDiv', 'innerHTML');\"";
		print ( "<select name=\"selectedModule\" id=\"selectedModule\" $ajaxS>\n" );
		$slected = (! $selectedModule) ? " selected='selected'" : '';
		print ( "<option{$slected}>...</option>\n" );
		foreach ( $structure as $key => $module )
		{
			// if parent does not have access than chiled olso coulnd not have access
			if (! self::HasModuleAccess ( $key, $userTypeID ))
			{
				continue;
			}
			
			// get structure name
			$name = $module ['name'] [CMS::GetCMSLang ()];
			$slected = ($selectedModule && $selectedModule == $key) ? "selected='selected'" : '';
			print ( "<option value='$key'{$slected}>$name</option>\n" );
		}
		print ( '</select>' );
		print ( '</div>
		<div class="titleColR"><input name="" type="submit" value="Done" /></div>
                <br class="clearFloat" />
            </div>' );
		
		// display tables
		print ( '<div class="someTableArea" id="moduleTablesDiv" name="moduleTablesDiv">' );
		
		if ($selectedModule != null)
		{
			print $this->GetModuleTablesCode ( $selectedModule, $userTypeID );
		}
		print ( '</div>' );
		
		print ( '</form><!--//Tables Access-->' );
	}
	
	/**
	 * This function displays interfase to change user privilegies on module tables.
	 *
	 */
	function DisplayFieldAccess()
	{
		$userTypeID = $this->m_usertype->GetID ();
		if ($userTypeID == CMS::GetUser ()->GetUserTypeID ())
		{
			return;
		}
		
		$selectedModule = DB::REQUEST ( 'selectedModule' );
		$selectedTable = DB::REQUEST ( 'selectedTable' );
		
		$structure = CMSObject::$CMS->GetStructure ();
		
		print ( '<!--Tables Accesst-->' );
		print ( '<div class="someGTitleBox">Fields Access</div>' );
		$this->DisplayFormHeadr ( 'changeAccessesField', 'fieldForm' );
		$this->DisplayHidden ( 'usertype', $userTypeID );
		print ( '<div class="titleBox">
                <div class="titleColL">
                	<label>Choose Modul: </label>' );
		
		$ajaxS = " onchange=\"xajax_callAjax('usertype={$userTypeID}&action=getModuleTablesSelect&module=' + this.value, 'moduleTablesFieldDiv', 'innerHTML', 'tableFieldsDiv');\"";
		print ( "<select name=\"selectedModule\" id=\"selectedModule\" $ajaxS>\n" );
		$slected = (! $selectedModule) ? " selected='selected'" : '';
		print ( "<option{$slected}>...</option>\n" );
		foreach ( $structure as $key => $module )
		{
			// if parent does not have access than chiled olso coulnd not have access
			if (! self::HasModuleAccess ( $key, $userTypeID ))
			{
				continue;
			}
			
			// get structure name
			$name = $module ['name'] [CMS::GetCMSLang ()];
			$slected = ($selectedModule && $selectedModule == $key) ? "selected='selected'" : '';
			print ( "<option value='$key'{$slected}>$name</option>\n" );
		}
		print ( '</select>' );
		
		// display tables select
		print ( '<div id="moduleTablesFieldDiv">' );
		if ($selectedModule != null)
		{
			print $this->GetModuleTablesSelect ( $selectedModule, $userTypeID, $selectedTable );
		}
		print ( '</div>' );
		
		print ( '</div>
		<div class="titleColR"><input name="" type="submit" value="Done" /></div>
                <br class="clearFloat" />
            </div>' );
		
		// display tables
		print ( '<div class="someTableArea" id="tableFieldsDiv" name="tableFieldsDiv">' );
		
		if ($selectedTable != null)
		{
			print $this->GetTableFieldsCode ( $selectedTable, $userTypeID );
		}
		print ( '</div>' );
		
		print ( '</form><!--//Tables Access-->' );
	}
	/**
	 * Enter description here...
	 *
	 * @param string $module
	 * @return string
	 */
	protected function GetModuleTablesCode($module = null, $userTypeID = null)
	{
		// get module main class
		$mainModule = CMSObject::$CMS->GetMudlueMainClass ( $module );
		
		// get CMS object tables
		$tables = $mainModule->GetObjectTables ();
		
		// result
		if (! $tables)
		{
			return '';
		}
		
		$res = '<div class="tableEven">';
		$res .= '<div class="tableFieldTitle"><strong>Tables</strong></a></div>';
		$res .= $this->DisplayGroupCheck ( 'Show', 'status_read[]', 'tableForm' );
		$res .= $this->DisplayGroupCheck ( 'Edit', 'status_write[]', 'tableForm' );
		$res .= $this->DisplayGroupCheck ( 'Insert', 'status_insert[]', 'tableForm' );
		$res .= $this->DisplayGroupCheck ( 'Del', 'status_delete[]', 'tableForm' );
		$res .= $this->DisplayGroupCheck ( 'uShow', 'status_owner_read[]', 'tableForm' );
		$res .= $this->DisplayGroupCheck ( 'uEdit', 'status_owner_write[]', 'tableForm' );
		$res .= $this->DisplayGroupCheck ( 'uDel', 'status_owner_delete[]', 'tableForm' );
		$res .= '</div>';
		
		if ($tables)
		{
			// retrive CMS current user type ID 
			if (! $userTypeID)
			{
				$userTypeID = CMS::GetUser ()->GetUserTypeID ();
			}
			
			$currentType = new UserType ( $userTypeID );
			$parentID = $currentType->GetParentID ();
			
			$index = 0;
			
			foreach ( $tables as $table )
			{
				$isRead = $isDelete = $isUpdate = $isInsert = true;
				$isReadC = $isDeleteC = $isUpdateC = $isInsertC = '';
				if (self::HasController ())
				{
					// common current type
					$isRead = $this->IsReadable ( $table, null, $userTypeID );
					$isDelete = $this->IsDeletable ( $table, $userTypeID );
					$isUpdate = $this->IsUpdatable ( $table, null, $userTypeID );
					$isInsert = $this->IsInsertable ( $table, $userTypeID );
					
					// common parent
					$isParRead = $this->IsReadable ( $table, null, $parentID );
					$isParDelete = $this->IsDeletable ( $table, $parentID );
					$isParUpdate = $this->IsUpdatable ( $table, null, $parentID );
					$isParInsert = $this->IsInsertable ( $table, $parentID );
					
					// owner
					$isOwRead = $this->IsReadableByOwner ( $table, null, $userTypeID );
					$isOwDelete = $this->IsDeletableByOwner ( $table, $userTypeID );
					$isOwUpdate = $this->IsUpdatableByOwner ( $table, null, $userTypeID );
					
					// owner parent
					$isOwParRead = $this->IsReadableByOwner ( $table, null, $parentID );
					$isOwParDelete = $this->IsDeletableByOwner ( $table, $parentID );
					$isOwParUpdate = $this->IsUpdatableByOwner ( $table, null, $parentID );
					
					// common part
					$isReadC = ($isRead && $isParRead) ? "checked='checked' " : '';
					$isDeleteC = ($isDelete && $isParDelete) ? "checked='checked' " : '';
					$isUpdateC = ($isUpdate && $isParUpdate) ? "checked='checked' " : '';
					$isInsertC = ($isInsert && $isParInsert) ? "checked='checked' " : '';
					
					$isReadDisb = (! $isParRead) ? " disabled='disabled' " : '';
					$isDeleteDisb = (! $isParDelete) ? " disabled='disabled' " : '';
					$isUpdateDisb = (! $isParUpdate) ? " disabled='disabled' " : '';
					$isInsertDisb = (! $isParInsert) ? " disabled='disabled' " : '';
					
					// disable owner part
					$hasUserField = $this->HasTableUserField ( $table );
					
					// owner part
					$isOwReadC = ($hasUserField && $isOwRead && $isOwParRead) ? "checked='checked' " : '';
					$isOwDeleteC = ($hasUserField && $isOwDelete && $isOwParDelete) ? "checked='checked' " : '';
					$isOwUpdateC = ($hasUserField && $isOwUpdate && $isOwParUpdate) ? "checked='checked' " : '';
					
					$owPartReadDisb = (! $hasUserField || ! $isOwParRead) ? " disabled='disabled' " : '';
					$owPartDeleteDisb = (! $hasUserField || ! $isOwParDelete) ? " disabled='disabled' " : '';
					$owPartUpdateDisb = (! $hasUserField || ! $isOwParUpdate) ? " disabled='disabled' " : '';
				
				}
				
				$style = (($index ++) % 2 == 0) ? 'tableUneven' : 'tableEven';
				$res .= "<div class='$style'>\n";
				$res .= "<div class='tableFieldTitle'>$table</a></div>\n";
				// common
				$res .= "<div class='tableFieldCHB'><input name='status_read[]' id='status_read[]' type='checkbox' value='$table' {$isReadC}{$isReadDisb}/></div>\n";
				$res .= "<div class='tableFieldCHB'><input name='status_write[]' id='status_write[]' type='checkbox' value='$table' $isUpdateC{$isUpdateDisb}/></div>\n";
				$res .= "<div class='tableFieldCHB'><input name='status_insert[]' id='status_insert[]' type='checkbox' value='$table' $isInsertC{$isInsertDisb}/></div>\n";
				$res .= "<div class='tableFieldCHB'><input name='status_delete[]' id='status_delete[]' type='checkbox' value='$table' $isDeleteC{$isDeleteDisb}/></div>\n";
				// owner
				$res .= "<div class='tableFieldCHB'><input name='status_owner_read[]' id='status_owner_read[]' type='checkbox' value='$table' $isOwReadC{$owPartReadDisb}/></div>\n";
				$res .= "<div class='tableFieldCHB'><input name='status_owner_write[]' id='status_owner_write[]' type='checkbox' value='$table' $isOwUpdateC{$owPartUpdateDisb	}/></div>\n";
				$res .= "<div class='tableFieldCHB'><input name='status_owner_delete[]' id='status_owner_delete[]' type='checkbox' value='$table' $isOwDeleteC{$owPartDeleteDisb}/></div>\n";
				$res .= "</div>\n";
				$res .= "<br class='clearFloat' />\n\n";
			}
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $module
	 * @return string
	 */
	protected function GetModuleTablesSelect($module = null, $userTypeID = null, $selectedTable = null)
	{
		// get module main class
		$mainModule = CMSObject::$CMS->GetMudlueMainClass ( $module );
		
		// get CMS object tables
		$tables = $mainModule->GetObjectTables ();
		
		$res = '';
		
		if ($tables)
		{
			$res = '<p>Choose Table: ';
			$ajaxS = " onchange=\"xajax_callAjax('usertype={$userTypeID}&action=getModuleTablesFields&table=' + this.value, 'tableFieldsDiv', 'innerHTML');\"";
			$res .= "<select name='selectedTable'{$ajaxS}>";
			$empSelec = ($selectedTable) ? '' : " selected='selected'";
			$res .= "<option{$empSelec}>...</option>";
			foreach ( $tables as $table )
			{
				$tebSelec = ($table == $selectedTable) ? " selected='selected'" : '';
				$res .= "<option{$tebSelec} value='$table'>$table</option>";
			}
			$res .= '</select></p>';
		}
		
		return $res;
	}
	/**
	 * Enter description here...
	 *
	 * @param string $table
	 * @return string
	 */
	protected function GetTableFieldsCode($table = null, $userTypeID = null)
	{
		// result
		$res = '<div class="tableEven">';
		$res .= '<div class="tableFieldTitle"><strong>Fields</strong></a></div>';
		$res .= $this->DisplayGroupCheck ( 'Show', 'status_read[]', 'fieldForm' );
		$res .= $this->DisplayGroupCheck ( 'Edit', 'status_write[]', 'fieldForm' );
		$res .= $this->DisplayGroupCheck ( 'uShow', 'status_owner_read[]', 'fieldForm' );
		$res .= $this->DisplayGroupCheck ( 'uEdit', 'status_owner_write[]', 'fieldForm' );
		$res .= '</div>';
		
		// get table fields
		$fields = DBxp::GetFieldNamesOutLang ( $table );
		if ($fields)
		{
			// retrive CMS current user type ID 
			if (! $userTypeID)
			{
				$userTypeID = CMS::GetUser ()->GetUserTypeID ();
			}
			
			$currentType = new UserType ( $userTypeID );
			$parentID = $currentType->GetParentID ();
			
			$index = 0;
			
			// common current type
			$isTableRead = $this->IsReadable ( $table, null, $userTypeID );
			$isTableUpdate = $this->IsUpdatable ( $table, null, $userTypeID );
			
			$isTableOwRead = $this->IsReadableByOwner ( $table, null, $userTypeID );
			$isTableOwUpdate = $this->IsUpdatableByOwner ( $table, null, $userTypeID );
			
			foreach ( $fields as $field )
			{
				$isRead = $isUpdate = true;
				$isReadC = $isUpdateC = '';
				
				if (self::HasController ())
				{
					// disable owner part
					$hasUserField = $this->HasTableUserField ( $table );
					
					// common current type
					$isRead = $this->IsReadable ( $table, $field, $userTypeID );
					$isUpdate = $this->IsUpdatable ( $table, $field, $userTypeID );
					
					// common parent
					$isParRead = $this->IsReadable ( $table, $field, $parentID );
					$isParUpdate = $this->IsUpdatable ( $table, $field, $parentID );
					
					// owner
					$isOwRead = $this->IsReadableByOwner ( $table, $field, $userTypeID );
					$isOwUpdate = $this->IsUpdatableByOwner ( $table, $field, $userTypeID );
					
					// owner parent
					$isOwParRead = $this->IsReadableByOwner ( $table, $field, $parentID );
					$isOwParUpdate = $this->IsUpdatableByOwner ( $table, $field, $parentID );
					
					// common part
					$isReadC = ($isRead && $isTableRead) ? "checked='checked' " : '';
					$isUpdateC = ($isUpdate && $isTableUpdate) ? "checked='checked' " : '';
					
					$isReadDisb = (! $isParRead || ! $isTableRead) ? " disabled='disabled' " : '';
					$isUpdateDisb = (! $isParUpdate || ! $isTableUpdate) ? " disabled='disabled' " : '';
					
					// owner part
					$isOwReadC = ($hasUserField && $isOwRead && $isOwParRead) ? "checked='checked' " : '';
					$isOwUpdateC = ($hasUserField && $isOwUpdate && $isOwParUpdate) ? "checked='checked' " : '';
					
					$owPartReadDisb = (! ($hasUserField && $isOwParRead && $isTableOwRead)) ? " disabled='disabled' " : '';
					$owPartUpdateDisb = (! $hasUserField || ! $isOwParUpdate || ! $isTableOwUpdate) ? " disabled='disabled' " : '';
				}
				
				$style = (($index ++) % 2 == 0) ? 'tableUneven' : 'tableEven';
				$res .= "<div class='$style'>\n";
				$res .= "<div class='tableFieldTitle'>$field</a></div>\n";
				// common
				$res .= "<div class='tableFieldCHB'><input name='status_read[]' id='status_read[]' type='checkbox' value='$field' {$isReadC}{$isReadDisb}/></div>\n";
				$res .= "<div class='tableFieldCHB'><input name='status_write[]' id='status_write[]' type='checkbox' value='$field' $isUpdateC{$isUpdateDisb}/></div>\n";
				// owner
				$res .= "<div class='tableFieldCHB'><input name='status_owner_read[]' id='status_owner_read[]' type='checkbox' value='$field' $isOwReadC{$owPartReadDisb}/></div>\n";
				$res .= "<div class='tableFieldCHB'><input name='status_owner_write[]' id='status_owner_write[]' type='checkbox' value='$field' $isOwUpdateC{$owPartUpdateDisb	}/></div>\n";
				$res .= "</div>\n";
				$res .= "<br class='clearFloat' />\n\n";
			}
		}
		
		return $res;
	}
	
	/**
	 * Main ajax function 
	 *
	 * @param mixed $arg
	 * @return string
	 */
	function GetAjax($arg=null)
	{
		// pripear result
		$res = null;
		
		// retrive perimeters from encoded argument
		$params = parent::RetriveAjaxParams ( $arg );
		// switch action
		
		// main object to manipulate zone, street and map
		switch ($params ["action"])
		{
			case 'getModuleTables' :
				$res = $this->GetModuleTablesCode ( $params ['module'], $this->m_usertype->GetID () );
				break;
			case 'getModuleTablesSelect' :
				$res = $this->GetModuleTablesSelect ( $params ['module'], $this->m_usertype->GetID () );
				break;
			case 'getModuleTablesFields' :
				$res = $this->GetTableFieldsCode ( $params ['table'], $this->m_usertype->GetID () );
				break;
		}
		
		return $res;
	}
	
	/**
	 * this function displays name with + and - sign to chache all or nan from check boxes
	 *
	 * @param string $name
	 * @param string $id
	 * @param string $formName
	 * @return string
	 */
	protected function DisplayGroupCheck($name, $id, $formName)
	{
		$res = '<div class="tableFieldOptions">';
		$res .= "<a href=\"#\" onclick=\"chekeBoxes('{$formName}', '{$id}', true); return false\">+</a>";
		$res .= "<strong>$name</strong>";
		$res .= "<a href=\"#\" onclick=\"chekeBoxes('{$formName}', '{$id}', false); return false\">-</a>";
		$res .= '</div>';
		return $res;
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return array
	 */
	function CreateDBSQL()
	{
		$sql['module'] = 'CREATE TABLE `module_access` (
				  `ID` int(10) unsigned NOT NULL auto_increment,
				  `module` varchar(32) NOT NULL,
				  `userTypeID` tinyint(4) NOT NULL,
				  `access` tinyint(4) NOT NULL default "0",
				  PRIMARY KEY  (`ID`),
				  UNIQUE KEY `module` (`module`,`userTypeID`))';
		
		
		$sql['table'] = 'CREATE TABLE `table_access` (
		 `ID` smallint(5) unsigned NOT NULL auto_increment,
  		 `table` varchar(64) NOT NULL,
  		 `field` varchar(64) NOT NULL default "0",
  		 `userTypeID` tinyint(4) NOT NULL,
		 `status` int(10) unsigned NOT NULL,
		 PRIMARY KEY  (`ID`),
		 UNIQUE KEY `table` (`table`,`field`,`userTypeID`));';
		
		return $sql;
	}
}
?>