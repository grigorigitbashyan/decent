<?php

Site::IncludeFile ( 'classes/Common/CMSListObject.class.inc' );

Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_main/Pr_main.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_main/ShoppingCartList.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_main/ShoppingCart.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_main/ShoppingCartCMS.class.inc' );



class ShoppingCartListCMS extends CMSListObject {

	/**

	 * Description

	 *

	 * 

	 */

	

	protected $m_product;

	

	function __construct() {

		parent::__construct (new ShoppingCartList(), 'ShoppingCart');

		$this->ReadNameStrings('classes/Product/langs/', 'shoppingCart.xml');

	}

	

	function CreateDBSQL() {

		$str1 = "CREATE TABLE `buyer_data` (

  					`ID` int(10) unsigned NOT NULL auto_increment,

  					`buyerID` varchar(255) NOT NULL,

  					`prodID` int(10) unsigned NOT NULL,

  					`qty` int(10) unsigned NOT NULL,

  					`gift_type` varchar(255) NOT NULL,

  					PRIMARY KEY  (`ID`)

				) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=22;";

		

		$str2 = "CREATE TABLE `buyer_trans` (

					`ID` int(10) unsigned NOT NULL auto_increment,

					`shipped` tinyint(1) unsigned NOT NULL default '0',

					`buyerID` varchar(255) NOT NULL,

					`amount` float unsigned NOT NULL,

					`ship_address` text NOT NULL,

					`transaction` varchar(255) NOT NULL,

					`gift_msg` text,

					`date` timestamp NOT NULL default CURRENT_TIMESTAMP,

