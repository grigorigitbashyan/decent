<?php

Site::IncludeFile ( 'classes/RSS/RSS.class.inc' );
Site::IncludeFile ( 'classes/RSS/RSSCMS.class.inc' );
Site::IncludeFile ( 'classes/RSS/RSSList.class.inc' );

class RSSListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var RSSList
	 */
	protected $m_RSSList;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->Update ();
		
		$this->ReadNameStrings('classes/RSS/langs/', 'rss.xml');
	}
	
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		$currID = $this->m_RSSList->GetCurrentID ();
		
		if ($currID)
		{
			$RSSCMS = new RSSCMS ( $currID );
			$RSSCMS->DisplayFeatures ();
		}
		else
		{
			// get name strings
			$doneS = $this->GetNameString('done');
			$createFileS = $this->GetNameString('createFile');
			$imporFileS  = $this->GetNameString('importFile');
			$imporS  = $this->GetNameString('import');
			$uploadFileS  = $this->GetNameString('uploadFile');
			$uploadS  = $this->GetNameString('upload');
			
			// action owerloaded name
			$actionName = $this->GetFormActionName ();
			
			$this->DisplaySingleTextForm ( 'createFile', $createFileS, 'name', null, null, $doneS );
			print ( '<br />' );
			
			$this->DisplaySingleTextForm ( 'importFile', $imporFileS, 'name', null, null, $imporS );
			print ( '<br />' );
			
			print ( '<form enctype="multipart/form-data" action="" method="POST">' );
			$this->DisplayHidden ( $actionName, 'uploadFile' );
			?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
	<tr>
		<td>
		<div class="titleColL">
			<label><strong><?php echo $uploadFileS?></strong></label>
			<input name="userfile" type="file" size="64" maxlength="256" style="font-size: 14px" />
		</div>
		<div class="titleColR"><input name="" type="submit" value="<?php echo $uploadS?>" /></div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
</form>
<br />
<?php
		}
	}
	
	/**
	 * @see CMSInterface::DisplayMenu()
	 *
	 */
	function DisplayMenu()
	{
		$currID = $this->m_RSSList->GetCurrentID ();
		
		$boxS = $this->GetNameString('RSSName'); 

		if ($currID)
		{
			$linkName = $this->m_RSSList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>$boxS</a></div>" );
		}
		else
		{
			print ( "<div class='someWTitleBox'>$boxS</div>" );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_RSSList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * @see CMSInterface::DoAction()
	 *
	 */
	function DoAction()
	{
		$currID = $this->m_RSSList->GetCurrentID ();
		
		$res = null;
		
		if ($currID)
		{
			$RSSCMS = new RSSCMS ( $currID );
			$res = $RSSCMS->DoAction ();
		}
		
		switch ($this->GetFormAction ())
		{
			case 'createFile' :
				$fileName = $_POST ['name'];
				$string = '<?xml version="1.0" encoding="utf8"?>
							<rss version="2.0">
								<channel>
								</channel>
							</rss>';
				
				$xml = simplexml_load_string ( $string );
				$res = $xml->asXML ( 'rss/' . $fileName );
				
				$tempRss = new RSS('');
				$params = array ('rss' => 'rss/'.$fileName );
				$tempRss->Insert ( $params );
				break;
			case 'importFile' :
				$fileURL = $_POST ['name'];
				$xml = simplexml_load_file ( $fileURL );
				
				// get file name
				$pathInfo = pathinfo ( $fileURL );
				$fileName = $pathInfo ['basename'];
				$res = $xml->asXML ( 'rss/' . $fileName );
				break;
			case 'uploadFile' :
				$uploaddir = 'rss/';
				$uploadfile = $uploaddir . basename ( $_FILES ['userfile'] ['name'] );
				
				if (move_uploaded_file ( $_FILES ['userfile'] ['tmp_name'], $uploadfile ))
				{
					$res = true;
				}
				break;
		}
		if ($res)
		{
			$this->Update ();
		}
	}
	
	function Update()
	{
		$this->m_RSSList = new RSSList ( );
	}
}
?>