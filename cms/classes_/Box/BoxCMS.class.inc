<?php
// inculdes
Site::IncludeFile ( 'classes/Box/Box.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxList.class.inc' );

Site::IncludeFile ( 'classes/Content/ContentTypeCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package Box
 */
class BoxCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Box
	 */
	protected $m_box;
	
	function __construct()
	{
		parent::__construct ();
		
		$ID = DB::REQUEST('boxID' );
		
		$this->m_box = new Box ( $ID );
		
		$this->ReadNameStrings ( 'classes/Box/langs/', 'box.xml' );
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
			case 'rename' : // rename box
				$name = DB::POST ( 'name' );
				if ($name)
				{
					$newName = trim ( $name );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_box->SetName ( $newName );
					}
				}
				break;
			case 'addRelation' : // also delete, if paramValue is equal 0, ic comes from other class
				$paramValue = DB::POST ( 'paramValue' );
				if ($paramValue !== null)
				{
					// chakce if it is a case when is creting new content
					if ($paramValue == 0)
					{
						$contTypeCMS = new ContentTypeCMS ( );
						$newParamVal = $contTypeCMS->DoAction ();
						
						if (is_int ( intval ( $newParamVal ) ) && $newParamVal > 1)
						{
							$paramValue = $newParamVal;
						}
					}
					// rename
					$ar = $this->m_box->SetParam ( $paramValue );
				}
				break;
			case 'delete' :
				$ar = $this->m_box->Delete ();
				break;
		}
		
		// if there is a any action then update box  
		//				if ($ar)
		//				{
		//					// save
		//				}
		

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
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `box` (
				  `ID` int(11) NOT NULL auto_increment,
				  `name` varchar(255) NOT NULL default '',
				  `type` smallint(6) NOT NULL default '1',
				  `param` varchar(255) NOT NULL default '',
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		$ID = $this->m_box->GetID ();
		$boxName = $this->m_box->GetName ();
		
		// names
		$deleteS = $this->GetNameString ( 'delete' );
		$doneS = $this->GetNameString ( 'done' );
		$renameS = $this->GetNameString ( 'rename' );
		$noEmptyS = $this->GetNameString ( 'noempty' );
		
		/////////////////////////////////////////
		$overLink = 'boxID';
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		
		// if there is a any relation between box and box collection then 
		// do not allow to delete box
		$hasRelation = BoxCollectionBox::IsBoxRelated ( $ID );
		
		$delForm = null;
		
		if (! $hasRelation)
		{
			// delete form javascript
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
			
			// delete form 
			$delForm = "<div  class='titleColR'>
    					<form method='POST' $dJS>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='boxID' value='$ID'>
    					<input type='submit' value='$deleteS'>
    					</form>
    					</div>";
		}
		
		// display form title
		$this->DisplayTitle ( $boxName, $delForm );
		
		// diplay rename form
		$insertJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
		$this->DisplaySingleTextForm ( 'rename', $renameS, 'name', $boxName, array ($overLink => $ID ), $doneS, $insertJS );
		
		// display box relation feature
		$boxType = $this->m_box->GetType ();
		$paramID = $this->m_box->GetParam ();
		$boxID = $this->m_box->GetID ();
		switch ($boxType)
		{
			case 1 : // content
				//relate content
				

				$actionName = $this->GetFormActionName ();
				
				$contTypelist = new ContentTypeCMS ( );
				$contTypelist->DispalayContentRelation ( $paramID, 'boxID', $boxID, $actionName );
				break;
			case 2 :
				$relFileS = $this->GetNameString ( 'relFile' );
				// get all files
				// get all files from files folder
				$files = scandir ( '../files' );
				
				$firstOptions = "<option value='0'> - - - </option>";
				
				foreach ( $files as $file )
				{
					if ($file != '.' && $file != '..')
					{
						if ($file == $paramID)
						{
							$firstOptions .= "<option value='$file' selected>$file</option>\n";
						}
						else
						{
							$firstOptions .= "<option value='$file'>$file</option>\n";
						}
					}
				}
				
				//display all files
				print ( '<br />' );
				
				$this->DisplayFormHeadr ( 'addRelation' );
				$this->DisplayHidden ( 'boxID', $boxID );
				print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          <tr>
			            <td>
			                <div class="titleColL">' );
				print ( "<label><strong>$relFileS</strong></label>\n" );
				print ( '<select name="paramValue">' );
				print ( $firstOptions );
				print ( '</select>' );
				print ( '</div><div class="titleColR"><input name="" type="submit" value="' . $doneS . '" /></div>
			                <br class="clearFloat" />
			        	</td>
			          </tr>
			        </table>
			        </form>' );
				
				break;
		}
	
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function Delete()
	{
		return $this->m_box->Delete ();
	}
}
?>