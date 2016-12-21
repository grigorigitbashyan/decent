<?php
Site::IncludeFile ( 'classes/Common/TemplateView.interface.inc' );
Site::IncludeFile ( 'classes/Common/MyScript.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_main.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_type/Pr_type.class.inc');
Site::IncludeFile ( 'classes/Common/DB.class.inc' );
Site::IncludeFile ( 'classes/Common/Value.class.inc' );
Site::IncludeFile ('classes/Account/SiteUser/SiteUserView.class.inc');
//include_once ('Account/SiteUser/SiteUserView.class.inc');

define ('DESCRIPTION', 'ZodiacFoods Payment'); // Description of shopping cart
define ('TESTMODE', 'false');//'true');
//define ('BUSINESS', 'vahram_1291281845_biz@arattauna.com'); // PayPal ID or e-mail which is a login for paypal with business type
define ('BUSINESS', 'service@zodiacfoods.com'); // PayPal ID or e-mail which is a login for paypal with business type
define ('RETURN_N', "http://www.zodiacfoods.com/en/shopping-cart/"); // url where customer returns after payment
define ('CANCEL_R', "http://www.zodiacfoods.com/en/shopping-cart/"); // url where customer returns when cancels the payment
define ('NOTIFY_URL', "http://www.zodiacfoods.com/en/shopping-cart/"); // url where paypal send info about payment transaction
define ('URL', 'https://www.paypal.com/cgi-bin/webscr'); // url for payment
//define ('URL', 'https://www.sandbox.paypal.com/cgi-bin/webscr'); // for testing

//define ('TOKEN', 'X2f9ycQq6fJXxeQuoMFsLo781WxJnQXXXizKH4KoPxXPcOwHzK8_i1YLAES'); // Test X2f9ycQq6fJXxeQuoMFsLo781WxJnQXXXizKH4KoPxXPcOwHzK8_i1YLAES
define ('TOKEN', 'HqEeenQMhAJcCjgPq9CrxT7zVC444yJojfnXar4RajhE-zWDDuChhfJIPAe'); // REAL HqEeenQMhAJcCjgPq9CrxT7zVC444yJojfnXar4RajhE-zWDDuChhfJIPAe

class ShoppingCartView extends View implements  TemplateNamedView{
	
	protected $m_products;
	
	function __construct() {
		
		parent::__construct();
		$this->m_products = $this->GetCookieToArray(); 
		$this->ReadNameStrings('classes/Product/langs/', 'shoppingCart.xml');
	}
	
