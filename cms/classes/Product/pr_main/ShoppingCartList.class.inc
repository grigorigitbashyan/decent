<?php

class ShoppingCartList extends TreeView
{
	
	protected $m_structBuyers;
	
	protected $m_structProds;
	/**
	 * Enter description here...
	 *
	*/
	function __construct(){
		parent::__construct();
		
		////////////////////////
			
		$sql = 'SELECT buyer_trans.buyerID,
							buyer_trans.userID,
							buyer_trans.shipped, 
							buyer_trans.transaction, 
							buyer_trans.shipCountry, 
							buyer_trans.shipState, 
							buyer_trans.shipCity, 
							buyer_trans.shipZip, 
							buyer_trans.shipAddress, 
							buyer_trans.shipFirstName, 
							buyer_trans.payerEmail, 
							buyer_trans.amount, 
							buyer_trans.gift_msg, 
							buyer_trans.date, 
				GROUP_CONCAT( CONCAT_WS( " - ", pr_main.name_en, buyer_data.qty ) SEPARATOR "#" ) AS products
				FROM buyer_trans
				JOIN buyer_data ON buyer_data.buyerID = buyer_trans.buyerID
				JOIN pr_main ON buyer_data.prodID = pr_main.ID
				GROUP BY buyer_data.buyerID;';
		
		$this->SetQueryString($sql);
	}
	
	
}

?>