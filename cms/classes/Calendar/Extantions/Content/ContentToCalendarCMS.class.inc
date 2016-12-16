<?php

Site::IncludeFile ( 'classes/Content/Content.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentTypeList.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentListView.class.inc' );

Site::IncludeFile ( 'classes/Calendar/CalendarDataList.class.inc' );
Site::IncludeFile ( 'classes/Calendar/Extantions/Content/ContentToCalendarList.class.inc' );

/**
 * This is a Content to calendar (CMS) class.
 *
 */
class ContentToCalendarCMS extends CalendarExtantionCMS
{
	/**
	 * Enter description here...
	 *
	 * @param int $calendarID
	 */
	function __construct($calendarID = 1)
	{
		parent::__construct ( $calendarID );
		
		$this->ReadNameStrings ( 'classes/Calendar/Extantions/Content/langs/', 'content.xml' );
	}
	
	/* (non-PHPdoc)
	 * @see cms/classes/Calendar/Extantions/CalendarExtantionCMS#DoAction()
	 */
	function DoAction()
	{
		$res = parent::DoAction ();
		
		switch ($this->GetFormAction ())
		{
			case 'create' :
				// get name and content directory
				$name = DB::POST ( 'contentName' );
				$contentTypeID = DB::POST ( 'contentTypeID' );
				
				// create content
				$content = new Content ( );
				$content->Insert ( $name, '', $contentTypeID );
				$contID = $content->GetID ();
				
				// add relatein
				$ctdl = new CalendarDataList ( );
				$ctdl->Add ( $contID, $this->m_calendarID, 1, $this->m_currentDate );
				break;
		}
		
		return $res;
	}
	
	/**
	 * @see CMSObject::DisplayFeatures
	 *
	 * @return bool
	 */
	function DisplayFeatures()
	{
		parent::DisplayFeatures ();
		
		// get calendar to content relation object
		$dataList = new ContentToCalendarList ( $this->m_currentDate, $this->m_calendarID );
		$relIDs = $dataList->GetStructure ();
		
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
		
		if ($seleCatID)
		{
			$freeContentList = new ContentListView ( $seleCatID, 20 );
			$freeCount = $freeContentList->GetItemCount ();
		}
		
		print ("<div class='someGTitleBox'>$contentRelationS</div>") ;
		?>
<div class="someGBox">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
	<tr>
		<td>
		<form action="" method="POST" class="clearPD"><label><?php echo $selectDirectoryS?></label>
                      <?php
		//		print ( "<imput type='hidden' name='subcalendarID' value='$catID'>" );
		print ("<select name='contentTypeID' onchange='submit();'>") ;
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
			print ("<li class='TabbedPanelsTab' tabindex='0'>$relatedContentS</li>") ;
		}
		elseif ($seleCatID)
		{
			print ("<li class='TabbedPanelsTab' tabindex='0'>$createRelateS</li>") ;
		}
		
		$freeContentS = $this->GetNameString ( 'freeContents' );
		
		// display two tabs if the section has been selected
		if ($seleCatID)
		{
			// if content types does not selected then skeek free and used tabs
			if ($freeCount)
			{
				print ("<li class='TabbedPanelsTab' tabindex='0'>$freeContentS</li>") ;
			}
		}
		
		//
		print ('</ul>') ;
		
