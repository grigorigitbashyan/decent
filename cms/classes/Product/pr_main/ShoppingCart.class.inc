<?php
/**
 * Enter class description
 */
class ShoppingCart extends Record
{
	/**
	* Description
	*
	* @var bool
	*/
	protected $m_shipped;

	/**
	* Description
	*
	* @var char
	*/
	protected $m_buyerID;

	/**
	* Description
	*
	* @var int
	*/
	protected $m_userID;
	
	/**
	* Description
	*
	* @var 
	*/
	protected $m_amount;

	/**
	* Description
	*
	* @var string
	*/
	protected $m_ship_address;

	/**
	* Description
	*
	* @var char
	*/
	protected $m_transaction;

	/**
	* Description
	*
	* @var string
	*/
	protected $m_gift_msg;

	/**
	 * Enter description here...
	 *
	 * @param int $ID
	*/
	function __construct($buyerID)
	{
		parent::__construct($buyerID, 'buyer_trans', 'buyerID');
		
		$this->Read();
	}
	/**
	 * Insert BuyerTrans 
	* @return bool
	*/
	function Insert($properties)
	{
		// delete from content DB
		return $this->InsertRecord($properties);	
	}

	/**
	 * Delete BuyerTrans 
	* @return bool
	*/
	function Delete()
	{
		// delete from content DB
	//	return $this->DeleteRecord();
		$sql = "DELETE FROM `buyer_data` WHERE `buyerID` = '{$this->m_ID}';";	
		DB::Query($sql, null, true);
		return $this->DeleteRecord();
	}

	/**
	 * Set value of m_shipped
	 *
	 * @param int $shipped
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetShipped($shipped, $dbupdate = true)
	{
		return $this->UpdateValue('shipped', $shipped, $dbupdate);
	}

	/**
	 * Set value of m_amount
	 *
	 * @param  $amount
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetAmount($amount, $dbupdate = true)
	{
		return $this->UpdateValue('amount', $amount, $dbupdate);
	}

	/**
	 * Set value of m_ship_address
	 *
	 * @param string $ship_address
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetShipAddress($ship_address, $dbupdate = true)
	{
		return $this->UpdateValue('ship_address', $ship_address, $dbupdate);
	}

	/**
	 * Set value of m_transaction
	 *
	 * @param char $transaction
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetTransaction($transaction, $dbupdate = true)
	{
		return $this->UpdateValue('transaction', $transaction, $dbupdate);
	}

	/**
	 * Set value of m_gift_msg
	 *
	 * @param string $gift_msg
	 * @param bool $dbupdate
	 * @return bool
	*/
	function SetGiftMsg($gift_msg, $dbupdate = true)
	{
		return $this->UpdateValue('gift_msg', $gift_msg, $dbupdate);
	}

	/**
	 * Return value of m_shipped
	 *
	 * @return bool
	*/
	function GetShipped()
	{
		return $this->m_shipped;
	}

	/**
	 * Return value of m_ID
	 *
	 * @return char
	*/
	function GetBuyerID()
	{
		return $this->m_buyerID;
	}
	
	/**
	 * Return value of m_userID
	 *
	 * @return int
	*/
	function GetUserID()
	{
		return $this->m_userID;
	}

	/**
	 * Return value of m_amount
	 *
	 * @return 
	*/
	function GetAmount()
	{
		return $this->m_amount;
	}

	/**
	 * Return value of m_ship_address
	 *
	 * @return string
	*/
	function GetShipAddress()
	{
		return $this->m_ship_address;
	}

	/**
	 * Return value of m_transaction
	 *
	 * @return char
	*/
	function GetTransaction()
	{
		return $this->m_transaction;
	}

	/**
	 * Return value of m_gift_msg
	 *
	 * @return string
	*/
	function GetGiftMsg()
	{
		return $this->m_gift_msg;
	}

}
?>