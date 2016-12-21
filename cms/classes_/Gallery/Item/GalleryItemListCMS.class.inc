<?php
Site::IncludeFile ( 'classes/Common/CMSListObject.class.inc' );
//TODO corrent the include
Site::IncludeFile ( 'classes/Gallery/Item/GalleryItemCMS.class.inc' );
Site::IncludeFile ( 'classes/Gallery/Item/GalleryItemList.class.inc' );
Site::IncludeFile ( 'classes/Common/Image.class.inc' );
Site::IncludeFile ( 'classes/Folder/FolderXP.class.inc' );
/**
 * Enter class description
 */
class GalleryItemListCMS extends CMSListObject
{
	protected $m_galleryID;
	
	/**
	 * Enter description here...
	 */
	function __construct($galleryID = null)
	{
		parent::__construct ( new GalleryItemList ( $galleryID ), 'GalleryItem' );
		$this->m_galleryID = $galleryID;
		
		$this->ReadNameStrings ( 'classes/Gallery/langs/', 'gallery_item.xml' );
		
		$this->Update ();
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
		
		// call DoAction of GalleryItemCMS
		$overLink = $this->m_list->GetLinkIDName ();
		$owerLinkID = DB::POST ( $overLink );
		if ($owerLinkID)
		{
			$galleryItemCMS = new GalleryItemCMS ( $owerLinkID );
			$ar = $galleryItemCMS->DoAction ();
		}
		
		$gallery = new Gallery ( $this->m_galleryID );
		
		// do actions for GalleryItem list
		switch ($this->GetFormAction ())
		{
			case 'uploadImage' :
				// upload file to folder according to product ID
				$path = $this->UploadPicture ( $this->m_galleryID . '/' );
				
				$proURL = $this->m_site->GetGalleryImageRoot ( false );
				$fullPath = $proURL . "$this->m_galleryID/" . $path;
				
				if (is_string ( $path ))
				{
					// insert new pr_picture with name
					$newGalleryItem = new GalleryItem ( );
					$params = array ('galleryID' => $this->m_galleryID, 'path' => $fullPath);
					$ar = $newGalleryItem->Insert ( $params );
				}
				break;
			case 'setpath' :
				
				$path = DB::POST ( 'path' );
				
				if (strlen ( trim ( $path ) ) > 0)
				{
					$newGalleryItem = new GalleryItem ( );
					$params = array ('galleryID' => $this->m_galleryID, 'path' => $path, 'isuploaded' => 0 );
					$ar = $newGalleryItem->Insert ( $params );
					
					// get gallery
					$prPic = $gallery->GetThumbnail ();
					
					if ($prPic == 'img/thumbnail.gif')
					{
						$gallery->SetThumbnail ( $path );
					}
				}
				break;
			case 'pictureGroupAction' :
				$IDs = DB::POST ( 'picIDs' );
				
				if ($IDs)
				{
					// delete pictures
					if (isset ( $_POST ['delete'] ))
					{
						foreach ( $IDs as $ID )
						{
							$glPicture = new GalleryItem ( $ID );
							$ar = $glPicture->Delete ( false, true );
						}
					}
					
					// reposition
					$newPos = DB::POST ( 'newPos' );
					if ($newPos !== null)
					{
						DBxp::Reposition ( 'gallery_item', $IDs, $newPos, "`galleryID` = {$this->m_galleryID}" );
						$ar = $this->Update ();
					}
				}
				break;
			case 'selectDefPic' :
				$defPicID = DB::POST ( 'defpic' );
				
				if ($defPicID)
				{
					$pic = new GalleryItem ( $defPicID );
					
					$defPath = $pic->GetPath ();
					$pos = strrpos ( $defPath, '.' );
					$esxt = substr ( $defPath, $pos + 1 );
					
					//
					$prID = $gallery->GetID ();
					$prLocalRoot = $this->m_site->GetGalleryImageRoot ( true );
					$prGlobalRoot = $this->m_site->GetGalleryImageRoot ( false );
					
					$prLocalRoot .= $prID . '.' . $esxt;
					$prGlobalRoot .= $prID . '.' . $esxt;
					
					// create icon
					$im = new Image ( );
					$im->ChangeSizeInBox ( $defPath, 200, 200, $prLocalRoot );
					
					$ar = $gallery->SetThumbnail ( $prGlobalRoot );
				}
				else
				{
					$gallery->SetDefPicNull ();
				}
				
				break;
		}
		
		// if there is a any action then update object  
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
		$currID = $this->m_list->GetCurrentID ();
		
		if ($currID)
		{
			$linkName = $this->m_list->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>GalleryItem</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">GalleryItem</div>' );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_list->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// get names
		$extURLS = $this->GetNameString ( 'exFileURL' );
		$imageOptionsS = $this->GetNameString ( 'imageOptions' );
		$uploadS = $this->GetNameString ( 'upload' );
		$uploadFileS = $this->GetNameString ( 'uploadFile' );
		$moveS = $this->GetNameString ( 'move' );
		$repositionS = $this->GetNameString ( 'reposition' );
		/////////////////////////////////////////
		print ( '<br />' );
		$currentGalleryItemID = $this->m_list->GetCurrentID ();
		
		// get cooretn pr_picture
		$pr_picture = new GalleryItem ( $currentGalleryItemID );
		
		$currentGalleryItemID = $pr_picture->GetID ();
		
		$pictureOverLink = $this->m_list->GetLinkIDName ();
		//display upload //////////////////////////////////////////////////////////
		$className = $this->GetFormActionName ();
		
		$this->DisplaySingleTextForm ( 'setpath', $extURLS, 'path', null, array ('galleryID' => $this->m_galleryID ) );
		print ( '<br />' );
		
		// get upload maxi size
		$maxFileSize = ini_get ( 'upload_max_filesize' );
		
		print ( '<div class="titleBox">' );
		print ( '<form action="#pictures" method="POST" class="clearPD" enctype="multipart/form-data">' );
		$this->DisplayHidden ( 'galleryID', $this->m_galleryID );
		$this->DisplayHidden ( $className, 'uploadImage' );
		
		print ( '<div class="titleColL"> 
            		<label><strong>' . $uploadFileS . ' </strong></label>
            		<input type="file" name="gl2_img" size="48" />' );
		print ( "<span>[ Max = $maxFileSize ]</span>" );
		
		print ( '</div>
		            <div class="titleColR"><input name="" type="submit" value="' . $uploadS . '" /></div>
		            <br class="clearFloat" />
		        	</form>
		        	</div>' );
		
		// display imtem CMS display feature
		// if is selected pr_picture, then diplay pr_picture features
		if ($currentGalleryItemID)
		{
			print ( '<div class="someGTitleBox">' . $imageOptionsS . '</div>
        			<div class="someWBox">' );
			$galleryItemCMS = new GalleryItemCMS ( $currentGalleryItemID );
			$galleryItemCMS->DisplayFeatures ();
			print ( '</div>' );
		
		}
		// display all pictures in list	////////////////////////////////////////////////////////		
		$picCount = $this->m_list->GetItemCount ();
		
		if ($picCount > 0)
		{
			$pattern = "<div class='mediaImgBox'>
		                    <div class='mediaImgUnit'>
		           			<a href='?subgalleryID={$this->m_galleryID}&$pictureOverLink=@@ID@@#imageDescription'>         
		        			<img src='files/ImageMeaker.php?w=100&h=80&p=@@path@@' border='0' />
		        			</a>
		        			</div>
		                    <div class='mediaImgChackbox'>
		                    <input name='picIDs[]' id='picIDs[]' type='checkbox' value='@@ID@@' />
		                    </div> 
		                    <div class='mediaImgTitle'><span>@@path@@</span></div>
		                </div>";
			
			print ( '<br /><a name="pictures"></a>' );
			print ( '<div class="titleBox">' );
			
			// form for repostion /////////////////////////////////////////
			$this->DisplayFormHeadr ( 'pictureGroupAction', 'pictureForm' );
			$this->DisplayHidden ( 'galleryID', $this->m_galleryID );
			print ( '<div class="titleColL">
					<label><strong>' . $repositionS . '</strong></label>
					<select name="newPos" id="newPos">' );
			
			for($index = 0; $index < $picCount; $index ++)
			{
				$indexName = $index + 1;
				print ( "<option value='$index'>$indexName</option>\n" );
			}
			print ( '</select>' );
			print ( "&nbsp;<input name='move' type='submit' value='$moveS' />" );
			print ( '</div>' );
			//////////////////////////////////////////////////////////////
			

			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$noselectedS = $this->GetNameString ( 'noselected' );
			$deleteS = $this->GetNameString ( 'delete' );
			$allS = $this->GetNameString ( 'all' );
			$noneS = $this->GetNameString ( 'none' );
			$selectS = $this->GetNameString ( 'select' );
			?>
<div class="titleColR"><span style="padding-right: 20px"> <strong><?php echo $selectS?></strong>
<a href="#"
	onclick="chekeBoxes('pictureForm', 'picIDs[]', true); return false"><?php echo $allS?></a>,
<a href="#"
	onclick="chekeBoxes('pictureForm', 'picIDs[]', false); return false"><?php echo $noneS?></a>
</span> <input name="delete" type="submit"
	onclick="return ChecksFormDelete('pictureForm', 'picIDs[]', '<?php echo $noselectedS?>', '<?php echo $sureDeleteS?>');"
	value="<?php echo $deleteS?>" /></div>
<br class="clearFloat" />
</div>
<table width="100%" border="0" cellspacing="0" cellpadding="5"
	class="mediaFileAreArea">
	<tr>
		<td>
                <?php
			$this->m_list->DisplayListDiv ( $pattern );
			?>
                <br class="clearFloat" />
		</td>
	</tr>
</table>
</form>
<?php
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Update()
	{
		$this->m_list = new GalleryItemList ( $this->m_galleryID );
	}
	
	/**
	 * Upload picture
	 * 
	 * @param string $addPath 
	 * @return ActionResult
	 */
	function UploadPicture($addPath = null)
	{
		// get product folder, if not exist create it
		$galleryFolderPath = '../' . $this->m_site->GetGalleryImageRoot ( true, true );
		// if addition link is given, then add to URL
		if ($addPath)
		{
			$galleryFolderPath .= $addPath;
		}
		
		// if folder do not exist then try to create
		if (! file_exists ( $galleryFolderPath ))
		{
			//create forlder
			$res = FolderXP::Mkdir ( $galleryFolderPath, 0753 );
			
			if (! $res)
			{
				return false;
			}
		}
		
		// get upload file properties
		$name = str_replace ( " ", "", $_FILES ['gl2_img'] ['name'] );
		$size = str_replace ( " ", "", $_FILES ['gl2_img'] ['size'] );
		
		// image size is 0 , then return 
		if ($size < 1)
		{
			return false;
		}
		
		// get uploaded file extension
		$path_parts = pathinfo ( $_FILES ['gl2_img'] ['name'] );
		$ext = $path_parts ['extension'];
		$ext = strtolower ( $ext );
		
		// replase uper chares to lower
		$pos = strripos ( $name, $ext );
		$name = substr ( $name, 0, $pos ) . $ext;
		
		//////////////////////////////////////////////////////
		

		// all posible extentions
		$types = array ('jpg', 'jpeg', 'png', 'gif' );
		$res = array_search ( $ext, $types );
		
		if (! is_int ( $res ))
		{
			return false;
		}
		//////////////////////////////////////////////////////
		

		$fileAddress = $galleryFolderPath . $name;
		if (file_exists ( $fileAddress ))
		{
			return false;
		}
		
		// uploading
		FolderXP::Upload ( $_FILES ['gl2_img'] ['tmp_name'], $fileAddress );
		
		//// if file is image and most be resize, create Image class item and resize image
		$maxWidth = $this->m_site->GetMaxImageWidth ();
		$image = new Image ( );
		$image->ChangeSizeInBox ( $fileAddress, $maxWidth );
		////
		
		$newFileAddress = $galleryFolderPath . 'thumbnail_' . $name;
		$image->ChangeSizeInBox ( $fileAddress, 160, null,  $newFileAddress);

		return $name;
	}
	
	/**
	 * delete all pictures of product and folder 
	 *
	 * @return bool
	 */
	function Delete($writeToDB = true)
	{
		$toStore = null;
		$allPics = $this->m_list->GetStructure ();
		
		if ($allPics)
		{
			foreach ( $allPics as $picA )
			{
				
				$pic = new GalleryItem ( $picA ['ID'] );
				$value = $pic->Delete ( $writeToDB );
				
				if ($writeToDB)
				{
					$toStore = $value;
				}
				else
				{
					if (! $toStore)
					{
						$toStore = array ();
					}
					
					array_push ( $toStore, $value );
				}
			}
		}
		
		// delete folder
		$path = $this->m_site->GetGalleryImageRoot ();
		$path .= $this->m_galleryID . '/';
		// remofe file
		if (file_exists ( $path ))
		{
			FolderXP::Rmdir ( $path );
		}
		
		return $toStore;
	}
	
	/**
	 * Delete all images and folder
	 *
	 * @return unknown
	 */
	function FullDelete()
	{
		$ar = true;
		// delete all images
		$strct = $this->m_list->GetStructure();
		if( count($strct) > 0)
		{
			foreach ( $strct as $item)
			{
				$glPicture = new GalleryItem ( $item['ID']);
				$ar &= $glPicture->Delete ( false, true );
			}
		}
		
		// remove dir
		$folderPath = '../' . $this->m_site->GetGalleryImageRoot(true, true);
		$folderPath .= $this->m_galleryID . '/';
		
		$ar &= FolderXP::Rmdir($folderPath);
		
		return $ar;
	}
}
?>