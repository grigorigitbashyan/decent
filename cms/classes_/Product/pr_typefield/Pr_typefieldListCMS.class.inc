<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_typefield/Pr_typefield.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_typefield/Pr_typefieldCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_typefield/Pr_typefieldList.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_field/Pr_fieldList.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_typefieldListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_typefieldList
	 */
	protected $m_pr_typefieldList;
	
	protected $m_productTypeID;
	
	function __construct($typeID = null)
	{
		parent::__construct ();
		
		$this->m_productTypeID = $typeID;
		
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
		
		// call DoAction of Pr_typefieldCMS
		
		$overLink = $this->m_pr_typefieldList->GetLinkIDName ();
		
		if (($overLinkID = DB::POST ( $overLink )))
		{
			$pr_typefieldCMS = new Pr_typefieldCMS ( $overLinkID );
			$ar = $pr_typefieldCMS->DoAction ();
		}
		
		// do actions for Pr_typefield list
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				if (($IDs = DB::POST ('fieldIDs')))
				{
					$newPr_typefield = new Pr_typefield ( );
					
					$params ['producttypeID'] = $this->m_productTypeID;
					
					foreach ( $IDs as $ID )
					{
						$params ['fieldID'] = $ID;
						$ar = $newPr_typefield->Insert ( $params );
					}
				}
				break;
			case 'delete' :
				if (($IDs = DB::POST ('fieldIDs')))
				{
					if ( isset($_POST["makeM"]) )
					{
						foreach ( $IDs as $ID )
						{
							$optionsdata = new Pr_typefield ( $ID );
							$ar = $optionsdata->SetSingle(2);
						}
					}
					elseif ( isset($_POST["makeS"]) )
					{
						foreach ( $IDs as $ID )
						{
							$optionsdata = new Pr_typefield ( $ID );
							$ar = $optionsdata->SetSingle(1);
						}
					}
					else 
					{
						foreach ( $IDs as $ID )
						{
							$optionsdata = new Pr_typefield ( $ID );
							$ar = $optionsdata->Delete ();
						}
					}
				}
				elseif ( isset($_POST["updateOrder"]) )
				{
					$fields = $this->m_pr_typefieldList->GetStructure();
					for ( $i=0; $i<count($fields); $i++ )
					{
						$ID = $fields[$i]["ID"];
						$order = DB::POST('order_'.$ID);
						$optionsdata = new Pr_typefield ( $ID );
						$ar = $optionsdata->SetOrder($order);
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
		$currID = $this->m_pr_typefieldList->GetCurrentID ();
		
		if ($currID)
		{
			$linkName = $this->m_pr_typefieldList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>Pr_typefield</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">Pr_typefield</div>' );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_pr_typefieldList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// chaking the product type exsist or not
		if ($this->m_productTypeID == 0)
			return;
		
		$prType = new Pr_type ( $this->m_productTypeID );
		$this->m_productTypeID = $prType->GetID ();
		if ($this->m_productTypeID == 0)
			return;
		
		// get names
		$deleteS = $this->GetNameString ( 'delete' );
		$selectS = $this->GetNameString ( 'select' );
		$allS = $this->GetNameString ( 'all' );
		$noneS = $this->GetNameString ( 'none' );
		// display related
		{
			$opdatCount = $this->m_pr_typefieldList->GetItemCount ();
			if ($opdatCount > 0)
			{
				$sureDeleteS = $this->GetNameString ( 'sureDelete' );
				$noSelectedS = $this->GetNameString ( 'noselected' );
				
				// display form
				$this->DisplayFormHeadr ( 'delete', 'relatedfielddelete' );
				?>
				<br />
				<h2>Related Fields</h2>
				<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
					<tr>
						<td>
							<div class="titleColR">
								<span style="padding-right: 20px">
									<strong><?php echo $selectS?></strong>
									<a href="#" onclick="chekeBoxes('relatedfielddelete', 'fieldIDs', true); return false"><?php echo $allS?></a>,
									<a href="#" onclick="chekeBoxes('relatedfielddelete', 'fieldIDs', false); return false"><?php echo $noneS?></a>
								</span> 
								<input name="makeM" type="submit" value="Make multiple" />
								<input name="makeS" type="submit" value="Make single" />
								<input name="updateOrder" type="submit" value="Update orders" />
								<input name="" type="submit" value="<?php echo $deleteS?>" onclick="return ChecksFormDelete('relatedfielddelete', 'fieldIDs', '<?php echo $noSelectedS?>', '<?php echo $sureDeleteS?>');" />
							</div>
							<br class="clearFloat" />
						</td>
					</tr>
				</table>
				<table width="100%" border="0" cellspacing="0" cellpadding="0" style="border-left: 1px solid #777777; border-right: 1px solid #777777">
					<tr>
						<td>
							<?php
							$patern = "
								<div class='tableCHB' style='height:25px;padding-top:4px;'><input name='fieldIDs[]' id='fieldIDs' type='checkbox' value='@@ID@@' /></div>
			                    <div class='tableT'><input name='order_@@ID@@' id='order_@@ID@@' type='text' value='@@order@@' style='width:50px;' /></div>
			                    <div class='tableT' style='padding-top:4px;'>@@name@@@@single@@</div>
				                <br class='clearFloat' />
				            ";
							$this->m_pr_typefieldList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
							?>
						</td>
					</tr>
				</table>
				</form>
				<?php
			}
		}
		
		// display new relation
		$fieldList = new Pr_fieldList ( );
		$opdatCount = $fieldList->GetItemCount ();
		if ($opdatCount > 0)
		{
			// display form
			$this->DisplayFormHeadr ( 'insert', 'newrelation' );
			$this->DisplayHidden ( 'pr_typeID', $this->m_productTypeID );
			?>
			<br />
			<h2>New Relation</h2>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
				<tr>
					<td>
					<div class="titleColR">
						<span style="padding-right: 20px"> 
							<strong><?php echo $selectS?></strong>
							<a href="#" onclick="chekeBoxes('newrelation', 'fieldIDs', true); return false"><?php echo $allS?></a>,
							<a href="#" onclick="chekeBoxes('newrelation', 'fieldIDs', false); return false"><?php echo $noneS?></a>
						</span> 
						<input name="" type="submit" value="Add" />
					</div>
					<br class="clearFloat" />
					</td>
				</tr>
			</table>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" style="border-left: 1px solid #777777; border-right: 1px solid #777777">
				<tr>
					<td>
						<?php
						$patern = "
							<div class='tableCHB'><input name='fieldIDs[]' id='fieldIDs' type='checkbox' value='@@ID@@' /></div>
			                <div class='tableT'>@@name@@</div>
				            <br class='clearFloat' />
				        ";
						$fieldList->DisplayListDiv ( $patern, array ('tableUneven', 'tableEven' ) );
						?>
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
		$this->m_pr_typefieldList = new Pr_typefieldList ( $this->m_productTypeID );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $fieldID
	 */
	function DeleteFieldFromAllTypes($fieldID)
	{
		$sql = "DELETE FROM `pr_typefield` WHERE `fieldID` = $fieldID";
		return DB::Query ( $sql );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $productTypeID
	 */
	function DeleteTypeFields()
	{
		$sql = "DELETE FROM `pr_typefield` WHERE `producttypeID` = $this->m_productTypeID";
		return DB::Query ( $sql );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$typeFiled = new Pr_typefieldCMS ( );
		return $typeFiled->CreateDB ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$typeFiled = new Pr_typefieldCMS ( );
		return $typeFiled->SelfTest ();
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		$typeFiled = new Pr_typefieldCMS ( );
		return $typeFiled->GetObjectTables ();
	}
}
?>