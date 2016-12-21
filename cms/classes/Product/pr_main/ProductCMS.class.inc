<?php
// inculdes
Site::IncludeFile ( 'classes/Product/pr_main/Pr_mainListCMS.class.inc' );
Site::IncludeFile('classes/Folder/FolderXP.class.inc');
/**
 * Enter description here...
 *
 * @package Item
 */
class ProductCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_typeList
	 */
	protected $m_pr_typeList;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->Update ();
	}
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		$productTypeID = $this->m_pr_typeList->GetCurrentID ();
		// display insert new product // if it is not selected item
		

		if ($productTypeID > 0)
		{
			// display insert form for new product
			$newProduct = new Pr_mainListCMS ( $productTypeID );
			$newProduct->DisplayFeatures ();
		}
	}
	
	/**
	 * @see CMSInterface::DisplayMenu()
	 *
	 */
	function DisplayMenu()
	{
		$currID = $this->m_pr_typeList->GetCurrentID ();
		
		// corrent product
		$prList = new Pr_mainList();
		$curPrID = $prList->GetCurrentID();
		$pr = new Pr_main($curPrID);
		$curPrID = $pr->GetID();
		
		if ($currID && false)
		{
			$linkName = $this->m_pr_typeList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>Articles</a></div>" );
		}
		else
		{
			print ( '<div class="someWTitleBox">Articles</div>' );
		}
		print ( '<div class="someWBox">' );
		
		if ( DB::GET("status")!="all" )
		{
			$this->m_pr_typeList->SetAdditionalPathSelected("&status=".DB::GET("status"));
		}
		
		if($curPrID)
		{
			$this->m_pr_typeList->DisplayList ( 'directoryMenu1', 'selLink', 'selectedLink' );
		}
		else 
		{
			$this->m_pr_typeList->DisplayList ( 'directoryMenu1', 'selLink' );
		}
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * @see CMSInterface::DoAction()
	 *
	 */
	function DoAction()
	{
		$productTypeID = $this->m_pr_typeList->GetCurrentID ();
		
		// get product overloading link name
		$newProduct = new Pr_mainListCMS ( $productTypeID );
		$newProduct->DoAction ();
	}
	
	function Update()
	{
		$this->m_pr_typeList = new Pr_typeList ( );
	}
	
	function CreateDB()
	{
		$prList = new Pr_mainListCMS();
		$prList->CreateDB();
	}
	
/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		// chack database
		$prList = new Pr_mainListCMS();
		$res = $prList->SelfTest();
		
		// chack files /////////////////////////////////
		
		// cache xml
		if(!file_exists('config/product.xml'))
		{
			$res = false;
			print('The config file <b>product.xml</b> does not exist<br />');	
		}
		
		// get product folder, if not exist create it
		$prodFolderPath = '../' . $this->m_site->GetProImageRoot (true, false);
		
		// if the dir does not exist and it is allowed to make it then try to do that. 
		if(!file_exists("$prodFolderPath") && $this->CanBeCorrected())
		{print 'mkdir';
			FolderXP::Mkdir("$prodFolderPath", 0755);
		}

		// check is dir exist
		if(!file_exists("$prodFolderPath"))
		{
			$res = false;
			print("The product's folder <b>$prodFolderPath</b> does not exist<br />");	
		}
		
		return $res; 
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		// chack database
		$prList = new Pr_mainListCMS();
		return $prList->GetObjectTables();
	}
}
?>