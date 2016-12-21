<?php
$includes = array ('classes/Product/pr_main/Pr_main.class.inc', 'classes/Product/pr_main/ProductList.class.inc', 'classes/Product/pr_typefield/Pr_typefieldList.class.inc', 'classes/Product/pr_picture/Pr_pictureList.class.inc', 'classes/Product/pr_type/Pr_typeList.class.inc' );

Site::IncludeFiles ( $includes );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_mainView extends View
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_main
	 */
	protected $m_product;
	
	/**
	 * Enter description here...
	 *
	 * @var Pr_typeList
	 */
	protected $m_prTypeList;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID)
	{
		parent::__construct ();
		
		$this->m_product = new Pr_main ( $ID );
		$this->m_prTypeList = new Pr_typeList();
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Display()
	{
		// read date format from xml
		$config = simplexml_load_file('cms/config/product.xml');
		
		$datePattern = null;
		// load date formats
		if (isset ( $config->dateformats ))
		{
			foreach ( $config->dateformats->dateformat as $format )
			{
				$langID = ( string ) $format ['id'];
				
				if($langID == $this->m_displayLang)
				{
					$datePattern = ( string ) $format;
					break;
				}
			}
		}
		
		// get product type
		$prRelations = $this->m_product->GetRelations();
		
		$typeID = $this->m_product->GetTypeID ();
		$prType = new Pr_type ( $typeID );
		
		// get all fields according for this type
		$typeFields = new Pr_typefieldList ( $typeID );
		$fields = $typeFields->GetStructure ();
		
		// get patern
		$pattern = $prType->GetPattern ( $this->m_displayLang );
		
		// gallery patter
		$gallPattern = $prType->GetPatternGallery ( $this->m_displayLang );
		
		$othersExIDs = array();
		// replace
		while ( ($start = strpos ( $pattern, '@@' )) !== false )
		{
			$end = strpos ( $pattern, '@@', $start + 1 );
			
			if (! $end)
			{
				break;
			}
			
			$key = substr ( $pattern, $start, $end - $start + 2 );
			$key = substr ( $key, 2, strlen ( $key ) - 4 );
			
			$subKey = $key; 
			$pos = strpos($key, ':');
			$param = null;
			if($pos > 0)
			{
				$subKey = substr($key, 0, $pos);
				$param = substr($key, $pos + 1);
			}
			$value = null;
			switch ($subKey)
			{
				case 'ID' :
					$value = $this->m_product->GetID();
					break;
				case 'typeID' :
					$value = $typeID;
					break;
				case 'random' :
					$value = $this->m_product->GetRandom();
					break;
				case 'name' :
                    if ( $this->m_product->GetLang($this->m_displayLang) )
                    {
                        $language = $this->m_displayLang;
                    }
                    else
                    {
                        $language = $this->m_product->GetLanguage();
                        if ( !$language )
                        {
                            $language = $this->m_displayLang;
                        }
                    }
                    $value = $this->m_product->GetName ( $language );
					break;
				case 'subtitle' :
                    if ( $this->m_product->GetLang($this->m_displayLang) )
                    {
                        $language = $this->m_displayLang;
                    }
                    else
                    {
                        $language = $this->m_product->GetLanguage();
                        if ( !$language )
                        {
                            $language = $this->m_displayLang;
                        }
                    }
                    $value = $this->m_product->GetSubtitle( $language );
					break;
                case 'language' :
                    if ( $this->m_product->GetLang($this->m_displayLang) )
                    {
                        $value = $this->m_displayLang;
                    }
                    else
                    {
                        $value = $this->m_product->GetLanguage();
                        if ( !$value )
                        {
                            $value = $this->m_displayLang;
                        }
                    }
                    break;
				case 'desc' :
                    if ( $this->m_product->GetLang($this->m_displayLang) )
                    {
                        $language = $this->m_displayLang;
                    }
                    else
                    {
                        $language = $this->m_product->GetLanguage();
                        if ( !$language )
                        {
                            $language = $this->m_displayLang;
                        }
                    }
                    $value = $this->m_product->GetDesc($language);
					break;
                case 'short_desc' :
                    if ( $this->m_product->GetLang($this->m_displayLang) )
                    {
                        $language = $this->m_displayLang;
                    }
                    else
                    {
                        $language = $this->m_product->GetLanguage();
                        if ( !$language )
                        {
                            $language = $this->m_displayLang;
                        }
                    }
                    $value = $this->m_product->GetDescShort( $language );
                    break;
				case 'picture' :
					$value = $this->m_product->GetPicture ();
                    if ( $value=="img/thumbnail.gif" )
                    {
                        $value = "";
                    }
					break;
				case 'cover' :
					$value = $this->m_product->GetCover ();
					if ( $value=="img/cover.gif" )
					{
						$value = "";
					}
					break;
                case 'viewCount':
                    $value = $this->m_product->GetViewCount();
                    break;
				case 'date':
					$value = $this->m_product->GetDate();
					$value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, $datePattern);
					break;
				case 'gallery' :
					$gallery = new Pr_pictureList ( $this->m_product->GetID () );
					$value = $gallery->GetPatternDesign ( $gallPattern );
					break;
				case 'lang' :
					$value = $this->m_displayLang;
					break;
				case 'siteURL' :
					$value = $this->m_site->GetSiteURL();
					break;
				default :
					$value = "";
					if ( substr($subKey, 0, 3)=="PT|" )		//	check if key belongs to product type
					{
						$prName = substr($subKey, 3);
						$showCount = 1000;
						$relField = 0;
						if ( strpos($prName, "|") )
						{
							$showCount = trim(substr($prName, strpos($prName, "|") + 1));
							$prName = substr($prName, 0, strpos($prName, "|"));
							
							if ( strpos($showCount, "|") )
							{
								$relField = trim(substr($showCount, strpos($showCount, "|") + 1));
								$showCount = intval(substr($showCount, 0, strpos($showCount, "|")));
								
								$query = "select `ID` from `pr_field` where `name` = '$relField'";
								$relField = intval(DB::QueryToValue($query));
							}
						}
						$relPr = $this->m_prTypeList->GetPrTypeByName($prName);
						
						$rPrPatternHeader = str_replace("@@lang@@", $this->m_displayLang, $relPr[0]["patternrelationheader_".$this->m_displayLang]);
						$rPrPatternFooter = str_replace("@@lang@@", $this->m_displayLang, $relPr[0]["patternrelationfooter_".$this->m_displayLang]);
						$rPrPatternMain = $relPr[0]["patternrelation_".$this->m_displayLang];
						$rPrPatternGallery = $relPr[0]["patterngallery_".$this->m_displayLang];
						
						$prRelations = $this->m_product->GetRelations($showCount, $relPr[0]["ID"], $relField);
						for ( $r=0; $r<count($prRelations); $r++ )
						{
							$tempPr = new Pr_main($prRelations[$r]);
							
							$rPrPattern = $rPrPatternMain;
							while ( ($start = strpos($rPrPattern, '@@'))!==false )
							{
								$end = strpos($rPrPattern, '@@', $start + 1);
								if ( !$end )
								{
									break;
								}
								
								$kkey = substr($rPrPattern, $start, $end - $start + 2);
								$kkey = substr($kkey, 2, strlen($kkey) - 4);
								
								$subKkey = $kkey;
								
								//TODO modified
								$vvalue = null;
								if ( $kkey=='name' || $kkey=='short_desc' || $kkey=='picture' || $kkey=='ID' || $kkey=='date' || $kkey=='random' || $kkey=='desc' || $kkey=='categoryTitle' )
								{
									switch ( $kkey )
									{
										case 'name': $vvalue = $tempPr->GetName($this->m_displayLang); break;
										case 'short_desc': $vvalue = $tempPr->GetDescShort($this->m_displayLang); break;
										case 'picture': $vvalue = $tempPr->GetPicture(); break;
										case 'cover': $vvalue = $tempPr->GetCover(); break;
										case 'ID': $vvalue = $tempPr->GetID(); break;
										case 'desc': $vvalue = $tempPr->GetDesc($this->m_displayLang); break;
										case 'random': $vvalue = $tempPr->GetRandom(); break;
										case 'categoryTitle': $vvalue = $tempPr->GetCategoryName(); break;
										case 'date':
											$vvalue = $tempPr->GetDate();
											$vvalue = $this->m_site->GetDateTransformation($vvalue, $this->m_displayLang, $datePattern);
											break;
									}
								}
								elseif ( strpos($kkey, "desc")!==false && strpos($kkey, "desc")==0 && strpos($kkey, "|")!==false )
								{
									$vvalue = $tempPr->GetDesc($this->m_displayLang);
									$vvalue = str_replace("</p>", "</p> ", $vvalue);
									$vvalue = strip_tags($vvalue);
									$werbCount = intval(substr($kkey, strpos($kkey, "|")+1));
									if ( strlen($vvalue)>$werbCount )
									{
										$vvalue = substr($vvalue, 0, strpos($vvalue, " ", $werbCount))."...";
									}
								}
								elseif ( $kkey=='subRootPath' )
								{
									$vvalue = $tempPr->GetCategoryPath();
								}
								elseif ( $kkey=='lang' )
								{
									$vvalue = $this->m_displayLang;
								}
								elseif ( $kkey=='gallery' )
								{
									$gallery = new Pr_pictureList ( $tempPr->GetID() );
									$vvalue = $gallery->GetPatternDesign ( $rPrPatternGallery );
								}
								else 
								{
									$pos = strpos($kkey, ':');
									$separator = null;
									$param = null;
									if ( $pos > 0 )
									{
										$separator = $param = substr($kkey, $pos + 1);
										$kkey = substr($kkey, 0, $pos);
									}
									
									$typeFields1 = new Pr_typefieldList ( $tempPr->GetTypeID() );
									$fields1 = $typeFields1->GetStructure ();
									foreach ( $fields1 as $field )
									{
										if ( strcmp($field['name'], $kkey)==0 )
										{
											$vvalue = $tempPr->GetFieldValue ( $field['fieldID'], $this->m_displayLang, true, $param);
											if ( ($field['fieldID']==42 || $field['fieldID']==43) && $vvalue!="" )
											{
												$vvalue = $this->m_site->GetDateTransformation($vvalue, $this->m_displayLang, $datePattern);
											}
											break;
										}
									}
									
									if( is_array($vvalue) )
									{
										$vvalue = implode($separator, $vvalue);
									}
								}
								// replace
								$rPrPattern = str_replace("@@$subKkey@@", $vvalue, $rPrPattern);
								
								if ( $vvalue )
								{
									$rPrPattern = str_replace("<!--$subKkey", "", $rPrPattern);
									$rPrPattern = str_replace("$subKkey-->", "", $rPrPattern);
								}
							}
							
							$value .= $rPrPattern;
						}
						
						if ( $value!="" )
						{
							$rPrPatternHeader = str_replace("@@lang@@", $this->m_displayLang, $rPrPatternHeader);
							$rPrPatternHeader = str_replace("@@subRootPath@@", $tempPr->GetMainCategoryPath(), $rPrPatternHeader);
							
							$value = $rPrPatternHeader.$value.$rPrPatternFooter;
						}
					}
					elseif ( substr($subKey, 0, 9)=="Products|" )		//	check if key belongs to product type
					{
						$prName = substr($subKey, 9);
						$showCount = 1000;
						if ( strpos($prName, "|") )
						{
							$prName = explode("|", $prName);
							
							$showCount = $prName[1];
							$prName = $prName[0];
						}
						$relPr = $this->m_prTypeList->GetPrTypeByName($prName);
						
						$rPrPatternHeader = str_replace("@@lang@@", $this->m_displayLang, $relPr[0]["patternrelationheader_".$this->m_displayLang]);
						$rPrPatternFooter = str_replace("@@lang@@", $this->m_displayLang, $relPr[0]["patternrelationfooter_".$this->m_displayLang]);
						$rPrPatternMain = $relPr[0]["patternrelation_".$this->m_displayLang];
						$rPrPatternGallery = $relPr[0]["patterngallery_".$this->m_displayLang];
						
						$prRelations = new ProductList();
						$prRelations = $prRelations->GetAllProducts($relPr[0]["ID"], $showCount, "`date`");
						for ( $r=0; $r<count($prRelations); $r++ )
						{
							$tempPr = new Pr_main($prRelations[$r]["ID"]);
							
							$rPrPattern = $rPrPatternMain;
							while ( ($start = strpos($rPrPattern, '@@'))!==false )
							{
								$end = strpos($rPrPattern, '@@', $start + 1);
								if ( !$end )
								{
									break;
								}
								
								$kkey = substr($rPrPattern, $start, $end - $start + 2);
								$kkey = substr($kkey, 2, strlen($kkey) - 4);
								
								$subKkey = $kkey;
								
								//TODO modified
								$vvalue = null;
								if ( $kkey=='ID' || $kkey=='typeID' || $kkey=='name' || $kkey=='subtitle' || $kkey=='desc' || $kkey=='short_desc' || $kkey=='picture' || $kkey=='cover' || $kkey=='date' || $kkey=='random' )
								{
									$vvalue = $prRelations[$r][$kkey];
									if ( $kkey=="date" )
									{
										$vvalue = $this->m_site->GetDateTransformation($vvalue, $this->m_displayLang, $datePattern);
									}
								}
								elseif ( $kkey=='categoryTitle' )
								{
									$vvalue = $tempPr->GetCategoryName();
								}
								elseif ( $kkey=='date-year' || $kkey=='date-month' || $kkey=='date-day' )
								{
									$date = explode("-", $prRelations[$r]["date"]);
									switch ( $kkey )
									{
										case 'date-year': $vvalue = $date[0]; break;
										case 'date-month': $vvalue = $date[1]; break;
										case 'date-day': $vvalue = $date[2]; break;
									}
									
									if ( $relPr[0]["ID"]==12 && $r<3 )
									{
										$vvalue = "Биография";
									}
								}
								elseif ( strpos($kkey, "desc")!==false && strpos($kkey, "desc")==0 && strpos($kkey, "|")!==false )
								{
									$vvalue = $tempPr->GetDesc($this->m_displayLang);
									$vvalue = str_replace("</p>", "</p> ", $vvalue);
									$vvalue = strip_tags($vvalue);
									$werbCount = intval(substr($kkey, strpos($kkey, "|")+1));
									if ( strlen($vvalue)>$werbCount )
									{
										$vvalue = substr($vvalue, 0, strpos($vvalue, " ", $werbCount))."...";
									}
								}
								elseif ( $kkey=='subRootPath' )
								{
									$vvalue = $tempPr->GetCategoryPath();
								}
								elseif ( $kkey=='lang' )
								{
									$vvalue = $this->m_displayLang;
								}
								elseif ( $kkey=='gallery' )
								{
									$gallery = new Pr_pictureList ( $tempPr->GetID() );
									$vvalue = $gallery->GetPatternDesign ( $rPrPatternGallery );
								}
								else 
								{
									$pos = strpos($kkey, ':');
									$separator = null;
									$param = null;
									if ( $pos > 0 )
									{
										$separator = $param = substr($kkey, $pos + 1);
										$kkey = substr($kkey, 0, $pos);
									}
									
									$typeFields1 = new Pr_typefieldList ( $tempPr->GetTypeID() );
									$fields1 = $typeFields1->GetStructure ();
									foreach ( $fields1 as $field )
									{
										if ( strcmp($field['name'], $kkey)==0 )
										{
											$vvalue = $tempPr->GetFieldValue ( $field['fieldID'], $this->m_displayLang, true, $param);
											if ( ($field['fieldID']==18 || $field['fieldID']==19) && $vvalue!="" )
											{
												$vvalue = $this->m_site->GetDateTransformation($vvalue, $this->m_displayLang, $datePattern);
											}
											break;
										}
									}
									
									if( is_array($vvalue) )
									{
										$vvalue = implode($separator, $vvalue);
									}
								}
								// repalse
								$rPrPattern = str_replace("@@$subKkey@@", $vvalue, $rPrPattern);
								
								if ( $vvalue )
								{
									$rPrPattern = str_replace("<!--$subKkey", "", $rPrPattern);
									$rPrPattern = str_replace("$subKkey-->", "", $rPrPattern);
								}
							}
							
							$value .= $rPrPattern;
						}
						
						if ( $value!="" )
						{
							$rPrPatternHeader = str_replace("@@lang@@", $this->m_displayLang, $rPrPatternHeader);
							$rPrPatternHeader = str_replace("@@subRootPath@@", $tempPr->GetMainCategoryPath(), $rPrPatternHeader);
							
							$value = $rPrPatternHeader.$value.$rPrPatternFooter;
						}
					}
					elseif ( substr($subKey, 0, 3)=="OP|" )		//	check if key belongs to same product type
					{
						$othersPattern = $prType->GetPatternOther($this->m_displayLang);
						$othersHeaderPattern = $prType->GetPatternOtherHeader($this->m_displayLang);
						$othersFooterPattern = $prType->GetPatternOtherFooter($this->m_displayLang);
						
						$subKey = substr($subKey, 3);
						
						$tempKey = explode("|", $subKey);
						
						$showCount = intval($tempKey[0]);
						if ( isset($tempKey[1]) && ($tempKey[1]=="date" || $tempKey[1]=="random" || $tempKey[1]=="popular") )
						{
							$orderBy = $tempKey[1];
						}
						else 
						{
							$orderBy = "date";
						}
						if ( isset($tempKey[2]) && $tempKey[2]=="true" )
						{
							$selectCurrent = true;
						}
						else 
						{
							$selectCurrent = false;
						}
						
						$prOthers = $this->m_product->GetOthers($showCount, $orderBy, $othersExIDs, $selectCurrent);
						for ( $r=0; $r<count($prOthers); $r++ )
						{
							$tempPr = new Pr_main($prOthers[$r]["ID"]);

                            if ( $tempPr->GetLang($this->m_displayLang) )
                            {
                                $language = $this->m_displayLang;
                            }
                            else
                            {
                                $language = $tempPr->GetLanguage();
                                if ( !$language )
                                {
                                    $language = $this->m_displayLang;
                                }
                            }

							$value .= $othersPattern;
							$value = str_replace("@@name@@", $tempPr->GetName($language), $value);
							$value = str_replace("@@subtitle@@", $tempPr->GetSubtitle($language), $value);
							$value = str_replace("@@date@@", $this->m_site->GetDateTransformation($tempPr->GetDate(), $this->m_displayLang, $datePattern), $value);
							$value = str_replace("@@random@@", $tempPr->GetRandom(), $value);
							$value = str_replace("@@picture@@", $tempPr->GetPicture(), $value);
							$value = str_replace("@@desc@@", $tempPr->GetDesc($language), $value);
							$value = str_replace("@@short_desc@@", $tempPr->GetDescShort($language), $value);
							$value = str_replace("@@lang@@", $this->m_displayLang, $value);
							$value = str_replace("@@subRootPath@@", $tempPr->GetCategoryPath(), $value);
							$value = str_replace("@@categoryTitle@@", $tempPr->GetCategoryName(), $value);
							
							if ( strpos($value, "@@desc|")!==false )
							{
								$desc = $tempPr->GetDesc($language);
								$desc = str_replace("</p>", "</p> ", $desc);
								$desc = strip_tags($desc);
								
								$werbCount = intval(substr($value, strpos($value, "@@desc|")+7, strpos($value, "@@", strpos($value, "@@desc|")+7)-strpos($value, "@@desc|")-7));
								if ( strlen($desc)>$werbCount )
								{
									$desc = substr($desc, 0, strpos($desc, " ", $werbCount))."...";
								}
								
								$value = str_replace("@@desc|$werbCount@@", $desc, $value);
							}
						}
						
						if ( $value!="" )
						{
							$value = $othersHeaderPattern.$value.$othersFooterPattern;
						}
					}
					elseif ( substr($subKey, 0, 8)=="catPath|" )
					{
						$catID = substr($subKey, 8);
						$tempCat = new Category($catID);
						$value = $tempCat->GetPath();
					}
					elseif ( substr($subKey, 0, 8)=="rootPath" )
					{
						$query = "select PO.`relatedID` from `pr_options` PO join `pr_field` PF on (PO.`fieldID` = PF.`ID` and PF.`type` = 2) where `productID` = ".$this->m_product->GetID()."";
						$catID = DB::QueryToValue($query);
						$tempCat = new Category($catID);
						$value = $tempCat->GetPath();
					}
					else 
					{
						// get from parimiter
						foreach ( $fields as $field )
						{
							if (strcmp($field['name'], $subKey) == 0)
							{
                                $value = $this->m_product->GetFieldValue ( $field ['fieldID'], $this->m_displayLang, true , $param);
                                if ( ($field['fieldID']==18 || $field['fieldID']==19) && $value!="" )
                                {
                                    $value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, $datePattern);
                                }
								
								break;
							}
						}
					}
					
					break;
			}
			
			if(is_array($value))
			{
				$value = implode($param, $value);
			}
			
			// replace pattern value and destroy comments
			if ( $value )
			{
				if ( strpos($pattern, "<!--".$key)!==false )
				{
					$patPosS = strpos($pattern, "<!--".$key);
					$patPosE = strpos($pattern, $key."-->", $patPosS);
					while ( $patPosS )
					{
						$pattern = substr_replace ( $pattern, "", $patPosE, strlen($key."-->"));
						$pattern = substr_replace ( $pattern, "", $patPosS, strlen("<!--".$key));
						
						$patPosS = strpos($pattern, "<!--".$key);
						$patPosE = strpos($pattern, $key."-->", $patPosS);
					}
				}
			}
			else
			{
				$value = '';
			}			
			
			// replace only current $key value ($key values can be more then one)
			$patPos = strpos($pattern, "@@$key@@");
			$pattern = substr_replace ( $pattern, $value, $patPos, strlen("@@$key@@"));
			if ( !$value )
			{
				if ( strpos($pattern, "<!--".$key)!==false )
				{
					$patPosS = strpos($pattern, "<!--".$key);
					$patPosE = strpos($pattern, $key."-->", $patPosS);
					
					$pattern = substr_replace ( $pattern, "", $patPosS, $patPosE - $patPosS + strlen("<!--".$key));
				}
			}
		}
		
		print ( $pattern );
		
		$query = "update `pr_main` set `viewCount` = `viewCount` + 1 where `ID` = ".$this->m_product->GetID()."";
		DB::Query($query);
	}
	
	/**
	 * Enter description here...
	 *
	 * @return Pr_main;
	 */
	function GetProduct()
	{
		return $this->m_product;
	}
	
	/**
	 * Get price if sale, so sale, if no sale so real price
	 * Enter description here ...
	 * @param $pID
	 */
	function GetProductRelationWithStructure($pID)
	{
		// get sale price
		$relation = DB::QueryToValue("SELECT `relatedID` FROM `pr_options` WHERE `productID` = $pID AND `fieldID` = 13;");
		
		return $relation;
	}
	
	/**
	 * Get price if sale, so sale, if no sale so real price
	 * Enter description here ...
	 * @param $pID
	 */
	function GetOtherProductsWithSameRelation($relID, $limit="0, 5")
	{
		$query = "select `ID` from `structure` where `parentID` = (select `parentID` from `structure` where `ID` = $relID)";
		$StrArr = DB::QueryToArray($query);
		
		$RelStr = "(PO.`relatedID` = $relID ";
		for ( $j=0; $j<count($StrArr) && $StrArr[$j]!=""; $j++ ) {
			
			$RelStr .= " or PO.`relatedID` = ".$StrArr[$j]["ID"]." ";
		}
		$RelStr .= " )";
		
		$query = "
			select 
				PM.`ID`, 
				PM.`random` as link, 
				PM.`name_$this->m_displayLang` as name, 
				PM.`subtitle_$this->m_displayLang` as subtitle, 
				PM.`short_desc_$this->m_displayLang` as short_desc, 
				PM.`picture`, 
				PM.`cover`, 
				PM.`date`, 
				S.`path` 
			from 
				`pr_main` PM 
			join 
				`pr_options` PO on (PM.`ID` = PO.`productID`)
			join
				`structure` S on (PO.`relatedID` = S.`ID`) 
			where 
				PO.`fieldID` = 13 and $RelStr and PM.`ID` <> ".$this->m_product->GetID()."
			order by 
				RAND()
			limit 
				$limit
		";
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
}
?>