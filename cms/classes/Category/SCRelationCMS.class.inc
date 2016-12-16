<?php
// includes
Site::IncludeFile ( 'classes/Category/SCRelation.class.inc' );
Site::IncludeFile ( 'classes/Content/Content.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentTypeList.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentListView.class.inc' );

/**
 * Enter description here...
 *
 * @package Structure
 */
class SCRelationCMS extends CMSObject
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->ReadNameStrings ( 'classes/Category/langs/', 'category.xml' );
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		// create structure-content table
		$sql = "CREATE TABLE `structure_content` (
                  `structureID` smallint(5) unsigned NOT NULL,
                  `contentID` smallint(5) unsigned NOT NULL,
                  PRIMARY KEY  (`structureID`,`contentID`)
                )";
		
		return $sql;
	}
	
	/**
	 * Draw fitures as add relation or delete
	 *
	 * @param int $catID
	 */
	function DisplayFeatures($catID, $lang)
	{
		// get current category
		$category = new Category ( $catID );
		$singOrMulti = $category->GetRelContentCount ();
		
		// if category does not support relation to content then return
		if (! ($singOrMulti == 1 || $singOrMulti == 2))
			return;
			
		// get structure to content relation object
		$scr = new SCRelation ( );
		$relIDs = $scr->GetCatRelations ( $catID, $lang );
		
		// get content type list object
		$catTypes = new ContentTypeList ( );
		
		// get current ID of Content types
		$seleCatID = $catTypes->GetCurrentID ();
		
		// get content types link ID name for get overloading
		$ctLinkName = $catTypes->GetLinkIDName ();
		$ctLinkName = "sub" . $ctLinkName;
		
		// get names
		$contentRelationS = $this->GetNameString ( 'contentRelation' );
		$relatedContentS = $this->GetNameString ( 'relatedContent' );
		$createRelateS = $this->GetNameString ( 'createRelate' );
		$selectDirectoryS = $this->GetNameString ( 'selectDirectory' );
		$doneS = $this->GetNameString ( 'done' );
		
		////////////////////////////////////////////////////////////////////////////
		

		// lists for userd and free
		$freeContentList = null;
		$freeCount = 0;
		
		$usedContentList = null;
		$userCount = 0;
		
		if ($seleCatID)
		{
			$freeContentList = new ContentListView ( $seleCatID, null, 2 );
			$freeCount = $freeContentList->GetItemCount ();
			
			$usedContentList = new ContentListView ( $seleCatID, null, 1 );
			$userCount = $usedContentList->GetItemCount ();
		}
		
		print ( "<div class='someGTitleBox'>$contentRelationS</div>" );
		?>
<div class="someGBox">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
	<tr>
		<td>
		<form action="" method="POST" class="clearPD"><label><?php echo $selectDirectoryS?></label>
                      <?php
		print ( "<imput type='hidden' name='catID' value='$catID'>" );
		print ( "<select name='contentTypeID' onchange='submit();'>" );
		$catTypes->DisplaySelect ( "---" );
		?>
		</select></form>
		</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td>
		<div id="TabbedPanels1" class="TabbedPanels">
		<ul class="TabbedPanelsTabGroup">
                        <?php
		// if the category has relation then display it's tab
		if ($relIDs)
		{
			print ( "<li class='TabbedPanelsTab' tabindex='0'>$relatedContentS</li>" );
		}
		elseif ($seleCatID)
		{
			print ( "<li class='TabbedPanelsTab' tabindex='0'>$createRelateS</li>" );
		}
		
		$freeContentS = $this->GetNameString ( 'freeContents' );
		$usedContentsS = $this->GetNameString ( 'usedContents' );
		
		// display two tabs if the section has been selected
		if ($seleCatID)
		{
			// if content types does not selected then skeek free and used tabs
			if ($freeCount)
			{
				print ( "<li class='TabbedPanelsTab' tabindex='0'>$freeContentS</li>" );
			}
			
			if ($userCount)
			{
				print ( "<li class='TabbedPanelsTab' tabindex='0'>$usedContentsS</li>" );
			}
		}
		
		//
		print ( '</ul>' );
		
		if ($seleCatID || $relIDs)
		{
			print ( '<div class="TabbedPanelsContentGroup">
					<div class="TabbedPanelsContent">' );
		}
		
		// if there are related contents then display form for deleting relations
		if ($relIDs)
		{
			$this->DisplayFormHeadr ( 'delete', 'related' );
		}
		
		print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox"><tr><td>' );
		
		$deleteRelationS = $this->GetNameString ( 'deleteRelation' );
		$selectS = $this->GetNameString ( 'select' );
		
		$allS = $this->GetNameString ( 'all' );
		$noneS = $this->GetNameString ( 'none' );
		
		// display related contents
		if ($relIDs)
		{
			print ( '<div class="titleColR">' );
			print ( '<span style="padding-right:20px">' );
			print ( '<strong>' . $selectS . '</strong> ' );
			print ( '<a href="#" onclick="chekeBoxes(\'related\', \'contIDs[]\', true); return false;">' . $allS . '</a>, ' );
			print ( '<a href="#" onclick="chekeBoxes(\'related\', \'contIDs[]\', false); return false;">' . $noneS . '</a>,' );
			print ( '</span>' );
			print ( '<input name="" type="submit" value="' . $deleteRelationS . '" /></div>' );
		
		}
		
		//display abibility to create and relate new content
		elseif ($seleCatID)
		{
			$noEmptyS = $this->GetNameString ( 'noempty' );
			$renameJS = "onsubmit='return SingleForm(\"contentName\", \"$noEmptyS\");'";
			// display form
			$this->DisplayFormHeadr ( 'create', 'createForm', 'post', $renameJS );
			$this->DisplayHidden ( 'contentTypeID', $seleCatID );
			print ( '<div class="titleColL">' );
			print ( "<input type='text' value='' name='contentName' id='contentName' size='64' maxlength='256' />\n" );
			print ( '</div>' );
			print ( '<div class="titleColR">' );
			$this->DisplayFormFooter ( $doneS );
			print ( '</div>' );
		}
		
		print ( '<br class="clearFloat" /></td></tr></table>' );
		
		//display related contents
		if ($relIDs)
		{
			print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" style="border-left: 1px solid #777777; border-right: 1px solid #777777"><tr><td>' );
			
			$styleInd = 0;
			foreach ( $relIDs as $relCont )
			{
				$styleing = (($styleInd ++) % 2 == 0) ? 'tableUneven' : 'tableEven';
				print ( "<div class='$styleing'>" );
				print ( '<div class="tableCHB">' );
				print ( "<input name='contIDs[]' id='contIDs[]' type='checkbox' value='{$relCont['ID']}' />" );
				print ( '</div>' );
				print ( '<div class="tableT">' );
				print ( "<a href='?structure=content&{$ctLinkName}={$relCont['purpose']}&contID={$relCont['ID']}'>{$relCont['title']}</a>" );
				print ( '</div>' );
				print ( "<div class='tableD'>{$relCont['date']}</div>" );
				print ( '</div>' );
				print ( '<br class="clearFloat" />' );
			}
			
			print ( '</td></tr></table></form>' );
		}
		
		print ( '</div>' );
		
		// display free contents
		if ($freeCount)
		{
			print ( '<div class="TabbedPanelsContent">' );
			
			// display form for adding content(s)
			$this->DisplayFormHeadr ( 'add', 'freeContents' );
			$this->DisplayHidden ( 'contentTypeID', $seleCatID );
			?>
			<table width="100%" border="0" cellspacing="0" cellpadding="0"
				class="titleBox">
				<tr>
					<td>
					<div class="titleColR">
				<?php
			if ($singOrMulti == 2)
			{
				print ( '<span style="padding-right:20px">' );
				print ( '<strong>Select:</strong> ' );
				print ( '<a href="#" onclick="chekeBoxes(\'freeContents\', \'contIDs[]\', true); return false">'.$allS.'</a>, ' );
				print ( '<a href="#" onclick="chekeBoxes(\'freeContents\', \'contIDs[]\', false); return false">'.$noneS.'</a>,' );
				print ( '</span>' );
			}
			?>
					<input name="" type="submit" value="Add Relation" /></div>
					<br class="clearFloat" />
					</td>
				</tr>
			</table>
			<table width="100%" border="0" cellspacing="0" cellpadding="0"
				style="border-left: 1px solid #777777; border-right: 1px solid #777777">
				<tr>
					<td>
	            <?php
			if ($singOrMulti == 1 || $singOrMulti == 2)
			{
				$patern = null;
				
				if ($singOrMulti == 1)
				{
					$patern = '<div class="tableCHB">
	                                   <input name="contIDs[]" id="contIDs[]" type="radio" value="@@ID@@" /></div>
	                                   <div class="tableT">
	                                   <a href="?structure=content&' . "$ctLinkName=$seleCatID" . '&contID=@@ID@@">@@title@@</a>
	                                   </div>
	                                   <div class="tableD">@@art_date@@</div>
	                               <br class="clearFloat" />';
				}
				elseif ($singOrMulti == 2)
				{
					$patern = '<div class="tableCHB">
										<input name="contIDs[]" id="contIDs[]" type="checkbox" value="@@ID@@" />
								    </div>
	                                <div class="tableT">
	                                     <a href="?structure=content&' . "$ctLinkName=$seleCatID" . '&contID=@@ID@@">@@title@@</a>
	                                 </div>
	                                 <div class="tableD">@@art_date@@</div>
	                                 <br class="clearFloat" />';
				}
				
				$freeContentList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
			}
			
			print ( '</td></tr></table></form></div>' );
		}
		
		// display tabs for used contents
		if ($userCount)
		{
			print ( '<div class="TabbedPanelsContent">' );
			// display used content form
			$this->DisplayFormHeadr ( 'add', 'used' );
			$this->DisplayHidden ( 'contentTypeID', $seleCatID );
			
			// display select features
			?>
		<table width="100%" border="0" cellspacing="0" cellpadding="0"
						class="titleBox">
						<tr>
							<td>
							<div class="titleColR"><span style="padding-right: 20px"> <strong>Select:</strong>
							<a href="#"
								onclick="chekeBoxes('used', 'contIDs[]', true); return false">All</a>,
							<a href="#"
								onclick="chekeBoxes('used', 'contIDs[]', false); return false">None</a>,
							</span> <input name="" type="submit" value="Add Relation" /></div>
							<div class="titleColR"></div>
							</td>
						</tr>
					</table>
					<table width="100%" border="0" cellspacing="0" cellpadding="0"
						style="border-left: 1px solid #777777; border-right: 1px solid #777777">
						<tr>
							<td>
                                    <?php
			if ($singOrMulti == 1 || $singOrMulti == 2)
			{
				
				$patern = null;
				
				if ($singOrMulti == 1)
				{
					$patern = '<div class="tableCHB"><input name="contIDs[]" type="radio" value="@@ID@@" /></div>
					                                        <div class="tableT">
					                                        <a href="?structure=content&' . "$ctLinkName=$seleCatID" . '&contID=@@ID@@">@@title@@</a>
					                                        </div>
					                                        <div class="tableD">@@art_date@@</div>
					                                    <br class="clearFloat" />';
				}
				elseif ($singOrMulti == 2)
				{
					$patern = '<div class="tableCHB"><input name="contIDs[]" id="contIDs[]" type="checkbox" value="@@ID@@" /></div>
					                                        <div class="tableT">
					                                        <a href="?structure=content&' . "$ctLinkName=$seleCatID" . '&contID=@@ID@@">@@title@@</a>
					                                        </div>
					                                        <div class="tableD">@@art_date@@</div>
					                                    <br class="clearFloat" />';
				}
				
				$usedContentList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
			}
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
	}
	
	/**
	 * Do action` delete  or add relation
	 * @return bool
	 */
	function DoAction($catID)
	{
		// if no action then return
		if (! $this->GetFormAction ())
			return true;
		
		$sc = new SCRelation ( );
		switch ($this->GetFormAction ())
		{
			case 'add' :
				// get content IDs
				if (($contIDs = DB::POST ( 'contIDs' )))
				{
					foreach ( $contIDs as $contID )
					{
						$res = $sc->Add ( $catID, $contID );
					}
				}
				return $res;
			case 'delete' :
				// get content IDs
				if (($contIDs = DB::POST ( 'contIDs' )))
				{
					foreach ( $contIDs as $contID )
					{
						$res = $sc->Delete ( $catID, $contID );
					}
				}
				return $res;
			case 'create' :
				// get name and content directory
				$name = DB::POST ( 'contentName' );
				$contentTypeID = DB::POST ( 'contentTypeID' );
				
				// create content
				$content = new Content ( );
				$content->Insert ( $name, '', $contentTypeID );
				$contID = $content->GetID ();
				
				// add relatein
				$res = $sc->Add ( $catID, $contID );
				break;
		}
		
		return false;
	}
}
?>