	function Display() {
		// get strings from xml
		//Shipping info form
		/*	
	 	$shipInfo = $this->GetNameString ( 'shippingInfo' );
		$shipFirst = $this->GetNameString ( 'firstname' );
		$shipLast = $this->GetNameString ( 'lastname' );
		$shipCompany = $this->GetNameString ( 'company' );
		$shipAddress = $this->GetNameString ( 'address' );
		$shipCity = $this->GetNameString ( 'city' );
		$shipState = $this->GetNameString ( 'state' );
		$shipZip = $this->GetNameString ( 'zip' );
		$shipCountry = $this->GetNameString ( 'country' );
		$shipInfoForm = "<div>$shipInfo</div>
						 <div class=\"shipForm clearfix\" id=\"shipForm\">
							<label for=\"firstname\">$shipFirst</label>
							<input class=\"firstname\" type=\"text\" name=\"firstname\" value=\"{$product['qty']}\" /><br />
							<label for=\"lastname\">$shipLast</label>
							<input class=\"lastname\" type=\"text\" name=\"lastname\" value=\"{$product['qty']}\" /><br />
							<label for=\"company\">$shipCompany</label>
							<input class=\"company\" type=\"text\" name=\"company\" value=\"{$product['qty']}\" /><br />
							<label for=\"address\">$shipAddress</label>
							<input class=\"address\" type=\"text\" name=\"address\" value=\"{$product['qty']}\" /><br />
							<label for=\"city\">$shipCity</label>
							<input class=\"city\" type=\"text\" name=\"city\" value=\"{$product['qty']}\" /><br />
							<label for=\"state\">$shipState</label>
							<input class=\"state\" type=\"text\" name=\"state\" value=\"{$product['qty']}\" /><br />
							<label for=\"zip\">$shipZip</label>
							<input class=\"zip\" type=\"text\" name=\"zip\" value=\"{$product['qty']}\" /><br />
							<label for=\"country\">$shipCountry</label>
							<input class=\"country\" type=\"text\" name=\"country\" value=\"{$product['qty']}\" />								
							<label for=\"shipPrice\">$shipPrice</label><div id=\"shipPrice\" name=\"shipPrice\"></div>
						</div>";
		$giftMsgS = $this->GetNameString ( 'giftMsg' );
		$giftCustMsgS = $this->GetNameString ( 'giftCustMsg' );
		$giftMsgS = $this->GetNameString ( 'insertGiftMsg' );
		*/
		$shipPriceS = $this->GetNameString ( 'shipPrice' );
		$totalAmountS = $this->GetNameString ( 'totalAmount' );
		$totalMerchS = $this->GetNameString ( 'totalMerch' );
		$thankyouMsg = $this->GetNameString ( 'thankyouMsg' );
		$unSuccessfulMsg2 = $this->GetNameString('unsuccessfulPayment2');
		$unSuccessfulMsg3 = $this->GetNameString('unsuccessfulPayment3');
		$unSuccessfulMsg4 = $this->GetNameString('unsuccessfulPayment4');
		
//		print 'GET: ';print_r($_GET);
//		print 'POST: ';print_r($_POST);
		
		$url = $_SERVER['REQUEST_URI'];
		$urlInfo = parse_url($url);
		
		if (isset($urlInfo['query']))
		{
			$urlInfo['query'] = urldecode($urlInfo['query']);
			$urlInfo['query'] = HTML::RemoveXSS($urlInfo['query']);
			
			parse_str($urlInfo['query'], $resp);
//			print_r($resp);
		}
		
		// To show shopping cart with chosen items
		if (!empty($this->m_products))
		{
			$totalPrice = 0;
			print "<div><table id=\"shoppingCartTable\"  cellspacing=\"0\">";
			print "<tr class=\"firstTR\">
						<th></th>
						<th style=\"text-align:left;\">Name</th>
						<!-- th>Description</th-->
						<th>Price</th>
						<th>Quantity</th>
						<th>Total</th>
						<th></th>
					</tr>";
			
			/*
			$addField = "<div id=\"gift_msg\"><div>$giftMsgS</div><textarea name=\"gift_msg\" rows=\"4\" cols=\"80\">$giftCustMsgS</textarea></div>";
			$link = $this->m_site->GetSiteURL() . $this->m_site->GetDefaultLanguage();
			*/
			$pathes = $this->GetProdPath();
			
			foreach ($this->m_products as $product)
			{
				$prodItem = new Pr_main($product['ID']);
				$prodRand = $prodItem->GetRandom();
				
				$prodTypeItem = $prodItem->GetTypeID();
				
				$prodTypeItem = new Pr_type($prodTypeItem);
				$prodType = $prodTypeItem->GetName();
				
				$prodType = strtolower($prodType);
				$prodType = str_replace(' ', '-', $prodType);
				
				
				$price = $this->GetPrice($product['ID']);
				$realPrice = $this->GetPriceIfSaleIs($product['ID']);
				$realPrice = ($realPrice) ? "&nbsp;/&nbsp;&#36;<span style=\"text-decoration: line-through;\">$realPrice</span>" : '';
				$weight = $this->GetWeight($product['ID']);
				
				$productTotal = sprintf('%.2f', ($price * $product['qty']));
				$productWeight = sprintf('%.2f', ($weight * $product['qty']));
				$totalWeight += $productWeight;
				$totalPrice += $productTotal;
				foreach ($pathes as $id => $prodPath)
				{
					if ($id == $product['ID'])
					{
						$path = str_replace('%2F', '/', $prodPath['path']);
					}
				}
				//http://www.zodiacfoods.com/{$this->m_displayLang}/buy-preserves/{$prodType}/
				print "<tr id=\"product_{$product['ID']}\">
							<td class=\"pPic\"><a href=\"{$this->m_displayLang}/{$path}{$prodRand}\"><img src=\"{$prodItem->GetPicture()}\" width=\"100px\"/></a></td>
							<td class=\"pName\"><a href=\"{$this->m_displayLang}/{$path}{$prodRand}\">{$prodItem->GetName($this->m_displayLang)}</a></td>
							<!-- td class=\"pDesc\">{$prodItem->GetDescShort($this->m_displayLang)}</td -->
							<td class=\"pPrice\">&#36;<span>{$price}</span>$realPrice</td>
							<td class=\"pQty\"><input class=\"qty\" type=\"text\" name=\"qty\" maxlength=\"2\" style=\"width:20px;\" value=\"{$product['qty']}\" /></td>
							<td class=\"pTotal\">&#36;<span>{$productTotal}</span></td>
							<td><div class=\"deleteP\"></div></td>
						</tr>";
			}
			if ($totalPrice == 0)
				$shipPrice = 0;
			else
				$shipPrice = $this->GetShippingPrice($totalWeight);
			$totalMerch = $totalPrice;
//			$_SESSION['Payment_Amount'] = $totalMerch; // this is must be set if we use PayPal API
			$amount = $totalMerch + $shipPrice;
			$totalPrice += $shipPrice;
			
			print "</table>
					<table id=\"productsTotal\">
						<tr>
							<td class=\"shipPrice\">$totalMerchS</td>
							<td class=\"shipPriceVal\">&#36;<span id=\"merch\">{$totalMerch}</span></td>
							<td class=\"endTotalBut\">
							</td>
						</tr>
						<tr>
							<td class=\"shipPrice\">$shipPriceS</td>
							<td class=\"shipPriceVal\">&#36;<span id=\"shipPrice\">{$shipPrice}</span></td>
							<td class=\"endTotalBut\">
							</td>
						</tr>
						<tr>
							<td class=\"endTotal\">$totalAmountS</td>
							<td class=\"endTotalVal\">&#36;<span id=\"totalAmount\">{$totalPrice}</span></td>
							<td class=\"endTotalBut\">
								<form action=\"". URL . "\" method=\"post\" id=\"buyNowForm\">									
									<input type=\"button\" id=\"buyNowBtn\"  class=\"buyNow\" value=\"Check Out\">
								</form>						
							</td>
						</tr>
					</table></div>";
		}
		// to get tranaction information for to show success page and insert final data into db or unsuccess page
		elseif (isset ($resp['st']) && ($resp['st'] === 'Completed'))
		{
			// Receive Transaction ID
			$transID = $resp['tx'];
			
			// Create POST request for getting transaction all information
			$entry = array (
									'cmd' => '_notify-synch',
									'tx' => "{$transID}",
									'at' => TOKEN
								);
			// Maybe it should be like a string
//			$entry = 'cmd=_notify-synch&tx='.$transID.'&at=HqEeenQMhAJcCjgPq9CrxT7zVC444yJojfnXar4RajhE-zWDDuChhfJIPAe';

			
			// set a handle and starts cURL session
			$ch = curl_init();
			// set the url
			curl_setopt($ch, CURLOPT_URL, URL);
			// return server header response
			curl_setopt($ch, CURLOPT_HEADER, 1); // maybe set false
			// don't send the response straight to the browser...
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
			// send the form response via POST
			curl_setopt($ch, CURLOPT_POST, 1);
			// Set information to be sent
			curl_setopt ($ch, CURLOPT_POSTFIELDS, $entry);
			
			/*
			curl_setopt($ch, CURLOPT_HTTPHEADER, Array("Content-Type: text/html; charset=utf-8"));
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
			curl_setopt($ch, CURLOPT_FAILONERROR, 1);
			$content = curl_exec($ch);
			if ($this->curl_debug)
			{
				$error = curl_error($ch);
				print $error;
			}		
			// remember to always close the session and free all resources
			curl_close($ch);
			*/
			
			// Execute 
			$content = curl_exec ($ch);
			// remember to always close the session and free all resources
			curl_close ($ch);
			
			$content = urldecode($content);
			
			//TODO: USE $content which can be such
			/*
			 * SUCCESS
				first_name=Jane+Doe
				last_name=Smith
				payment_status=Completed
				payer_email=janedoesmith%40hotmail.com
				payment_gross=3.99
				mc_currency=USD
				custom=For+the+purchase+of+the+rare+book+Green+Eggs+%26+Ham
				
				//BUYER INFORMATION//
				address_country - Country of customer�s address, Length: 64 characters
				address_city - City of customer�s address, Length: 40 characters
				address_country_code - ISO 3166 country code associated with customer�s address, Length: 2 characters
				address_name - Name used with address (included when the customer provides a Gift Address), Length: 128 characters
				address_state - State of customer�s address, Length: 40 characters
				address_status - Whether the customer provided a confirmed address. It is one of the following values:
					* confirmed � Customer provided a confirmed address.
					* unconfirmed � Customer provided an unconfirmed address.
				address_street - Customer�s street address., Length: 200 characters
				address_zip - Zip code of customer�s address. Length: 20 characters
				contact_phone - Customer�s telephone number. Length: 20 characters
				first_name - Customer�s first name, Length: 64 characters
				last_name - Customer�s last name, Length: 64 characters
				payer_business_name - Customer�s company name, if customer is a business, Length: 127 characters
				payer_email - Customer�s primary email address. Use this email to provide any credits. Length: 127 characters
				payer_id - Unique customer ID. Length: 13 characters
				//BUYER INFORMATION//
				

				//PAYMENT INFORMATION//
				
				//PAYMENT INFORMATION//
			 */
			// OR such
			/*
			 * FAIL
			 * ......
			 */
//			$this->InsertFinalBuyerData($resp);

			if ( strpos($content, "SUCCESS")!==false )
			{
				$shiipingArr = array();
				
				//TODO: everything is ok, we need to make final insert to db and send mails
				$successPos = strpos($content, "SUCCESS");
				$content = substr($content, $successPos + 7);
				
				//		getting address status
				$addressStatusPos = strpos($content, "address_status=");
				$content = substr($content, $addressStatusPos + strlen("address_status="));
				
				$separatorPos = strpos($content, "item_number1");
				$shiipingArr["address_status"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos + strlen("item_number1"));
				
				//		getting address_street
				$addressStreetPos = strpos($content, "address_street=");
				$content = substr($content, $addressStreetPos + strlen("address_street="));
				
				$separatorPos = strpos($content, "payment_date");
				$shiipingArr["address_street"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				//		getting payment date
				$paymentDatePos = strpos($content, "payment_date=");
				$content = substr($content, $paymentDatePos + strlen("payment_date="));
				
				$separatorPos = strpos($content, "payment_status");
				$shiipingArr["payment_date"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				//		getting address zip
				$addressZIPPos = strpos($content, "address_zip=");
				$content = substr($content, $addressZIPPos + strlen("address_zip="));
				
				$separatorPos = strpos($content, "mc_shipping");
				$shiipingArr["address_zip"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				//		getting address country code
				$addressCountryCodePos = strpos($content, "address_country_code=");
				$content = substr($content, $addressCountryCodePos + strlen("address_country_code="));
				
				$separatorPos = strpos($content, "address_name");
				$shiipingArr["address_country_code"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				//		getting address name
				$addressNamePos = strpos($content, "address_name=");
				$content = substr($content, $addressNamePos + strlen("address_name="));
				
				$separatorPos = strpos($content, "custom");
				$shiipingArr["address_name"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				//		getting address country
				$addressCountryPos = strpos($content, "address_country=");
				$content = substr($content, $addressCountryPos + strlen("address_country="));
				
				$separatorPos = strpos($content, "num_cart_items");
				$shiipingArr["address_country"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				//		getting address city
				$addressCityPos = strpos($content, "address_city=");
				$content = substr($content, $addressCityPos + strlen("address_city="));
				
				$separatorPos = strpos($content, "payer_email");
				$shiipingArr["address_city"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				//		getting payer email
				$payerEmailPos = strpos($content, "payer_email=");
				$content = substr($content, $payerEmailPos + strlen("payer_email="));
				
				$separatorPos = strpos($content, "mc_shipping1");
				$shiipingArr["payer_email"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				//		getting address state
				$addressStatePos = strpos($content, "address_state=");
				$content = substr($content, $addressStatePos + strlen("address_state="));
				
				$separatorPos = strpos($content, "item_name1");
				$shiipingArr["address_state"] = substr($content, 0, $separatorPos);
				
				$content = substr($content, $separatorPos);
				
				
				$shiipingArr["bayer_id"] = DB::GET("p2");
				$shiipingArr["trans_id"] = $transID;
				
//				$this->InsertFinalBuyerData($shiipingArr);
				
				$resp["x_email"] = $shiipingArr["payer_email"];
				$resp["x_description"] = "";
//				$this->SendEmailsNew($shiipingArr);
				
				print ("
						Dear ".$shiipingArr['address_name'].", thank you for shop with us.<br><br>
						
						Your shipping information is follow:<br><br>
						
						Country code: ".$shiipingArr['address_country_code']."<br>
						Country: ".$shiipingArr['address_country']."<br>
						State: ".$shiipingArr['address_state']."<br>
						City: ".$shiipingArr['address_city']."<br>
						ZIP: ".$shiipingArr['address_zip']."<br>
						Address: ".$shiipingArr['address_street']."<br>
						E-mail: ".$shiipingArr['payer_email']."<br><br>
						
						We sent to you payment confirmation e-mail.
				");
	//			$this->InsertFinalBuyerData($resp);
			}
			elseif ( strpos($content, "FAIL")!==false )
			{
				//TODO: we didn't get response from Paypal about the transaction... bad =(
				$this->DeleteInitialBuyerData(DB::GET("p2"));
				print "<div style=\"text-align: left;\">$unSuccessfulMsg2</div>";	
			}
			
//			$resp = $_POST;
//			switch ($respCode)
//			{
//				case 1:
//					if(strlen($invoiceNum) < 14)
//					{
//						$this->DeleteInitialBuyerData(DB::POST('x_cust_id'));
//					}
//					else
//					{
//						$this->InsertFinalBuyerData($resp);
						
//						print "<div style=\"text-align: left;\">$thankyouMsg</div>";
//						$this->SendEmails($resp);
//					}
//					break;
//				case 2:
//					$this->DeleteInitialBuyerData(DB::POST('x_cust_id'));
//					print "<div style=\"text-align: left;\">$unSuccessfulMsg2</div>";
//					break;
//				case 3:
//					$this->DeleteInitialBuyerData(DB::POST('x_cust_id'));
//					print "<div style=\"text-align: left;\">$unSuccessfulMsg3</div>";
//					break;
//				case 4:
//					print "<div style=\"text-align: left;\">$unSuccessfulMsg4</div>";
//					break;
//			}
		}
		else
		{
			$this->showEmptyCart();
		}	
	}
	
	/**
	 * Show empty shopping cart when there isn't any item or after payment
	 * 
	 * @return string
	 */
	function showEmptyCart(){
		$noItemsS = $this->GetNameString('noItemsInYourCart');
		$continueShoppingS = $this->GetNameString('continueShopping');
		$link = $this->m_site->GetSiteURL() . $this->m_site->GetDefaultLanguage();
		print "<div style=\"text-align: left;\">$noItemsS<br/><a href=\"$link\">$continueShoppingS</a></div>";
	}
	
	function DoAction() {
		
		$action = DB::POST('action');		
		
		switch ($action)
		{
			case 'getAmount' :
				echo $this->GetAmount(DB::POST('ID'));
				break;
			case 'getName' :
				echo $this->GetProductName(DB::POST('ID'));
				break;
			case 'getStatusValue' :
				echo $this->GetStatusValue(DB::POST('ID'));
				break;
			case 'getGiftTypes' :
				echo $this->GetGiftTypes(DB::POST('ID'), DB::POST('name'));
				break;
			case 'getEmptyMsg' :
				echo $this->showEmptyCart();
				break;
			case 'checkout' :
				$buyerID = GetRandCode(16);
				$this->InsertInitialBuyerData($buyerID);
				$secData = $this->getSecurityData();
				
				$data = $this->getDataForCheckout($secData, $buyerID);
				
				//Remove cookie before checkout
				setcookie('shopCartHS', '', mktime(12,0,0,1, 1, 1990), '/');
								
				echo $data;
				
				//$this->checkout($data);
				break;
			default:
				return;
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $req
	 * @return string
	 */
	function GetTemplateValue($req, $resp = false)
	{
		if ( !$resp ) {
			
			$resp = $_POST;
		}
		
		$productInfo = '';
		$cookieInfo = ($resp['cookie']) ? DB::EscapeInjections($resp['cookie']) : null;
		$products = $this->GetCookieToArray($cookieInfo);
		
		$buyerID = DB::EscapeInjections($resp['bayer_id']);
		$query = "select * from `buyer_data` where `buyerID` = '".$buyerID."'";
		$sell = DB::Query($query);
		$prodArr = "";
		
		while ( is_array($arr=mysql_fetch_array($sell)) ) {
			
			$prodArr[] = $arr;
		}
		
		if ( is_array($prodArr) ) {
			
			$prodTotalPriceAll = 0;
			$prodTotalWeightAll = 0;
			$productInfo .= '<table  cellpadding="5" cellspacing="0" border="1" style="border: 1px solid #ccc;text-align:center;">
								<tr style="text-align:left;">
									<th>Product Name</th>
									<th>Quantity</th>
									<th>Price</th>
									<th>Total Price</th>
								</tr>';
			
			for ( $pI=0; $pI<count($prodArr); $pI++ ) {
				
				$prodID = $prodArr[$pI]["prodID"];
				$prodQty = $prodArr[$pI]["qty"];
				$prodName = $this->GetProductName($prodID);
				$prodPrice = $this->GetProductPrice($prodID);
				$prodWeight = $this->GetProductWeight($prodID);
				$prodTotalPrice = sprintf('%.2f', ($prodPrice * $prodQty));
				
				$prodTotalPriceAll = $prodTotalPriceAll + $prodTotalPrice;
				$prodTotalWeightAll = $prodTotalWeightAll + ($prodWeight*$prodQty);
				
				$productInfo .= "<tr>
									<td style=\"text-align:left;\">$prodName<br/>$giftTypes</td>
									<td>$prodQty</td>
									<td>$prodPrice$</td>
									<td>$prodTotalPrice$</td>
								</tr>";
			}
			$shipPrice = $this->GetShippingPrice($prodTotalWeightAll);
			$productInfo .= "
							<tr><td colspan=\"4\">&nbsp;</td></tr>
							<tr><td align=\"left\" colspan=\"3\">Shipping Price</td><td>".$shipPrice."</td></tr>
							<tr><td align=\"left\" colspan=\"3\">Total Price</td><td>".($shipPrice+$prodTotalPriceAll)."</td></tr>
							</table>";
		}
		
		
		/*if (is_array($products))
		{
			$productInfo .= '<table  cellpadding="5" cellspacing="0" border="1" style="border: 1px solid #ccc;text-align:center;">
								<tr style="text-align:left;">
									<th>Name</th>
									<th>Quantity</th>
									<th>Price</th>
									<th>Total Price</th>
								</tr>';
			foreach ($products as $key => $value)
			{
				$giftTypes = '';
				$giftType = ($resp['giftTypesHS'.$value['ID']]) ? DB::EscapeInjections($resp['giftTypesHS'.$value['ID']]) : null;
				if (!empty($giftType))
				{
					$giftTypeArray = explode('&', $giftType);
					$giftTypes = str_replace('#', ', ', $giftTypeArray[1]);
				}
				
				$prodName = $this->GetProductName($value['ID']);
				$prodPrice = $this->GetPrice($value['ID']);
				$prodQty = $value['qty'];
				$prodTotalPrice = sprintf('%.2f', ($prodPrice * $prodQty));
				$productInfo .= "<tr>
									<td style=\"text-align:left;\">$prodName<br/>$giftTypes</td>
									<td>$prodQty</td>
									<td>$prodPrice$</td>
									<td>$prodTotalPrice$</td>
								</tr>";
			}
			$productInfo .= '</table>';
			$shipPrice = $this->GetShippingTotal($cookieInfo);
			$merchPrice = $this->GetTotalPrice($cookieInfo);
			$subTotalPrice = $shipPrice + $merchPrice;
		}*/
		$name = $req ['name'];
		$res = '';
		
		switch ($name)
		{
			case 'siteDesc' :
				$res = DB::EscapeInjections($resp['x_description']);
				break;
			case 'transID' :
//				$res = DB::EscapeInjections($resp['x_trans_id']);
				$res = DB::EscapeInjections($resp['trans_id']);
				break;
			case 'buyerID' :
//				$res = DB::EscapeInjections($resp['x_cust_id']);
				$res = DB::EscapeInjections($resp['bayer_id']);
				break;
			case 'cardType' :
				$res = DB::EscapeInjections($resp['x_card_type']);
				break;
			case 'cardNum' :
				$res = DB::EscapeInjections($resp['x_account_number']);
				break;
			case 'invoiceNum' :
				$res = DB::EscapeInjections($resp['x_invoice_num']);
				break;
			case 'fName' :
				$res = DB::EscapeInjections($resp['x_first_name']);
				break;
			case 'lName' :
				$res = DB::EscapeInjections($resp['x_last_name']);
				break;
			case 'cCompany' :
				$res = ($resp['x_company']) ? DB::EscapeInjections($resp['x_company']) : 'NA';
				break;
			case 'cAddress' :
				$res = DB::EscapeInjections($resp['x_address']);
				break;
			case 'cCity' :
				$res = DB::EscapeInjections($resp['x_city']);
				break;
			case 'cState' :
				$res = DB::EscapeInjections($resp['x_state']);
				break;
			case 'cZip' :
				$res = DB::EscapeInjections($resp['x_zip']);
				break;
			case 'cCountry' :
				$res = ($resp['x_country']) ? DB::EscapeInjections($resp['x_country']) : 'NA';
				break;
			case 'cPhone' :
				$res = DB::EscapeInjections($resp['x_phone']);
				break;
			case 'cFax' :
				$res = ($resp['x_fax']) ? DB::EscapeInjections($resp['x_fax']) : 'NA';
				break;
			case 'cMail' :
				$res = DB::EscapeInjections($resp['x_email']);
				break;
			case 'shipFirstName' :
//				$res = DB::EscapeInjections($resp['x_ship_to_first_name']);
				$res = DB::EscapeInjections($resp['address_name']);
				break;
			case 'shipLastName' :
				$res = DB::EscapeInjections($resp['x_ship_to_last_name']);
				break;
			case 'shipCompanyName' :
				$res = ($resp['x_ship_to_company']) ? DB::EscapeInjections($resp['x_ship_to_company']) : 'NA';
				break;
			case 'shipAddress' :
//				$res = DB::EscapeInjections($resp['x_ship_to_address']);
				$res = DB::EscapeInjections($resp['address_street']);
				break;
			case 'shipCity' :
//				$res = DB::EscapeInjections($resp['x_ship_to_city']);
				$res = DB::EscapeInjections($resp['address_city']);
				break;
			case 'shipState' :
//				$res = DB::EscapeInjections($resp['x_ship_to_state']);
				$res = DB::EscapeInjections($resp['address_state']);
				break;
			case 'shipZip' :
//				$res = DB::EscapeInjections($resp['x_ship_to_zip']);
				$res = DB::EscapeInjections($resp['address_zip']);
				break;
			case 'shipCountry' :
//				$res = ($resp['x_ship_to_country']) ? DB::EscapeInjections($resp['x_ship_to_country']) : 'NA';
				$res = DB::EscapeInjections($resp['address_country_code'])." ".DB::EscapeInjections($resp['address_country']);
				break;
			case 'shipPrice' :
				$res = $shipPrice;
				break;
			case 'merchPrice' :
				$res = $merchPrice;
				break;
			case 'cAmount' :
				$res = DB::EscapeInjections($resp['x_amount']);
				break;
			case 'giftMsg' :
				$res = ($resp['gift_msg']) ? DB::EscapeInjections($resp['gift_msg']) : 'NA';
				break;
			case 'prodInfo' :
				$res = $productInfo;
				break;
			default :
				$res = $name;
				break;
		}
		
		$res = str_replace("\n", "", $res);
		return $res;
	}
	
	/**
	 * Get product name by given ID
	 * 
	 * @param $pID
	 * @return string
	 */
	function GetProductName($pID)
	{
		$sql = "SELECT `name_{$this->m_displayLang}` as `name` FROM `pr_main` WHERE `ID` = $pID";
		return DB::QueryToValue($sql);
	}
	
	/**
	 * Get product price by given ID
	 * 
	 * @param $pID
	 * @return char
	 */
	function GetProductPrice($pID)
	{
		$sql = "SELECT `value` FROM `pr_char` WHERE `productID` = $pID and `fieldID` = 5";
		return DB::QueryToValue($sql);
	}
	
	/**
	 * Get product weight by given ID
	 * 
	 * @param $pID
	 * @return double
	 */
	function GetProductWeight($pID)
	{
		$sql = "SELECT `value` FROM `pr_double` WHERE `productID` = $pID and `fieldID` = 6";
		return DB::QueryToValue($sql);
	}
	
	/**
	 * Get product status (SOLD OUT or AVAILABLE) by given ID
	 * It is used for frontend view via AJAX
	 * 
	 * @param $pID
	 * @return JSON object or false
	 */
	function GetStatusValue($pID)
	{
		$sql = "SELECT `relatedID` FROM `pr_options` WHERE `productID` = $pID AND `fieldID` = 7";
		$prod['prodID'] = $pID;
		$prod['soldID'] = DB::QueryToValue($sql);
		
		if ($prod['soldID'] == '5')
			return json_encode($prod);
		else
			return false;
	}
	
	/**
	 * Get product's gift types by given ID and name
	 * It is used for frontend view via AJAX
	 * 
	 * @param $pID
	 * @param $pName
	 * @return JSON object or false
	 */
	function GetGiftTypes($pID, $pName)
	{
		$prodType = array();
		$options = array();
		
		if ($pName == 'Classic Gift Basket')
		{
			$prodType[0] = $this->GetNameString('apricot');
			$prodType[1] = $this->GetNameString('apricotwhitecherry');
			$prodType[2] = $this->GetNameString('appleandpear');
			$prodType[3] = $this->GetNameString('peach');
			$prodType[4] = $this->GetNameString('pumpkinandapple');
			$prodType[5] = $this->GetNameString('quince');
			$prodType[6] = $this->GetNameString('tearosepetal');
			$prodType[7] = $this->GetNameString('wildstrawberry');
			$prodType[8] = $this->GetNameString('walnut');
			$prodType[9] = $this->GetNameString('goldenfig');			
			
			$typ1 = '<div>Choose the flavors:</div>';
			$typ1 .= '<label>1.</label><select name="type_1" id="type_1">';
			$typ2 = '<label>2.</label><select name="type_2" id="type_2">';
			$typAll = '';
			foreach ($prodType as $type)
			{
				$typAll .= "<option value=\"$type\">$type</option>";
			}
			
			$typ1 .= $typAll.'</select><br />';
			$typ2 .= $typAll.'</select><br />';
			
			$options[0] = $typ1;
			$options[1] = $typ2;
			
			return json_encode($options);
		}
		else if ($pName == 'Sampler Gift Basket')
		{
			$prodType[0] = $this->GetNameString('apricot');
			$prodType[1] = $this->GetNameString('goldenfig');
			$prodType[2] = $this->GetNameString('freshwalnut');
			$prodType[3] = $this->GetNameString('tearosepetal');
			$prodType[4] = $this->GetNameString('wildstrawberry');
			$prodType[5] = $this->GetNameString('quince');
			
			$typ1 = '<div>Choose the flavors:</div>';
			$typ1 .= '<label>1.</label><select name="type_1" id="type_1">';
			$typ2 = '<label>2.</label><select name="type_2" id="type_2">';
			$typ3 = '<label>3.</label><select name="type_3" id="type_3">';
			$typ4 = '<label>4.</label><select name="type_4" id="type_4">';
			$typ5 = '<label>5.</label><select name="type_5" id="type_5">';
			$typ6 = '<label>6.</label><select name="type_6" id="type_6">';
			$typ7 = '<label>7.</label><select name="type_7" id="type_7">';
			$typ8 = '<label>8.</label><select name="type_8" id="type_8">';
			$typ9 = '<label>9.</label><select name="type_9" id="type_9">';
			$typ10 = '<label>10.</label><select name="type_10" id="type_10">';
			$typ11 = '<label>11.</label><select name="type_11" id="type_11">';
			$typ12 = '<label>12.</label><select name="type_12" id="type_12">';
			$typAll = '';
			foreach ($prodType as $type)
			{
				$typAll .= "<option value=\"$type\">$type</option>";
			}
			
			$typ1 .= $typAll.'</select><br />';
			$typ2 .= $typAll.'</select><br />';
			$typ3 .= $typAll.'</select><br />';
			$typ4 .= $typAll.'</select><br />';
			$typ5 .= $typAll.'</select><br />';
			$typ6 .= $typAll.'</select><br />';
			$typ7 .= $typAll.'</select><br />';
			$typ8 .= $typAll.'</select><br />';
			$typ9 .= $typAll.'</select><br />';
			$typ10 .= $typAll.'</select><br />';
			$typ11 .= $typAll.'</select><br />';
			$typ12 .= $typAll.'</select><br />';
			
			$options[0] = $typ1;
			$options[1] = $typ2;
			$options[2] = $typ3;
			$options[3] = $typ4;
			$options[4] = $typ5;
			$options[5] = $typ6;
			$options[6] = $typ7;
			$options[7] = $typ8;
			$options[8] = $typ9;
			$options[9] = $typ10;
			$options[10] = $typ11;
			$options[11] = $typ12;
			
			return json_encode($options);
		}
		else if ($pName == 'Trio Gift Basket')
		{
			$prodType[0] = $this->GetNameString('apricot');
			$prodType[1] = $this->GetNameString('apricotwhitecherry');
			$prodType[2] = $this->GetNameString('goldenfig');
			$prodType[3] = $this->GetNameString('peach');
			$prodType[4] = $this->GetNameString('sourcherry');
			$prodType[5] = $this->GetNameString('tearosepetal');
			$prodType[6] = $this->GetNameString('wildstrawberry');
			
			$typ1 = '<div>Choose the flavors:</div>';
			$typ1 .= '<label>1.</label><select name="type_1" id="type_1">';
			$typ2 = '<label>2.</label><select name="type_2" id="type_2">';
			$typ3 = '<label>3.</label><select name="type_3" id="type_3">';
			$typAll = '';
			foreach ($prodType as $type)
			{
				$typAll .= "<option value=\"$type\">$type</option>";
			}
			
			$typ1 .= $typAll.'</select><br />';
			$typ2 .= $typAll.'</select><br />';
			$typ3 .= $typAll.'</select><br />';
			
			$options[0] = $typ1;
			$options[1] = $typ2;
			$options[2] = $typ3;
			
			return json_encode($options);
		}
		$sql = "SELECT `relatedID` FROM `pr_options` WHERE `productID` = $pID AND `fieldID` = 10";
		$prod['prodID'] = $pID;
		$prod['soldID'] = DB::QueryToValue($sql);
		
		if ($prod['soldID'] == '2')
			return json_encode($prod);
		else
			return false;
	}
	
	/**
	 * Get product weight by given ID
	 * 
	 * @param $pID
	 * @return sting
	 */
	function GetWeight($pID)
	{
		$sql = "SELECT `value` FROM `pr_double` WHERE `productID` = $pID";
		return DB::QueryToValue($sql);		
	}
	
	/**
	 * Get product shipping price by its weight
	 * 
	 * @param $weight
	 * @return int
	 */
	function GetShippingPrice($weight)
	{
		if ($weight >= 0 && $weight < 3)
			return 12;
		if ($weight >= 3 && $weight < 5)
			return 13;
		if ($weight >= 5 && $weight < 10)
			return 18;
		if ($weight >= 10 && $weight < 20)
			return 26;
		if ($weight >= 20)
			return 26;
	}
	
	/**
	 * Count whole shipping price for all products
	 * It is used for frontend view via AJAX
	 * 
	 * @param $reqCookie
	 * @return int
	 */
	function GetShippingTotal($reqCookie = null)
	{
		$newList = $this->GetCookieToArray($reqCookie);
		
		$weight = 0.00;
		foreach ($newList as $listItem)
		{
			$weight = $this->GetWeight($listItem['ID']);
			$totalWeight += sprintf('%.2f', ($listItem['qty'] * $weight));
		}
		$shipPrice = $this->GetShippingPrice($totalWeight);
		return $shipPrice;
	}
	
	/**
	 * Get product price by its ID
	 * 
	 * @param $pID
	 * @return string
	 */
	function GetPrice($pID)
	{
		// get sale price
		$price = DB::QueryToValue("SELECT `value` FROM `pr_char` WHERE `productID` = $pID AND `fieldID` = 15;");
	
		// check if it doesn't exist
		if (!$price)
		{
			$price = DB::QueryToValue("SELECT `value` FROM `pr_char` WHERE `productID` = $pID AND `fieldID` = 5;");
		}
		
		return $price;
	}
	
	/**
	 * Get product real price by its ID, if it has sale price
	 * 
	 * @param $pID
	 * @return string
	 */
	function GetPriceIfSaleIs($pID)
	{
		$price = DB::QueryToValue("SELECT `value` FROM `pr_char` WHERE `productID` = $pID AND `fieldID` = 15;");
		
		if ($price)
		{
			$sql = "SELECT `value` FROM `pr_char` WHERE `productID` = $pID AND `fieldID` = 5;";
			return DB::QueryToValue($sql);
		}
		
		return false;
	}
	
	/**
	 * Get products total price without shipping price
	 * Merchant price
	 * 
	 * @param $reqCookie
	 * @return string
	 */
	function GetTotalPrice ($reqCookie = null)
	{
		$newList = $this->GetCookieToArray($reqCookie);
		
		$amount = 0;
		foreach ($newList as $listItem)
		{
			$price = $this->GetPrice($listItem['ID']);
			$amount += sprintf('%.2f', ($listItem['qty'] * $price));
		}
		return $amount;
	}
	
	/**
	 * Get whole price: merchant + shipping
	 * It is used for frontend view via AJAX
	 * 
	 * @param int $pID
	 * @return string
	 * @return JSON object or false
	 */
	function GetAmount($pID) {
		
		if ($this->GetTotalPrice() == 0)
			$prices['shipprice'] = 0;
		else
			$prices['shipprice'] = $this->GetShippingTotal();
		$prices['merch'] = $this->GetTotalPrice();
		$prices['amount'] = $prices['merch'] + $prices['shipprice'];
		$prices['price'] = $this->GetPrice($pID);
		
		return json_encode($prices);
	}
	
	/**
	 * Get information from Cookies and convert it into array
	 * 
	 * @param $reqCookie
	 * @return array
	 */
	function GetCookieToArray($reqCookie = null)
	{
		$products = array();
		$productTemp = array();
		$IDs = array();		
		
		$cookieStr = (isset($_COOKIE['shopCartHS'])) ? $_COOKIE['shopCartHS'] : $reqCookie;
//		if (isset($_COOKIE['shopCartHS']))
//		{
//			$cookieStr = $_COOKIE['shopCartHS'];
					
			$cookieArr = explode('#', $cookieStr);
			foreach ($cookieArr as $cookieItem)
			{
				$cookiePc = explode('&', $cookieItem);
				$product = array();
				foreach ($cookiePc as $value)
				{
					$valueC = explode('=', $value);
					if ($valueC[0]) {
						$product[DB::EscapeInjections($valueC[0])] = DB::EscapeInjections($valueC[1]);
					}
					
					if ($valueC[0] == 'ID')
					{
						array_push($IDs, $valueC[1]);
					}					
				}			
				array_push($productTemp, $product);
			}
			
			//Sorting array
			sort($IDs);
			foreach ($IDs as $pID)
			{
				foreach ($productTemp as $pItem)
				{
					if ($pID == $pItem['ID'])
						$products[$pID] = $pItem;
				}
			}
//		}
		
		return $products;
	}
	/**
	 * Get arrays from cookie
	 * 
	 * Enter description here ...
	 * @param $reqCookie
	 */
	function GetProdPath($reqCookie = null)
	{
		$products = array();
		$productTemp = array();
		$IDs = array();		
		
		$cookieStr = (isset($_COOKIE['prodPathHS'])) ? urldecode($_COOKIE['prodPathHS']) : $reqCookie;
//		if (isset($_COOKIE['shopCartHS']))
//		{
//			$cookieStr = $_COOKIE['shopCartHS'];
				
			$cookieArr = explode('#', $cookieStr);
			foreach ($cookieArr as $cookieItem)
			{
				$cookiePc = explode('&', $cookieItem);
				$product = array();
				foreach ($cookiePc as $value)
				{
					$valueC = explode('=', $value);
					if ($valueC[0]) {
						$product[DB::EscapeInjections($valueC[0])] = DB::EscapeInjections($valueC[1]);
					}
					
					if ($valueC[0] == 'ID')
					{
						array_push($IDs, $valueC[1]);
					}					
				}			
				array_push($productTemp, $product);
			}
			
			//Sorting array
			sort($IDs);
			foreach ($IDs as $pID)
			{
				foreach ($productTemp as $pItem)
				{
					if ($pID == $pItem['ID'])
						$products[$pID] = $pItem;
				}
			}
//		}
		
		return $products;
	}
	
	/**
	 * This function gets data from cookie as array and creates form 
	 * to send to PayPal or another payment system
	 * 
	 * return HTML form elements, for submition
	 * @return string
	 */
	function getDataForCheckout($secData, $buyerID)
	{
		
		$totalAmount = $secData['amount'];
		// Geting products info///////////////////////////
		$prodInfo = '';
		$products = $this->GetCookieToArray();
		$numItems = count($products);
		if (!empty($products))
		{
			foreach ($products as $prodItem)
			{
				$name = $this->GetProductName($prodItem['ID']);
				$price = $this->GetPrice($prodItem['ID']);
				$weight = $this->GetWeight($prodItem['ID']);
				$prodInfo .= '<input type="hidden" name="item_name_'.$numItems.'" value="'.$name.'">
							<input type="hidden" name="amount_'.$numItems.'" value="'.$price.'">
							<input type="hidden" name="weight_'.$numItems.'" value="'.$weight.'">
							<input type="hidden" name="quantity_'.$numItems.'" value="'.$prodItem['qty'].'"';
				
				$totalAmount = $totalAmount - ($price * $prodItem['qty']);
				
				if ( $numItems==1 ) {
					
					$prodInfo .= '
								<input type="hidden" name="shipping_'.$numItems.'" value="'.$totalAmount.'" />
								<input type="hidden" name="shipping2_'.$numItems.'" value="0.00" />
								<input type="hidden" name="handling_'.$numItems.'" value="0.00">';
				} else {
					
					$prodInfo .= '
								<input type="hidden" name="shipping_'.$numItems.'" value="0.00" />
								<input type="hidden" name="shipping2_'.$numItems.'" value="0.00" />
								<input type="hidden" name="handling_'.$numItems.'" value="0.00">';
				}
				
				$numItems--;
			}
		}
		
		$shippingAmount = $totalAmount;
		$totalAmount = $secData['amount'] - $shippingAmount;
		/////////////////////////////////////////////////////
		//TODO: shipping info 
		$cookieStr = $_COOKIE['shopCartHS'];
		$returnURL = RETURN_N."$buyerID/";
		$cancelURL = CANCEL_R."$buyerID/";
//		$notifyURL = NOTIFY_URL."$buyerID/";
		$notifyURL = "http://www.zodiacfoods.com/files/shippingValidate.php?buyerID=$buyerID";
		return '<input type="hidden" name="cmd" value="_cart">
				<input type="hidden" name="upload" value="1">
				<input type="hidden" name="business" value="G8XE2Y5C238YJ">
				<input type="hidden" name="return" value="'.$returnURL.'" />
				<input type="hidden" name="cancel_return" value="'.$cancelURL.'" />
				<input type="hidden" name="notify_url" value="'.$notifyURL.'" />
				<input type="hidden" name="description" value="'.DESCRIPTION.'" />
				<input type="hidden" name="amount" value="'.$totalAmount.'" />
				'.$prodInfo.'
				<input type="hidden" name="cookie" value="'.$cookieStr.'" />
				';
	}
	
	/**
	 * This function insert initial data about buyer into db BEFORE payment
	 * buyerID, amount, prodID, qty, gift_type
	 * 
	 * @param $buyerID
	 */
	function InsertInitialBuyerData($buyerID)
	{
		$siteUser = new SiteUserView();
		$userID = $siteUser->GetUserID();
		$products = $this->GetCookieToArray();
		$shipPrice = $this->GetShippingTotal();
		$merchPrice = $this->GetTotalPrice();		
		$totalAm = $shipPrice + $merchPrice;
		$sql_prod = "INSERT INTO `buyer_trans` (`buyerID`, `userID`, `amount`) valueS ('{$buyerID}', '{$userID}', '{$totalAm}');";
		DB::Query($sql_prod);
		$sql = "INSERT INTO `buyer_data` (`buyerID`, `prodID`, `qty`, `gift_type`) valueS ";
		$prodID = 0;
		$numItems = count($products);
		foreach ($products as $listItem)
		{
			$giftTypes = '';
			$sql .= "('{$buyerID}',";
			$prodID = $listItem['ID'];
			$qty = $listItem['qty'];
			$price = $this->GetPrice($listItem['ID']);
			if ($giftTypeCookie = $_COOKIE['giftTypesHS'.$prodID])
			{
				$giftTypeArray = explode('&', $giftTypeCookie);
				$giftTypes = str_replace('#', ', ', $giftTypeArray[1]);
				$giftTypes = '(' . $giftTypes . ')';
				setcookie('giftTypesHS'.$prodID, '', mktime(12,0,0,1, 1, 1990), '/');
			}
			if($numItems != 1) 
			{
				$sql .= "'{$prodID}','{$qty}','{$giftTypes}'),";
			} else {
				$sql .= "'{$prodID}','{$qty}','{$giftTypes}');";
			}
			
			$numItems--;
		}
		
		DB::Query($sql);
	}
	
	/**
	 * This function insert final info about buyer and transaction into db AFTER payment
	 * 
	 * @param $resp
	 */
	function InsertFinalBuyerData($resp)
	{
		/*$shipInfo = DB::EscapeInjections($resp['x_ship_to_first_name']) . ' ' . 
					DB::EscapeInjections($resp['x_ship_to_last_name']) . '^' .
					DB::EscapeInjections($resp['x_ship_to_company']) . '^' .
					DB::EscapeInjections($resp['x_ship_to_address']) . '^' .
					DB::EscapeInjections($resp['x_ship_to_city']) . '^' .
					DB::EscapeInjections($resp['x_ship_to_state']) . '^' .
					DB::EscapeInjections($resp['x_ship_to_zip']) . '^' .
					DB::EscapeInjections($resp['x_ship_to_country']);
			
		$buyerID = DB::EscapeInjections($resp['x_cust_id']);
		
		$giftMsg = '';
		$giftMsg = ($resp['gift_msg']) ?  DB::EscapeInjections($resp['gift_msg']) : 'NA';
		
		$transID = DB::EscapeInjections($resp['tx']);*/ // Transation ID
		
		/*$shipInfo = DB::EscapeInjections($resp['address_name']) . ' ' . 
					DB::EscapeInjections($resp['company']) . '^' .
					DB::EscapeInjections($resp['address_street']) . '^' .
					DB::EscapeInjections($resp['address_city']) . '^' .
					DB::EscapeInjections($resp['address_state']) . '^' .
					DB::EscapeInjections($resp['address_zip']) . '^' .
					DB::EscapeInjections($resp['address_country']);*/
					
		$shipPayerEmail = DB::EscapeInjections($resp['payer_email']);
		$shipAddressName = DB::EscapeInjections($resp['address_name']);
		$shipAddressStreet = DB::EscapeInjections($resp['address_street']);
		$shipAddressCity = DB::EscapeInjections($resp['address_city']);
		$shipAddressState = DB::EscapeInjections($resp['address_state']);
		$shipAddressZIP = DB::EscapeInjections($resp['address_zip']);
		$shipAddressCountry = DB::EscapeInjections($resp['address_country_code'])." ".DB::EscapeInjections($resp['address_country']);
			
		$buyerID = DB::EscapeInjections($resp['bayer_id']);
		
		$giftMsg = '';
		$giftMsg = ($resp['gift_msg']) ?  DB::EscapeInjections($resp['gift_msg']) : 'NA';
		
		$transID = DB::EscapeInjections($resp['trans_id']); // Transaction ID
		
		$sql = "
				UPDATE 
					`buyer_trans` 
				SET 
					`shipFirstName`='{$shipAddressName}', 
					`shipCountry`='{$shipAddressCountry}', 
					`shipCity`='{$shipAddressCity}', 
					`shipState`='{$shipAddressState}', 
					`shipZip`='{$shipAddressZIP}', 
					`shipAddress`='{$shipAddressStreet}', 
					`payerEmail`='{$shipPayerEmail}', 
					`transaction`='{$transID}', 
					`gift_msg`='{$giftMsg}' 
				WHERE 
					`buyerID`='{$buyerID}';";
		
		DB::Query($sql, null, true);
	}
	
	/**
	 * Delete initial buyer data, because it buyer didn't make payment or for another reason
	 * 
	 * @param $custID
	 */
	function DeleteInitialBuyerData($custID)
	{
		$sql_prod = "DELETE FROM `buyer_trans` WHERE `buyerID` = '$custID'";
		DB::Query($sql_prod);
		$sql = "DELETE FROM `buyer_data` WHERE `buyerID` = '$custID'";
		DB::Query($sql);
	}
	
	/**
	 * Get secure data for submition,
	 * like login, password, ....
	 * 
	 * @return array
	 */
	function getSecurityData()
	{
		// With Shipping 
		$shipPrice = $this->GetShippingTotal();
		$merchPrice = $this->GetTotalPrice();
		$secData['amount'] = $merchPrice + $shipPrice;

		return $secData;
	}
	
	/**
	 * Send emails to merchant and customer
	 *
	 * @return string
	 */
	function SendEmails($resp)
	{
		$custEmail = DB::EscapeInjections($resp['x_email']);		
        $desc = DB::EscapeInjections($resp['x_description']); //ZodiacFoods Payment 
		
		// read content from database
		$emailPageValObj = new Value ( 'shoppingEmailBuyerContentID' );
		$emailContentID = $emailPageValObj->GetValue ();
		
		$emailContnetObj = new Content ( $emailContentID );
		
		$emailContnet = $emailContnetObj->GetContent ( $this->m_displayLang );
		/////////////////////////
		$ms = new MyScript ( $emailContnet, $this );
		
		// from email
		$emailValObj = new Value ( 'fromShoppingEmail' );
		
		// final values
		$fromEmail = $emailValObj->GetValue ();
		$subject = $emailContnetObj->GetName ( $this->m_displayLang );
		$message = $ms->GetParsedTemplate ();
		exit();
		// send email to customer
		$this->m_site->Mail ( trim ( $custEmail ), $subject, $message, $fromEmail );
		
		// send emails to stuf
		$emailValObj = new Value ( 'toEmail' );
		$emailsV = $emailValObj->GetValue ();
		
		////////////////// get admin email content
		$emailPageValObj = new Value ( 'shoppingEmailAdminContentID' );
		$emailContentID = $emailPageValObj->GetValue ();
		
		$emailContnetObj = new Content ( $emailContentID );
		
		$emailContnet = $emailContnetObj->GetContent ( $this->m_displayLang );
		/////////////////////////
		$ms = new MyScript ( $emailContnet, $this );
		
		// final values
		$subject = $emailContnetObj->GetName ( $this->m_displayLang );
		$message = $ms->GetParsedTemplate ();
		/////////////////
		
		
		if (trim ( $emailsV ))
		{
			$emails = explode ( ';', $emailsV );
			if ($emails)
			{
				foreach ( $emails as $email )
				{
					$this->m_site->Mail ( trim ( $email ), $subject, $message, $fromEmail );
				}
			}
			else
				$this->m_site->Mail ( trim ( $emailsV ), $subject, $message, $fromEmail );
		}
	}
	
	
	function SendEmailsNew($resp)
	{
		$custEmail = DB::EscapeInjections($resp['payer_email']);		
        
		// read content from database
		$emailPageValObj = new Value ( 'shoppingEmailBuyerContentID' );
		$emailContentID = $emailPageValObj->GetValue ();
		
		$emailContnetObj = new Content ( $emailContentID );
		
		$emailContnet = $emailContnetObj->GetContent ( $this->m_displayLang );
		/////////////////////////
		$ms = new MyScript ( $emailContnet, $this );
		
		// from email
		$emailValObj = new Value ( 'fromShoppingEmail' );
		
		// final values
		$fromEmail = $emailValObj->GetValue ();
		$subject = $emailContnetObj->GetName ( $this->m_displayLang );
		$message = $ms->GetParsedTemplate (false, $resp);
		// send email to customer
		$this->m_site->Mail ( trim ( $custEmail ), $subject, $message, $fromEmail );
		
		// send emails to stuf
		$emailValObj = new Value ( 'toEmail' );
		$emailsV = $emailValObj->GetValue ();
		
		////////////////// get admin email content
		$emailPageValObj = new Value ( 'shoppingEmailAdminContentID' );
		$emailContentID = $emailPageValObj->GetValue ();
		
		$emailContnetObj = new Content ( $emailContentID );
		
		$emailContnet = $emailContnetObj->GetContent ( $this->m_displayLang );
		/////////////////////////
		$ms = new MyScript ( $emailContnet, $this );
		
		// final values
		$subject = $emailContnetObj->GetName ( $this->m_displayLang );
		$message = $ms->GetParsedTemplate (false, $resp);
		/////////////////
		
		
		if (trim ( $emailsV ))
		{
			$emails = explode ( ';', $emailsV );
			if ($emails)
			{
				foreach ( $emails as $email )
				{
					$this->m_site->Mail ( trim ( $email ), $subject, $message, $fromEmail );
				}
			}
			else
				$this->m_site->Mail ( trim ( $emailsV ), $subject, $message, $fromEmail );
		}
	}
	
	/**
	 * Delete old data which were entered only initial
	 * 
	 * @return bool
	 */
	static function DeleteOld($duration)
	{
		// get IDs with empty info or old 
		$sql = "SELECT `buyerID` FROM `buyer_trans` WHERE `transaction` = '' AND UNIX_TIMESTAMP(`date`) + $duration < UNIX_TIMESTAMP();";
		$oldData = DB::QueryToArray($sql);
		
		if ($oldData != null)
		{
			foreach ($oldData as $custID)
			{
				// delete empty trans and data
				DB::Query("DELETE FROM `buyer_trans` WHERE `buyerID` = '$custID'");
				DB::Query("DELETE FROM `buyer_data` WHERE `buyerID` = '$custID'");
			}
		}
		
//		OLD 
//		// delete empty stat and data 
//		DB::Query("DELETE FROM `buyer_trans` WHERE `transaction` = '' AND UNIX_TIMESTAMP(`date`) + $duration < UNIX_TIMESTAMP();");
	}
}


?>