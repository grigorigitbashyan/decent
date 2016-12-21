<?php
Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );
//TODO corrent the include
Site::IncludeFile ( 'classes/Videogallery/Videogallery.class.inc' );
Site::IncludeFile ( 'classes/Videogallery/Item/VideogalleryItemListCMS.class.inc' );
site::IncludeFile('classes/Folder/FolderXP.class.inc');
/**
 * Enter class description
 */
class VideogalleryCMS extends CMSObject
{
	/**
	 * Description
	 *
	 * @var Videogallery
	 */
	protected $m_videogallery;
	
	/**
	 * @var VideogalleryItemListCMS
	 */
	protected $m_gallItemListCMS;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_videogallery = new Videogallery ( $ID );
		$this->m_gallItemListCMS = new VideogalleryItemListCMS ( $ID );
		
		$this->ReadNameStrings ( 'classes/Videogallery/langs/', 'videogallery.xml' );
	}
	/**
	 * Return sql statement to create table 
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$langs = $this->m_site->GetLanguages ();
		$langS = '';
		foreach ( $langs as $lang )
		{
			$langS .= "`name_{$lang}` varchar(255) NOT NULL,";
		}
		$str = "CREATE TABLE `videogallery` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  $langS
  `type` tinyint(4) NOT NULL DEFAULT '0',
  `thumbnail` varchar(255) NOT NULL DEFAULT 'img/thumbnail.gif',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8";
		
		return $str;
	}
	
	/* (non-PHPdoc)
	 * @see cms/classes/Common/CMSObject#CreateDB()
	 */
	function CreateDB()
	{
		$res = parent::CreateDB ();
		$res &= $this->m_gallItemListCMS->CreateDB ();
		
		return $res;
	}
	/**
	 * All actions must be done hear
	 *
	 * @return bool
	 */
	function DoAction()
	{
		$ar = $this->m_gallItemListCMS->DoAction ();
		
		if ($ar)
		{
			$this->m_gallItemListCMS = new VideogalleryItemListCMS ( $this->m_videogallery->GetID () );
		}
		
		$videogalleryID = $this->m_videogallery->GetID ();
		
		switch ($this->GetFormAction ())
		{
			case 'changeName' :
				$value = DB::POST ( 'Name' );
				$ar = $this->m_videogallery->SetName ( $value, $this->m_displayLang );
				break;
			case 'changeThumbnail' :
				$value = DB::POST ( 'Thumbnail' );
				$ar = $this->m_videogallery->SetThumbnail ( $value );
				break;
			case 'delete' :
				$this->m_videogallery->Delete ();
				$this->m_gallItemListCMS->FullDelete();
				break;
			case 'deleteIcon' :
				$this->m_videogallery->SetDefPicNull ();
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
					$prLocalRoot = $this->m_site->GetVideogalleryImageRoot ( true );
					$prLocalRoot = $prLocalRoot["demo"];
					
					$prGlobalRoot = $this->m_site->GetVideogalleryImageRoot ( false );
					$prGlobalRoot = $prGlobalRoot["demo"];
					
					$prLocalRoot .= $videogalleryID . '.' . $esxt;
					$prGlobalRoot .= $videogalleryID . '.' . $esxt;
					
					// create icon
					$im = new Image ( );
					$im->ChangeSizeInBox ( $defPath, 200, 200, $prLocalRoot );
					
					$ar = $this->m_videogallery->SetThumbnail ( $prGlobalRoot );
				}
				else
				{
					$this->m_videogallery->SetDefPicNull ();
				}
				
				break;
			case 'setpath' :
				$path = DB::POST ( 'path' );
				
				if (strlen ( trim ( $path ) ) > 0)
				{
					$pos = strrpos ( $defPath, '.' );
					$esxt = substr ( $defPath, $pos + 1 );
					
					$prGlobalRoot = $this->m_site->GetVideogalleryImageRoot ( false );
					$prGlobalRoot = $prGlobalRoot["demo"];
					$prGlobalRoot .= $videogalleryID . '.' . $esxt;
					
					$this->m_videogallery->SetThumbnail ( $path );
				}
				break;
			case 'uploadImage' :
				// upload file to folder according to product ID
				$path = $this->UploadPicture ();
				
				if ($path)
				{
					$prGlobalRoot = $this->m_site->GetVideogalleryImageRoot ( false );
					$prGlobalRoot = $prGlobalRoot["demo"];
					$prLocalRoot = $this->m_site->GetVideogalleryImageRoot ( true );
					$prLocalRoot = $prLocalRoot["demo"];
					// create icon
					$im = new Image ( );
					$im->ChangeSizeInBox ( $prLocalRoot . $path, 200, 200, $prLocalRoot . $path );
					
					$this->m_videogallery->SetThumbnail ( $prGlobalRoot . $path );
				}
				
				break;
		}
		
		// update
		if ($ar)
		{
			$this->m_videogallery = new Videogallery ( $this->m_videogallery->GetID () );
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
	 * Module features must be displayed hear
	 *
	 */
	function DisplayFeatures()
	{
		if (! $this->m_videogallery->GetID ())
		{
			return false;
		}
		
		$videogalleryID = $this->m_videogallery->GetID ();
		// language strings 
		$doneS = $this->GetNameString ( 'done' );
		$optionsS = $this->GetNameString ( 'options' );
		
		$NameS = $this->GetNameString ( 'rename' );
		$ThumbnailS = $this->GetNameString ( 'thumbnail' );
		$removeThumbnailS = $this->GetNameString ( 'rmvThumbnail' );
		$makeTumbS = $this->GetNameString ( 'mkThumbnail' );
		$extURLS = $this->GetNameString ( 'exFileURL' );
		$uploadS = $this->GetNameString ( 'upload' );
		$uploadFileS = $this->GetNameString ( 'uploadFile' );
		// values
		$NameV = $this->m_videogallery->GetName ( $this->m_displayLang );
		$ThumbnailV = $this->m_videogallery->GetThumbnail ();
		
		//Display title
		$name = $this->m_videogallery->GetName ( $this->m_displayLang );
		$form = null;
		if ($this->IsDeletable ( $this->m_videogallery->GetTableName () ))
		{
			$sureDeleteS = $this->GetNameString ( 'sureDelete' );
			$deleteS = $this->GetNameString ( 'delete' );
			$actionName = $this->GetFormActionName ();
			$form = "<div class='titleColR'>
					 <form method='post' class='clearPD'> 
					 <input type='hidden' name='$actionName' value='delete' />
					 <input type='hidden' name='calendarID' value='{$this->m_videogallery->GetID()}' />
					 <input type='submit' onclick='return confirm(\"$sureDeleteS\");' value='$deleteS' />
					 </form>
					 </div>";
		}
		$this->DisplayTitle ( $name, $form, false );
		//////////////////////////////////////
		$panelIndex = 0;
		
		print ( "<div class='someGTitleBox'>$optionsS</div>" );
		print ( '<div class="someGBox">' );
		
		//Forms
		$reading = $this->IsRecReadable ( $this->m_videogallery, 'name' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery, 'name' );
		
		if ($reading || $updating)
		{
			
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $NameS );
			if ($updating)
			{
				$this->DisplayFormHeadr ( 'changeName' );
				$this->DisplayHidden ( 'videogalleryID', $videogalleryID );
				print ( "<input type='text' value='$NameV' name='Name' id='Name' size='40' maxlength='32' />\n" );
				$this->DisplayFormFooter ( $doneS );
			}
			else
			{
				print ( $NameV );
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		// display thumbuail
		$this->IsRecReadable ( $this->m_videogallery, 'thumbnail' );
		$updating = $this->IsRecUpdatable ( $this->m_videogallery, 'thumbnail' );
		
//		if ($reading || $updating)
		if (0)
		{
			// tab header
			$this->DisplayTabHeader ( ++ $panelIndex, $ThumbnailS );
			if ($updating)
			{
				// form fro making icon ///////////////////////////////////////
				

				$this->DisplayFormHeadr ( 'selectDefPic' );
				$this->DisplayHidden ( 'videogalleryID', $this->m_videogallery->GetID () );
				
				print ( '<div class="titleBox">' );
				print ( '<div class="titleColL">
					<label><strong>' . $makeTumbS . '</strong></label>
					<select name="defpic" id="defpic">' );
				
				// get default picture ID
				$list = new VideogalleryItemList ( $videogalleryID );
				print('<option value=""> ... </option>');
				print $list->GetPatternDesign("<option value='@@ID@@'>@@path@@</option>");
				
				print ( '</select></div>' );
				print ( '<div class="titleColR"><input name="" type="submit" value="' . $doneS . '" /></div></form>
                       <br clear="all" /></div><br />' );
				//////////////////////////////////////////////////////////////
				

				// external thumbnail URL
				$this->DisplaySingleTextForm ( 'setpath', $extURLS, 'path', null, array ('videogalleryID' => $this->m_videogallery->GetID () ) );
				print ( '<br />' );
				
				// get upload maxi size
				$maxFileSize = ini_get ( 'upload_max_filesize' );
				$className = $this->GetFormActionName ();
				
				print ( '<div class="titleBox">' );
				print ( '<form action="" method="POST" class="clearPD" enctype="multipart/form-data">' );
				$this->DisplayHidden ( 'videogalleryID', $this->m_videogallery->GetID () );
				$this->DisplayHidden ( $className, 'uploadImage' );
				
				print ( '<div class="titleColL"> 
	            		<label><strong>' . $uploadFileS . ' </strong></label>
	            		<input type="file" name="gl2_img" size="48" />
	            		<span>[ Max = ' . $maxFileSize . ' ]</span>
	            		</div>' );
				
//				print ( "<span>[ Max = $maxFileSize ]</span>" );
				
				print ( '
			            <div class="titleColR"><input name="" type="submit" value="' . $uploadS . '" /></div>
			            <br class="clearFloat" />
			        	</form>
			        	</div>' );
				
				print ( '<br />' );
				////////////////////////////////////////////
				

				// display thumbnail without remove button
				if ($ThumbnailV != '')
				{
					print ( '<div class="titleBox"><div class="titleColL">' );
					print ( '<img src="' . $ThumbnailV . '" alt=" " />' );
					print ( '</div>' );
					print ( '<div class="titleColR">' );
					
					if ($ThumbnailV != 'img/thumbnail.gif')
					{
						$this->DisplayFormHeadr ( 'deleteIcon' );
						$this->DisplayHidden ( 'videogalleryID', $videogalleryID );
						$this->DisplayFormFooter ( $removeThumbnailS );
					}
					
					print ( '</div><br clear="all" /></div>' );
				}
			}
			else
			{
				// display thumbnail without remove button
				if ($ThumbnailV != '')
				{
					print ( '<div class="titleBox">
                                <div class="titleColL">
                                    <img src="' . $ThumbnailV . '" width="100" height="100" alt=" " />
                                </div>
                                <br clear="all" />
                            </div>' );
				}
			}
			// tab footer
			$this->DisplayTabFooter ();
		}
		
		print ( '</div>' );
		
		// display javascript
		print ( '<script type="text/javascript"><!--' );
		
		for($index = 0; $index <= $panelIndex; $index ++)
			print ( "var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n" );
		print ( '//--></script>' );
		
		// display videogallery items
		$this->m_gallItemListCMS->DisplayFeatures ();
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$res = parent::SelfTest ();
		
		$gallItem = new VideogalleryItemCMS ( );
		$res &= $gallItem->SelfTest ();

		// get videogallery folder, if not exist create it
		$prodFolderPath = $this->m_site->GetVideogalleryImageRoot ();
		$prodFolderPath = $prodFolderPath["demo"];

		// if the dir does not exist and it is allowed to make it then try to do that. 
		if (! file_exists ( "$prodFolderPath" ) && $this->CanBeCorrected ())
		{
			$res &= FolderXP::Mkdir( "$prodFolderPath" );
		}
		
		// check is dir exist
		if (! file_exists ( "$prodFolderPath" ))
		{
			$res = false;
			print ( "The product's folder <b>$prodFolderPath</b> does not exist<br />" );
		}
		
		return $res;
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
		$videogalleryFolderPath = $this->m_site->GetVideogalleryImageRoot ();
		$videogalleryFolderPath = $videogalleryFolderPath["demo"];
		// if addition link is given, then add to URL
		if ($addPath)
		{
			$videogalleryFolderPath .= $addPath;
		}
		
		// if folder do not exist then try to create
		if (! file_exists ( $videogalleryFolderPath ))
		{
			//create forlder
			$res = FolderXP::Mkdir( $videogalleryFolderPath, 0753 );
			
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
		

		$gallID = $this->m_videogallery->GetID ();
		$newFileName = "{$gallID}.$ext";
		$fileAddress = $videogalleryFolderPath . $newFileName;
		
		// uploading
		$res = FolderXP::Upload( $_FILES ['gl2_img'] ['tmp_name'], $fileAddress );
		
		//// if file is image and most be resize, create Image class item and resize image
		$maxWidth = $this->m_site->GetMaxImageWidth ();
		$image = new Image ( );
		$image->ChangeSizeInBox ( $fileAddress, $maxWidth );
		
		////
		

		return $newFileName;
	}
}
?>