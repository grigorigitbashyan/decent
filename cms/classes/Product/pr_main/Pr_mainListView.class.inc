<?php
Site::IncludeFile ( 'classes/Product/pr_main/ProductList.class.inc' );

/**
 * Enter description here...
 * 
 * @package Item
 */
class Pr_mainListView extends View
{
	/**
	 * Enter description here...
	 *
	 * @var ProductList
	 */
	protected $m_productList;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_catPath;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_catID;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_exParams;
	
	/**
	 * Enter description here...
	 *
	 * @var unknown_type
	 */
	protected $m_countInPage;
	
	/**
	 * Enter description here...
	 *
	 * @param int $catID
	 * @param string $rootPath
	 * @param int $limit
	 * @param array $externalParmas
	 */
	function __construct($catID, $rootPath, $limit = 10, $externalParmas = null)
	{
		parent::__construct ();
		$this->m_catPath = $rootPath;
		$this->m_catID = $catID;
		$this->m_exParams = $externalParmas;
		$this->m_countInPage = $limit;
		$this->m_productList = new ProductList($catID, $limit, $externalParmas);
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Display($totalCount)
	{
		$datePattern = null;
		// load date formats
		if ( isset($config->dateformats) )
		{
			foreach ( $config->dateformats->dateformat as $format )
			{
				$langID = ( string ) $format['id'];
				
				if( $langID==$this->m_displayLang )
				{
					$datePattern = ( string ) $format;
					break;
				}
			}
		}
		
		$patterns = array ();
		// read pattern from product type
		$products = $this->m_productList->GetStructure ();
		
		$additionLink = $this->m_productList->GetExternalLink ();
		
		if ( count($products)>0 )
		{
			$totalPattern = "";
			$typeArr = array();
			$next = -1;
			foreach ( $products as &$currProduct )
			{
				$next++;
				
				$ID = $currProduct['ID'];
				$type = $currProduct['typeID'];
				if ( !in_array($type, $typeArr) )
				{
					$typeArr[] = $type;
				}
				
				// if types is not loaded then load
				if ( !isset($patterns[$type]) )
				{
					$prType = new Pr_type($type);
					$patterns[$type]["main"] = $prType->GetPatternList($this->m_displayLang);
					$patterns[$type]["secondary"] = $prType->GetPatternList1($this->m_displayLang);
				}
				
				$nPr = new Pr_main($ID);
				
				// get all fields according for this type
				$typeFields = new Pr_typefieldList ( $type );
				$fields = $typeFields->GetStructure ();
				
				// replace
				if ( $nPr->GetPattern()=="secondary" && $patterns[$type]["secondary"]!="" )
				{
					$pattern = $patterns[$type]["secondary"];
				}
				else 
				{
					$pattern = $patterns[$type]["main"];
				}
				
				while ( ($start = strpos($pattern, '@@'))!==false )
				{
					$end = strpos($pattern, '@@', $start + 1);
					
					if ( !$end )
					{
						break;
					}
					
					$key = substr($pattern, $start, $end - $start + 2);
					$key = substr($key, 2, strlen($key) - 4);
					
					$subKey = $key;
					
					//TODO modified
					$value = null;
                    if ( $key=='name' || $key=='subtitle' || $key=='short_desc' || $key=='desc' )
                    {
                        if ( $currProduct["lang_".$this->m_displayLang] )
                        {
                            $value = $currProduct[$key];
                        }
                        else
                        {
                            if ( $currProduct["language"] )
                            {
                                $value = $currProduct[$key."_".$currProduct["language"]];
                            }
                            else
                            {
                                $value = $currProduct[$key];
                            }
                        }
                    }
					elseif ( $key=='picture' || $key=='cover' || $key=='ID' || $key=='date' || $key=='random' || $key=='viewCount' )
					{
						$value = $currProduct[$key];
						if ( $key=='date' )
						{
							$value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, $datePattern);
						}
					}
					elseif ( strpos($key, "desc")!==false && strpos($key, "desc")==0 && strpos($key, "|")!==false )
					{
                        if ( $currProduct["lang_".$this->m_displayLang] )
                        {
                            $value = $currProduct["desc"];
                        }
                        else
                        {
                            if ( $currProduct["language"] )
                            {
                                $value = $currProduct["desc_".$currProduct["language"]];
                            }
                            else
                            {
                                $value = $currProduct["desc"];
                            }
                        }
						$value = str_replace("</p>", "</p> ", $value);
						$value = strip_tags($value);
						$werbCount = intval(substr($key, strpos($key, "|")+1));
						if ( strlen($value)>$werbCount )
						{
							$value = substr($value, 0, strpos($value, " ", $werbCount))."...";
						}
					}
					elseif ( $key=='rootPath' )
					{
						$value = $this->m_catPath . $additionLink;
					}
					elseif ( $key=='categoryTitle' )
					{
						$value = $nPr->GetCategoryName($this->m_catID);
					}
					else 
					{
						$pos = strpos($key, ':');
						$separator = null;
						$param = null;
						if ( $pos > 0 )
						{
							$separator = $param = substr($key, $pos + 1);
							$key = substr($key, 0, $pos);
						}
						
						foreach ( $fields as $field )
						{
							if ( strcmp($field['name'], $key)==0 )
							{
								$value = $nPr->GetFieldValue ( $field['fieldID'], $this->m_displayLang, true, $param);
								
								$prField = new Pr_field($field['fieldID']);
								if ( $prField->GetSubtype()=="datefield" && $value!="" )
								{
									$value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, $datePattern);
								}
								
								if ( $prField->GetSubtype()=="url" && $value!="" )
								{
									if ( strpos($value, "http")!==0 )
									{
										$value = "http://".$value;
									}
								}
								break;
							}
						}
						
						if( is_array($value) )
						{
							$value = implode($separator, $value);
						}
					}
					// repalse
					$pattern = str_replace("@@$subKey@@", $value, $pattern);
					
					if ( $value )
					{
						$pattern = str_replace("<!--$subKey", "", $pattern);
						$pattern = str_replace("$subKey-->", "", $pattern);
					}
				}
				
				$totalPattern .= $pattern;
			}
			
			if ( count($typeArr)>1 )
			{
				print ( $totalPattern );
			}
			else 
			{
				print ( $prType->GetPatternListHeader($this->m_displayLang) );
				print ( $totalPattern );
				print ( $prType->GetPatternListFooter($this->m_displayLang) );
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayNavigation($afterLink="")
	{
		$this->m_productList->DisplayNavigation ( $this->m_catPath, true, null, null, 'span', null, $afterLink, true );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ProductList
	 */
	function GetProductList()
	{
		return $this->m_productList;
	}
	
	/**
	 * Return last few sorted products from products.
	 * The result array contains rows of product and each row contains only 
	 * `ID`, `typeID`, `name`, `short_desc`, `picture`, `sdate`, `date`, `random`, `catID`.
	 *
	 * @param array $typeID
	 * @param int $count
	 * @return array
	 */
	function GetLastProducts($typeID, $count = 5)
	{
		if (empty($typeID))
		{			
			$sql = "SELECT `pr_main`.`ID`, 
									`pr_main`.`typeID`, 
									`pr_main`.`name_{$this->m_displayLang}` as `name`, 
									`pr_main`.`short_desc_{$this->m_displayLang}` as `short_desc`, 
									`pr_main`.`picture`,
									`pr_main`.`date`, 
									`pr_main`.`random`, 
									`pr_options`.`relatedID` as `catID`
						  FROM `pr_main`  
						  LEFT JOIN `pr_options` ON `pr_main`.`ID` = `pr_options`.`productID`					  
						  ORDER BY `date` DESC LIMIT $count";
			
			return DB::QueryToArray($sql);			
		}
		else
		{
			$types = '';
			$typesCount = count($typeID);
			foreach ($typeID as $type)
			{
				$typesCount--;
				if ($typesCount == 0)
				{
					$types .= "'$type'";
				}
				else
				{
					$types .= "'$type',";
				}
			}
			// query string
			$sql = "SELECT `pr_main`.`ID`, 
									`pr_main`.`typeID`, 
									`pr_main`.`name_{$this->m_displayLang}` as `name`, 
									`pr_main`.`short_desc_{$this->m_displayLang}` as `short_desc`, 
									`pr_main`.`picture`,
									`pr_main`.`date`, 
									`pr_main`.`random`, 
									`pr_options`.`relatedID` as `catID`
						  FROM `pr_main`  
						  LEFT JOIN `pr_options` ON `pr_main`.`ID` = `pr_options`.`productID`					  
						  WHERE `typeID` IN ($types) ORDER BY `date` DESC LIMIT $count";
			
			return DB::QueryToArray($sql);		
		}
	}
}
?>