					PRIMARY KEY  (`ID`)

				) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=8 ;" ;

	

		return array($str1, $str2);

	}

	

	function DoAction() {

		// mark customers whose products have been sent, if don't want to delete

		// delete customers whose products have been sent, if don't want to keep.

		if (DB::POST('action'))

		{

			$deleteS = $this->GetNameString ( 'delete' );

			$markShippedS = $this->GetNameString ( 'markShipped' );

			$markNotShippedS = $this->GetNameString ( 'markNotShipped' );

			$shopIDs = DB::POST('shopIDs');

//			foreach (DB::POST('action') as $key => $value)

//			{

//				$action = $value;

//			}

			$action = DB::POST('action');

			switch ($action) {

				case "$deleteS" :

					foreach ($shopIDs as $shopID)

					{

						$shCart = new ShoppingCart($shopID);

						$shCart->Delete();

					}

					break;

				case "$markShippedS" :

					foreach ($shopIDs as $shopID)

					{

						$shCart = new ShoppingCart($shopID);

						if (!$shCart->GetShipped())

						{

							$shipped = 1;

							$shCart->SetShipped($shipped);

						}

					}

	//				$buyerID = DB::POST ( 'buyerID' );

	//				$shCart = new ShoppingCart($buyerID);

	//				$shipped = ($shCart->GetShipped())? 0 : 1;

	//				$shCart->SetShipped($shipped);

					break;

				case "$markNotShippedS" :

					foreach ($shopIDs as $shopID)

					{

						$shCart = new ShoppingCart($shopID);

						if ($shCart->GetShipped())

						{

							$shipped = 0;

							$shCart->SetShipped($shipped);

						}

					}

	//				$buyerID = DB::POST ( 'buyerID' );

	//				$shCart = new ShoppingCart($buyerID);

	//				$shipped = ($shCart->GetShipped())? 0 : 1;

	//				$shCart->SetShipped($shipped);

					break;

			}

		}

		$this->Update();

	}

	

	function DisplayMenu()

	{

		$overLinkName = 'shoppingCartList';

		$overLink = DB::GET_POST ( $overLinkName );

		

		// email

		print ( "<div class='someWTitleBox'>Shopping Cart List</div>" );

	}

	

	function DisplayFeatures() {

		$overLinkName = 'shoppingCartList';

		$overLink = DB::GET_POST ( $overLinkName );

		$panelIndex = 0;

		

		$doneS = $this->GetNameString ( 'done' );

		$optionsS = $this->GetNameString ( 'options' );		

		print ( '<script type="text/javascript"><!--' );

		

		for($index = 0; $index <= $panelIndex; $index ++)

			print ( "var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n" );

		print ( '//--></script>' );

		// show info from two dbs:

		// transaction number, product list with quantities, total amount of payment, shipping info

		// show unshipped first

		

		$rows = $this->m_list->GetStructure();

		

		$doneS = $this->GetNameString ( 'done' );

		$sureDeleteS = $this->GetNameString ( 'sureDelete' );

		$deleteS = $this->GetNameString ( 'delete' );

		$transactionIDS = $this->GetNameString ( 'transactionID' );

		$transactionIDTS = $this->GetNameString ( 'transactionIDT' );

		$productS = $this->GetNameString ( 'products' );

		$totalAmountS = $this->GetNameString ( 'totalAmount' );

		$giftMsgS = $this->GetNameString ( 'giftMsg' );

		$shippingInfoS = $this->GetNameString ( 'shippingInfo' );

		$shippedS = $this->GetNameString ( 'shipped' );

		$markShippedS = $this->GetNameString ( 'markShipped' );

		$markNotShippedS = $this->GetNameString ( 'markNotShipped' );

		$noemptyS = $this->GetNameString ( 'noempty' );

		$emptyS = $this->GetNameString ( 'empty' );

		

		$totalAmountTS = $this->GetNameString ( 'totalAmountT' );

		$giftMsgTS = $this->GetNameString ( 'giftMsgT' );

		$dateS = $this->GetNameString ( 'date' );

		

		/////////////////////////////////////////AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

		// read language string

		

		// get content type count

		$shoppedCount = count($rows);

		

		// get current content type

//		$currID = $this->m_list->GetCurrentID();

		$coutShopOverLink = $this->m_list->GetLinkIDName ();		

//		if ($currID < 1 || $typeCount == 0)

//		{

//			if ($this->IsInsertable ( 'contenttype' ))

//			{

//				$insertJS = "onsubmit='return SingleForm(\"name\", \"$noemptyS\")'";

//				$this->DisplaySingleTextForm ( 'insert', $createDirectoriesS, 'name', null, null, $doneS, $insertJS );

//			}

//			

//			return;

//		}

				

		// get content cms object

//		if ($shopID !== null)

//		{

//			$shoppingCartCMS = new ShoppingCartCMS ( $shopID );

//			

//			// if there was a ation delete then set content to null

//			if (! $shoppingCartCMS->GetContent ())

//			{

//				$shoppingCartCMS = null;

//				$shopID = null;

//			}

//		}

		

//		if ($currID !== null)

//		{

//			$contentType = new ContentType ( $currID );

//			$currID = $contentType->GetID ();

//		}

		// if is selected content from content types, then display content features

//		if (isset ( $content ))

//		{

//			$content->DisplayFeatures ( $currID );

//		}

//		elseif ($currID !== null) // display content type features if it has selected and exist

//		{

			

				// display contents

		if ($shoppedCount > 0)

		{

			$this->DisplayFormHeadr ( 'groupAction', 'shoppingCart' );

			//$this->DisplayHidden ( $coutShopOverLink, $currID );

					?>

<table width="100%" border="0" cellspacing="0" cellpadding="0"

	class="titleBox">

	<tr>

		<td>

		<div class="titleColL"><?php

					print "<div style=\"margin 4px 0px;\">$shoppedCount</div>"; //$contentList->DisplayNavigation ( "TypeID=$currID" );

					?></div>

		<div class="titleColR"><span style="padding-right: 20px">

			       <?php

					// get names

					$moveS = $this->GetNameString ( 'move' );

					$allS = $this->GetNameString ( 'all' );

					$noneS = $this->GetNameString ( 'none' );

					$selectS = $this->GetNameString ( 'select' );

					$noSelectedS = $this->GetNameString ( 'noselected' );

					?>

		</span> <span style="padding-right: 20px"> <strong><?php echo $selectS?></strong>

		<a href="#"

			onclick="chekeBoxes('shoppingCart', 'shopIDs', true); return false"><?php echo $allS?></a>,

		<a href="#"

			onclick="chekeBoxes('shoppingCart', 'shopIDs', false); return false"><?php echo $noneS?></a>

		</span>

			<?php

			if($this->IsUpdatable('buyer_trans'))

			{

				print("<input name=\"action\" type=\"submit\" value=\"$markShippedS\" onclick=\"return ChecksForm('shoppingCart', 'shopIDs', '$noSelectedS');\" />");	

			} 

			?>

			<?php

			if($this->IsUpdatable('buyer_trans'))

			{

				print("<input name=\"action\" type=\"submit\" value=\"$markNotShippedS\" onclick=\"return ChecksForm('shoppingCart', 'shopIDs', '$noSelectedS');\" />");	

			} 

			?> 

			<?php

			if($this->IsDeletable('buyer_trans'))

			{

				print("<input name=\"action\" type=\"submit\" value=\"$deleteS\" onclick=\"return ChecksFormDelete('shoppingCart', 'shopIDs', '$noSelectedS', '$sureDeleteS');\" />");	

			} 

			?> 

		

		</div>

		<br class="clearFloat" />

		</td>

	</tr>

</table>

<table width="100%" border="0" cellspacing="0" cellpadding="0"

	style="border-left: 1px solid #777777;border-top: 1px solid #777777;" class="shoppingTableCMS">

		<?php

			$patern = '';

			$patern .= "<tr>

							<th></th>

							<th>$transactionIDTS</th>

							<th>$productS</th>

							<th>Country</th>

							<th>State</th>

							<th>City</th>

							<th>Zip</th>

							<th>Address</th>

							<th>Payer Email</th>

							<th>$totalAmountTS</th>

							<!-- th>$giftMsgTS</th -->

							<!-- th>$shippingInfoS</th -->

							<th>$shippedS</th>

							<th>$dateS</th>

						</tr>";

			$actionName = $this->GetFormActionName ();		

					

					

					

								

	//				$delete = "<form method='post' class='clearPD'> 

	//						 		<input type='hidden' name='$actionName' value='delete' />

	//								<input type='hidden' name='buyerID' value='{$row['buyerID']}' />

	//								<input type='submit' onclick='return confirm(\"$sureDeleteS\");' value='$deleteS' />

	//						 </form>

	//						 </div>";

					

	//				$shC = new ShoppingCart($row['buyerID']);

	//				

	//				$shippedS = ($shC->GetShipped()) ? $markNotShippedS : $markShippedS; 

	//				//$this->GetNameString ( 'delete' );

	//				

	//				$setShipped = "<form method='post' class='clearPD'> 

	//						 		<input type='hidden' name='$actionName' value='setShipped' />

	//								<input type='hidden' name='buyerID' value='{$row['buyerID']}' />

	//								<input type='submit' value='$shippedS' />

	//						 </form>";

		

			function sortByDate($a, $b)

			{

			  $at = strtotime($a['date']);

			  $bt = strtotime($b['date']);

			  if ($at == $bt) return 0;

			  return ($at > $bt) ? -1 : 1;

			}

			usort ($rows, 'sortByDate');

			foreach ($rows as $row)

			{

				$shipped = ($row['shipped']) ? 'Yes' : 'No';

				$products = join('<br/>', explode('#', $row['products']));

				$country = $row['shipCountry'];

				$state = $row['shipState'];

				$city = $row['shipCity'];

				$zip = $row['shipZip'];

				$address = $row['shipAddress'];

				$email = $row['payerEmail'];

				$amount = '$' . $row['amount'];

				$shippInfo = explode('^', $row['shipAddress']);

				foreach ($shippInfo as $key => $value)

				{

					if ($value == '')

						unset($shippInfo[$key]);

				}

				$shippInfo = join('<br/>', $shippInfo);

				

				$patern .= "<tr>

								<td>

									<input name='shopIDs[]' id='shopIDs' type='checkbox' value=\"{$row['buyerID']}\" />

								</td>

								<td>{$row['transaction']}</td>

								<td>{$products}</td>

								<td>{$country}</td>

								<td>{$state}</td>

								<td>{$city}</td>

								<td>{$zip}</td>

								<td>{$address}</td>

								<td>{$email}</td>

								<td>{$amount}</td>

								<!-- td>{$row['gift_msg']}</td -->

								<!-- td>{$shippInfo}</td -->

								<td>{$shipped}</td>

								<td>{$row['date']}</td>

							</tr>";

			}

			print $patern;

			print ( "&nbsp;" );

		

?>

</table>

</form>

<?php

		}

	}

		/////////////////////////////////////////AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA



	

	function SelfTest() {

		$result = parent::SelfTest ();

		return $result;

	}

}

?>