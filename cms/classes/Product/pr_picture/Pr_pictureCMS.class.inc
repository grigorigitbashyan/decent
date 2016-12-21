<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_picture/Pr_picture.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_pictureCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_picture
	 */
	protected $m_pr_picture;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->Update ();

		$ID = DB::REQUEST ('pr_pictureID');
		$this->m_pr_picture = new Pr_picture ( $ID );
		
		// read strings
		$this->ReadNameStrings('classes/Product/langs/', 'picture.xml');
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
			case 'rename' : // rename Pr_picture
				if (($newName = DB::POST('name')))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_pr_picture->SetName ( $newName );
					}
				}
				break;
			case 'delete' : // delete Pr_picture
				$ar = $this->m_pr_picture->Delete (false, true);
				break;
			case 'changeinfo' :
				$title = DB::POST ('title');
				$ar = $this->m_pr_picture->SetTitle( $title, $this->m_displayLang );
				
				$desc = DB::POST ('desc');
				$ar = $this->m_pr_picture->SetDesc ( $desc, $this->m_displayLang );
				
				$gallery = $_POST["gallery"]=="on" ? 1: 0;
				$ar = $this->m_pr_picture->SetGallery( $gallery );
				break;
		}
		
		//	 if there is a any action then update Pr_picture  
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
	
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// get language strings
		$doneS = $this->GetNameString('done');
		$backS = $this->GetNameString('back');
		$descS = $this->GetNameString('desc');
		$titleS = $this->GetNameString('title');
		$galleryS = $this->GetNameString('gallery');
		
		$ID = $this->m_pr_picture->GetID ();
		$title = $this->m_pr_picture->GetTitle ( $this->m_displayLang );
		$desc = $this->m_pr_picture->GetDesc ( $this->m_displayLang );
		$gallery = $this->m_pr_picture->GetGallery ( );
		
		$productID = $this->m_pr_picture->GetProductID ();
		
		$product = new Pr_main ( $productID );
		$proTypeID = $product->GetTypeID ();
		
		$overLink = 'pr_pictureID';
		
		// display picture
		print ( '<a name="imageDescription"></a>' );
		print('<div class="titleBox"><div class="titleColL">');
		print ( "<a href='?pr_mainID=$productID&subpr_typeID=$proTypeID#pictures'>$backS</a>" );
		print('</div><br class="clearFloat"></div>');
		$imageURL = $this->m_pr_picture->GetPath ( );
		$siteURL = $this->m_site->GetSiteURL();
		
		print ( '<table width="100%" border="0" cellspacing="0" cellpadding="5"	class="mediaFileAreAreaWithBT">
			<tr>
				<td>' );
		print ( "<img src='{$siteURL}cms/files/ImageMeaker.php?w=300&h=300&p=$imageURL' border='0'/>" );
		print ( '</td>
			</tr>
		</table>' );
		
		// diplay rename form
		?>
		<form action="" class="clearPD" id="" name="" method="post">
			<input type="hidden" value="changeinfo" name="pr_picturecmsAction">
			<input type="hidden" value="<?php echo $ID;?>" name="pr_pictureID">
			<input type="hidden" value="<?php echo $productID;?>" name="pr_mainID">
			<input type="hidden" value="<?php echo $proTypeID;?>" name="pr_typeID">
			<table cellspacing="0" cellpadding="0" border="0" width="100%" class="titleBox">
				<tr>
					<td>
						<div class="titleColL" style="width:50px;"><label><strong><?php echo $titleS;?></strong></label></div>
						<div class="titleColL"><input type="text" style="font-size:14px" maxlength="256" size="64" value="<?php echo $title;?>" id="title" name="title"></div>
						<br class="clearFloat">
					</td>
				</tr>
				<tr>
					<td>
						<div class="titleColL" style="width:50px;"><label><strong><?php echo $descS;?></strong></label></div>
						<div class="titleColL"><textarea id="desc" name="desc"><?php echo $desc;?></textarea></div>
						<br class="clearFloat">
					</td>
				</tr>
				<tr>
					<td>
						<div class="titleColL" style="width:50px;"><label><strong><?php echo $galleryS;?></strong></label></div>
						<div class="titleColL"><input type="checkbox" name="gallery"<?php echo ($gallery ? " checked": "");?>></div>
						<br class="clearFloat">
					</td>
				</tr>
				<tr>
					<td>
						<div class="titleColL" style="width:50px;">&nbsp;</div>
						<div class="titleColL"><input type="submit" value="<?php echo $doneS;?>" name=""></div>
						<br class="clearFloat">
					</td>
				</tr>
		</table>
		</form>
		<?php
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function Delete()
	{
		return $this->m_pr_picture->Delete ();
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Update()
	{
		$ID = DB::REQUEST ('pr_pictureID');
		$this->m_pr_picture = new Pr_picture ( $ID );
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$langs = $this->m_site->GetLanguages();

		$nameS = '';
		foreach ($langs as $lang)
		{
			$nameS .= "`desc_$lang` text,";
		}
		
		$sql = "CREATE TABLE `pr_picture` (
				  `ID` int(10) unsigned NOT NULL auto_increment,
				  `productID` int(10) unsigned NOT NULL,
				  `path` varchar(255) NOT NULL,
				  `position` int(11) default NULL,
				  $nameS
				  `isuploaded` tinyint(1) NOT NULL default '1',
				  PRIMARY KEY  (`ID`)
				)DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>