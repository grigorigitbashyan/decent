<?php
$includes = array (
	'classes/Product/pr_main/Pr_main.class.inc', 
	'classes/Product/pr_main/ProductList.class.inc', 
	'classes/Product/pr_typefield/Pr_typefieldList.class.inc', 
	'classes/Product/pr_picture/Pr_pictureList.class.inc', 
	'classes/Product/pr_video/Pr_videoList.class.inc', 
	'classes/Product/pr_type/Pr_typeList.class.inc'
);

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
		$monthArr = array("01"=>"янв.", "02"=>"фев.", "03"=>"мар.", "04"=>"апр.", "05"=>"мая", "06"=>"июн.", "07"=>"июл.", "08"=>"авг.", "09"=>"сен.", "10"=>"окт.", "11"=>"ноя.", "12"=>"дек.");
		
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
		$cover = $this->m_product->GetCover ();
        $pattern = $prType->GetPattern ( $this->m_displayLang );
		
		// gallery pattern
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
                case 'shortDesc' :
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
				case 'noCoverClass':
					$value = "";
					$cover = $this->m_product->GetCover ();
					if ( $cover=="img/cover.gif" || $cover=="" )
					{
						$value = "heading-no-img";
					}
					
					break;
				case 'viewCount':
					$value = $this->m_product->GetViewCount();
					break;
				case 'author':
					$authorCat = new Category(122);
					$value = "<a href='".$this->m_site->GetSiteURL().$authorCat->GetPath().$this->m_product->GetAuthorRandom()."' style='color:#fff;'>".$this->m_product->GetAuthorSiteName()."</a>";
