<?php
ini_set('memory_limit', '-1');
// inculdes
Site::IncludeFile ( 'classes/Product/pr_picture/Pr_picture.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_picture/Pr_pictureCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_picture/Pr_pictureList.class.inc' );
Site::IncludeFile ( 'classes/Common/DBxp.class.inc' );
/**
 * Enter description here...
 * 
 * @package Item
 */
class Pr_pictureListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_pictureList
	 */
	protected $m_pr_pictureList;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_productID;
	
	function __construct($productID)
	{
		parent::__construct ();
		
		$this->m_productID = $productID;
		
		$this->Update ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function WaterMark($image) {
		
		if ( strpos($image, "media/")!==false ) {
			
			$image = "../".substr($image, strpos($image, "media/"));
		}
		
		$imagesource =  $image;
		
		$filetype = substr($imagesource,strlen($imagesource)-4,4);
		$filetype = strtolower($filetype);
		
		if ($filetype == ".gif")  $image = @imagecreatefromgif($imagesource);
		if ($filetype == ".jpg")  $image = imagecreatefromjpeg($imagesource);
		if ($filetype == "jpeg")  $image = imagecreatefromjpeg($imagesource);
		if ($filetype == ".png")  $image = @imagecreatefrompng($imagesource);
		
		$filetype = str_replace(".", "", $filetype);
		
		if (!$image) {
			
			
		} else {
			
			$watermark = @imagecreatefrompng('watermark.png');
			
			$imagewidth = imagesx($image);
			$imageheight = imagesy($image);
			
			$watermarkwidth =  imagesx($watermark);
			$watermarkheight =  imagesy($watermark);
			
			//	top center
			$startwidth = ceil($imagewidth/2) - ceil($watermarkwidth/2);
			$startheight = 10;
			
			imagecopy($image, $watermark,  $startwidth, $startheight, 0, 0, $watermarkwidth, $watermarkheight);
			imagejpeg($image, $imagesource);
			
			imagedestroy($image);
			imagedestroy($watermark);
		}
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
		
		// call DoAction of Pr_pictureCMS
		$overLink = $this->m_pr_pictureList->GetLinkIDName ();
		if (($owerLinkID = DB::POST ( $overLink )))
		{
			$pr_pictureCMS = new Pr_pictureCMS ( $owerLinkID );
			$ar = $pr_pictureCMS->DoAction ();
		}
		
		$product = new Pr_main ( $this->m_productID );
		
		// do actions for Pr_picture list
		switch ($this->GetFormAction ())
		{
			case 'uploadImage' :
				$uploadedCount = count($_FILES['gl2_img']['name']);
				for ( $i=0; $i<$uploadedCount; $i++ )
				{
					$imageID = rand(111111, 999999);
					$path = $this->UploadPicture ( $this->m_productID . '/', $imageID, $i );
					if ( $path )
					{
						// insert new pr_picture with name
						$newPr_picture = new Pr_picture ( );
						$params = array ('productID' => $this->m_productID, 'path' => "", 'iconPath' => "");
						$ar = $newPr_picture->Insert ( $params );

						// upload icon to folder according product ID
						if ( $pos = strrpos($path, ".") )
						{
							$defPath = substr($path, 0, $pos)."-icon".substr($path, $pos);
							$pos = strrpos ( $defPath, '.' );
							$esxt = substr ( $defPath, $pos + 1 );
							
							$PR = new Pr_main($this->m_productID);
							$prTypeID = $PR->GetTypeID();
							
							//	get icon parameters from DB
							$prType = new Pr_type($prTypeID);
							$imageSize = $prType->GetThumbnailSize();
							$imageSize = explode(";", $imageSize);
							$DPW = $imageSize[0];
							$DPH = $imageSize[1];
							$cropImage = intval($prType->GetThumbnailCrop());
							
							$prLocalRoot = $this->m_site->GetProImageRoot ( true ) . $this->m_productID . '/' . $defPath;
							$prGlobalRoot = $this->m_site->GetProImageRoot ( false ) . $this->m_productID . '/' . $defPath;
							
							// create icon
							$im = new Image ( );
							$im->ChangeSizeInBox ( $this->m_site->GetProImageRoot ( true ) . $this->m_productID . '/' .$path, 1920, 1920, $prLocalRoot );
							
							//	added by Grigor		start
							if ( $cropImage )
							{
								list ( $width, $height ) = getimagesize ( $prLocalRoot );
								if ( $width*$DPH > $height*$DPW ) {
									$width = round($width * $DPH / $height);
									$height = $DPH;
									$ch = 0;
									$cW = ceil(($width - $DPW) / 2);
								} else {
									$height = round($height * $DPW / $width);
									$width = $DPW;
									$cW = 0;
									$ch = 0;
								}
								$im->ChangeImageSize( $prLocalRoot, $width, $height, $prLocalRoot);
								
								switch ( $esxt )
								{
									case 'jpg':
									case 'jpeg':
										$source = imagecreatefromjpeg($prLocalRoot);
										break;
									case 'png':
										$source = imagecreatefrompng($prLocalRoot);
										break;
									case 'gif':
										$source = imagecreatefromgif($prLocalRoot);
										break;
								}
								$new = imagecreatetruecolor($DPW, $DPH);
								
								// Copy
								imagecopy($new, $source, 0, 0, $cW, $ch, $DPW, $DPH);
								switch ( $esxt )
								{
									case 'jpg':
									case 'jpeg':
										imagejpeg ( $new, $prLocalRoot, 99 );
										break;
									case 'png':
										imagepng( $new, $prLocalRoot, 9 );
										break;
									case 'gif':
										imagegif( $new, $prLocalRoot, 99 );
										break;
								}
							}
							else 
							{
								list ( $width, $height ) = getimagesize ( $prLocalRoot );
								$DPH = round($height * $DPW / $width);
								$im->ChangeImageSize( $prLocalRoot, $DPW, $DPH, $prLocalRoot);
							}
							//	added by Grigor		end
							
							$newPr_picture->SetIconPath($prGlobalRoot);
						}
						
						$proURL = $this->m_site->GetProImageRoot ( false );
						$fullPath = $proURL . "$this->m_productID/" . $path;
						
						$newPr_picture->SetPath($fullPath);
					}
				}
				break;
			case 'setpath' :
				$path = DB::POST ( 'path' );
				if (strlen ( trim ( $path ) ) > 0)
				{
					$newPr_picture = new Pr_picture ( );
					$params = array ('productID' => $this->m_productID, 'path' => $path, 'isuploaded' => 0 );
					$ar = $newPr_picture->Insert ( $params );
					
					// get product
					$prPic = $product->GetPicture ();
					
					if ($prPic == 'img/thumbnail.gif')
					{
						$product->SetPicture ( $path );
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
							$prPicture = new Pr_picture ( $ID );
							$ar = $prPicture->Delete ( false, true );
						}
					}
					
					// add watermark
					if (isset ( $_POST ['addwatermark'] ))
					{
						foreach ( $IDs as $ID )
						{
							$prPicture = new Pr_picture ( $ID );
							$this->WaterMark($prPicture->GetPath());
						}
					}
					
					// reposition
					if (($newPos = DB::POST ( 'newPos' )))
					{
						DBxp::Reposition ( 'pr_picture', $IDs, $newPos, "`productID` = {$this->m_productID}" );
					}
				}
				break;
			case 'selectDefPic' :
				$defPicID = DB::POST ('defpic');
				if ($defPicID)
				{
					$pic = new Pr_picture ( $defPicID );
					
					$defPath = $pic->GetPath ();
					$pos = strrpos ( $defPath, '.' );
					$esxt = substr ( $defPath, $pos + 1 );
					
					$PR = new Pr_main($this->m_productID);
					$prTypeID = $PR->GetTypeID();
					
					//	get icon parameters from DB
					$prType = new Pr_type($prTypeID);
					$imageSize = $prType->GetIconSize();
					$imageSize = explode(";", $imageSize);
					$DPW = $imageSize[0];
					$DPH = $imageSize[1];
					$cropImage = $prType->GetIconCrop();
					
					$prID = $product->GetID ();
					$prLocalRoot = $this->m_site->GetProImageRoot ( true );
					$prGlobalRoot = $this->m_site->GetProImageRoot ( false );
					
					$prLocalRoot .= $prID . '.' . $esxt;
					$prGlobalRoot .= $prID . '.' . $esxt;
					
					// create icon
					$im = new Image ( );
					$im->ChangeSizeInBox ( $defPath, 1280, 1280, $prLocalRoot );
					
					//	added by Grigor		start
					if ( $cropImage )
					{
						list ( $width, $height ) = getimagesize ( $prLocalRoot );
						if ( $width*$DPH > $height*$DPW ) {
							$width = round($width * $DPH / $height);
							$height = $DPH;
							$ch = 0;
							$cW = ceil(($width - $DPW) / 2);
						} else {
							$height = round($height * $DPW / $width);
							$width = $DPW;
							$cW = 0;
							$ch = 0;
						}
						$im->ChangeImageSize( $prLocalRoot, $width, $height, $prLocalRoot);
						
						switch ( $esxt )
						{
							case 'jpg':
							case 'jpeg':
								$source = imagecreatefromjpeg($prLocalRoot);
								break;
							case 'png':
								$source = imagecreatefrompng($prLocalRoot);
								break;
							case 'gif':
								$source = imagecreatefromgif($prLocalRoot);
								break;
						}
						$new = imagecreatetruecolor($DPW, $DPH);
						
						// Copy
						imagecopy($new, $source, 0, 0, $cW, $ch, $DPW, $DPH);
						switch ( $esxt )
						{
							case 'jpg':
							case 'jpeg':
								imagejpeg ( $new, $prLocalRoot, 99 );
								break;
							case 'png':
								imagepng( $new, $prLocalRoot, 9 );
								break;
							case 'gif':
								imagegif( $new, $prLocalRoot, 99 );
								break;
						}
					}
					else 
					{
						list ( $width, $height ) = getimagesize ( $prLocalRoot );
						$DPH = round($height * $DPW / $width);
						$cW = 0;
						$ch = 0;
						$im->ChangeImageSize( $prLocalRoot, $DPW, $DPH, $prLocalRoot);
					}
					//	added by Grigor		end
					
					$ar = $product->SetPicture ( $prGlobalRoot );
				}
				else
				{
					$product->SetDefPicNull ();
				}
				
				break;
			case 'selectCoverPic' :
				ini_set('memory_limit', '-1');
				$defPicID = DB::POST ('coverpic');
				if ($defPicID)
				{
					$pic = new Pr_picture ( $defPicID );
					
					$defPath = $pic->GetPath ();
					$pos = strrpos ( $defPath, '.' );
					$esxt = substr ( $defPath, $pos + 1 );
					
					$PR = new Pr_main($this->m_productID);
					$prTypeID = $PR->GetTypeID();
					
					//	get icon parameters from DB
					$prType = new Pr_type($prTypeID);
					$imageSize = $prType->GetCoverSize();
					$imageSize = explode(";", $imageSize);
					$DPW = $imageSize[0];
					$DPH = $imageSize[1];
					$cropImage = $prType->GetCoverCrop();
					
					$prID = $product->GetID ();
					$prLocalRoot = $this->m_site->GetProImageRoot ( true );
					$prGlobalRoot = $this->m_site->GetProImageRoot ( false );
					
					$prLocalRoot .= $prID . '_cover.' . $esxt;
					$prGlobalRoot .= $prID . '_cover.' . $esxt;
					
					// create icon
					$im = new Image ( );
					$im->ChangeSizeInBox ( $defPath, 1280, 1280, $prLocalRoot );
					
					//	added by Grigor		start
					if ( $cropImage )
					{
						list ( $width, $height ) = getimagesize ( $prLocalRoot );
						if ( $width > $height*$DPW/$DPH ) {
							$width = round($width * $DPH / $height);
							$height = $DPH;
							$ch = 0;
							$cW = ceil(($width - $DPW) / 2);
						} else {
							$height = round($height * $DPW / $width);
							$width = $DPW;
							$cW = 0;
							$ch = 0;
						}
						$im->ChangeImageSize( $prLocalRoot, $width, $height, $prLocalRoot);
						
						switch ( $esxt )
						{
							case 'jpg':
							case 'jpeg':
								$source = imagecreatefromjpeg($prLocalRoot);
								break;
							case 'png':
								$source = imagecreatefrompng($prLocalRoot);
								break;
							case 'gif':
								$source = imagecreatefromgif($prLocalRoot);
								break;
						}
						$new = imagecreatetruecolor($DPW, $DPH);
						
						// Copy
						imagecopy($new, $source, 0, 0, $cW, $ch, $DPW, $DPH);
						switch ( $esxt )
						{
							case 'jpg':
							case 'jpeg':
								imagejpeg ( $new, $prLocalRoot, 99 );
								break;
							case 'png':
								imagepng( $new, $prLocalRoot, 9 );
								break;
							case 'gif':
								imagegif( $new, $prLocalRoot, 99 );
								break;
						}
					}
					else 
					{
						list ( $width, $height ) = getimagesize ( $prLocalRoot );
						$DPH = round($height * $DPW / $width);
						$cW = 0;
						$ch = 0;
						$im->ChangeImageSize( $prLocalRoot, $DPW, $DPH, $prLocalRoot);
					}
					//	added by Grigor		end
					
					$ar = $product->SetCover( $prGlobalRoot );
				}
				else
				{
					$product->SetCoverPicNull ();
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
		$currID = $this->m_pr_pictureList->GetCurrentID ();
		
		if ($currID)
		{
			$linkName = $this->m_pr_pictureList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>Pr_picture</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">Pr_picture</div>' );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_pr_pictureList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		print ( '<br />' );
		print ( "<div class='someGTitleBox'>Gallery</div>" );
		
		$currentPr_pictureID = $this->m_pr_pictureList->GetCurrentID ();
		
		// get current pr_picture
		$pr_picture = new Pr_picture ( $currentPr_pictureID );
		
		$currentPr_pictureID = $pr_picture->GetID ();
		
		// if is selected pr_picture, then diplay pr_picture features
		if ($currentPr_pictureID)
		{
			$pr_pictureCMS = new Pr_pictureCMS ( $currentPr_pictureID );
			$pr_pictureCMS->DisplayFeatures ();
		
		} // diplay new pr_picture form
		else
		{
			$pictureOverLink = $this->m_pr_pictureList->GetLinkIDName ();
			$product = new Pr_main ( $this->m_productID );
			$productTypeID = $product->GetTypeID ();
			
			//display upload
			$className = $this->GetFormActionName ();
			
			// get upload maxi size
			$maxFileSize = ini_get ( 'upload_max_filesize' );
			
			print ( '<div class="titleBox">' );
			print ( '<form action="#pictures" method="POST" class="clearPD" enctype="multipart/form-data">' );
			$this->DisplayHidden ( 'pr_mainID', $this->m_productID );
			$this->DisplayHidden ( $className, 'uploadImage' );
			
			print ( '<div class="titleColL"> 
            		<label><strong>File Upload (multiple files allowed): </strong></label>
            		<input type="file" name="gl2_img[]" size="48" multiple />' );
			print ( "<span>[ Max = $maxFileSize ]</span>" );
			
			print ( '</div>
		            <div class="titleColR"><input name="" type="submit" value="Upload" /></div>
		            <br class="clearFloat" />
		        	</form>
		        	</div>' );
			
			// display all pictures in list
			$picCount = $this->m_pr_pictureList->GetItemCount ();
			
			if ($picCount > 0)
			{
				$patern = "<div class='mediaImgBox'>
		                    <div class='mediaImgUnit'>
		           			<a href='?subpr_typeID={$productTypeID}&pr_mainID={$this->m_productID}&$pictureOverLink=@@ID@@#imageDescription'>         
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
				
				// form for making icon 
				if ( $productTypeID==32 )
				{
					$this->DisplayFormHeadr ( 'selectDefPicM' );
				}
				else 
				{
					$this->DisplayFormHeadr ( 'selectDefPic' );
				}
				$this->DisplayHidden ( 'pr_mainID', $this->m_productID );
				
				print ( '<div class="titleColL">
					<label><strong>Make icon from:</strong></label>
					<select name="defpic" id="defpic" onchange="submit();">' );
				
				// get default picture ID
				$this->m_pr_pictureList->DisplaySelect ( ' ... ' );
				
				print ( '</select></div></form>' );
				
				// form for making cover
				if ( $productTypeID==32 )
				{
					$this->DisplayFormHeadr ( 'selectCoverPicM' );
				}
				else 
				{
					$this->DisplayFormHeadr ( 'selectCoverPic' );
				}
				$this->DisplayHidden ( 'pr_mainID', $this->m_productID );
				
				print ( '<div class="titleColL">
					<label><strong>Make cover from:</strong></label>
					<select name="coverpic" id="cover" onchange="submit();">' );
				
				// get default picture ID
				$this->m_pr_pictureList->DisplaySelect ( ' ... ' );
				
				print ( '</select></div></form>' );
				
				// form for repostion
				$this->DisplayFormHeadr ( 'pictureGroupAction', 'pictureForm' );
				$this->DisplayHidden ( 'pr_mainID', $this->m_productID );
				print ( '<div class="titleColL"><label><strong>Repostion:</strong></label>
						<select name="newPos" id="newPos">' );
						for($index = 0; $index < $picCount; $index ++)
						{
							$indexName = $index + 1;
							print ( "<option value='$index'>$indexName</option>\n" );
						}
				print ( '</select>' );
				print ( "<input name='move' type='submit' value='Move' />" );
				print ( '</div>' );
				
				$sureDeleteS = $this->GetNameString ( 'sureDelete' );
				$noselectedS = $this->GetNameString ( 'noselected' );
				$deleteS = $this->GetNameString ( 'delete' );
				$allS = $this->GetNameString('all');
				$noneS = $this->GetNameString('none');
				$selectS = $this->GetNameString('select'); 
				?>
<div class="titleColR"><span style="padding-right: 20px"> <strong><?php echo $selectS?></strong>
<a href="#"	onclick="chekeBoxes('pictureForm', 'picIDs[]', true); return false"><?php echo $allS?></a>,
<a href="#"	onclick="chekeBoxes('pictureForm', 'picIDs[]', false); return false"><?php echo $noneS?></a>
</span> 
<input name="delete" type="submit" onclick="return ChecksFormDelete('pictureForm', 'picIDs[]', '<?php echo $noselectedS?>', '<?php echo $sureDeleteS?>');" value="<?php echo $deleteS?>" /></div>
<br class="clearFloat" />
</div>
<table width="100%" border="0" cellspacing="0" cellpadding="5"
	class="mediaFileAreArea">
	<tr>
		<td>
            <?php
			$this->m_pr_pictureList->DisplayListDiv ( $patern );
			?>
            <br class="clearFloat" />
		</td>
	</tr>
</table>
</form>
<?php
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Update()
	{
		$this->m_pr_pictureList = new Pr_pictureList ( $this->m_productID );
	}
	
	/**
	 * Upload picture
	 * 
	 * @param string $addPath 
	 * @return ActionResult
	 */
	function UploadPicture($addPath = null, $imageID, $i=0)
	{
		// get product folder, if not exist create it
		$prodFolderPath = '../' . $this->m_site->GetProImageRoot (true, false);
		// if addition link is given, then add to URL
		if ( $addPath )
		{
			$prodFolderPath .= $addPath;
		}
		
		// if folder do not exist then try to create
		if ( !file_exists($prodFolderPath) )
		{
			//create forlder
			$res = FolderXP::Mkdir( "$prodFolderPath", 0777 );
			
			if ( !$res )
			{
				return false;
			}
		}
		
		// get upload file properties
		$name = str_replace ( " ", "", $_FILES['gl2_img']['name'][$i] );
		$size = str_replace ( " ", "", $_FILES['gl2_img']['size'][$i] );
		
		// image size is 0 , then return 
		if ( $size < 1 )
		{
			return false;
		}
		
		// get uploaded file extension
		$path_parts = pathinfo ( $_FILES['gl2_img']['name'][$i] );
		$ext = $path_parts ['extension'];
		$ext = strtolower ( $ext );
		
		// replase uper chares to lower
		$pos = strripos ( $name, $ext );
//		$name = substr ( $name, 0, $pos ) . $ext;
		$name = $imageID.".".$ext;
		
		// all posible extentions
		$types = array ('jpg', 'jpeg', 'png', 'gif' );
		$res = array_search ( $ext, $types );
		
		if ( !is_int($res) )
		{
			return false;
		}
		
		$fileAddress = $prodFolderPath . $name;
		if ( file_exists($fileAddress) )
		{
			return false;
		}
		
		// uploading
		$res = FolderXP::Upload( $_FILES['gl2_img']['tmp_name'][$i], $fileAddress );

		//// if file is image and most be resize, create Image class item and resize image
		$maxWidth = $this->m_site->GetMaxImageWidth ();
		$image = new Image ( );
		$image->ChangeSizeInBox ( $fileAddress, $maxWidth );
		
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
		$allPics = $this->m_pr_pictureList->GetStructure ();
		
		if ($allPics)
		{
			foreach ( $allPics as $picA )
			{
				
				$pic = new Pr_picture ( $picA ['ID'] );
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
		$path = '../' . $this->m_site->GetProImageRoot (true, false);
		$path .= $this->m_productID . '/';
		// remofe file
		if (file_exists ( $path ))
		{
			FolderXP::Rmdir( $path );
		}
		
		return $toStore;
	}
}
?>