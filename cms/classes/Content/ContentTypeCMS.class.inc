<?php
// inculdes
Site::IncludeFile ( 'classes/Content/Content.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentCMS.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentListView.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentType.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentTypeList.class.inc' );

Site::IncludeFile ( 'classes/Category/SCRelationCMS.class.inc' );

/**
 * Enter description here...
 * @package Content
 *
 */
class ContentTypeCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var ContentTypeList
	 */
	protected $m_contTypeList;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->ReadNameStrings ( 'classes/Content/langs/', 'content.xml' );
		
		$this->Update ();
	}
	
	/**
	 * Return (int) last inserted content ID or (bool) in other actions result
	 *
	 * @return bool|int
	 */
	function DoAction()
	{
		// if exist the content ID, then call DoAction for content
		if (($contID = DB::POST ( 'contID' )))
		{
			$contentCMS = new ContentCMS ( $contID );
			$contentCMS->DoAction ();
		}
		
		// content type actoin
		$ar = null;
		
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				$name = DB::POST ( 'name' );
				
				if ($name)
				{
					$contentType = new ContentType ( );
					$res = $contentType->Insert ( $name );
					
					if ($res)
					{
						$ar = $this->m_contTypeList = new ContentTypeList ( );
					}
				}
				break;
			case 'rename' :
				$name = DB::POST ( 'name' );
				
				if (strlen ( $name ) > 0)
				{
					$currentID = $this->m_contTypeList->GetCurrentID ();
					$contentType = new ContentType ( $currentID );
					
					$ar = $contentType->SetName ( $name );
				}
				break;
			case 'delete' :
				// get current content type ID
				$currentID = $this->m_contTypeList->GetCurrentID ();
				
				// get related list of contents
				$contentList = new ContentListView ( $currentID );
				
				// get related contents count
				$contCount = $contentList->GetItemCount ();
				
				// delete action
				if ($contCount == 0)
				{
					$contentType = new ContentType ( $currentID );
					$ar = $contentType->Delete ();
				}
				
				// move action
				break;
			case 'insertContent' :
				if (($name = DB::POST ( 'insertName' )))
				{
					$content = new Content ( );
					$ar = $content->Insert ( $name, "", $this->m_contTypeList->GetCurrentID () );
					
					if ($ar)
					{
						$ar = $content->GetID ();
					}
				}
				else
				{
					return false;
				}
				break;
			case 'groupAction' :
				if (($IDs = DB::POST ( 'contIDs' )))
				{
					// delete contents
					if (isset ( $_POST ['delete'] ))
					{
						foreach ( $IDs as $ID )
						{
							$content = new Content ( $ID );
							$ar = $content->Delete ();
						}
					}
					
					// move contents
					if (isset ( $_POST ['move'] ))
					{
						$moveTo = DB::POST ( 'moveto' );
						foreach ( $IDs as $ID )
						{
							$content = new Content ( $ID );
							$ar = $content->UpdatePurpose ( $moveTo );
						}
					}
				}
				break;
		}
		
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
		$currID = $this->m_contTypeList->GetCurrentID ();
		
		$directoriesS = $this->GetNameString ( 'directories' );
		
		if ($currID)
		{
			$linkName = $this->m_contTypeList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>$directoriesS</a></div>" );
		}
		else
		{
			print ( "<div class='someWTitleBox'>$directoriesS</div>" );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_contTypeList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// read language string
		$doneS = $this->GetNameString ( 'done' );
		$deleteS = $this->GetNameString ( 'delete' );
		$createDirectoriesS = $this->GetNameString ( 'createDirectories' );
		$noemptyS = $this->GetNameString ( 'noempty' );
		
		// get content type count
		$typeCount = $this->m_contTypeList->GetItemCount ();
		
		// get current content type
		$currID = $this->m_contTypeList->GetCurrentID ();
		$coutTypOverLink = $this->m_contTypeList->GetLinkIDName ();
		
		if ($currID < 1 || $typeCount == 0)
		{
			if ($this->IsInsertable ( 'contenttype' ))
			{
				$insertJS = "onsubmit='return SingleForm(\"name\", \"$noemptyS\")'";
				$this->DisplaySingleTextForm ( 'insert', $createDirectoriesS, 'name', null, null, $doneS, $insertJS );
			}
			
			return;
		}
		
		// if content is selected display content properties 
		if ($currID)
		{
			$contentList = new ContentListView ( $currID );
			$contID = $contentList->GetCurrentID ();
		}
		
		// get content cms object
		if ($contID !== null)
		{
			$content = new ContentCMS ( $contID );
			
			// if there was a ation delete then set content to null
			if (! $content->GetContent ())
			{
				$content = null;
				$contID = null;
			}
		}
		
		if ($currID !== null)
		{
			$contentType = new ContentType ( $currID );
			$currID = $contentType->GetID ();
		}
		// if is selected content from content types, then display content features
		if (isset ( $content ))
		{
			$content->DisplayFeatures ( $currID );
		}
		elseif ($currID !== null) // display content type features if it has selected and exist
		{
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			
			// get current content type
			$title = $contentType->GetName ( $this->m_displayLang );
			$contentsInType = $contentList->GetItemCount ();
			
			if ($contentsInType > 0) // then content type connot be delete
			{
				$this->DisplayTitle ( $title );
			}
			else
			{
				// menu delete controll
				$form = null;
				if ($this->IsDeletable ( 'contenttype' ))
				{
					$deleteS = $this->GetNameString ( 'delete' );
					$actionName = $this->GetFormActionName ();
					
					$form = "<div class='titleColR'>
						 <form method='post' class='clearPD' name='delForm'> 
						 <input type='hidden' name='$actionName' value='delete' />
						 <input type='hidden' name='$coutTypOverLink' value='$currID' />
						 <input type='submit' onclick='return confirm(\"$sureDeleteS\");' value='$deleteS' />
						 </form></div>";
				}
				
				$this->DisplayTitle ( $title, $form );
			}
			
			// rename
			if ($this->IsUpdatable ( 'contenttype', 'name' ))
			{
				$renameS = $this->GetNameString ( 'rename' );
				$insertJS = "onsubmit='return SingleForm(\"name\", \"$noemptyS\")'";
				$this->DisplaySingleTextForm ( 'rename', $renameS, 'name', $title, array ($coutTypOverLink => $currID ), $doneS, $insertJS );
				print ( '<br />' );
			}
			
			//if content type has been selected, then display insert content feature
			if ($currID)
			{
				// display form for creating new content
				if ($this->IsInsertable ( 'content' ))
				{
					$createContentS = $this->GetNameString ( 'createContent' );
					
					$insertJS = "onsubmit='return SingleForm(\"insertName\", \"$noemptyS\");'";
					$this->DisplaySingleTextForm ( 'insertContent', $createContentS, 'insertName', null, array ($coutTypOverLink => $currID ), $doneS, $insertJS );
					print ( '<br class="clearFloat" />' );
				}
				
				$isContentDele = $this->IsDeletable('content');
				$isContentMove = $this->IsUpdatable('content', 'purpose');
				// display contents
				if ($contentsInType > 0)
				{
					$this->DisplayFormHeadr ( 'groupAction', 'contents' );
					$this->DisplayHidden ( $coutTypOverLink, $currID );
					?>
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL"><?php
					$contentList->DisplayNavigation ( "contentTypeID=$currID" );
					?></div>
		<div class="titleColR"><span style="padding-right: 20px">
			                <?php
					// print sort
					$sortByS = $this->GetNameString ( 'sortby' );
					$titleS = $this->GetNameString ( 'title' );
					$dateS = $this->GetNameString ( 'date' );
					
					print ( $sortByS . "&nbsp;" );
					
					$contentList->DisplaySortLinks ( "name_{$this->m_displayLang}", $titleS, "contentTypeID=$currID" );
					print ( '&nbsp;' );
					$contentList->DisplaySortLinks ( "art_date", $dateS, "contentTypeID=$currID" );
					print ( '&nbsp;' );
					
					?>
			                <select name="moveto">
			                <?php
					$this->m_contTypeList->DisplaySelect ( null, $currID );
					
					// get names
					$moveS = $this->GetNameString ( 'move' );
					$allS = $this->GetNameString ( 'all' );
					$noneS = $this->GetNameString ( 'none' );
					$selectS = $this->GetNameString ( 'select' );
					$noSelectedS = $this->GetNameString ( 'noselected' );
					?>
			                </select> <input type="submit" name="move"
			value="<?php echo $moveS?>"
			onclick="return ChecksForm('contents', 'contIDs', '<?php echo $noSelectedS?>');" />
		</span> <span style="padding-right: 20px"> <strong><?php echo $selectS?></strong>
		<a href="#"
			onclick="chekeBoxes('contents', 'contIDs', true); return false"><?php echo $allS?></a>,
		<a href="#"
			onclick="chekeBoxes('contents', 'contIDs', false); return false"><?php echo $noneS?></a>
		</span>
		<?php
		if($this->IsDeletable('content'))
		{
			print("<input name=\"delete\" type=\"submit\" value=\"$deleteS\" onclick=\"return ChecksFormDelete('contents', 'contIDs', '$noSelectedS', '$sureDeleteS');\" />");	
		} 
		?> 
		
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
					$patern = "<div class='tableCHB'>
						                    <input name='contIDs[]' id='contIDs' type='checkbox' value='@@ID@@' />
						                </div>
					                    <div class='tableT'>
					                    	<a href='?subcontentTypeID=$currID&contID=@@ID@@'>@@title@@</a>
					                    </div>
						                <div class='tableD'>@@art_date@@</div>
						                <br class='clearFloat' />";
					
					$contentList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
					?>
			            </td>
	</tr>
</table>
</form>
<?php
				}
			}
		}
		
		print ( "&nbsp;" );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Update()
	{
		$this->m_contTypeList = new ContentTypeList ( );
	}
	
	function CreateDB()
	{
		$sql = "CREATE TABLE `contenttype` (
				  `ID` tinyint(4) unsigned NOT NULL auto_increment,
				  `name` varchar(255) NOT NULL default 'content type',
				  PRIMARY KEY  (`ID`)
				)DEFAULT CHARSET=utf8;";
		
		$res = parent::CreateDB ( $sql );
		
		if($res)
		{
			$contType = new ContentType();
			$contType->Insert('Common');
		}
		
		// create content table
		$content = new ContentCMS ( );
		$res = $content->CreateDB ();
		
		// create relation table
		$scRelation = new SCRelationCMS ( );
		$scRelation->CreateDB ();
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $contentID
	 * @param string $overIDname
	 * @param int $overID
	 * @param string $actionName
	 */
	public function DispalayContentRelation($contentID, $overIDname, $overID, $actionName, $count = 50)
	{
		// names
		$contRelS = $this->GetNameString ( 'contRel' );
		$selectDestinationS = $this->GetNameString ( 'selectDestination' );
		$deleteRelS = $this->GetNameString ( 'deleteRelation' );
		$relatedContentS = $this->GetNameString ( 'relatedContent' );
		$addRelationS = $this->GetNameString ( 'addRelation' );
		$contentS = $this->GetNameString ( 'content' );
		$createRelateS = $this->GetNameString ( 'createRelate' );
		
		///////////////////////////////////////////////////
		// get content type list object
		$catTypes = new ContentTypeList ( );
		
		// get current ID of Content types
		$contentTypeID = $catTypes->GetCurrentID ();
		
		// get content types link ID name for get overloading
		$ctLinkName = $catTypes->GetLinkIDName ();
		$ctLinkName = "sub" . $ctLinkName;
		
		print ( "<div class='someGTitleBox'>$contRelS</div>" );
		?>
<div class="someGBox">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
	<tr>
		<td>
		<form action="" method="POST" class="clearPD"><label><?php echo $selectDestinationS?></label>
        <?php
		print ( "<imput type='hidden' name='catID' value='$overID'>" );
		print ( "<select name='contentTypeID' onchange='submit();'>" );
		$catTypes->DisplaySelect ( '---' );
		print ( '</select>' );
		?>
         </form>
		</td>
	</tr>
		<?php
		if ($contentTypeID || $contentID)
		{
			print ( '<tr>
				<td>&nbsp;</td>
			</tr>
			<tr>
				<td>' );
			print ( '<div id="TabbedPanels1" class="TabbedPanels">' );
			print ( '<ul class="TabbedPanelsTabGroup">' );
			
			if ($contentID)
			{
				print ( "<li class='TabbedPanelsTab' tabindex='0'>$relatedContentS</li>" );
			}
			elseif ($contentTypeID)
			{
				print ( "<li class='TabbedPanelsTab' tabindex='0'>$createRelateS</li>" );
			}
			
			if ($contentTypeID)
			{
				// if content types does not selected then skeek free and used tabs
				print ( "<li class='TabbedPanelsTab' tabindex='0'>$contentS</li>" );
			}
			
			// show related content form with delete feature //////////////////////////////////////////////////
			print ( "</ul>" );
			
			print ( '<div class="TabbedPanelsContentGroup">' );
			print ( '<div class="TabbedPanelsContent">' );
			
			//////////////////////////////////////////////////////////////////////////////		
			if ($contentID && $contentTypeID)
			{
				print ( "\n<form method='post' name='delete' class='clearPD' action=''>\n" );
			}
			else
			{
				$noEmptyS = $this->GetNameString ( 'noempty' );
				$renameJS = "onsubmit='return SingleForm(\"insertName\", \"$noEmptyS\");'";
				print ( "\n<form method='post' name='insert' class='clearPD' action='' $renameJS>\n" );
			}
			
			$this->DisplayHidden ( $actionName, 'addRelation' );
			$this->DisplayHidden ( $overIDname, $overID );
			$this->DisplayHidden ( 'paramValue', 0 );
			?>
		<table width="100%" border="0" cellspacing="0" cellpadding="0"
		class="titleBox">
		<tr>
			<td>
			        <?php
			// if there is a related content
			if ($contentID)
			{
				print ( '<div class="titleColR">' );
				print ( "<input name='' type='submit' value='$deleteRelS' /></div>" );
			}
			// display form for createing new content
			elseif ($contentTypeID)
			{
				$doneS = $this->GetNameString ( 'done' );
				$contActName = $this->GetFormActionName ();
				// set action name
				$this->DisplayHidden ( $contActName, 'insertContent' );
				// set current content type ID
				$this->DisplayHidden ( 'contentTypeID', $catTypes->GetCurrentID () );
				// display text input for content name
				print ( '<div class="titleColL">' );
				print ( "<input type='text' value='' name='insertName' id='insertName' size='64' maxlength='256' />\n" );
				print ( '</div>' );
				print ( '<div class="titleColR">' );
				$this->DisplayFormFooter ( $doneS );
				print ( '</div>' );
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
			//display related contents
			if ($contentID)
			{
				$cont = new Content ( $contentID );
				$contentID = $cont->GetID ();
				
				if ($contentID)
				{
					$purpose = $cont->GetPurpose ();
					$title = $cont->GetName ( $this->m_displayLang );
					$date = $cont->GetDate ();
					
					print ( "<div class='tableUneven'>" );
					print ( '<div class="tableCHB">' );
					print ( '<input type="radio"  name="" id="" checked="checked" />' );
					print ( "<input name='contIDs' id='contIDs' type='hidden' value='{$contentID}' />" );
					print ( '</div>' );
					print ( '<div class="tableT">' );
					print ( "<a href='?structure=content&{$ctLinkName}={$purpose}&contID={$contentID}'>{$title}</a>" );
					print ( '</div>' );
					print ( "<div class='tableD'>{$date}</div>" );
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
			// show contents in seletected content category form with add feature /////////////////////
			if ($contentTypeID)
			{
				print ( '<div class="TabbedPanelsContent">' );
				
				print ( "\n<form method='post' name='free' class='clearPD' action=''>\n" );
				$this->DisplayHidden ( $actionName, 'addRelation' );
				$this->DisplayHidden ( $overIDname, $overID );
				
				$contentList = new ContentListView ( $contentTypeID, 128 );
				$freeContentCount = $contentList->GetItemCount ();
				?>
		<table width="100%" border="0" cellspacing="0" cellpadding="0"
		class="titleBox">
		<tr>
			<td>
				<?php
				if ($freeContentCount > 0)
				{
					print ( "<div class='titleColR'>
							<input type='submit' value='$addRelationS' />
						 </div>
						 <br class='clearFloat' />" );
				}
				else
				{
					print ( "&nbsp;" );
				}
				?>
				</td>
		</tr>
	</table>
	<table width="100%" border="0" cellspacing="0" cellpadding="0"
		style="border-left: 1px solid #777777; border-right: 1px solid #777777">
		<tr>
			<td>
            <?php
				// get free contents from content list
				if ($freeContentCount > 0)
				{
					// patern for display of each item
					$patern = '<div class="tableCHB">
                               <input name="paramValue" id="paramValue" type="radio" value="@@ID@@" />
                           </div>
                           <div class="tableT">
                           <a href="?structure=content&' . "$ctLinkName=$contentTypeID" . '&contID=@@ID@@">@@title@@</a>
                           </div>
                           <div class="tableD">@@art_date@@</div>
                          <br class="clearFloat" />';
					
					// diplay list by patern
					$contentList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
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
			print ( '</td>
	</tr>' );
		}
		?>

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
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$fields = array ('ID', 'name' );
		$res = $this->TestTable ( 'contenttype', $fields);
		
		// create content table
		$content = new ContentCMS ( );
		$res &= $content->SelfTest ();
		
		// create relation table
		$scRelation = new SCRelationCMS ( );
		$res &= $scRelation->SelfTest ();
		
		return $res;
	}
	
	/**
	 * Return tables of object
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		$res = array ('contenttype' );
		
		// create content table
		$content = new ContentCMS ( );
		$resCont = $content->GetObjectTables ();
		
		// create relation table
		$scRelation = new SCRelationCMS ( );
		$resRel = $scRelation->GetObjectTables ();
		
		return array_merge ( $res, $resCont, $resRel );
	}
	
	/**
	 * Return Ajax query responce
	 *
	 * @param string $arg
	 * @return string
	 */
	function GetAjax($arg)
	{
		return "CONTENT $arg";
	}
}
?>