		if ($seleCatID || $relIDs)
		{
			print ('<div class="TabbedPanelsContentGroup">
					<div class="TabbedPanelsContent">') ;
		}
		
		// if there are related contents then display form for deleting relations
		if ($relIDs)
		{
			$this->DisplayFormHeadr ( 'relatedAction', 'related' );
		}
		
		print ('<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox"><tr><td>') ;
		
		$deleteRelationS = $this->GetNameString ( 'deleteRelation' );
		$selectS = $this->GetNameString ( 'select' );
		
		$allS = $this->GetNameString ( 'all' );
		$noneS = $this->GetNameString ( 'none' );
		
		// display related contents
		if ($relIDs)
		{
			print ('<div class="titleColR">') ;
			
			// display last date input //////////////////////////////////////////////
			print ('<span style="padding-right:20px">') ;
			print('<input type="text" name="lastDate" id="lastDate" value="'.$this->m_currentDate.'" size="8" readonly="readonly"/>');
			print ('<button id="calBtn" onclick="return false;">...</button>') ;
			print('<script type="text/javascript">//<![CDATA[
					 var cal1 = Calendar.setup({
				          onSelect: function(cal1) { cal1.hide() }
				      });
				      cal1.manageFields("calBtn", "lastDate", "%Y-%m-%d");
			    //]]></script> ');
			print('<input type="submit" name="setLastDate" id="setLastDate" value="Set last date" />');
			print ('</span>') ;
			// end display last date input //////////////////////////////////////////////
			print ('<span style="padding-right:20px">') ;
			print ('<strong>' . $selectS . '</strong> ') ;
			print ('<a href="#" onclick="chekeBoxes(\'related\', \'dataIDs[]\', true); return false;">' . $allS . '</a>, ') ;
			print ('<a href="#" onclick="chekeBoxes(\'related\', \'dataIDs[]\', false); return false;">' . $noneS . '</a>,') ;
			print ('</span>') ;
			print ('<input name="delete" type="submit" value="' . $deleteRelationS . '" /></div>') ;
		
		}
		
		//display abibility to create and relate new content
		elseif ($seleCatID)
		{
			$noEmptyS = $this->GetNameString ( 'noempty' );
			$renameJS = "onsubmit='return SingleForm(\"contentName\", \"$noEmptyS\");'";
			// display form
			$this->DisplayFormHeadr ( 'create', 'createForm', 'post', $renameJS );
			$this->DisplayHidden ( 'contentTypeID', $seleCatID );
			print ('<div class="titleColL">') ;
			print ("<input type='text' value='' name='contentName' id='contentName' size='64' maxlength='256' />\n") ;
			print ('</div>') ;
			print ('<div class="titleColR">') ;
			$this->DisplayFormFooter ( $doneS );
			print ('</div>') ;
		}
		
		print ('<br class="clearFloat" /></td></tr></table>') ;
		
		//display related contents
		if ($relIDs)
		{
			print ('<table width="100%" border="0" cellspacing="0" cellpadding="0" style="border-left: 1px solid #777777; border-right: 1px solid #777777"><tr><td>') ;
			
			$styleInd = 0;
			foreach ( $relIDs as $relCont )
			{
				$styleing = (($styleInd ++) % 2 == 0) ? 'tableUneven' : 'tableEven';
				print ("<div class='$styleing'>") ;
				print ('<div class="tableCHB">') ;
				print ("<input name='dataIDs[]' id='dataIDs[]' type='checkbox' value='{$relCont['ID']}' />") ;
				print ('</div>') ;
				print ('<div class="tableT">') ;
				print ("<a href='?structure=content&{$ctLinkName}={$relCont['purpose']}&contID={$relCont['dataID']}'>{$relCont['title']}</a>") ;
				print ('</div>') ;
				print ("<div class='tableD'>{$relCont['lastDate']}</div>") ;
				print ("<div class='tableD'>{$relCont['startDate']}</div>") ;
				print ('</div>') ;
				print ('<br class="clearFloat" />') ;
			}
			
			print ('</td></tr></table></form>') ;
		}
		
		print ('</div>') ;
		
		// display free contents
		if ($freeCount)
		{
			print ('<div class="TabbedPanelsContent">') ;
			
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
			
			print ('<span style="padding-right:20px">') ;
			print ('<strong>Select:</strong> ') ;
			print ('<a href="#" onclick="chekeBoxes(\'freeContents\', \'dataIDs[]\', true); return false">' . $allS . '</a>, ') ;
			print ('<a href="#" onclick="chekeBoxes(\'freeContents\', \'dataIDs[]\', false); return false">' . $noneS . '</a>,') ;
			print ('</span>') ;
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
			$patern = '<div class="tableCHB">
										<input name="dataIDs[]" id="dataIDs[]" type="checkbox" value="@@ID@@" />
								    </div>
	                                <div class="tableT">
	                                     <a href="?structure=content&' . "$ctLinkName=$seleCatID" . '&contID=@@ID@@">@@title@@</a>
	                                 </div>
	                                 <div class="tableD">@@art_date@@</div>
	                                 <br class="clearFloat" />';
			
			$freeContentList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
			
			print ('</td></tr></table></form></div>') ;
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
		return true;
	}
}
?>