//					$value = $this->m_product->GetAuthorSiteName();
					break;
				case 'short_desc' :
					$value = $this->m_product->GetDescShort ( $this->m_displayLang );
					break;
				case 'date':
					$pDate = $this->m_product->GetPublishedDate();
					$uDate = $this->m_product->GetUpdatedDate();
					$value = "<span><i class='icon-uploaded'></i> ".ComputeDate($pDate)."</span>";
					if ( strtotime($uDate)>strtotime($pDate) )
					{
						$value .= "<span><i class='icon-updated'></i> ".ComputeDate($uDate)."</span>";
					}
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
				case 'map':
					$lat = $this->m_product->GetFieldValue(55, $this->m_displayLang, true);
					$lng = $this->m_product->GetFieldValue(56, $this->m_displayLang, true);
					if ( $lat && $lng )
					{
						$value = "<div style=\"width:100%;height:100%;\"><iframe src=\"mapSingle.php?id=".$this->m_product->GetID()."\" frameborder=\"0\" width=\"100%\" height=\"100%\"></iframe></div>";
					}
					break;
				case 'prev-next-items':
					$prOthers = $this->m_product->GetOthers(2, "random");
					if ( is_array($prOthers) )
					{
						$othersExIDs[] = $prOthers[0]["ID"];
						$othersExIDs[] = $prOthers[1]["ID"];
						
						$tempPr = new Pr_main($prOthers[0]["ID"]);
						$value = '
							<div class="content-wrap thumb-nav-b">
								<div class="inner-wrap">
									<div class="thumb-nav-wrap">
										<h2 style="text-align:center;">Смотрите Также</h2>
										
										<div class="button-block prev-b">
											<a href="'.$this->m_site->GetSiteURL().$this->m_displayLang.'/'.$tempPr->GetRandom().'">
												<div>
													<img src="'.$tempPr->GetCover().'" />
													<div class="icon-holder"><i class="fa fa-angle-left"></i></div>
													<h2>'.$tempPr->GetName($this->m_displayLang).'</h2>
													<div class="bl-overlay"></div>
												</div>
												
											</a>
										</div>
						';
						
						$tempPr = new Pr_main($prOthers[1]["ID"]);
						$value .= '
										<div class="button-block">
											<a href="'.$this->m_site->GetSiteURL().$this->m_displayLang.'/'.$tempPr->GetRandom().'">
												<div>
													<img src="'.$tempPr->GetCover().'" />
													<div class="icon-holder"><i class="fa fa-angle-right"></i></div>
													<h2>'.$tempPr->GetName($this->m_displayLang).'</h2>
												<div class="bl-overlay"></div>
												</div>
												
											</a>
										</div>
										<br clear="all" />
									</div>
								</div>
							</div>
						';
					}
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
									
									foreach ( $fields as $field )
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
						if ( strpos($subKey, "|") )
						{
							$showCount = intval(substr($subKey, 0, strpos($subKey, "|")));
							$orderBy = strtolower(substr($subKey, strpos($subKey, "|")+1));
							if ( $orderBy!="date" && $orderBy!="random" && $orderBy!="popular" && $orderBy!="recommended" )
							{
								$orderBy = "date";
							}
						}
						else 
						{
							$showCount = intval($subKey);
							$orderBy = "date";
						}
						
						$prOthers = $this->m_product->GetOthers($showCount, $orderBy, $othersExIDs);
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
							if (strcmp($field ['name'] , $subKey) == 0)
							{
								if ( $field['fieldType']==9 )
								{
									$tempField = new Pr_field($field['fieldID']);
									$fieldFields = $tempField->GetFields();
									$fieldFields = explode(",", $fieldFields);
									$fieldPattern = $tempField->GetFieldPattern();
									
									$tField = new Pr_field ( $fieldFields[0] );
									$fArr = $this->m_product->GetGFC($fieldFields[0], $tField->GetType());
									
									if ( is_array($fArr) && $fieldPattern )
									{
										if ( count($fArr)>1 )
										{
											$value .= "<div style=\"width:100%;height:100%;\"><iframe src=\"mapSingle.php?id=".$this->m_product->GetID()."&group=all\" frameborder=\"0\" width=\"100%\" height=\"100%\"></iframe></div>";
										}
										else 
										{
											for ( $i=0; $i<count($fArr); $i++ )
											{
												$fieldPatternTemp = $fieldPattern;
												$group = $fArr[$i];
												foreach ( $fieldFields as $fID )
												{
													$tField = new Pr_field($fID);
													$fieldValue = $this->m_product->GetFieldValue ( $fID, $this->m_displayLang, true, "key", $group );
													$fieldName = $tField->GetName($this->m_displayLang);
													$fieldPatternTemp = str_replace("@@$fieldName@@", $fieldValue, $fieldPatternTemp);
													if ( $fieldValue )
													{
														$fieldPatternTemp = str_replace("<!--$fieldName", "", $fieldPatternTemp);
														$fieldPatternTemp = str_replace("$fieldName-->", "", $fieldPatternTemp);
													}
												}
												
												if ( strpos($fieldPatternTemp, "@@map@@")!==false )
												{
													$fieldPatternTemp = str_replace("@@map@@", "<div style=\"width:370px;height:260px;\"><iframe src=\"mapSingle.php?id=".$this->m_product->GetID()."&group=$group\" frameborder=\"0\" width=\"370\" height=\"260\"></iframe></div>", $fieldPatternTemp);
												}
												elseif ( strpos($fieldPatternTemp, "@@map")!==false )
												{
													$mapKeyS = strpos($fieldPatternTemp, "@@map") + 6;
													$mapKeyE = strpos($fieldPatternTemp, "@@", $mapKeyS);
													$mapKey = $mapKey1 = substr($fieldPatternTemp, $mapKeyS, $mapKeyE - $mapKeyS);
													$mapKey = explode("|", $mapKey);
													$fieldPatternTemp = str_replace("@@map|$mapKey1@@", "<div style=\"width:".$mapKey[0]."px;height:".$mapKey[1]."px;\"><iframe src=\"mapSingle.php?id=".$this->m_product->GetID()."&group=$group\" frameborder=\"0\" width=\"".$mapKey[0]."\" height=\"".$mapKey[1]."\"></iframe></div>", $fieldPatternTemp);
												}
												
												$value .= $fieldPatternTemp;
											}
										}
									}
								}
								else 
								{
									$value = $this->m_product->GetFieldValue ( $field['fieldID'], $this->m_displayLang, true, $param);
								}
								
								break;
							}
						}
					}
					
					break;
			}
			
			if ( is_array($value) )
			{
				if ( $field['fieldID']==1 )
				{
					$articlesCat = new Category(124);
					$valueT = $value;
					$value = '';
					foreach ( $valueT as $val )
					{
						$tempOD = new Optionsdata($val);
						$value .= '<a href="'.$this->m_site->GetSiteURL().$articlesCat->GetPath().$tempOD->GetKey().'">'.$tempOD->GetName($this->m_displayLang).'</a>'.$param;
					}
					$value = substr($value, 0, strlen($value) - strlen($param));
				}
				else 
				{
					$value = implode($param, $value);
				}
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
						PM.`short_desc_$this->m_displayLang` as short_desc, 
						PM.`picture`, 
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
						$limit";
		$ProdArr = DB::QueryToArray($query);
		
		return $ProdArr;
	}
}
?>