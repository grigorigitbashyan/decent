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
		$this->m_productList = new ProductList($catID, $limit, $externalParmas);
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Display($tagName="")
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
			//	get page content tags
			$overallTags = array();
			foreach ( $products as &$currProduct )
			{
				$ID = $currProduct['ID'];
				$nPr = new Pr_main($ID);
				$prTags = $nPr->GetFieldValue(71);
				$overallTags = array_merge($overallTags, $prTags);
			}
			$overallTags = array_unique($overallTags);
			
			//	get current tag
			$query = "select `ID` from `optionsdata` where `key` = '".$this->m_exParams[0]."'";
			$currentTagID = DB::QueryToValue($query);
			
			//	remove current tag from overall tags
			$currentTagKey = array_search($currentTagID, $overallTags);
			unset($overallTags[$currentTagKey]);
			$overallTags = array_values($overallTags);
			
			
			$totalPattern = "";
			$typeArr = array();
			$index = 0;

			foreach ( $products as &$currProduct )
			{
				$index++;
				
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
				
				//	check if article has icon
				$useSecondaryPattern = false;
				if ( $type==6 && $index%2==1 )
				{
					$useSecondaryPattern = true;
				}
				
				// replace
				if ( ($nPr->GetPattern()=="secondary" || $useSecondaryPattern) && $patterns[$type]["secondary"]!="" )
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
					if ( $key=='name' || $key=='short_desc' || $key=='desc' )
					{
                        if ( $currProduct["lang_".$this->m_displayLang] )
                        {
                            $value = $currProduct[$key];
                        }
                        else
                        {
                            $value = $currProduct[$key."_".$currProduct["language"]];
                        }
					}
					elseif ( $key=='picture' || $key=='ID' || $key=='updated_date' || $key=='random' || $key=='viewCount' || $key=='date' )
                    {
                        $value = $currProduct[$key];
                        if ( $key=='date' )
                        {
                            $value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, $datePattern);
                        }
                    }
					elseif ( $key=='picture1' || $key=='picture2' || $key=='picture3' || $key=='picture4' )
					{
						if ( $currProduct[$key] )
						{
							$value = $currProduct[$key];
						}
						else 
						{
							$value = $currProduct["picture"];
						}
					}
					elseif ( $key=='author' )
					{
						$authorCat = new Category(122);
						$value = "<a href='".$this->m_site->GetSiteURL().$authorCat->GetPath().$nPr->GetAuthorRandom()."'>".$nPr->GetAuthorSiteName()."</a>";
					}
					elseif ( $key=='published_date' )
					{
						$value = ComputeDate($currProduct[$key]);
					}
					elseif ( strpos($key, "desc")!==false && strpos($key, "desc")==0 && strpos($key, "|")!==false )
					{
                        if ( $currProduct["language"] )
                        {
                            $value = $currProduct["desc_".$currProduct["language"]];
                        }
						else
                        {
                            $value = $currProduct["desc"];
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
								if ( $field['fieldID']==71 )
								{
									$value = $nPr->GetFieldValue ( $field['fieldID'], $this->m_displayLang, false, $param);
								}
								else 
								{
									$value = $nPr->GetFieldValue ( $field['fieldID'], $this->m_displayLang, true, $param);
								}
								
								if ( ($field['fieldID']==42 || $field['fieldID']==43) && $value!="" )
								{
									$value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, $datePattern);
								}
								break;
							}
						}
						
						if( is_array($value) )
						{
							if ( $field['fieldID']==71 )
							{
								$articlesCat = new Category(124);
								$valueT = $value;
								$value = '';
								foreach ( $valueT as $val )
								{
									$tempOD = new Optionsdata($val);
									$value .= '<a href="'.$this->m_site->GetSiteURL().$articlesCat->GetPath().$tempOD->GetKey().'">'.$tempOD->GetName($this->m_displayLang).'</a>'.$separator;
								}
								$value = substr($value, 0, strlen($value) - strlen($separator));
							}
							else 
							{
								$value = implode($separator, $value);
							}
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
				if ( strpos($totalPattern, "#booking_area#")!==false )
				{
					$totalPattern_1 = substr($totalPattern, 0, strpos($totalPattern, "#booking_area#"));
					$totalPattern_2 = substr($totalPattern, strpos($totalPattern, "#booking_area#") + 14);
					
					print ( $totalPattern_1 );
					include("files/booking_form.php");
					print ( $totalPattern_2 );
				}
				else 
				{
					print ( $totalPattern );
				}
			}
			else 
			{
				$patternHeader = $prType->GetPatternListHeader($this->m_displayLang);
				$patternHeader = str_replace("@@tagName@@", $tagName, $patternHeader);
				
				print ( $patternHeader );
				if ( strpos($totalPattern, "#booking_area#")!==false )
				{
					$totalPattern_1 = substr($totalPattern, 0, strpos($totalPattern, "#booking_area#"));
					$totalPattern_2 = substr($totalPattern, strpos($totalPattern, "#booking_area#") + 14);
					
					print ( $totalPattern_1 );
					include("files/booking_form.php");
					print ( $totalPattern_2 );
				}
				else 
				{
					print ( $totalPattern );
				}
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
	
	function GetProductsByTags($tags, $currentTagID, $limit=3)
	{
		$tagsStr = implode(", ", $tags);
		
		$query = "
			SELECT 
				PM.`ID`, 
				PM.`typeID`, 
				PM.`name_{$this->m_displayLang}` as `name`, 
				PM.`desc_{$this->m_displayLang}` as `desc`, 
				PM.`short_desc_{$this->m_displayLang}` as `short_desc`, 
				PM.`picture`, 
				PM.`cover`, 
				PM.`date` as `sdate`, 
				PM.`published_date`, 
				PM.`updated_date`, 
				PM.`viewCount`, 
				PM.`googleViewCount`, 
				PM.`googleUniqueViewCount`, 
				PM.`random` 
			FROM `pr_main` PM 
			JOIN `pr_options` PO ON (PM.`ID` = PO.`productID`) 
			JOIN `optionsdata` OD ON (OD.`ID` = PO.`relatedID` and OD.`ID` in ($tagsStr) and OD.`optionsID` = 28) 
			WHERE PM.`status` = 1
			GROUP BY PM.`ID`";
		$retArr = DB::QueryToArray($query);
		
		$newArr = array();
		for ( $i=0; $i<count($retArr); $i++ )
		{
			$tempPr = new Pr_main($retArr[$i]["ID"]);
			$tempTags = $tempPr->GetFieldValue(71);
			if ( !in_array($currentTagID, $tempTags) )
			{
				$newArr[] = $retArr[$i];
				
				if ( count($newArr)==3 )
				{
					break;
				}
			}
		}
		
		return $newArr;
	}
}
?>