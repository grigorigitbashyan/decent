<?php
Site::IncludeFile ( 'classes/Content/Content.class.inc' );
/**
 * This class is used to manage booking options like email comtent or administration emila. 
 *
 */
class ShoppingOptionsCMS extends CMSObject
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->ReadNameStrings ( 'classes/Product/langs/', 'shoppingCart.xml' );
	}
	
	/**
	 * @see CMSObject::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		$overLinkName = 'shoppingOptions';
		$overLink = DB::GET_POST ( $overLinkName );
		$panelIndex = 0;
		
		$doneS = $this->GetNameString ( 'done' );
		$optionsS = $this->GetNameString ( 'options' );
		
		switch ($overLink)
		{
			case 'email' :
				// read name strings
				$nameS = $this->GetNameString ( 'emailSectionTitle' );
				$toTemilS = $this->GetNameString ( 'toEmailTabName' );
				$fromTemilS = $this->GetNameString ( 'fromEmailTabName' );
				
				// display title
				$this->DisplayTitle ( $nameS, null, false );
				
				// get to email
				$emailValObj = new Value ( 'toEmail' );
				$email = $emailValObj->GetValue ();
				
				// get from booking email
				$emailValObj = new Value ( 'fromShoppingEmail' ); //Add to table!!!
				$fromEmail = $emailValObj->GetValue ();
				
				//////////////////////////////////////
				print ( "<div class='someGTitleBox'>$optionsS</div>" );
				print ( '<div class="someGBox">' );
				// display from email form
				$this->DisplayTabHeader ( ++ $panelIndex, $fromTemilS );
				
				// form
				$this->DisplayFormHeadr ( 'setFromEmail' );
				print ( "<input type='text' name='fromEmail' value='$fromEmail' size='64' /> " );
				$this->DisplayFormFooter ( $doneS );
				// end form
				$this->DisplayTabFooter ();
				
				// display to email form
				$this->DisplayTabHeader ( ++ $panelIndex, $toTemilS );
				
				// form
				$this->DisplayFormHeadr ( 'setToEmail' );
				print ( "<input type='text' name='toEmail' value='$email' size='64' /> " );
				$this->DisplayFormFooter ( $doneS );
				// end form
				$this->DisplayTabFooter ();
				
				print ( '</div>' );
				break;
			case 'content' :
				// get name strings
				$nameS = $this->GetNameString ( 'comntentSectionTitle' );
				$shoppingEmailAdminContS = $this->GetNameString ( 'shoppingEmailAdminCont' );
				$shoppingEmailBuyerContS = $this->GetNameString ( 'shoppingEmailBuyerCont' );
				
				// display title
				$this->DisplayTitle ( $nameS, null, false );
				
							
				// get content for shopping to send to user  ////////////
				$emailValObj = new Value ( 'shoppingEmailAdminContentID' );
				$contentID = $emailValObj->GetValue ();
				
				$shoppingEmailAdminContentObj = new Content ( $contentID );
				$shoppingEmailAdminContent = '';
				$shoppingEmailAdminTitle = '';
				
				if ($shoppingEmailAdminContentObj->GetID ())
				{
					$shoppingEmailAdminContent = $shoppingEmailAdminContentObj->GetContent ( $this->m_displayLang, false, null, false );
					$shoppingEmailAdminTitle = $shoppingEmailAdminContentObj->GetName($this->m_displayLang);
				}
				
				// get content for shopping to send to admin  ////////////
				$emailValObj = new Value ( 'shoppingEmailBuyerContentID' );
				$contentID = $emailValObj->GetValue ();
				
				$shoppingEmailBuyerContentObj = new Content ( $contentID );
				$shoppingEmailBuyerContent = '';
				$shoppingEmailBuyerTitle = '';
				
				if ($shoppingEmailBuyerContentObj->GetID ())
				{
					$shoppingEmailBuyerContent = $shoppingEmailBuyerContentObj->GetContent ( $this->m_displayLang, false, null, false );
					$shoppingEmailBuyerTitle = $shoppingEmailBuyerContentObj->GetName($this->m_displayLang);
				}
				
				//////////////////////////////////////
				print ( "<div class='someGTitleBox'>$optionsS</div>" );
				print ( '<div class="someGBox">' );

				// form email content //////////////////////////////////////////////
				$this->DisplayTabHeader ( ++ $panelIndex, $shoppingEmailAdminContS );
				
				$this->DisplayFormHeadr ( 'setShoppingEmailAdminContent' );
				
				print('<div style="padding-left: 85px">
					   <ol>
							<li>@@siteDesc@@ - Payment site description</li> 
							<li>@@transID@@ - Customer transaction ID</li>
							<li>@@buyerID@@ - Customer ID</li>
							<li>@@cardType@@ - Customer card type</li>
							<li>@@cardNum@@ - Customer card number</li>
							<li>@@invoiceNum@@ - Customer invoice number</li>
							<li>@@fName@@ - Customer first name</li>
							<li>@@lName@@ - Customer last name</li>
							<li>@@cCompany@@ - Customer company name</li>
							<li>@@cAddress@@ - Customer address</li>
							<li>@@cCity@@ - Customer city</li>
							<li>@@cState@@ - Customer state</li>
							<li>@@cZip@@ - Customer zip</li>
							<li>@@cCountry@@ - Customer county</li>
							<li>@@cPhone@@ - Customer phone</li>
							<li>@@cFax@@ - Customer fax</li>
							<li>@@cMail@@ - Customer email</li>
							<li>@@shipFirstName@@ - Shipping first name</li> 
							<li>@@shipLastName@@ - Shipping last name</li>
							<li>@@shipCompanyName@@ - Shipping company name</li>
							<li>@@shipAddress@@ - Shipping address</li>
							<li>@@shipCity@@ - Shipping city</li>
							<li>@@shipState@@ - Shipping state</li>
							<li>@@shipZip@@ - Shipping zip</li>
							<li>@@shipCountry@@ - Shipping county</li>
							<li>@@shipPrice@@ - Shipping price</li>
							<li>@@merchPrice@@ - Merchandise price</li>
							<li>@@cAmount@@ - Customer total paid amount</li>
							<li>@@giftMsg@@ - Customer message for gift</li>
							<li>@@prodInfo@@ - Products info (in table)</li>
					    </ol>
					    </div>
					  <br clear="all" />');
				
				print('<div class="userFormLabel"><label><strong>Suject:</strong></label></div>');
                print('<div><input name="shoppingEmailAdminTitle" type="text" size="48" maxlength="256" style="font-size:14px" value="'.$shoppingEmailAdminTitle.'" /></div>');
                print('<br clear="all" />');
                
                print('<div class="userFormLabel"><label><strong>Message:</strong></label></div>');
                print('<div><textarea name="shoppingEmailAdminContent" cols="56" rows="5">'.$shoppingEmailAdminContent.'</textarea></div>');
                print('<br clear="all" />');
                
                print('<div class="userFormLabel">&nbsp;</div>
                            <div><input type="submit" value="'.$doneS.'"></div>
                            <br clear="all" />');
				// display form footer
				print('</form>');
				$this->DisplayTabFooter ();
				
				// booking email content for admin /////////////////////////////////////
				$this->DisplayTabHeader ( ++ $panelIndex, $shoppingEmailBuyerContS );
				
				$this->DisplayFormHeadr ( 'setShoppingEmailBuyerContent' );
				
				print('<div style="padding-left: 85px">
					   <ol>
							<li>@@siteDesc@@ - Payment site description</li> 
							<li>@@transID@@ - Customer transaction ID</li>
							<li>@@buyerID@@ - Customer ID</li>
							<li>@@cardType@@ - Customer card type</li>
							<li>@@cardNum@@ - Customer card number</li>
							<li>@@invoiceNum@@ - Customer invoice number</li>
							<li>@@fName@@ - Customer first name</li>
							<li>@@lName@@ - Customer last name</li>
							<li>@@cCompany@@ - Customer company name</li>
							<li>@@cAddress@@ - Customer address</li>
							<li>@@cCity@@ - Customer city</li>
							<li>@@cState@@ - Customer state</li>
							<li>@@cZip@@ - Customer zip</li>
							<li>@@cCountry@@ - Customer county</li>
							<li>@@cPhone@@ - Customer phone</li>
							<li>@@cFax@@ - Customer fax</li>
							<li>@@cMail@@ - Customer email</li>
							<li>@@shipFirstName@@ - Shipping first name</li> 
							<li>@@shipLastName@@ - Shipping last name</li>
							<li>@@shipCompanyName@@ - Shipping company name</li>
							<li>@@shipAddress@@ - Shipping address</li>
							<li>@@shipCity@@ - Shipping city</li>
							<li>@@shipState@@ - Shipping state</li>
							<li>@@shipZip@@ - Shipping zip</li>
							<li>@@shipCountry@@ - Shipping county</li>
							<li>@@shipPrice@@ - Shipping price</li>
							<li>@@merchPrice@@ - Merchandise price</li>
							<li>@@cAmount@@ - Customer total paid amount</li>
							<li>@@giftMsg@@ - Customer message for gift</li>
							<li>@@prodInfo@@ - Products info (in table)</li>
					    </ol>
					    </div>
					  <br clear="all" />');
					  
				print('<div class="userFormLabel"><label><strong>Suject:</strong></label></div>');
                print('<div><input name="shoppingEmailBuyerTitle" type="text" size="48" maxlength="256" style="font-size:14px" value="'.$shoppingEmailBuyerTitle.'" /></div>');
                print('<br clear="all" />');
                
                print('<div class="userFormLabel"><label><strong>Message:</strong></label></div>');
                print('<div><textarea name="shoppingEmailBuyerContent" cols="56" rows="5">'.$shoppingEmailBuyerContent.'</textarea></div>');
                print('<br clear="all" />');
                
                print('<div class="userFormLabel">&nbsp;</div>
                            <div><input type="submit" value="'.$doneS.'"></div>
                            <br clear="all" />');
				// display form footer
				print('</form>');
				$this->DisplayTabFooter ();
				
				print ( '</div>' );
				break;
		}
		
		// display javascript
		print ( '<script type="text/javascript"><!--' );
		
		for($index = 0; $index <= $panelIndex; $index ++)
			print ( "var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n" );
		print ( '//--></script>' );
		
		return true;
	}
	
	function DisplayMenu()
	{
		// display
		$overLinkName = 'shoppingOptions';
		$overLink = DB::GET_POST ( $overLinkName );
		
		$links = array ('email' => 'Email', 'content' => 'Content' );
		
		// email
		print ( "<div class='someWTitleBox'>Shopping options</div>
				<div class='someWBox'>" );
		foreach ( $links as $key => $value )
		{
			if ($key == $overLink)
			{
				print ( "<div class='directoryMenu1'>$value</div>" );
			}
			else
			{
				print ( "<div class='directoryMenu1'><a href='?$overLinkName=$key'>$value</a></div>" );
			}
		}
		print ( '</div>' );
		// comtent
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function DoAction()
	{
		$ar = null;
		
		switch ($this->GetFormAction ())
		{
			case 'setToEmail' :
				$val = DB::POST ( 'toEmail' );
				// get to email
				$emailValObj = new Value ( 'toEmail' );
				$ar = $emailValObj->SetValue ( $val );
				break;
			case 'setFromEmail' :
				$val = DB::POST ( 'fromEmail' );
				// get to email
				$emailValObj = new Value ( 'fromShoppingEmail' );
				$ar = $emailValObj->SetValue ( $val );
				break;
			case 'setShoppingEmailAdminContent' :
				$cont = DB::POST('shoppingEmailAdminContent');
				$title = DB::POST('shoppingEmailAdminTitle');
				// get values ////////////
				$emailValObj = new Value ( 'shoppingEmailAdminContentID' );
				$contentID = $emailValObj->GetValue ();
				
				$emailContnetObj = new Content ( $contentID );
				if (!$emailContnetObj->GetID ())
				{
					$emailContnetObj->Insert('content for admin');
					$contentID = $emailContnetObj->GetID();
					$emailValObj->SetValue($contentID);
					$emailContnetObj = new Content ( $contentID );
				}
				
				$emailContnetObj->UpdateText($cont, $this->m_displayLang);
				$emailContnetObj->SetName($title, $this->m_displayLang);
				
				break;
			case 'setShoppingEmailBuyerContent' : ///blah
				$cont = DB::POST('shoppingEmailBuyerContent');
				$title = DB::POST('shoppingEmailBuyerTitle');
				// get values ////////////
				$emailValObj = new Value ( 'shoppingEmailBuyerContentID' );
				$contentID = $emailValObj->GetValue ();
				
				$emailContnetObj = new Content ( $contentID );
				if (!$emailContnetObj->GetID ())
				{
					$emailContnetObj->Insert('content for buyer');
					$contentID = $emailContnetObj->GetID();
					$emailValObj->SetValue($contentID);
					$emailContnetObj = new Content ( $contentID );
				}
				
				$emailContnetObj->UpdateText($cont, $this->m_displayLang);
				$emailContnetObj->SetName($title, $this->m_displayLang);
				
				break;
		}
		return $ar;
	}

}
?>