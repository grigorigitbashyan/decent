<?php
Site::IncludeFile ( 'classes/Common/CMSListObject.class.inc' );
//TODO corrent the include
Site::IncludeFile ( 'classes/Videogallery/Item/VideogalleryItemCMS.class.inc' );
Site::IncludeFile ( 'classes/Videogallery/Item/VideogalleryItemList.class.inc' );
Site::IncludeFile ( 'classes/Common/Image.class.inc' );
Site::IncludeFile ( 'classes/Folder/FolderXP.class.inc' );
/**
 * Enter class description
 */
class VideogalleryItemListCMS extends CMSListObject
{
	protected $m_videogalleryID;
	
	/**
	 * Enter description here...
	 */
	function __construct($videogalleryID = null)
	{
		parent::__construct ( new VideogalleryItemList ( $videogalleryID ), 'VideogalleryItem' );
		$this->m_videogalleryID = $videogalleryID;
		
		$this->ReadNameStrings ( 'classes/Videogallery/langs/', 'videogallery_item.xml' );
		
		$this->Update ();
	}
	
	
	/**
	 * Enter description here...
	 *
	 * @return random string
	 */
	function genRandomString($length = 10)
	{
//	    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
	    $characters[] = '0';
	    $characters[] = '1';
	    $characters[] = '2';
	    $characters[] = '3';
	    $characters[] = '4';
	    $characters[] = '5';
	    $characters[] = '6';
	    $characters[] = '7';
	    $characters[] = '8';
	    $characters[] = '9';
	    
	    $string = '';    
	
	    for ($p = 0; $p < $length; $p++) {
	    	
//	        $string .= $characters[mt_rand(0, strlen($characters))];
			$char = intval($characters[rand(0, count($characters))]);
	        $string .= "".$char."";
	    }
	
	    return $string;
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
		
		// call DoAction of VideogalleryItemCMS
		$overLink = $this->m_list->GetLinkIDName ();
		$owerLinkID = DB::POST ( $overLink );
		if ($owerLinkID)
		{
			$videogalleryItemCMS = new VideogalleryItemCMS ( $owerLinkID );
			$ar = $videogalleryItemCMS->DoAction ();
		}
		
		$videogallery = new Videogallery ( $this->m_videogalleryID );
		
		// do actions for VideogalleryItem list
		switch ($this->GetFormAction ())
		{
			case 'insert':
				$videoName = trim(DB::POST("videoName"));
				$videoID = $this->genRandomString(6);
				
				$params = array ('videogalleryID' => $this->m_videogalleryID, 'videoID' => $videoID);
				$newVideogalleryItem = new VideogalleryItem ( );
				$ar = $newVideogalleryItem->Insert ( $params, $videoName );
				
				break;
			case 'uploadImage' :
				// upload file to folder according to product ID
//				$path = $this->UploadPicture ( $this->m_videogalleryID . '/' );
				$path = "";
				
				$proURL = $this->m_site->GetVideogalleryImageRoot ( false );
				$fullPath = $proURL["demo"] . "$this->m_videogalleryID/" . $path;
				
				$videoID = $this->genRandomString(6);
				
				if (is_string ( $path ))
				{
					// insert new pr_picture with name
					$newVideogalleryItem = new VideogalleryItem ( );
//					$params = array ('videoID' => $videoID, 'videogalleryID' => $this->m_videogalleryID, 'path' => $fullPath);
					$params = array ('videoID' => $videoID, 'videogalleryID' => $this->m_videogalleryID, 'path' => $proURL["demo"]);
					$ar = $newVideogalleryItem->Insert ( $params );
					
					$newVideoID = DB::GetLastID();
					
					$this->UploadPicture ( $this->m_videogalleryID . '/', $newVideoID );
				}
				break;
			case 'setpath' :
				
				$path = DB::POST ( 'path' );
				
				if (strlen ( trim ( $path ) ) > 0)
				{
					$newVideogalleryItem = new VideogalleryItem ( );
					$params = array ('videogalleryID' => $this->m_videogalleryID, 'path' => $path, 'isuploaded' => 0 );
					$ar = $newVideogalleryItem->Insert ( $params );
					
					// get videogallery
					$prPic = $videogallery->GetThumbnail ();
					
					if ($prPic == 'img/thumbnail.gif')
					{
						$videogallery->SetThumbnail ( $path );
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
							$glPicture = new VideogalleryItem ( $ID );
							$ar = $glPicture->Delete ( false, true );
						}
					}
					
					// reposition
					$newPos = DB::POST ( 'newPos' );
					if ($newPos !== null)
					{
						DBxp::Reposition ( 'videogallery_item', $IDs, $newPos, "`videogalleryID` = {$this->m_videogalleryID}" );
						$ar = $this->Update ();
					}
				}
				break;
			case 'selectDefPic' :
				$defPicID = DB::POST ( 'defpic' );
				
				if ($defPicID)
				{
					$pic = new VideogalleryItem ( $defPicID );
					
					$defPath = $pic->GetPath ();
					$pos = strrpos ( $defPath, '.' );
					$esxt = substr ( $defPath, $pos + 1 );
					
					//
					$prID = $videogallery->GetID ();
					$prLocalRoot = $this->m_site->GetVideogalleryImageRoot ( true );
					$prGlobalRoot = $this->m_site->GetVideogalleryImageRoot ( false );
					
					$prLocalRoot .= $prID . '.' . $esxt;
					$prGlobalRoot .= $prID . '.' . $esxt;
					
					// create icon
					$im = new Image ( );
					$im->ChangeSizeInBox ( $defPath, 200, 200, $prLocalRoot );
					
					$ar = $videogallery->SetThumbnail ( $prGlobalRoot );
				}
				else
				{
					$videogallery->SetDefPicNull ();
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
			print ( "<div class='someWTitleBox'><a href='?$linkName='>VideogalleryItem</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">VideogalleryItem</div>' );
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
		$doneS = $this->GetNameString ( 'done' );
		$uploadFileS = $this->GetNameString ( 'uploadFile' );
		$moveS = $this->GetNameString ( 'move' );
		$repositionS = $this->GetNameString ( 'reposition' );
		/////////////////////////////////////////
		print ( '<br />' );
		$currentVideogalleryItemID = $this->m_list->GetCurrentID ();
		
		// get cooretn pr_picture
		$pr_picture = new VideogalleryItem ( $currentVideogalleryItemID );
		
		$currentVideogalleryItemID = $pr_picture->GetID ();
		
		$pictureOverLink = $this->m_list->GetLinkIDName ();
		//display upload //////////////////////////////////////////////////////////
		$className = $this->GetFormActionName ();
		
//		$this->DisplaySingleTextForm ( 'setpath', $extURLS, 'path', null, array ('videogalleryID' => $this->m_videogalleryID ) );
//		print ( '<br />' );
		
		// get upload maxi size
		$maxFileSize = ini_get ( 'upload_max_filesize' );
		
		print ( '<div class="titleBox">' );
		print ( '<form action="#pictures" method="POST" class="clearPD" enctype="multipart/form-data">' );
		$this->DisplayHidden ( 'videogalleryID', $this->m_videogalleryID );
		$this->DisplayHidden ( $className, 'insert' );
		
		print ( '<div class="titleColL"> 
            		<label><strong>Title</strong></label>
            		<input type="text" name="videoName" size="48" />
            		</div>' );
		
		print ( '<div class="titleColR"><input name="" type="submit" value="' . $doneS . '" /></div>
		            <br class="clearFloat" />
		        	</form>
		        	</div>' );
		
		// display imtem CMS display feature
		// if is selected pr_picture, then diplay pr_picture features
		if ($currentVideogalleryItemID)
		{
			print ( '<div class="someGTitleBox">' . $imageOptionsS . '</div>
        			<div class="someWBox">' );
			$videogalleryItemCMS = new VideogalleryItemCMS ( $currentVideogalleryItemID );
			$videogalleryItemCMS->DisplayFeatures ();
			print ( '</div>' );
		
		}
		// display all pictures in list	////////////////////////////////////////////////////////		
		$picCount = $this->m_list->GetItemCount ();
		
		if ($picCount > 0)
		{
			$pattern = "<div class='mediaImgBox'>
		                    <div class='mediaImgUnit'>
			           			<a href='?subvideogalleryID={$this->m_videogalleryID}&$pictureOverLink=@@ID@@#imageDescription'><img src='@@videoCodeImg@@' border='0' /></a>
		        			</div>
		                    <div class='mediaImgChackbox'>
			                    <input name='picIDs[]' id='picIDs[]' type='checkbox' value='@@ID@@' />
		                    </div> 
		                    <div class='mediaImgTitle'><span>@@name@@</span></div>
		                </div>";
			
			print ( '<br /><a name="pictures"></a>' );
			print ( '<div class="titleBox">' );
			
			// form for repostion /////////////////////////////////////////
			$this->DisplayFormHeadr ( 'pictureGroupAction', 'pictureForm' );
			$this->DisplayHidden ( 'videogalleryID', $this->m_videogalleryID );
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
		$this->m_list = new VideogalleryItemList ( $this->m_videogalleryID );
	}
	
	/**
	 * Upload picture
	 * 
	 * @param string $addPath 
	 * @return ActionResult
	 */
	function UploadPicture($addPath = null, $videoID = 0)
	{
		// get product folder, if not exist create it
//		$videogalleryFolderPath = '../' . $this->m_site->GetVideogalleryImageRoot ( true, true );
		$videogalleryFolderPath = $this->m_site->GetVideogalleryImageRoot ( true, true );
		// if addition link is given, then add to URL
		if ($addPath)
		{
			//$videogalleryFolderPath .= $addPath;
		}
		
		if ( $videoID == 0 ) {
			
			return false;
		}
		
		// if folder do not exist then try to create
		if (! file_exists ( $videogalleryFolderPath["demo"] ))
		{
			//create forlder
			$res = FolderXP::Mkdir ( $videogalleryFolderPath["demo"], 0753 );
			
			if (! $res)
			{
				return false;
			}
		}
		
		//		DEMO VERSION
		// get upload file properties
		$name = str_replace ( " ", "", $_FILES ['gl2_img_demo'] ['name'] );
		$size = str_replace ( " ", "", $_FILES ['gl2_img_demo'] ['size'] );
		
		// image size is 0 , then return 
		if ($size > 0)
		{
			// get uploaded file extension
			$path_parts = pathinfo ( $_FILES ['gl2_img_demo'] ['name'] );
			$ext = $path_parts ['extension'];
			$ext = strtolower ( $ext );
			
			// replase uper chares to lower
			$pos = strripos ( $name, $ext );
			$name = substr ( $name, 0, $pos ) . $ext;
			
			//////////////////////////////////////////////////////
			
			// all posible extentions
	//		$types = array ('jpg', 'jpeg', 'png', 'gif' );
			$types = array ('flv');
			$res = array_search ( $ext, $types );
//			$fileAddress = $videogalleryFolderPath . "demo/" . $name;
			$fileAddress = $videogalleryFolderPath["demo"] . $videoID . "." . $types[$res];
			
			if ( is_int ( $res ) && !file_exists ( $fileAddress ))
			{
				FolderXP::Upload ( $_FILES ['gl2_img_demo'] ['tmp_name'], $fileAddress );
			}
			
		}
		
		//// if file is image and most be resize, create Image class item and resize image
		/*$maxWidth = $this->m_site->GetMaxImageWidth ();
		$image = new Image ( );
		$image->ChangeSizeInBox ( $fileAddress, $maxWidth );
		////
		
		$newFileAddress = $videogalleryFolderPath . 'thumbnail_' . $name;
		$image->ChangeSizeInBox ( $fileAddress, 160, null,  $newFileAddress);*/
		
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
				
				$pic = new VideogalleryItem ( $picA ['ID'] );
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
		$path = $this->m_site->GetVideogalleryImageRoot ();
		$path = $path["demo"];
		$path .= $this->m_videogalleryID . '/';
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
				$glPicture = new VideogalleryItem ( $item['ID']);
				$ar &= $glPicture->Delete ( false, true );
			}
		}
		
		// remove dir
		$folderPath = '../' . $this->m_site->GetVideogalleryImageRoot(true, true);
		$folderPath = $folderPath["demo"];
		$folderPath .= $this->m_videogalleryID . '/';
		
		$ar &= FolderXP::Rmdir($folderPath);
		
		return $ar;
	}
}
?>