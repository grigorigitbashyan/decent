<?php
Site::IncludeFile ( 'classes/Subscribe/Subscribe.class.inc' );
Site::IncludeFile ( 'classes/Subscribe/SURelation.class.inc' );

Site::IncludeFile ( 'classes/Content/ContentTypeCMS.class.inc' );
Site::IncludeFile ( 'classes/Category/Category.class.inc' );

/**
 * Enter description here...
 *
 * @package Notification
 */
class SubscribeCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Subscribe
	 */
	protected $m_subscribe;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		
		$this->m_subscribe = new Subscribe ( $ID );
		$this->ReadNameStrings ( 'classes/Subscribe/langs/', 'subscribe.xml' );
	}
	
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		// display subscribe name with delete
		$subscribeID = $this->m_subscribe->GetID ();
		
		if(!$subscribeID)
		{
			return false;
		}
		
		$name = $this->m_subscribe->GetName ();
		
		if ($subscribeID == 1)
		{
			$this->DisplayTitle ( $name );
		}
		else
		{
			// delete form javascript
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
			
			// get names
			$deleteS = $this->GetNameString ( 'delete' );
			
			$this->DisplayFormHeadr ( 'delete', 'delForm', 'post', $dJS );
			$this->DisplayHidden ( 'SubscribeListID', $subscribeID );
			?>
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
		<div class="titleColL">
		<h1><?php echo $name?></h1>
		</div>
		<div class="titleColR"><input name="" type="submit"
			value="<?php echo $deleteS?>" /></div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
</form>
<br class="clearFloat" />
<?php
		}
		
		// display status
		$status = $this->m_subscribe->GetStatus ();
		// if status is enabled then
		if ($status)
		{
			//display procent
			$sur = new SURelation ( $subscribeID );
			$procent = $sur->GetSubscribeProcent ();
			
			// abort
			$processS = $this->GetNameString ( 'process' );
			$abortS = $this->GetNameString ( 'abort' );
			
			print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
		          <tr>
		            <td>
		                <div class="titleColL"><h1>' . $processS . ' ' . $procent . '%</h1></div>
		                <br class="clearFloat" />
		        	</td>' );
			print ( '<td align="right">' );
			print ( '<div class="titleColR">' );
			$js = ' onsubmit=\'return confirm("Are you sure?");\'';
			$this->DisplayFormHeadr ( 'abort', null, 'post', $js );
			$this->DisplayHidden ( 'SubscribeListID', $subscribeID );
			$this->DisplayFormFooter ( $abortS );
			print ( '</div><br class="clearFloat" /></td>' );
			print ( '</tr></table><br />' );
			
			// start
			

			// display related content
			$contentID = $this->m_subscribe->GetContentID ();
			$content = new Content ( $contentID );
			
			// getting defalult language for choosing article language
			$defLang = $this->m_site->GetDefaultLanguage ();
			
			$title = $content->GetName ( $defLang );
			$article = $content->GetContent ( $defLang );
			
			$subjectS = $this->GetNameString ( 'subject' );
			
			print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
		          <tr>
		            <td>
		                <div class="titleColL"><h1>' . $subjectS . ' ' . $title . '</h1></div>
		                <br class="clearFloat" />
		        	</td>' );
			print ( '<td align="right">' );
			print ( '<div class="titleColR">' );
			$siteURL = $this->m_site->GetSiteURL ();
			
			$openSubscribeJS = ' onsubmit=\'window.open("' . $siteURL . 'cms/files/subscribe.php?subscribeID=' . $subscribeID . '","subscribe","width=400,height=200"); return true;\'';
			
			//display start
			if ($status == 1)
			{
				$startS = $this->GetNameString ( 'start' );
				
				$this->DisplayFormHeadr ( 'start', null, 'post', $openSubscribeJS );
				$this->DisplayHidden ( 'SubscribeListID', $subscribeID );
				$this->DisplayFormFooter ( $startS );
			
			}
			//display pause
			elseif ($status == 2)
			{
				$pauseS = $this->GetNameString ( 'pause' );
				$this->DisplayFormHeadr ( 'pause' );
				$this->DisplayHidden ( 'SubscribeListID', $subscribeID );
				$this->DisplayFormFooter ( $pauseS );
			}
			//display continue
			else
			{
				$continueS = $this->GetNameString ( 'continue' );
				$this->DisplayFormHeadr ( 'countinue', null, 'post', $openSubscribeJS );
				$this->DisplayHidden ( 'SubscribeListID', $subscribeID );
				$this->DisplayFormFooter ( $continueS );
			}
			
			print ( '</div><br class="clearFloat" /></td>' );
			print ( '</tr></table><br />' );
			
			print ( $article );
		}
		// 		else
		{
			// relate content
			$contentID = $this->m_subscribe->GetContentID ();
			$subscribeID = $this->m_subscribe->GetID ();
			$actionName = $this->GetFormActionName ();
			
			$contTypelist = new ContentTypeCMS ( );
			$contTypelist->DispalayContentRelation ( $contentID, 'SubscribeListID', $subscribeID, $actionName );
		}
		
		return true;
	}
	
	/**
	 * @see CMSInterface::DisplayMenu()
	 *
	 */
	function DisplayMenu()
	{
	
	}
	
	/**
	 * @see CMSInterface::DoAction()
	 *
	 */
	function DoAction()
	{
		$ar = null;
		
		switch ($this->GetFormAction ())
		{
			case 'addRelation' :
				$newContentID = DB::POST ( 'paramValue' );
				
				// insert new content section
				if ($newContentID == 0)
				{
					$contTypeCMS = new ContentTypeCMS ( );
					$newParamVal = $contTypeCMS->DoAction ();
					
					if (is_int ( intval ( $newParamVal ) ) && $newParamVal > 1)
					{
						$newContentID = $newParamVal;
					}
				}
				
				$ar = $this->m_subscribe->SetContentID ( $newContentID );
				
				$ar = $this->m_subscribe->SetLastUserID ( 0 );
				$ar = $this->m_subscribe->SetStatus ( 1 );
				break;
			case 'abort' :
				$ar = $this->m_subscribe->SetContentID ( 0 );
				$ar = $this->m_subscribe->SetLastUserID ( 0 );
				$ar = $this->m_subscribe->SetStatus ( 0 );
				break;
			case 'start' :
				$ar = $this->m_subscribe->SetStatus ( 2 );
				break;
			case 'pause' :
				$status = $this->m_subscribe->GetStatus ();
				
				if ($status != 0)
				{
					$ar = $this->m_subscribe->SetStatus ( 3 );
				}
				break;
			case 'countinue' :
				$status = $this->m_subscribe->GetStatus ();
				
				if ($status != 0)
				{
					$ar = $this->m_subscribe->SetStatus ( 2 );
				}
				break;
			case 'delete' :
				$ar = $this->m_subscribe->Delete ();
				break;
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$res = parent::CreateDB ();
		
		// create subscrube users
		$su = new SURelationCMS ( );
		$res1 = $su->CreateDB ();
		
		return ($res && $res1);
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `subscribe` (
				  `ID` int(11) NOT NULL auto_increment,
				  `name` varchar(256) NOT NULL,
				  `status` tinyint(1) NOT NULL default '0',
				  `contentID` int(11) NOT NULL default '0',
				  `lastUserID` int(11) NOT NULL default '0',
				  PRIMARY KEY  (`ID`)
				) DEFAULT CHARSET=utf8 ;";
		
		return $sql;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$res = $this->TestTable ();
		
		// create subscrube users
		$su = new SURelationCMS ( );
		$res &= $su->TestTable ();
		
		return $res;
	}
}
?>