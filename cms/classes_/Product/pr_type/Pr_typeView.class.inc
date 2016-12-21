<?php
Site::IncludeFile ( 'classes/Product/pr_type/Pr_type.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_typeView extends View
{
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_types;
	
	function __construct()
	{
		parent::__construct ();
	}
	
	/**
	 * Not implemented!
	 *
	 * @param Pr_main $product
	 * @return string
	 */
	function MakeItemView()
	{
		return '';
	}
	
	/**
	 * Not implemented!
	 *
	 * @param Pr_main $product
	 * @return string
	 */
	function MakeListView()
	{
		return '';
	}
	
	/**
	 * Enter description here...
	 *
	 * @param Pr_main $product
	 * @return string
	 */
	function MakeGroupView($product, $subCatPath, $subCatName = '', $products=null)
	{
		$prType = $this->GetProductType ( $product );
		$pattern = $prType->GetPatternGroup ( $this->m_displayLang );
		while ( ($start = strpos ( $pattern, '@@' )) !== false )
		{
			$end = strpos ( $pattern, '@@', $start + 1 );
			
			if (! $end)
			{
				echo 'Error in template!';
				break;
			}
			
			$key = substr ( $pattern, $start, $end - $start + 2 );
			$key = substr ( $key, 2, strlen ( $key ) - 4 );
			
			$value = null;
			switch ($key)
			{
				case 'name' :
					$value = $product->GetName ( $this->m_displayLang );
					break;
				case 'short_desc' :
					$value = $product->GetDescShort ( $this->m_displayLang );
					break;
				case 'picture' :
					$value = $product->GetPicture ();
					break;
				case 'cover' :
					$value = $product->GetCover ();
					break;
				case 'ID' :
					$value = $product->GetID ();
					break;
				case 'subRootPath' :
					$value = $subCatPath;
					break;
				case 'subRootName' :
					$value = $subCatName;
					break;
				case 'random' :
					$value = $product->GetRandom();
					break;
				case 'products':
					$value = '';
					if ( is_array($products) )
					{
						for ( $p=0; $p<count($products); $p++ )
						{
							$value .= '<h4><i class="fa fa-angle-right"></i> <a href="'.$this->m_displayLang.'/'.$products[$p]["random"].'">'.$products[$p]["name"].'</a></h4>';
						}
					}
					break;
				default:
					$value = '';
			}
			
			// repalse
			$pattern = str_replace ( "@@$key@@", $value, $pattern );
			
		}
		return $pattern;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param Pr_main $product
	 * @return string
	 */
	function MakeSearchView($product, $subCatPath, $subCatName = '')
	{
		$prType = $this->GetProductType($product);
		$pattern = $prType->GetPatternSearch($this->m_displayLang);
		while ( ($start = strpos($pattern, '@@'))!==false )
		{
			$end = strpos($pattern, '@@', $start + 1);
			
			if ( !$end )
			{
				echo 'Error in template!';
				break;
			}
			
			$key = substr($pattern, $start, $end - $start + 2);
			$key = substr($key, 2, strlen($key) - 4);
			
			$value = null;
			switch ( $key )
			{
				case 'name' :
					$value = $product->GetName($this->m_displayLang);
					break;
				case 'short_desc' :
					$value = $product->GetDescShort($this->m_displayLang);
					break;
				case 'picture' :
					$value = $product->GetPicture();
					break;
				case 'ID' :
					$value = $product->GetID();
					break;
				case 'fullPath' :
					$value = $this->m_displayLang."/".$product->GetCategoryPath();
					break;
				case 'random' :
					$value = $product->GetRandom();
					break;
				default:
					$value = '';
			}
			
			// repalse
			$pattern = str_replace ( "@@$key@@", $value, $pattern );
			
		}
		return $pattern;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param Pr_main $product
	 * @return Pr_type
	 */
	protected function GetProductType($product)
	{
		$ptTypeID = $product->GetTypeID ();
		
		if (! isset ( $this->m_types [$ptTypeID] ))
		{
			$this->m_types [$ptTypeID] = new Pr_type ( $ptTypeID );
		}
		
		return $this->m_types [$ptTypeID];
	}
}
?>