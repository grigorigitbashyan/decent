<?php
// inculdes
Site::IncludeFile ( 'classes/Box/BoxCollection.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxCollectionList.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxCollectionBox.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxCollectionBoxCMS.class.inc' );

Site::IncludeFile ( 'classes/Box/BoxCollectionCMS.class.inc' );

Site::IncludeFile ( 'classes/Box/SBRelation.class.inc' );

/**
 * Enter description here...
 * @package Box
 *
 */
class BoxCollectionListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var BoxCollectionList
	 */
	protected $m_boxCollectionList;
	
	function __construct()
	{
		parent::__construct ();
		
		$this->m_boxCollectionList = new BoxCollectionList ( );
		
		$this->ReadNameStrings ( 'classes/Box/langs/', 'box.xml' );
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
		
		$overLink = $this->m_boxCollectionList->GetLinkIDName ();
		
		$boxID = DB::POST ( $overLink );
		if (! $boxID)
		{
			$boxID = DB::GET ( $overLink );
		}
		
		if ($boxID)
		{
			$boxCollection = new BoxCollection ( $boxID );
			$bcb = new BoxCollectionBox ( $boxID );
		}
		
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				$name = DB::POST ( 'name' );
				if ($name)
				{
					if (strlen ( trim ( $name ) ) > 0)
					{
						// insert new box with name
						$newBox = new BoxCollection ( );
						$params = array ('name' => $name );
						$ar = $newBox->Insert ( $params );
					}
				}
				break;
			case 'delete' :
				if ($boxCollection)
				{
					// delete
					$ar = $boxCollection->Delete ();
				}
				break;
			case 'rename' :
				$newName = DB::POST ( 'name' );
				if ($boxCollection && $newName)
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $boxCollection->SetName ( $newName );
					}
				}
				break;
			case 'addRelation' :
				if ($bcb)
				{
					if (($newBoxes = DB::POST ( 'newbox' )))
					{
						foreach ( $newBoxes as $ar )
						{
							$bcb->AddBox ( $ar );
						}
					}
				}
				break;
			case 'deleteRelation' :
				if ($bcb && ($relboxes = DB::POST ('relbox')))
				{
					foreach ( $relboxes as $ar )
					{
						$bcb->DeleteBox ( $ar );
					}
				}
				break;
			case 'reposition' :
				$nrePoss = DB::GET('recid');
				if ($bcb && $nrePoss && isset ( $_GET ['recid'] ))
				{
					$direction = ($_GET ['direction'] == 'up') ? true : false;
					$bcb->Reposition ( $nrePoss, $direction );
				}
				break;
		}
		
		// if there is a any action then update object  
		if ($ar)
		{
			$this->__construct ();
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
		$currID = $this->m_boxCollectionList->GetCurrentID ();
		
		$boxCollectionS = $this->GetNameString ( 'boxCollection' );
		
		if ($currID)
		{
			$linkName = $this->m_boxCollectionList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>$boxCollectionS</a></div>" );
		}
		else
		{
			print ( "<div class='someWTitleBox'>$boxCollectionS</div>" );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_boxCollectionList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		/////////////////////////////////
		$currentBoxID = $this->m_boxCollectionList->GetCurrentID ();
		
		// get names
		$doneS = $this->GetNameString ( 'done' );
		$deleteS = $this->GetNameString ( 'delete' );
		$renameS = $this->GetNameString ( 'rename' );
		$selectS = $this->GetNameString ( 'select' );
		$allS = $this->GetNameString ( 'all' );
		$noneS = $this->GetNameString ( 'none' );
		$noEmptyS = $this->GetNameString ( 'noempty' );
		
		// get cooretn box
		$boxCollection = new BoxCollection ( $currentBoxID );
		
		$currentBoxID = $boxCollection->GetID ();
		// if is selected box, then diplay box features
		if ($currentBoxID)
		{
			$boxCollectionName = $boxCollection->GetName ();
			
			$overLink = $this->m_boxCollectionList->GetLinkIDName ();
			//  costruct delete form
			$className = $this->GetFormActionName ();
			
			$hasRelation = SBRelation::IsBoxCollectionRelated ( $currentBoxID );
			
			$delForm = null;
			
			if (! $hasRelation)
			{
				// delete form javascript
				$sureDeleteS = $this->GetNameString ( 'sureDelete' );
				$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
				
				$delForm = "<div  class='titleColR'>
	    					<form method='POST' $dJS>
	    					<input type='hidden' name='$className' value='delete'>
	    					<input type='hidden' name='$overLink' value='$currentBoxID'>
	    					<input type='submit' value='$deleteS'>
	    					</form>
	    					</div>";
			}
			// display form title
			$this->DisplayTitle ( $boxCollectionName, $delForm );
			
			// diplay rename form
			$renameJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
			$this->DisplaySingleTextForm ( 'rename', $renameS, 'name', $boxCollectionName, array ($overLink => $currentBoxID ), $doneS, $renameJS );
			
			// display related box to box collection ///////////////////////////////////
			// get all boxxis
			$boxList = new BoxList ( );
			
			// display all related boxes
			$bcb = new BoxCollectionBox ( $currentBoxID );
			$relboxes = $bcb->GetRelatedBoxList ();
			
			$relboxesCount = count ( $relboxes );
			// END display related box to box collection ///////////////////////////////
			

			// box features in tab ////////////////////////////////////////////////////
			// get current ID of box types
			$seleCatID = 1;
			
			// get content types link ID name for get overloading
			$collBoxesTitleS = $this->GetNameString ( 'collBoxesTitle' );
			$relatedBoxesS = $this->GetNameString ( 'relatedBoxes' );
			
			print ( "<div class='someGTitleBox'>$collBoxesTitleS</div>" );
			
			?>
<div class="someGBox">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
	<tr>
		<td>
		<div id="TabbedPanels1" class="TabbedPanels">
		<ul class="TabbedPanelsTabGroup">
			<li class="TabbedPanelsTab" tabindex="0"><?php echo $relatedBoxesS?></li>
            <?php
			if ($seleCatID)
			{
				$allBoxesS = $this->GetNameString ( 'allBoxes' );
				// if content types does not selected then skeek free and used tabs
				print ( '<li class="TabbedPanelsTab" tabindex="0">' . $allBoxesS . '</li>' );
			}
			?>
                      </ul>
		<div class="TabbedPanelsContentGroup">
		<div class="TabbedPanelsContent">
                        <?php
			$this->DisplayFormHeadr ( 'deleteRelation', 'relboxes' );
			$this->DisplayHidden ( $overLink, $currentBoxID );
			?>
           <table width="100%" border="0" cellspacing="0"
			cellpadding="0" class="titleBox">
			<tr>
				<td>
				<div class="titleColR">
            <?php
			if ($relboxesCount)
			{
				$deleteRelationS = $this->GetNameString ( 'deleteRelation' );
				
				print ( '<span style="padding-right:20px">' );
				print ( "<strong>$selectS</strong>" );
				print ( '<a href="#" onclick="chekeBoxes(\'relboxes\', \'relbox[]\', true); return false">' . $allS . '</a>,' );
				print ( '<a href="#" onclick="chekeBoxes(\'relboxes\', \'relbox[]\', false); return false">' . $noneS . '</a>' );
				print ( '</span>' );
				print ( '<input name="" type="submit" value="' . $deleteRelationS . '" /></div>' );
			
			}
			?>
				<br class="clearFloat" />
				
				</td>
			</tr>
		</table>
		<table width="100%" border="0" cellspacing="0" cellpadding="0"
			style="border-left: 1px solid #777777; border-right: 1px solid #777777">
			<tr>
				<td>
            <?php
			//display related boxes
			if ($relboxesCount)
			{
				$index = 1;
				
				foreach ( $relboxes as $currBox )
				{
					$recID = $currBox ['ID'];
					$currBoxID = $currBox ['boxID'];
					$currBoxObj = new Box ( $currBoxID );
					
					////
					$bstyle = ($index % 2 == 0) ? 'tableUneven' : 'tableEven';
					print ( "<div class='$bstyle'>" );
					
					print ( '<div class="tableCHB">' );
					print ( "<input name='relbox[]' id='relbox[]' type='checkbox' value='$recID' />" );
					print ( '</div>' );
					
					// display name
					print ( '<div class="tableT">' );
					print ( $currBoxObj->GetName () );
					print ( '</div>' );
					
					print ( '<div class="tableUD">' );
					if ($index != 1) // displau to up button
					{
						print ( " <a href='?$className=reposition&recid=$recID&$overLink=$currentBoxID&direction=up'>&uarr;</a> " );
					}
					else
					{
						print ( '&nbsp;&nbsp;&nbsp;' );
					}
					print ( '</div>' );
					
					print ( '<div class="tableUD">' );
					if ($index != $relboxesCount) // displau to down button
					{
						print ( " <a href='?$className=reposition&recid=$recID&$overLink=$currentBoxID&direction=down'>&darr;</a> " );
					}
					else
					{
						print ( '&nbsp;&nbsp;&nbsp;' );
					}
					print ( '</div' );
					
					$index ++;
					
					print ( '</div>' );
					print ( '<br class="clearFloat" />' );
				}
			}
			?>
               </td>
			</tr>
		</table>
		</form>
		</div>
            <?php
			if ($seleCatID)
			{
				print ( '<div class="TabbedPanelsContent">' );
				
				$this->DisplayFormHeadr ( 'addRelation', 'boxes' );
				$this->DisplayHidden ( $overLink, $currentBoxID );
				?>
		<table width="100%" border="0" cellspacing="0" cellpadding="0"
			class="titleBox">
			<tr>
				<td>
				<div class="titleColR">
               <?php
				$addRelationS = $this->GetNameString ( 'addRelation' );
				
				print ( '<span style="padding-right:20px">' );
				print ( "<strong>$selectS</strong>" );
				print ( '<a href="#" onclick="chekeBoxes(\'boxes\', \'newbox[]\', true); return false">' . $allS . '</a>,' );
				print ( '<a href="#" onclick="chekeBoxes(\'boxes\', \'newbox[]\', false); return false">' . $noneS . '</a>' );
				print ( '</span>' );
				?>
              <input name="" type="submit" value="<?php echo $addRelationS?>" /></div>
				<br class="clearFloat" />
				</td>
			</tr>
		</table>
		<table width="100%" border="0" cellspacing="0" cellpadding="0"
			style="border-left: 1px solid #777777; border-right: 1px solid #777777">
			<tr>
				<td>
                <?php
				$patern = '<div class="tableCHB">
							<input name="newbox[]" id="newbox[]" type="checkbox" value="@@ID@@" />
						   </div>
	                       <div class="tableT">@@name@@</div>
		                   <br class="clearFloat" />';
				
				$boxList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
				?>
                </td>
			</tr>
		</table>
		</form>
		</div>
		</div>
		</div>
           <?php
			}
			?>
        </td>
	</tr>
</table>
</div>
<script type="text/javascript">
		<!--
		var TabbedPanels1 = new Spry.Widget.TabbedPanels("TabbedPanels1");
		//-->
		</script>
<?php
			// END box features in tab
		} // diplay new box form
		else
		{
			$newboxCollectionS = $this->GetNameString ( 'newboxCollection' );
			$newboxCollNameS = $this->GetNameString ( 'newboxCollName' );
			$doneS = $this->GetNameString ( 'done' );
			
			// display form title
			$this->DisplayTitle ( $newboxCollectionS );
			
			// display seleted form features
			$insertJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
			$this->DisplaySingleTextForm ( 'insert', $newboxCollNameS, 'name', null, null, $doneS, $insertJS );
			// end new box form
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$bcc = new BoxCollectionCMS ( );
		$res = $bcc->CreateDB ();
		
		$bcb = new BoxCollectionBox ( );
		$res1 = $bcb->CreateDB ();
		
		return ($res && $res1);
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$bcc = new BoxCollectionCMS ( );
		$res = $bcc->SelfTest ();
		
		$bcb = new BoxCollectionBoxCMS ( );
		$res &= $bcb->SelfTest ();
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		$tables = array();
		
		$bcc = new BoxCollectionCMS ( );
		$resBCC = $bcc->GetObjectTables();
		
		
		$bcb = new BoxCollectionBoxCMS ( );
		$resBCBC = $bcb->SelfTest ();
		
		return array_push($tables, $resBCC, $resBCBC);
	}
}
?>