<?php
Site::IncludeFile ( 'classes/Content/Content.class.inc' );
/**
 * This class has fitchures to draw content forms and do action  with it.
 *
 * @package Content
 */
class ContentCMS extends CMSObject implements CMSInterface
{
	/**
	 * Content
	 *
	 * @protected Content
	 */
	protected $m_content;
	
	/**
	 * Constructor, if $contentID is 0, then need insert new content
	 *
	 * @param int $contentID
	 * @return ContentCMS
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$contentID = ($ID !== null) ? $ID : $this->m_site->GetValue ( 'contID' );
		
		$this->m_content = new Content ( $contentID );
		
		// if content does not exist then set null
		if (! $this->m_content->GetID ())
		{
			$this->m_content = null;
		}
		
		$this->ReadNameStrings ( 'classes/Content/langs/', 'content.xml' );
	}
	
	/**
	 * Display forms for title, article and date
	 *
	 */
	function DisplayFeatures()
	{
		if (! $this->m_content)
		{
			return false;
		}
			// get names
		$optionS = $this->GetNameString ( 'options' );
		$doneS = $this->GetNameString ( 'done' );
		$deleteS = $this->GetNameString ( 'delete' );
		$renameS = $this->GetNameString ( 'rename' );
		$showS = $this->GetNameString ( 'show' );
		$hideS = $this->GetNameString ( 'hide' );
		$dateS = $this->GetNameString ( 'date' );
		$titleStatus = $this->GetNameString ( 'titleStatus' );
		$shortContentS = $this->GetNameString ( 'shortContent' );
		$noEmptyS = $this->GetNameString ( 'noempty' );
		
		// initialzation variables
		$contentID = $this->m_content->GetID ();
		$title = $this->m_content->GetName ( $this->m_displayLang );
		$istitleShow = $this->m_content->GetTitleShow ();
		$article = $this->m_content->GetContent ( $this->m_displayLang, false, null, false, false );
		$shortArticle = $this->m_content->GetContent ( $this->m_displayLang, false, null, false, true );
		$date = $this->m_content->GetDate ();
		$isDelete = $this->m_content->GetDeleteStatus ();
		
		// Javascript Validation //////////////////////////////////////////////
		?>
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL">
		<h1><?php echo $title?></h1>
		</div>
	   <?php
		// delete
		if ($isDelete)
		{
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
			
			$this->DisplayFormHeadr ( 'delete', 'delForm', 'post', $dJS );
			$this->DisplayHidden ( 'contID', $contentID );
			
			print ( "<div class='titleColR'><input type='submit' value='$deleteS' /></div>" );
			print ( '</form>' );
		}
		?>
	                <br class="clearFloat" />
		</td>
	</tr>
</table>
<div class="someGTitleBox"><?php echo $optionS?></div>
<div class="someGBox">
<?php
		// tab header
		$this->DisplayTabHeader ( 1, $renameS );
		
		// rename form
		$renameJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
		$this->DisplayFormHeadr ( 'rename', 'renameForm', 'post', $renameJS );
		$this->DisplayHidden ( 'contID', $contentID );
		print ( "<input name=\"name\" id=\"name\" type=\"text\" size=\"64\" maxlength=\"256\" value=\"$title\" />" );
		$this->DisplayFormFooter ( $doneS );
		
		// tab footer
		$this->DisplayTabFooter ();
		
		// tab header
		$this->DisplayTabHeader ( 2, $titleStatus );
		
		$this->DisplayFormHeadr ( 'visibility' );
		$this->DisplayHidden ( 'contID', $contentID );
		
		print ( "<label>$showS </label>" );
		
		if ($istitleShow)
			print ( '<input name="showtitle" type="radio" value="on" checked />' );
		else
			print ( '<input name="showtitle" type="radio" value="on"/>' );
		
		print ( "<label>$hideS </label>" );
		if (! $istitleShow)
			print ( '<input name="showtitle" type="radio" value="off" checked />' );
		else
			print ( '<input name="showtitle" type="radio" value="off"/>' );
		
		print ( "<input type='submit' value='$doneS' />" );
		print ( '</form>' );
		
		// tab footer
		$this->DisplayTabFooter ();
		
		// tab header
		$this->DisplayTabHeader ( 3, $shortContentS );
		
		// require google language api class
		// translate text
		$this->DisplayFormHeadr ( 'updateShortContent' );
		$this->DisplayHidden ( 'contID', $contentID );
		print ( "<textarea name='article' id='article'>$shortArticle</textarea>" );
		?>
						        <br />
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL"><input name="" type="submit"
			value="<?php echo $doneS?>" /></div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
</form>
<br class="clearFloat" />
</div>
</div>
</div>
</div>
<br />
<div id="main">
<div id="branding"></div>
<form class="query-box" onsubmit="return submitChange('articleMain');"><select
	name="src" id="src"></select> >> <select name="dst" id="dst"></select>
<input class="button" type="submit" value="Translate" /></form>
</div>
<div id="aaa"></div>
<?php
		$this->DisplayFormHeadr ( 'update' );
		$this->DisplayHidden ( 'contID', $contentID );
		print ( "<textarea name='articleMain' id='articleMain'>$article</textarea>" );
		?>
<br />
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL"><input name="" type="submit"
			value="<?php echo $doneS?>" /></div>
		<div class="titleColR"><label><strong><?php echo $dateS?></strong></label>
		<input name="date" id="date" type="text" size="16" maxlength="32"
			style="font-size: 14px" value="<?php echo $date?>" readonly="readonly" /><button id="calBtn" onclick="return false;">...</button></div>
		<br class="clearFloat" />
		<script type="text/javascript">//<![CDATA[
		 var cal = Calendar.setup({
	          onSelect: function(cal) { cal.hide() }
	      });
	      
	      cal.manageFields("calBtn", "date", "%Y-%m-%d");

    //]]></script>    
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
			<!--
			var CollapsiblePanel1 = new Spry.Widget.CollapsiblePanel("CollapsiblePanel1", {contentIsOpen:false});
			var CollapsiblePanel2 = new Spry.Widget.CollapsiblePanel("CollapsiblePanel2", {contentIsOpen:false});
			var CollapsiblePanel3 = new Spry.Widget.CollapsiblePanel("CollapsiblePanel3", {contentIsOpen:false});
			//-->
			</script>
<?php
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $lang
	 * @return bool
	 */
	function DoAction()
	{
		$res = false;
		
		switch ($this->GetFormAction ())
		{
			case 'rename' :
				// update title
				$title = DB::POST ( 'name' );
				$res = $this->m_content->SetName ( $title, $this->m_displayLang );
				break;
			case 'visibility' :
				$show_title = (DB::POST ( 'showtitle' ) == 'on') ? 1 : 0;
				$res = $this->m_content->UpdateVisibility ( $show_title );
				break;
			case 'update' :
				// if m_contnet is null , create new contet
				$article = DB::POST ( 'articleMain' );
				
				// update article
				$res = $this->m_content->UpdateText ( $article, $this->m_displayLang, true );
				
				// update date
				$date = DB::POST ( 'date' );
				$res = $this->m_content->UpdateDate ( $date );
				break;
			case 'updateShortContent' :
				// if m_contnet is null , create new contet
				$article = DB::POST ( 'article' );
				
				// update article
				$res = $this->m_content->UpdateShortText ( $article, $this->m_displayLang );
				break;
			case 'delete' : /////////////////////////////////////////////////////////////
				$res = $this->m_content->Delete ();
				$this->m_content = null;
				break;
		}
		
		//        if($res && $contAction != 'delete') // update
		//        {
		//            $this->m_content->Read();
		//        }
		

		return $res;
	}
	
	function GetContent()
	{
		return $this->m_content;
	}
	
	/**
	 * create content databace (filds for all langages), if it doesn`t exist
	 *
	 * @return bool
	 */
	function CreateDB() //static function
	{
		// get all suported languages
		$res = parent::CreateDB ();
		
		$languages = $this->m_site->GetLanguages ();
		
		// filds for each language
		$homeFelds = "";
		$homeValues = "";
		
		foreach ( $languages as $language )
		{
			$homeFelds .= " , `name_$language` ";
			$homeValues .= " , 'Offline message' ";
		}
		
		// insert offline message content
		if ($res)
		{
			$sql = "INSERT INTO `content` ( `ID`, `purpose`, `isdelete` $homeFelds) VALUES (1, 1, 0 $homeValues);";
			$res = DB::Query ( $sql );
		}
		
		return $res;
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		// get all suported languages
		$site = View::GetSite ();
		$languages = $site->GetLanguages ();
		
		// filds for each language
		$lTitle = "";
		$lArticle = "";
		$lsortArticle = "";
		
		foreach ( $languages as $language )
		{
			$lTitle .= " `name_$language` varchar(255) NOT NULL,";
			$lArticle .= " `article_$language` mediumtext,";
			$lsortArticle .= " `shortArticle_$language` text,";
		}
		
		// you most enter how meny lnags do you use, for example eng, rus, arm....
		$insDate = date ( "Y-m-d" );
		
		$sql = "CREATE TABLE `content` (
				  `ID` smallint(5) unsigned NOT NULL auto_increment,
				  $lTitle
				  $lArticle
				  $lsortArticle
				  `art_date` date NOT NULL default '$insDate',
				  `purpose` tinyint(4) NOT NULL default '1',
				  `isdelete` tinyint(1) NOT NULL default '1',
				  `show_title` tinyint(1) NOT NULL default '1',
				  PRIMARY KEY  (`ID`)
				)DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
	
	function DisplayMenu()
	{
	
	}
}
?>