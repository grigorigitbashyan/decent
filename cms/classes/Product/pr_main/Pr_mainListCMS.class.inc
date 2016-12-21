<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_main/Pr_main.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_mainCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_mainList.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/ProductExtentions.class.inc' );
Site::IncludeFile ( 'classes/Product/ProductAll.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_options.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_typefield/Pr_typefieldList.class.inc' );

/**
 * Enter description here...
 * 
 * @package Item
 */
class Pr_mainListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_mainList
	 */
	protected $m_pr_mainList;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_productTypeID;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_status;
	
	/**
	 * Enter description here...
	 *
	 * @param int $productTypeID
	 */
	function __construct($productTypeID = null)
	{
		parent::__construct ();
		$this->m_productTypeID = $productTypeID;
		$status = DB::GET('status');
		$field = DB::GET('field');
		if ( $status=="0" || $status=="1" || $status=="2" || $status=="3" || $status=="4" )
		{
			
		}
		else 
		{
			$status = "all";
		}
		
		if ( $field=="" )
		{
			$field = 0;
		}
		
		$this->m_status = $status;
		$this->Update ();
		
		// read strings
		$this->ReadNameStrings ( 'classes/Product/langs/', 'product.xml' );
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
		
		// call DoAction of Pr_mainCMS
		$overLink = $this->m_pr_mainList->GetLinkIDName ();
		
		if (($owerLinkID = DB::POST ($overLink) ))
		{
			$productExtentions = new ProductExtentions ( $owerLinkID);
			$ar = $productExtentions->DoAction ();
		}
		
		// do actions for Pr_main list
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				if (($name = DB::POST ('name')))
				{
					if (strlen ( trim ( $name ) ) > 0)
					{
						// insert new pr_main with name
						$newPr_main = new Pr_main ( );
						$params = array ('typeID' => $this->m_productTypeID, 'date' => date("Y-m-d"), 'userID' => CMS::GetUser()->GetID() );
						
						$langs = $this->m_site->GetLanguages ();
						foreach ( $langs as $lang )
						{
							$params ["name_$lang"] = $name;
							$params ["seotitle_$lang"] = $name;
						}
						
						if ( CMS::GetUser()->GetUserTypeID()<12 )
						{
							$params ["status"] = 1;
						}
						
						$ar = $newPr_main->Insert ( $params );
					}
				}
				break;
			case 'deleteProducts' :
				$IDs = DB::POST ('productIDs');
				$prTypeID = DB::POST('moveprtype');
				if ( $IDs )
				{
					if ( isset($_POST["delete"]) )
					{
						foreach ( $IDs as $ID )
						{
							$product = new Pr_main($ID);
							$product->Delete();
						}
					}
					elseif ( $prTypeID ) 
					{
						foreach ( $IDs as $ID )
						{
							$product = new Pr_main($ID);
							
							$typeID = $product->GetTypeID ();
							$productTypeFields = new Pr_typefieldList ( $typeID );
							$fields = $productTypeFields->GetStructure ();
							if ( is_array($fields) )
							{
								foreach ( $fields as $relField )
								{
									$fieldID = $relField ['fieldID'];
									$field = new Pr_field ( $fieldID );
									switch ($field->GetType ())
									{
										case 2:
											$productOptions = new Pr_options ( null, $ID, $fieldID );
											if ($productOptions->GetID ()) // if exist, then set 0
											{
												$productOptions->SetRelatedID(0);
											}
											break;
									}
								}
							}
							
							$product->SetTypeID($prTypeID);
						}
					}
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
		$currID = $this->m_pr_mainList->GetCurrentID ();
		
		if ($currID)
		{
			$linkName = $this->m_pr_mainList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>Pr_main</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">Pr_main</div>' );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_pr_mainList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		$currentPr_mainID = $this->m_pr_mainList->GetCurrentID ();
		
		// get cooretn Product ID
		$pr_main = new Pr_main ( $currentPr_mainID );
		
		$currentPr_mainID = $pr_main->GetID ();
		// if Product has been selected, then diplay Product features
		if ($currentPr_mainID)
		{
			$pr_mainCMS = new ProductExtentions ( $currentPr_mainID );
			$pr_mainCMS->DisplayFeatures ();
		
		} // diplay new pr_main form
		else
		{
			// read language strings
			$doneS = $this->GetNameString ( 'done' );
			// create new product string
			$cNS = $this->GetNameString ( 'createProduct' );
			
			$noEmptyS = $this->GetNameString ( 'noempty' );
			// display insert form
			$iJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
			
			// hidden values
			$hV = array ('pr_typeID' => $this->m_productTypeID );
			
			$this->DisplaySingleTextForm ( 'insert', $cNS, 'name', null, $hV, $doneS, $iJS );
			// end new pr_main form
			
			// display product list
			$countProducts = $this->m_pr_mainList->GetAllCount ();
			if ($countProducts > 0)
			{
				$sureDeleteS = $this->GetNameString ( 'sureDelete' );
				$noSelectedS = $this->GetNameString ( 'noselected' );
				
				$overName = $this->m_pr_mainList->GetLinkIDName ();
				
				$patern = "
					<div class='mediaImgBox'>
						<div class='mediaImgUnit'>
							<a href='?subpr_typeID={$this->m_productTypeID}&$overName=@@ID@@&status={$this->m_status}'>
								<img src='files/ImageMeaker.php?w=100&h=80&p=@@picture@@' border='0' alt='No image available' />
							</a>
						</div>
						<div class='mediaImgChackbox'><input name='productIDs[]' id='productIDs[]' type='checkbox' value='@@ID@@' /></div> 
						<div class='mediaImgTitle'><span>@@name@@</span></div>
					</div>
				";
				
				$this->DisplayFormHeadr ( 'deleteProducts', 'products' );
				?>
<br />
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
			<div class="titleColL"><span style="padding-right: 20px">
				<?php
				$this->m_pr_mainList->DisplayNavigation ( "pr_typeID=$this->m_productTypeID&status=$this->m_status" );
				?>
			</div>
			<div class="titleColR">
				<span style="padding-right: 20px">
					<strong>Select:</strong>
					<a href="#"	onclick="chekeBoxes('products', 'productIDs[]', true); return false">All</a>,
					<a href="#"	onclick="chekeBoxes('products', 'productIDs[]', false); return false">None</a>
				</span> 
				<?php
				$tempPrTypeList = new Pr_typeList();
				$prTypeList = $tempPrTypeList->GetStructure();
				?>
				<span style="padding-right: 20px">
					<strong>Move to:</strong>
					<select name="moveprtype" onchange="this.form.submit();">
						<option value="0">---</option>
						<?php
						for ( $i=0; $i<count($prTypeList); $i++ )
						{
							if ( $prTypeList[$i]["ID"]!=$this->m_productTypeID )
							{
								?>
								<option value="<?php echo $prTypeList[$i]["ID"];?>"><?php echo $prTypeList[$i]["name"];?></option>
								<?php
							}
						}
						?>
					</select>
				</span> 
				<input name="delete" type="submit" value="Delete"	onclick="return ChecksFormDelete('products', 'productIDs[]', '<?php echo $noSelectedS?>', '<?php echo $sureDeleteS?>');" />
			</div>
			<br class="clearFloat" />
		</td>
	</tr>
</table>
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	style="border-left: 1px solid #777777; border-right: 1px solid #777777; border-bottom: 1px solid #777777">
	<tr>
		<td>
		<?php
				$this->m_pr_mainList->DisplayListDiv ( $patern );
				print ( '</td></tr></table></form>' );
			}
			else
			{
				?>
<br />
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	class="titleBox">
	<tr>
		<td>
			<div class="titleColL"></div>
			<div class="titleColR"></div>
			<br class="clearFloat" />
		</td>
	</tr>
</table>
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
		$this->m_pr_mainList = new Pr_mainList ( $this->m_productTypeID, 40, 'date', $this->m_status );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$prMain = new Pr_mainCMS ( );
		return $prMain->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$prMain = new Pr_mainCMS ( );
		return $prMain->SelfTest ();
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		$prMain = new Pr_mainCMS ( );
		return $prMain->GetObjectTables ();
	}
}
?>