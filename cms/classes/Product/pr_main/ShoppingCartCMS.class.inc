<?php
Site::IncludeFile ( 'classes/Common/CMSObject.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_main.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/ShoppingCart.class.inc' );

class ShoppingCartCMS extends CMSObject {
	/**
	 * Description
	 *
	 * 
	 */
	
	protected $m_product;
	
	function __construct() {
		parent::__construct ();
	}
	
	function DoAction() {
	}
	
	function DisplayMenu() {
		
	}
	
	function DisplayFeatures() {
		
	}
	
	function SelfTest() {
		$result = parent::SelfTest ();
		return $result;
	}
}
?>