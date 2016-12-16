<?php
// inculdes
Site::IncludeFile ( 'classes/Category/Category.class.inc' );
Site::IncludeFile ( 'classes/Menu/Menu.class.inc' );
Site::IncludeFile ( 'classes/Menu/MenuView.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_main/Pr_main.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_options.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_optionsList.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_optionsCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/ProductList.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_typefield/Pr_typefieldList.class.inc' );

Site::IncludeFile ( 'classes/Product/pr_double/Pr_doubleCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_int/Pr_intCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_char/Pr_charCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_text/Pr_textCMS.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_picture/Pr_pictureCMS.class.inc' );

Site::IncludeFile ( 'classes/UserManagement/UserTypeList.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserList.class.inc' );

/**
 * Enter description here...
 *
 * @package Item
 */
class Pr_mainCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Pr_main
	 */
	protected $m_pr_main;
	
	protected $m_errorText;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->Update ();
		
		$this->ReadNameStrings ( 'classes/Product/langs/', 'product.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function DoAction()
	{
		$ar = null;
		
		$productID = $this->m_pr_main->GetID ();
		switch ($this->GetFormAction ())
		{
			case 'rename' : // rename Pr_main
				if (($newName = DB::POST ( 'newName' )))
				{
					$newName = trim ( $newName );
					if (strlen ( $newName ) > 0)
					{
						// rename
						$ar = $this->m_pr_main->SetName ( $newName, $this->m_displayLang );
						
						if ( $this->m_pr_main->GetSEOTitleOpt()==0 || $this->m_pr_main->GetSEOTitleAuto()==1 )
						{
							$this->m_pr_main->SetSEOKeywords ( $newName, $this->m_displayLang, 2 );
						}
					}
				}
				break;
			case 'delete' : // delete Pr_main
				$ar = $this->m_pr_main->Delete ();
				break;
			case 'updateFields' :
				$special = isset ( $_POST ['special'] );
				$this->m_pr_main->SetSpecial( $special);
				
				// get types
				$typeID = $this->m_pr_main->GetTypeID ();
				$productTypeFields = new Pr_typefieldList ( $typeID );
				$fields = $productTypeFields->GetStructure ();
				$field55V = "";
				$field56V = "";
				foreach ( $fields as $relField )
				{
					$fildOverName = "productfield" . $relField ['fieldID'];
					
					$value = DB::POST ( $fildOverName );
					
					$fieldID = $relField ['fieldID'];
					$field = new Pr_field ( $fieldID );
					
					$props = array ();
					
					switch ($field->GetType ())
					{
						case 1: // options
							$productOptions = new Pr_options ( null, $productID, $fieldID );
							if ( $relField ['single']==2 )
							{
								// delete all relation 
								$prlm = new Pr_ListMan ( );
								$prlm->DeleteOptions ( $productID, $fieldID );
								
								// add new
								foreach ( $value as $sValue )
								{
									$props ['productID'] = $productID;
									$props ['fieldID'] = $fieldID;
									$props ['relatedID'] = $sValue;
									
									$ar = $productOptions->Insert ( $props );
								}
							}
							else 
							{
								if ($productOptions->GetID ()) // in exist and single
								{
									$ar = $productOptions->SetRelatedID ( $value );
								}
								else // insert 
								{
									$props ['productID'] = $productID;
									$props ['fieldID'] = $fieldID;
									$props ['relatedID'] = $value;
									
									$ar = $productOptions->Insert ( $props );
								}
							}
							break;
						case 2: // structure
							$productOptions = new Pr_options ( null, $productID, $fieldID );
							if ( $relField['multilang']!=2 ) // single options
							{
								if ($productOptions->GetID ()) // in exist and single
								{
									$ar = $productOptions->SetRelatedID ( $value );
								}
								else // insert 
								{
									$props ['productID'] = $productID;
									$props ['fieldID'] = $fieldID;
									$props ['relatedID'] = $value;
									
									$ar = $productOptions->Insert ( $props );
								}
							}
							else //multiple options
							{
								// delete all relation 
								$prlm = new Pr_ListMan ( );
								$prlm->DeleteOptions ( $productID, $fieldID );
								
								// add new
								foreach ( $value as $sValue )
								{
									$props ['productID'] = $productID;
									$props ['fieldID'] = $fieldID;
									$props ['relatedID'] = $sValue;
									
									$ar = $productOptions->Insert ( $props );
								}
							}
							break;
						case 3: // int
							$productInt = new Pr_int ( null, $productID, $fieldID );
							
							if ($productInt->GetID ()) // in exist
							{
								$ar = $productInt->SetValue ( $value );
							}
							else // insert 
							{
								$props ['productID'] = $productID;
								$props ['fieldID'] = $fieldID;
								$props ['value'] = $value;
								
								$ar = $productInt->Insert ( $props );
							}
							break;
						case 4: // double
							$productDouble = new Pr_double ( null, $productID, $fieldID );
							if ( ($fieldID==55 || $fieldID==56) && ($value=="" || $value==0) )
							{
								if ( $field55V=="" )
								{
									$field55V = "app";
									$field56V = "app";
									
									$address = trim($this->m_pr_main->GetFieldValue(28));
									$zip = trim($this->m_pr_main->GetFieldValue(29));
									$city = trim($this->m_pr_main->GetFieldValue(18, 'ru', true, "key"));
									if ( $city)
									{
										$address = str_replace(" ", "+", $address);
										
										$ch = curl_init();
										$timeout = 5;
										$userAgent = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.1.4322)';
										
										if ( $address )
										{
											if ( $zip )
											{
												curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$address.',+'.$zip.',+'.$city.',+Italy&sensor=false');
											}
											else 
											{
												curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$address.',+'.$city.',+Italy&sensor=false');
											}
										}
										else 
										{
											curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$city.',+Italy&sensor=false');
										}
										curl_setopt($ch,CURLOPT_USERAGENT, $userAgent);
										curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
										curl_setopt($ch,CURLOPT_CONNECTTIMEOUT,$timeout);
										curl_setopt($ch,CURLOPT_FAILONERROR, true);
										curl_setopt($ch,CURLOPT_FOLLOWLOCATION, true);
										curl_setopt($ch,CURLOPT_AUTOREFERER, true);
										curl_setopt($ch,CURLOPT_TIMEOUT, 10);
										$data = curl_exec($ch);
										curl_close($ch);
										
										$data = json_decode($data);
										if ( $data->status == "OK" && $data->results[0]->geometry->location_type == "ROOFTOP" )
										{
											$field55V = $data->results[0]->geometry->location->lat;
											$field56V = $data->results[0]->geometry->location->lng;
										}
									}
								}
								
								if ( $field55V != "app" )
								{
									if ( $fieldID==55 )
									{
										$value = $field55V;
									}
									else 
									{
										$value = $field56V;
									}
								}
							}
							
							if ($productDouble->GetID ()) // in exist
							{
								$ar = $productDouble->SetValue ( $value );
							}
							else // insert 
							{
								$props ['productID'] = $productID;
								$props ['fieldID'] = $fieldID;
								$props ['value'] = $value;
								
								$ar = $productDouble->Insert ( $props );
							}
							break;
						case 5: // char 255
							$productChar = new Pr_char ( null, $productID, $fieldID );
							
							if ($productChar->GetID ()) // in exist
							{
								$ar = $productChar->SetValue ( $value );
							}
							else // insert 
							{
								$props ['productID'] = $productID;
								$props ['subID'] = 0;
								$props ['fieldID'] = $fieldID;
								$props ['value'] = $value;
								
								$ar = $productChar->Insert ( $props );
							}
							break;
						case 6: // char 255 multi lang
							$productCharM = new Pr_char ( null, $productID, $fieldID, $this->m_displayLang );
							
							if ($productCharM->GetID ()) // in exist
							{
								$ar = $productCharM->SetValue ( $value );
							}
							else // insert 
							{
								$props ['productID'] = $productID;
								$props ['fieldID'] = $fieldID;
								$props ['value'] = $value;
								$props ['subID'] = $this->m_displayLang;
								
								$ar = $productCharM->Insert ( $props );
							}
							break;
						case 7: // text multi lang
							$productText = new Pr_text ( null, $productID, $fieldID, $this->m_displayLang );
							
							if ($productText->GetID ()) // in exist
							{
								$ar = $productText->SetValue ( $value );
							}
							else // insert 
							{
								$props ['productID'] = $productID;
								$props ['fieldID'] = $fieldID;
								$props ['value'] = $value;
								$props ['subID'] = $this->m_displayLang;
								
								$ar = $productText->Insert ( $props );
							}
							break;
						case 8: // structure multi choice
							$productOptions = new Pr_options ( null, $productID, $fieldID );
							
							if ($relField ['multilang'] != 2) // single options
							{
								$prodOptionsIDs = $productOptions->GetIDs ();
								if (!empty ($prodOptionsIDs)) // in exist and single
								{
									$productOptions->DeleteMultiValues();
									if (!empty($value))
									{
										foreach ($value as $valueCatID)
										{
											if ($valueCatID != null)
											{
												$props ['productID'] = $productID;
												$props ['fieldID'] = $fieldID;
												$props ['relatedID'] = $valueCatID;
											
												$ar = $productOptions->Insert ( $props );
											}
										}
									}
								}
								else // insert 
								{
									if (!empty($value))
									{
										foreach ($value as $valueCatID)
										{
											if ($valueCatID != null)
											{
												$props ['productID'] = $productID;
												$props ['fieldID'] = $fieldID;
												$props ['relatedID'] = $valueCatID;
											
												$ar = $productOptions->Insert ( $props );
											}
										}
									}
								}
							}
							else //multiple options
							{
								// delete all relation 
								$prlm = new Pr_ListMan ( );
								$prlm->DeleteOptions ( $productID, $fieldID );
								
								// add new
								foreach ( $value as $sValue )
								{
									$props ['productID'] = $productID;
									$props ['fieldID'] = $fieldID;
									$props ['relatedID'] = $sValue;
									
									$ar = $productOptions->Insert ( $props );
								}
							}
							break;
						case 9:
							$groupField = $field->GetFields();
							$groupField = explode(",", $groupField);
							$gCount = DB::POST ( "pf".$fieldID );
							$fieldIDMain = $fieldID;
							
							$tField = new Pr_field ( $groupField[0] );
							$fArr = $this->m_pr_main->GetGFC($groupField[0], $tField->GetType());
							if ( is_array($fArr) )
							{
								for ( $i=0; $i<count($fArr); $i++ )
								{
									$group = $fArr[$i];
									if ( is_array($_POST["pf_delete_".$fieldIDMain]) && in_array($group, $_POST["pf_delete_".$fieldIDMain]) )
									{
										foreach ( $groupField as $fieldID )
										{
											$fildOverName = "productfield" . $fieldID . "_" . $group;
											$value = DB::POST ( $fildOverName );
											
											$field = new Pr_field ( $fieldID );
											
											$props = array ();
											switch ($field->GetType ())
											{
												case 1: // options
													$productOptions = new Pr_options ( null, $productID, $fieldID, $group );
													if ( $productTypeFields->GetFieldSingle($typeID, $fieldID)==2 )
													{
														// delete all relation 
														$prlm = new Pr_ListMan ( );
														$prlm->DeleteOptions ( $productID, $fieldID, null, $group );
													}
													else 
													{
														$productOptions->DeleteMultiValues($group);
													}
													break;
												case 3: // int
													$productInt = new Pr_int ( null, $productID, $fieldID, $group );
													$productInt->Delete();
													break;
												case 4: // double
													$productDouble = new Pr_double ( null, $productID, $fieldID, $group );
													$productDouble->Delete();
													break;
												case 5: // char 255
													$productChar = new Pr_char ( null, $productID, $fieldID, null, $group );
													$productChar->Delete();
													break;
												case 6: // char 255 multi lang
													$productCharM = new Pr_char ( null, $productID, $fieldID, $this->m_displayLang, $group );
													$productCharM->Delete();
													break;
												case 7: // text multi lang
													$productText = new Pr_text ( null, $productID, $fieldID, $this->m_displayLang, $group );
													$productText->Delete();
													break;
											}
										}
									}
									else 
									{
										foreach ( $groupField as $fieldID )
										{
											$fildOverName = "productfield" . $fieldID . "_" . $group;
											$value = DB::POST ( $fildOverName );
											
											$field = new Pr_field ( $fieldID );
											
											$props = array ();
											switch ($field->GetType ())
											{
												case 1: // options
													$productOptions = new Pr_options ( null, $productID, $fieldID, $group );
													if ( $productTypeFields->GetFieldSingle($typeID, $fieldID)==2 )
													{
														// delete all relation 
														$prlm = new Pr_ListMan ( );
														$prlm->DeleteOptions ( $productID, $fieldID, null, $group );
														
														// add new
														foreach ( $value as $sValue )
														{
															$props ['productID'] = $productID;
															$props ['fieldID'] = $fieldID;
															$props ['relatedID'] = $sValue;
															$props ['group'] = $group;
															
															$ar = $productOptions->Insert ( $props );
														}
													}
													else 
													{
														if ($productOptions->GetID ()) // in exist and single
														{
															$ar = $productOptions->SetRelatedID ( $value );
														}
														else // insert 
														{
															$props ['productID'] = $productID;
															$props ['fieldID'] = $fieldID;
															$props ['relatedID'] = $value;
															$props ['group'] = $group;
															
															$ar = $productOptions->Insert ( $props );
														}
													}
													break;
												case 3: // int
													$productInt = new Pr_int ( null, $productID, $fieldID, $group );
													if ($productInt->GetID ()) // in exist
													{
														$ar = $productInt->SetValue ( $value );
													}
													else // insert 
													{
														$props ['productID'] = $productID;
														$props ['fieldID'] = $fieldID;
														$props ['value'] = $value;
														$props ['group'] = $group;
														
														$ar = $productInt->Insert ( $props );
													}
													break;
												case 4: // double
													$productDouble = new Pr_double ( null, $productID, $fieldID, $group );
													if ( ($fieldID==55 || $fieldID==56) && ($value=="" || $value==0) )
													{
														if ( $field55V=="" )
														{
															$field55V = "app";
															$field56V = "app";
															
															$address = trim($this->m_pr_main->GetFieldValue(28));
															$zip = trim($this->m_pr_main->GetFieldValue(29));
															$city = trim($this->m_pr_main->GetFieldValue(18, 'ru', true, "key"));
															if ( $city)
															{
																$address = str_replace(" ", "+", $address);
																
																$ch = curl_init();
																$timeout = 5;
																$userAgent = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.1.4322)';
																
																if ( $address )
																{
																	if ( $zip )
																	{
																		curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$address.',+'.$zip.',+'.$city.',+Italy&sensor=false');
																	}
																	else 
																	{
																		curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$address.',+'.$city.',+Italy&sensor=false');
																	}
																}
																else 
																{
																	curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$city.',+Italy&sensor=false');
																}
																curl_setopt($ch,CURLOPT_USERAGENT, $userAgent);
																curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
																curl_setopt($ch,CURLOPT_CONNECTTIMEOUT,$timeout);
																curl_setopt($ch,CURLOPT_FAILONERROR, true);
																curl_setopt($ch,CURLOPT_FOLLOWLOCATION, true);
																curl_setopt($ch,CURLOPT_AUTOREFERER, true);
																curl_setopt($ch,CURLOPT_TIMEOUT, 10);
																$data = curl_exec($ch);
																curl_close($ch);
																
																$data = json_decode($data);
																if ( $data->status == "OK" && $data->results[0]->geometry->location_type == "ROOFTOP" )
																{
																	$field55V = $data->results[0]->geometry->location->lat;
																	$field56V = $data->results[0]->geometry->location->lng;
																}
															}
														}
														
														if ( $field55V != "app" )
														{
															if ( $fieldID==55 )
															{
																$value = $field55V;
															}
															else 
															{
																$value = $field56V;
															}
														}
													}
													
													if ($productDouble->GetID ()) // in exist
													{
														$ar = $productDouble->SetValue ( $value );
													}
													else // insert 
													{
														$props ['productID'] = $productID;
														$props ['fieldID'] = $fieldID;
														$props ['value'] = $value;
														$props ['group'] = $group;
														
														$ar = $productDouble->Insert ( $props );
													}
													break;
												case 5: // char 255
													$productChar = new Pr_char ( null, $productID, $fieldID, null, $group );
													if ($productChar->GetID ()) // in exist
													{
														$ar = $productChar->SetValue ( $value );
													}
													else // insert 
													{
														$props ['productID'] = $productID;
														$props ['subID'] = 0;
														$props ['fieldID'] = $fieldID;
														$props ['value'] = $value;
														$props ['group'] = $group;
														
														$ar = $productChar->Insert ( $props );
													}
													break;
												case 6: // char 255 multi lang
													$productCharM = new Pr_char ( null, $productID, $fieldID, $this->m_displayLang, $group );
													if ($productCharM->GetID ()) // in exist
													{
														$ar = $productCharM->SetValue ( $value );
													}
													else // insert 
													{
														$props ['productID'] = $productID;
														$props ['fieldID'] = $fieldID;
														$props ['value'] = $value;
														$props ['subID'] = $this->m_displayLang;
														$props ['group'] = $group;
														
														$ar = $productCharM->Insert ( $props );
													}
													break;
												case 7: // text multi lang
													$productText = new Pr_text ( null, $productID, $fieldID, $this->m_displayLang, $group );
													if ($productText->GetID ()) // in exist
													{
														$ar = $productText->SetValue ( $value );
													}
													else // insert 
													{
														$props ['productID'] = $productID;
														$props ['fieldID'] = $fieldID;
														$props ['value'] = $value;
														$props ['subID'] = $this->m_displayLang;
														$props ['group'] = $group;
														
														$ar = $productText->Insert ( $props );
													}
													break;
											}
										}
									}
								}
								$group++;
							}
							else 
							{
								$group = 1;
							}
							
							$addOther = false;
							foreach ( $groupField as $fieldID )
							{
								$fildOverName = "productfield" . $fieldID . "_" . $group;
								$value = DB::POST ( $fildOverName );
								if ( $value )
								{
									$addOther = true;
									break;
								}
							}
							
							if ( $addOther )
							{
								foreach ( $groupField as $fieldID )
								{
									$fildOverName = "productfield" . $fieldID . "_" . $group;
									$value = DB::POST ( $fildOverName );
									
									$field = new Pr_field ( $fieldID );
									
									$props = array ();
									switch ($field->GetType ())
									{
										case 1: // options
											$productOptions = new Pr_options ( null, $productID, $fieldID, $group );
											if ( $productTypeFields->GetFieldSingle($typeID, $fieldID)==2 )
											{
												// delete all relation 
												$prlm = new Pr_ListMan ( );
												$prlm->DeleteOptions ( $productID, $fieldID, null, $group );
												
												// add new
												foreach ( $value as $sValue )
												{
													$props ['productID'] = $productID;
													$props ['fieldID'] = $fieldID;
													$props ['relatedID'] = $sValue;
													$props ['group'] = $group;
													
													$ar = $productOptions->Insert ( $props );
												}
											}
											else 
											{
												if ($productOptions->GetID ()) // in exist and single
												{
													$ar = $productOptions->SetRelatedID ( $value );
												}
												else // insert 
												{
													$props ['productID'] = $productID;
													$props ['fieldID'] = $fieldID;
													$props ['relatedID'] = $value;
													$props ['group'] = $group;
													
													$ar = $productOptions->Insert ( $props );
												}
											}
											break;
										case 3: // int
											$productInt = new Pr_int ( null, $productID, $fieldID, $group );
											if ($productInt->GetID ()) // in exist
											{
												$ar = $productInt->SetValue ( $value );
											}
											else // insert 
											{
												$props ['productID'] = $productID;
												$props ['fieldID'] = $fieldID;
												$props ['value'] = $value;
												$props ['group'] = $group;
												
												$ar = $productInt->Insert ( $props );
											}
											break;
										case 4: // double
											$productDouble = new Pr_double ( null, $productID, $fieldID, $group );
											if ( ($fieldID==55 || $fieldID==56) && ($value=="" || $value==0) )
											{
												if ( $field55V=="" )
												{
													$field55V = "app";
													$field56V = "app";
													
													$address = trim($this->m_pr_main->GetFieldValue(28));
													$zip = trim($this->m_pr_main->GetFieldValue(29));
													$city = trim($this->m_pr_main->GetFieldValue(18, 'ru', true, "key"));
													if ( $city)
													{
														$address = str_replace(" ", "+", $address);
														
														$ch = curl_init();
														$timeout = 5;
														$userAgent = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.1.4322)';
														
														if ( $address )
														{
															if ( $zip )
															{
																curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$address.',+'.$zip.',+'.$city.',+Italy&sensor=false');
															}
															else 
															{
																curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$address.',+'.$city.',+Italy&sensor=false');
															}
														}
														else 
														{
															curl_setopt($ch,CURLOPT_URL,'http://maps.googleapis.com/maps/api/geocode/json?address='.$city.',+Italy&sensor=false');
														}
														curl_setopt($ch,CURLOPT_USERAGENT, $userAgent);
														curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
														curl_setopt($ch,CURLOPT_CONNECTTIMEOUT,$timeout);
														curl_setopt($ch,CURLOPT_FAILONERROR, true);
														curl_setopt($ch,CURLOPT_FOLLOWLOCATION, true);
														curl_setopt($ch,CURLOPT_AUTOREFERER, true);
														curl_setopt($ch,CURLOPT_TIMEOUT, 10);
														$data = curl_exec($ch);
														curl_close($ch);
														
														$data = json_decode($data);
														if ( $data->status == "OK" && $data->results[0]->geometry->location_type == "ROOFTOP" )
														{
															$field55V = $data->results[0]->geometry->location->lat;
															$field56V = $data->results[0]->geometry->location->lng;
														}
													}
												}
												
												if ( $field55V != "app" )
												{
													if ( $fieldID==55 )
													{
														$value = $field55V;
													}
													else 
													{
														$value = $field56V;
													}
												}
											}
											
											if ($productDouble->GetID ()) // in exist
											{
												$ar = $productDouble->SetValue ( $value );
											}
											else // insert 
											{
												$props ['productID'] = $productID;
												$props ['fieldID'] = $fieldID;
												$props ['value'] = $value;
												$props ['group'] = $group;
												
												$ar = $productDouble->Insert ( $props );
											}
											break;
										case 5: // char 255
											$productChar = new Pr_char ( null, $productID, $fieldID, null, $group );
											if ($productChar->GetID ()) // in exist
											{
												$ar = $productChar->SetValue ( $value );
											}
											else // insert 
											{
												$props ['productID'] = $productID;
												$props ['subID'] = 0;
												$props ['fieldID'] = $fieldID;
												$props ['value'] = $value;
												$props ['group'] = $group;
												
												$ar = $productChar->Insert ( $props );
											}
											break;
										case 6: // char 255 multi lang
											$productCharM = new Pr_char ( null, $productID, $fieldID, $this->m_displayLang, $group );
											if ($productCharM->GetID ()) // in exist
											{
												$ar = $productCharM->SetValue ( $value );
											}
											else // insert 
											{
												$props ['productID'] = $productID;
												$props ['fieldID'] = $fieldID;
												$props ['value'] = $value;
												$props ['subID'] = $this->m_displayLang;
												$props ['group'] = $group;
												
												$ar = $productCharM->Insert ( $props );
											}
											break;
										case 7: // text multi lang
											$productText = new Pr_text ( null, $productID, $fieldID, $this->m_displayLang, $group );
											if ($productText->GetID ()) // in exist
											{
												$ar = $productText->SetValue ( $value );
											}
											else // insert 
											{
												$props ['productID'] = $productID;
												$props ['fieldID'] = $fieldID;
												$props ['value'] = $value;
												$props ['subID'] = $this->m_displayLang;
												$props ['group'] = $group;
												
												$ar = $productText->Insert ( $props );
											}
											break;
									}
								}
							}
							
							break;
					}
				}
				break;
			case 'updateSubtitle':
				$subtitle = DB::POST ( 'subtitle' );
				$ar = $this->m_pr_main->SetSubtitle( $subtitle, $this->m_displayLang );
				break;
			case 'updateDescription':
				$newDescription = DB::POST ( 'description' );
				$newDesc = $newDescription;
				$ar = $this->m_pr_main->SetDesc ( $newDescription, $this->m_displayLang );
				
				if ( $this->m_pr_main->GetSEODescriptionOpt()==0 || $this->m_pr_main->GetSEODescriptionAuto()==1 )
				{
					$newDescription = strip_tags($newDescription);
					if ( strlen($newDescription)>250 && strpos($newDescription, ".", 250)!==false )
					{
						$newDescription = substr($newDescription, 0, strpos($newDescription, ".", 250));
					}
					
					$this->m_pr_main->SetSEOKeywords ( $newDescription, $this->m_displayLang, 3 );
				}
				
				if ( $this->m_pr_main->GetSEOKeywordsOpt()==0 || $this->m_pr_main->GetSEOKeywordsAuto()==1 )
				{
					$newDesc = strip_tags($newDesc);
					$newDesc = $this->extract_keywords($newDesc);
					
					$this->m_pr_main->SetSEOKeywords ( $newDesc, $this->m_displayLang, 1 );
				}
				break;
			case 'updateShortDescription':
				$newDescription = DB::POST ( 'shortDescription' );
				$ar = $this->m_pr_main->SetDescShort ( $newDescription, $this->m_displayLang );
				break;
			case 'uploadImage':
				$this->m_pr_main->UploadPicture ();
				break;
			case 'setdate':
				$date = DB::POST ( 'name' );
				$this->m_pr_main->SetDate ( $date );
				break;
			case 'seticon':
				$iconPath = DB::POST ( 'icon' );
				$this->m_pr_main->SetPicture ( $iconPath );
				break;
			case 'setcover':
				$coverPath = DB::POST ( 'cover' );
				$this->m_pr_main->SetCover( $coverPath );
				break;
			case 'deleteIcon':
				$this->m_pr_main->SetDefPicNull ();
				break;
			case 'deleteCover':
				$this->m_pr_main->SetCoverPicNull ();
				break;
			case 'setController':
				$controller = isset ( $_POST ['controller'] );
				$this->m_pr_main->SetProductController ( $controller );
				break;
			case 'viewCount':
				$viewCount = intval( $_POST ['viewCount'] );
				$this->m_pr_main->SetViewCount( $viewCount );
				break;
			case 'updateRelations':
				$relations = $_POST ['relations'];
				$this->m_pr_main->DeleteRelations();
				$this->m_pr_main->SetRelations($relations);
				break;
			case 'metaoptions':
				$seotitle = DB::POST ( 'seotitle' );
				$seodesc = DB::POST ( 'seodescription' );
				$this->m_pr_main->SetSEOKeywords ( $seotitle, $this->m_displayLang, 2 );
				$this->m_pr_main->SetSEOKeywords ( $seodesc, $this->m_displayLang, 3 );
				
				$seotitleopt = DB::POST ( 'seotitleopt' );
				$seodescopt = DB::POST ( 'seodescopt' );
				$this->m_pr_main->SetSEOKeywordsOpt ( $seotitleopt, 2 );
				$this->m_pr_main->SetSEOKeywordsOpt ( $seodescopt, 3 );
				
				$seotitleauto = (isset($_POST["seotitleauto"]) && $_POST["seotitleauto"]=="on" ? 1: 0);
				$seodescauto = (isset($_POST["seodescauto"]) && $_POST["seodescauto"]=="on" ? 1: 0);
				$this->m_pr_main->SetSEOKeywordsAuto ( $seotitleauto, 2 );
				$this->m_pr_main->SetSEOKeywordsAuto ( $seodescauto, 3 );
				break;
			case 'setStatus':
				$this->m_errorText = "";
				$status = 0;
				if ( isset($_POST['status']) )
				{
					$status = 1;
				}

				$this->m_pr_main->SetStatus($status);
				break;
			case 'updateComments':
				$comments = $this->m_pr_main->GetComments();
				if ( $comments )
				{
					$comments .= "<br /><br /><b>".CMS::GetUser()->GetUserName()."(".date("Y-m-d H:i:s")."): </b>".strip_tags(DB::POST ( 'comments' ));
				}
				else 
				{
					$comments = "<b>".CMS::GetUser()->GetUserName()."(".date("Y-m-d H:i:s")."): </b>".strip_tags(DB::POST ( 'comments' ));
				}
				$ar = $this->m_pr_main->SetComments ( $comments );
				break;
			case 'setAuthor':
				$author = intval($_POST['author']);
				$ar = $this->m_pr_main->SetUserID ( $author );
				break;
			case 'setPattern':
				$pattern = trim($_POST['pattern']);
				$ar = $this->m_pr_main->SetPattern ( $pattern );
				break;
			case 'setLanguage':
				$language = trim($_POST['language']);
				$ar = $this->m_pr_main->SetLanguage ( $language );

				$langs = $_POST["langs"];
				$ar &= $this->m_pr_main->SetLang ( intval($langs["en"]), "en" );
				$ar &= $this->m_pr_main->SetLang ( intval($langs["cn"]), "cn" );
				$ar &= $this->m_pr_main->SetLang ( intval($langs["ru"]), "ru" );
				break;
		}
		
		// if there is a any action then update Pr_main  
		if ($ar)
		{
			// update
			$this->Update ();
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
	
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		// read language strings
		$deleteS = $this->GetNameString ( 'delete' );
		$renameS = $this->GetNameString ( 'rename' );
		$doneS = $this->GetNameString ( 'done' );
		$controllerS = $this->GetNameString ( 'controller' );
		$noEmptyS = $this->GetNameString ( 'noempty' );
		$optionsS = $this->GetNameString ( 'options' );
		$dateS = $this->GetNameString ( 'date' );
		
		$ID = $this->m_pr_main->GetID ();
		$objName = $this->m_pr_main->GetName ( $this->m_displayLang );
		
		// add slashes
		if (is_string ( $objName ))
		{
			$objName = str_replace ( "'", "&rsquo;", $objName );
		}
		
		$overLink = 'pr_mainID';
		
		//	display error text if needed
		if ( $this->m_errorText!="" )
		{
			?>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
				<tr>
					<td>
						<div class="titleColL"><?php echo $this->m_errorText;?></div>
						<br class="clearFloat" />
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
			<br />
			<?php
		}
		
		//  costruct delete form
		$className = $this->GetFormActionName ();
		
		// delete form javascript
		$sureDeleteS = $this->GetNameString ( 'sureDelete' );
		$dJS = "onsubmit='return confirm(\"$sureDeleteS\");'";
		
		$delForm = "<div  class='titleColR'>
    					<form method='POST' $dJS>
    					<input type='hidden' name='$className' value='delete'>
    					<input type='hidden' name='pr_mainID' value='$ID'>
    					<input type='submit' value='$deleteS'>
    					</form>
    					</div>";
		
		// display form title
		$this->DisplayTitle ( $objName, $delForm );
		$panelIndex = 0;
		
		print ( "<div class='someGTitleBox'>$optionsS</div>" );
		print ( '<div class="someGBox">' );
		
		
		// rename sj
		
		// hiddens
		// display rename form
		$this->DisplayTabHeader ( ++ $panelIndex, $renameS );
		$renameJS = "onsubmit='return SingleForm(\"newName\", \"$noEmptyS\");'";
		$this->DisplayFormHeadr ( 'rename', 'renameForm', 'post', $renameJS );
		$this->DisplayHidden ( $overLink, $ID );
		print ( "<input type='text' value='$objName' name='newName' id='newName' size='64' maxlength='256' />\n" );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter ();
		
		// subtitle form
		$subtitle = $this->m_pr_main->GetSubtitle($this->m_displayLang);
		$this->DisplayTabHeader ( ++ $panelIndex, "Subtitle" );
		$this->DisplayFormHeadr ( 'updateSubtitle' );
		$this->DisplayHidden ( $overLink, $ID );
		print ( "<input type='text' value='$subtitle' name='subtitle' id='subtitle' size='64' maxlength='256' />\n" );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter ();

		// short description form
		$shortDescS = $this->GetNameString ( 'shortDescription' );
		$this->DisplayTabHeader ( ++ $panelIndex, $shortDescS );
		$descShort = $this->m_pr_main->GetDescShort ( $this->m_displayLang );
		$this->DisplayFormHeadr ( 'updateShortDescription' );
		$this->DisplayHidden ( $overLink, $ID );
		print ( "<textarea name='shortDescription' rows='10' cols='80' >$descShort</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ();
		$this->DisplayTabFooter ();
		
		// description form
		$descS = $this->GetNameString ( 'description' );
		$this->DisplayTabHeader ( ++ $panelIndex, $descS );
		$desc = $this->m_pr_main->GetDesc ( $this->m_displayLang );
		$this->DisplayFormHeadr ( 'updateDescription' );
		$this->DisplayHidden ( $overLink, $ID );
		print ( "<textarea name='description' rows='10' cols='80' >$desc</textarea>" );
		print ( '<br /><br />' );
		$this->DisplayFormFooter ();
		$this->DisplayTabFooter ();
		// end description form
		
		// controller
		$controller = $this->m_pr_main->GetProductController ();
		$this->DisplayTabHeader ( ++ $panelIndex, $controllerS );
		$renameJS = "onsubmit='return SingleForm(\"newName\", \"$noEmptyS\");'";
		$this->DisplayFormHeadr ( 'setController' );
		$this->DisplayHidden ( $overLink, $ID );
		print ($controller) ? '<input type="checkbox" name="controller" id="controller" value="1" checked="checked" />' : '<input type="checkbox" name="controller" id="controller" value="0" />';
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter ();
		
		// data
		$date = $this->m_pr_main->GetDate ();
		$this->DisplayTabHeader ( ++ $panelIndex, $dateS );
		$renameJS = "onsubmit='return SingleForm(\"newName\", \"$noEmptyS\");'";
		$this->DisplayFormHeadr ( 'setdate', 'dateForm', 'post', $renameJS );
		$this->DisplayHidden ( $overLink, $ID );
		print ( "<input type='text' value='$date' name='name' id='name' size='64' maxlength='256' />\n" );
		print ( "<input name='' type='button' id='dateBtn'>\n" );
		print ( '<script type="text/javascript">
								//<![CDATA[
								  var cal = Calendar.setup({
									  onSelect: function(cal) { cal.hide() }
								  });
								  cal.manageFields("dateBtn", "name", "%Y-%m-%d");
								//]]>
							</script>' );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter ();
		
		// diplay viewCount form
		$viewCount = $this->m_pr_main->GetViewCount ( $this->m_displayLang );
		$descS = $this->GetNameString ( 'viewCount' );
		$this->DisplayTabHeader ( ++ $panelIndex, "View Count" );
		$renameJS = "";
		$this->DisplayFormHeadr ( 'viewCount', 'viewCountForm', 'post', $renameJS );
		$this->DisplayHidden ( $overLink, $ID );
		print ( "<input type='text' value='$viewCount' name='viewCount' id='viewCount' size='64' maxlength='256' />\n" );
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter ();
		
		//	display meta options
		// page options   
		$pageoptionsS = $this->GetNameString ( 'pageoptions' );
		
		$pageTitleS = $this->GetNameString ( 'pageTitle' );
		$pageKeywordS = $this->GetNameString ( 'pageKeyword' );
		$pageDescS = $this->GetNameString ( 'pageDesc' );
		
		$otherS = $this->GetNameString ( 'other' );
		$defoptS = $this->GetNameString ( 'defopt' );
		
		// tab header
		$this->DisplayTabHeader ( ++ $panelIndex, "META options" );
		
		$this->DisplayFormHeadr ( 'metaoptions' );
		$this->DisplayHidden ( $overLink, $ID );
		
		print ("<strong>Title</strong>");
		print ('<table border="0" cellspacing="0" cellpadding="5">');
		print ('<tr><td>');
		$seotitleopt = $this->m_pr_main->GetSEOTitleOpt();
		$seotitleauto = $this->m_pr_main->GetSEOTitleAuto();
		$seotitle = $this->m_pr_main->GetSEOTitle($this->m_displayLang);
		if ( $seotitleauto )
		{
			print ('<input name="seotitleauto" type="checkbox" checked /><label>Auto</label>');
		}
		else 
		{
			print ('<input name="seotitleauto" type="checkbox" /><label>Auto</label>');
		}
		if ( $seotitleopt==0 )
		{
			print ('<input name="seotitleopt" type="radio" value="2" /><label>Custom</label>
                	<input name="seotitleopt" type="radio" value="1" checked /><label>Default + Custom</label>');
		}
		elseif ( $seotitleopt==1 )
		{
			print ('<input name="seotitleopt" type="radio" value="2" /><label>Custom</label>
                	<input name="seotitleopt" type="radio" value="1" checked /><label>Default + Custom</label>');
		}
		else
		{
			print ('<input name="seotitleopt" type="radio" value="2" checked /><label>Custom</label>
                	<input name="seotitleopt" type="radio" value="1" /><label>Default + Custom</label>');
		}
		print ('</td></tr>');
		print ('<tr><td>');
		print ("<input type='text' value=\"$seotitle\" name='seotitle' size='64' maxlength='255' />\n");
		print ('</td></tr>');
		print ('</table>');
		print ('<br />');
		print ("<strong>Description</strong>");
		print ('<table border="0" cellspacing="0" cellpadding="5">');
		print ('<tr><td>');
		$seodescopt = $this->m_pr_main->GetSEODescriptionOpt();
		$seodescauto = $this->m_pr_main->GetSEODescriptionAuto();
		$seodesc = $this->m_pr_main->GetSEODescription($this->m_displayLang);
		if ( $seodescauto )
		{
			print ('<input name="seodescauto" type="checkbox" checked /><label>Auto</label>');
		}
		else 
		{
			print ('<input name="seodescauto" type="checkbox" /><label>Auto</label>');
		}
		if ( $seodescopt==0 )
		{
			print ('<input name="seodescopt" type="radio" value="2" /><label>Custom</label>
                	<input name="seodescopt" type="radio" value="1" checked /><label>Default + Custom</label>');
		}
		elseif ( $seodescopt==1 )
		{
			print ('<input name="seodescopt" type="radio" value="2" /><label>Custom</label>
                	<input name="seodescopt" type="radio" value="1" checked /><label>Default + Custom</label>');
		}
		else
		{
			print ('<input name="seodescopt" type="radio" value="2" checked /><label>Custom</label>
                	<input name="seodescopt" type="radio" value="1" /><label>Default + Custom</label>');
		}
		print ('</td></tr>');
		print ('<tr><td>');
		print ("<input type='text' value=\"$seodesc\" name='seodescription' size='64' maxlength='255' />\n");
		print ('</td></tr>');
		print ('</table>');
		print ('<br />');
		$this->DisplayFormFooter ( $doneS );
		
		// tab footer
		$this->DisplayTabFooter ();
		
		// status
		$status = $this->m_pr_main->GetStatus();
		if ( CMS::GetUser()->GetUserTypeID()<12 || CMS::GetUser()->GetPublish() )
		{
			$this->DisplayTabHeader ( ++ $panelIndex, "Status" );
			
			// diplay rename form
			$this->DisplayFormHeadr ( 'setStatus' );
			$this->DisplayHidden ( $overLink, $ID );

			// display label
			if ( $status==1 )
			{
				print 'Publish <input type="checkbox" name="status" checked />';
			}
			else 
			{
				print 'Publish <input type="checkbox" name="status" />';
			}
			
			$this->DisplayFormFooter ( $doneS );
			$this->DisplayTabFooter ();
		}
		elseif ( $status )
		{
			$this->DisplayTabHeader ( ++ $panelIndex, "Status" );
			
			// diplay rename form
			$this->DisplayFormHeadr ( 'setStatus' );
			$this->DisplayHidden ( $overLink, $ID );
			
			// display label
			if ( $status==1 )
			{
				print 'Published';
			}
			else 
			{
				print 'Not published';
			}
			
			$this->DisplayTabFooter ();
		}
		
		/*if ( CMS::GetUser()->GetUserTypeID()<12 )
		{
			// author
			$authorID = $this->m_pr_main->GetUserID();

			$this->DisplayTabHeader ( ++ $panelIndex, "Author" );

			// diplay rename form
			$this->DisplayFormHeadr ( 'setAuthor' );
			$this->DisplayHidden ( $overLink, $ID );

			//	get all user types
			$utl = new UserTypeList(1);
			$utlArr = $utl->GetStructure();
			?>
			<select name="author" id="author">
				<?php
				$ref = $utlArr[0]["ref"];
				while ( is_array($ref) )
				{
					$refName = $ref[0]["name"];
					$ref_ul = new UserList($ref[0]["ID"]);
					$ref_ulArr = $ref_ul->GetStructure();
					?>
					<option value="" disabled><?php echo $refName;?></option>
					<?php
					for ( $i=0; $i<count($ref_ulArr); $i++ )
					{
						?>
						<option value="<?php echo $ref_ulArr[$i]["ID"];?>"<?php echo ($authorID==$ref_ulArr[$i]["ID"] ? " selected": "");?>>&nbsp;&nbsp;&nbsp;<?php echo $ref_ulArr[$i]["username"];?></option>
						<?php
					}

					$ref = $ref[0]["ref"];
				}
				?>
			</select>
			<?php
			$this->DisplayFormFooter ( $doneS );
			$this->DisplayTabFooter ();
		}*/
		
		// pattern
		$pattern = $this->m_pr_main->GetPattern();
		
		$this->DisplayTabHeader ( ++ $panelIndex, "Pattern" );
		
		// diplay rename form
		$this->DisplayFormHeadr ( 'setPattern' );
		$this->DisplayHidden ( $overLink, $ID );
		
		//	get all user types
		?>
		<select name="pattern" id="pattern">
			<option value="main"<?php echo ($pattern=="main" ? " selected": "");?>>Main</option>
			<option value="secondary"<?php echo ($pattern=="secondary" ? " selected": "");?>>Secondary</option>
		</select>
		<?php
		$this->DisplayFormFooter ( $doneS );
		$this->DisplayTabFooter ();

		// language
		if ( $this->m_pr_main->GetTypeID()==9 )
		{
			$language = $this->m_pr_main->GetLanguage();
			$prLang["en"] = $this->m_pr_main->GetLang("en");
			$prLang["cn"] = $this->m_pr_main->GetLang("cn");
			$prLang["ru"] = $this->m_pr_main->GetLang("ru");
			$this->DisplayTabHeader ( ++ $panelIndex, "Language" );

			// diplay rename form
			$this->DisplayFormHeadr ( 'setLanguage' );
			$this->DisplayHidden ( $overLink, $ID );

			//	get all user types
			?>
			Default: <select name="language" id="language">
				<option value="">---</option>
				<?php
				$langs = $this->m_site->GetLanguages();
				foreach ( $langs as $lang )
				{
					?>
					<option value="<?php echo $lang;?>"<?php echo ($lang==$language ? " selected": "");?>><?php echo $lang;?></option>
					<?php
				}
				?>
			</select>
			<br /><br />
			Activate<br />
			<?php
			foreach ( $langs as $lang )
			{
				?>
				<input type="checkbox" name="langs[<?php echo $lang;?>]" value="1"<?php echo ($prLang[$lang] ? " checked": "");?>><?php echo $lang;?><br />
				<?php
			}
			?>
			<br />
			<?php
			$this->DisplayFormFooter ( $doneS );
			$this->DisplayTabFooter ();
		}
		
		print ( '</div>' );
		// end description form
		// display javascript
		print ( '<script type="text/javascript"><!--' );
		
		for($index = 0; $index <= $panelIndex; $index ++)
			print ( "var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n" );
		
		print ( '//--></script>' );
		
		// display all other fields
		// get current product type
		$typeID = $this->m_pr_main->GetTypeID ();
		
		// get all fields according for this type
		$productTypeFields = new Pr_typefieldList ( $typeID );
		$fields = $productTypeFields->GetStructure ();
		if ($fields)
		{
			print ( '<div class="someGTitleBox">Options:</div>' );
			print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
				          <tr>
				            <td valign="top" align="left">
				                <div class="titleColL">' );
			$this->DisplayFormHeadr ( 'updateFields' );
			$this->DisplayHidden ( $overLink, $ID );
			
			print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0">' );
			$groupFields = "";
			$showMainMap = false;
			foreach ( $fields as $relField )
			{
				$fieldID = $relField ['fieldID'];
				$field = new Pr_field ( $fieldID );
				
				$value = $this->m_pr_main->GetFieldValue ( $fieldID, $this->m_displayLang );
				if ( $field->GetType()!=9 )
				{
					print ( '<tr>' );
					print ( "<td align='right' style='padding:2px;' width='150'><strong>{$relField['name']}</strong></td>" );
					print ( "<td style='padding:2px;'>" );
				}
				
				// add slashes
				if (is_string ( $value ))
				{
					$value = str_replace ( "'", "&rsquo;", $value );
				}
				
				switch ( $field->GetType() )
				{
					case 1 : // options
						// get selected option type
						$optionsTypeId = $field->GetSubID ();
						$opDataList = new OptionsdataList ( $optionsTypeId );
						// get selected item
						if ( $relField['single']==2 )
						{
							$struc = $opDataList->GetStructure ();
							
							$prOptList = new Pr_optionsList ( $ID, $fieldID );
							
							// get related options
							$relOpts = $prOptList->GetStructure ();
							
							$relOptsA = array ();
							
							if ($relOpts)
							{
								foreach ( $relOpts as $relId )
								{
									array_push ( $relOptsA, $relId ['relatedID'] );
								}
							}
							// display options with checked
							print ( '<div class="multiOptionsBox">' );
							foreach ( $struc as $item )
							{
								
								$chack = (in_array ( $item ['ID'], $relOptsA )) ? "checked='checked'" : '';
								print ( "<input name='productfield{$fieldID}[]' id='productfield{$fieldID}[]' type='checkbox' value='{$item['ID']}' $chack /> {$item['name']} <br />" );
							}
							print ( '</div>' );
						}
						else 
						{
							print ( "<select name='productfield$fieldID'>\n" );
							$opDataList->DisplaySelect ( ' ... ', $value );
							print ( "</select>\n" );
						}
						break;
					case 2 : // structure
						$catID = $field->GetSubID ();
						$menuView = null;
						if ($catID > 0)
						{
							$category = new Category ( $catID );
							$menuID = $category->GetMenuID ();
							$menu = new Menu ( $menuID, true, $this->m_displayLang, true );
							
							$structure = array ();
							$menu->GetSubMenu ( $catID, $structure, null, true );
							$menuView = new MenuView ( $structure );
						}
						elseif ($catID < 0)
						{
							$menu = new Menu ( - $catID, true, $this->m_displayLang, true );
							$structure = $menu->GetStructure ();
							$menuView = new MenuView ( $structure );
						}
						
						if ($menuView)
						{
							print ( "<select name='productfield$fieldID'>\n" );
							$menuView->DisplaySimpleSelect ( $structure, $value, 1, '---' );
							print ( "</select>\n" );
						}
						
						break;
					case 3 : // int
						print ( "<input type='text' name='productfield$fieldID' value='$value' size='10' />" );
						break;
					case 4 : // double
						if ( $fieldID==55 )
						{
							$showMainMap = true;
						}
						print ( "<input type='text' name='productfield$fieldID' id='productfield$fieldID' value='$value' size='15' />" );
						break;
					case 5 : // char 255
						print ( "<input type='text' name='productfield$fieldID' id='productfield$fieldID' value='$value' size='30' />" );
						if ( $fieldID==3 || $fieldID==4 )
						{
							print ( "<input name='' type='button' id='dateBtn$fieldID'>\n" );
							print ( '<script type="text/javascript">
													//<![CDATA[
													  var cal = Calendar.setup({
														  onSelect: function(cal) { cal.hide() }
													  });
													  cal.manageFields("dateBtn'.$fieldID.'", "productfield'.$fieldID.'", "%Y-%m-%d");
													//]]>
												</script>' );
						}
						break;
					case 6 : // char 255 multi lang
						print ( "<input type='text' name='productfield$fieldID' value='$value' size='30' />" );
						break;
					case 7 : // textarea
						if ( $field->GetEditor() )
						{
							print ( "<textarea name='productfield$fieldID' cols='50' rows='7'>$value</textarea>" );
						}
						else 
						{
							print ( "<textarea name='productfield$fieldID' cols='50' rows='7' class='mceNoEditor'>$value</textarea>" );
						}
						break;
					case 8 : // structure multi choice
						$catID = $field->GetSubID ();
						$menuView = null;
						if ($catID > 0)
						{
							$category = new Category ( $catID );
							$menuID = $category->GetMenuID ();
							$menu = new Menu ( $menuID, true, $this->m_displayLang, true );
							
							$structure = array ();
							$menu->GetSubMenu ( $catID, $structure, null, true );
							$menuView = new MenuView ( $structure );
						}
						elseif ($catID < 0)
						{
							$menu = new Menu ( - $catID, true, $this->m_displayLang, true );
							$structure = $menu->GetStructure ();
							$menuView = new MenuView ( $structure );
						}
						
						if ($menuView)
						{
							print ( '<select name="productfield'.$fieldID.'[]" multiple="multiple" size="5">'."\n" );
							$menuView->DisplayMultiSelect ( $structure, $value, 1, '---' );
							print ( "</select>\n" );
						}
						
						break;
					case 9:
						$groupFields[$fieldID] = $field->GetFields();
						break;
				}
				if ( $field->GetType()!=9 )
				{
					print ( '<td>' );
					print ( '</tr>' );
				}
			}
			
			if ( is_array($groupFields) )
			{
				foreach ( $groupFields as $fieldID=>$groupField )
				{
					$groupField = explode(",", $groupField);
					
					$tField = new Pr_field ( $groupField[0] );
					$fArr = $this->m_pr_main->GetGFC($groupField[0], $tField->GetType());
					
					$field = new Pr_field ( $fieldID );
					$fieldName = $field->GetName($this->m_displayLang);
					$fieldMainID = $fieldID;
					?>
					<input type="hidden" name="pf<?php echo $fieldID;?>" id="pf<?php echo $fieldID;?>" value="1">
					<?php
					if ( is_array($fArr) )
					{
						for ( $i=0; $i<count($fArr); $i++ )
						{
							$group = $fArr[$i];
							?>
							<tr>
								<td align="left" valign="top" colspan="2">
									<table cellpadding="0" cellspacing="0" width="100%" border="0">
										<?php
										print ( '<tr>' );
										if ( $i>0 )
										{
											print ( '<td align="right" style="padding:20px 2px 2px 2px;" width="150" nowrap>delete <input type="checkbox" name="pf_delete_'.$fieldMainID.'[]" value="'.$group.'"></td>' );
										}
										else 
										{
											print ( '<td align="right" style="padding:20px 2px 2px 2px;" width="150">&nbsp;</td>' );
										}
										print ( "<td align='left' style='padding:20px 2px 2px 2px;'><strong>".$fieldName."</strong></td>" );
										print ( '</tr>' );
										$showMap = false;
										foreach ( $groupField as $fieldID )
										{
											$field = new Pr_field ( $fieldID );
											
											$value = $this->m_pr_main->GetFieldValue ( $fieldID, $this->m_displayLang, false, null, $group );
											print ( '<tr>' );
											print ( "<td align='right' style='padding:2px;'><strong>".$field->GetName($this->m_displayLang)."</strong></td>" );
											print ( "<td style='padding:2px;'>" );
											
											// add slashes
											if (is_string ( $value ))
											{
												$value = str_replace ( "'", "&rsquo;", $value );
											}
											
											switch ( $field->GetType() )
											{
												case 1 : // options
													// get selected option type
													$optionsTypeId = $field->GetSubID ();
													$opDataList = new OptionsdataList ( $optionsTypeId );
													// get selected item
													if ( $productTypeFields->GetFieldSingle($typeID, $fieldID)==2 )
													{
														$struc = $opDataList->GetStructure ();
														
														$prOptList = new Pr_optionsList ( $ID, $fieldID );
														
														// get related options
														$relOpts = $prOptList->GetStructure ();
														
														$relOptsA = array ();
														
														if ($relOpts)
														{
															foreach ( $relOpts as $relId )
															{
																array_push ( $relOptsA, $relId ['relatedID'] );
															}
														}
														// display options with checked
														print ( '<div class="multiOptionsBox">' );
														foreach ( $struc as $item )
														{
															$chack = (in_array ( $item ['ID'], $relOptsA )) ? "checked='checked'" : '';
															print ( "<input name='productfield{$fieldID}_{$group}[]' id='productfield{$fieldID}_{$group}[]' type='checkbox' value='{$item['ID']}' $chack /> {$item['name']} <br />" );
														}
														print ( '</div>' );
													}
													else 
													{
														print ( "<select name='productfield{$fieldID}_{$group}'>\n" );
														$opDataList->DisplaySelect ( ' ... ', $value );
														print ( "</select>\n" );
													}
													break;
												case 3 : // int
													print ( "<input type='text' name='productfield{$fieldID}_{$group}' value='$value' size='10' />" );
													break;
												case 4 : // double
													if ( $fieldID==55 )
													{
														$showMap = true;
													}
													print ( "<input type='text' name='productfield{$fieldID}_{$group}' id='productfield{$fieldID}_{$group}' value='$value' size='15' />" );
													break;
												case 5 : // char 255
													print ( "<input type='text' name='productfield{$fieldID}_{$group}' id='productfield{$fieldID}_{$group}' value='$value' size='30' />" );
													if ( $fieldID==42 || $fieldID==43 )
													{
														print ( "<input name='' type='button' id='dateBtn{$fieldID}_1'>\n" );
														print ( '<script type="text/javascript">
																	//<![CDATA[
																	  var cal = Calendar.setup({
																		  onSelect: function(cal) { cal.hide() }
																	  });
																	  cal.manageFields("dateBtn'.$fieldID.'_'.$group.'", "productfield'.$fieldID.'", "%Y-%m-%d");
																	//]]>
																</script>' );
													}
													break;
												case 6 : // char 255 multi lang
													print ( "<input type='text' name='productfield{$fieldID}_{$group}' value='$value' size='30' />" );
													break;
												case 7 : // textarea
													if ( $field->GetEditor() )
													{
														print ( "<textarea name='productfield{$fieldID}_{$group}' cols='50' rows='7'>$value</textarea>" );
													}
													else 
													{
														print ( "<textarea name='productfield{$fieldID}_{$group}' cols='50' rows='7' class='mceNoEditor'>$value</textarea>" );
													}
													break;
											}
											
											print ( '<td></tr>' );
										}
										?>
									</table>
								</td>
								<?php
								if ( $showMap )
								{
									?>
									<td>
										<table cellpadding="0" cellspacing="0" width="25%" border="0">
											<tr>
												<td>
													<iframe src="geo.php?id=<?php echo $this->m_pr_main->GetID();?>&group=<?php echo $group;?>" width="400" height="350" frameborder="0"></iframe>
												</td>
											</tr>
										</table>
									</td>
									<?php
								}
								?>
							</tr>
							<?php
						}
						$group++;
					}
					else 
					{
						$group = 1;
					}
					
					print ( '<tr>' );
					print ( "<td align='center' style='padding:20px 2px 2px 2px;' colspan=\"2\"><strong>".$fieldName."</strong></td>" );
					print ( '</tr>' );
					
					foreach ( $groupField as $fieldID )
					{
						$field = new Pr_field ( $fieldID );
						
						$value = $this->m_pr_main->GetFieldValue ( $fieldID, $this->m_displayLang, false, null, $group );
						print ( '<tr>' );
						print ( "<td align='right' style='padding:2px;'><strong>".$field->GetName($this->m_displayLang)."</strong></td>" );
						print ( "<td style='padding:2px;'>" );
						
						// add slashes
						if (is_string ( $value ))
						{
							$value = str_replace ( "'", "&rsquo;", $value );
						}
						
						switch ( $field->GetType() )
						{
							case 1 : // options
								// get selected option type
								$optionsTypeId = $field->GetSubID ();
								$opDataList = new OptionsdataList ( $optionsTypeId );
								// get selected item
								if ( $productTypeFields->GetFieldSingle($typeID, $fieldID)==2 )
								{
									$struc = $opDataList->GetStructure ();
									
									$prOptList = new Pr_optionsList ( $ID, $fieldID );
									
									// get related options
									$relOpts = $prOptList->GetStructure ();
									
									$relOptsA = array ();
									
									if ($relOpts)
									{
										foreach ( $relOpts as $relId )
										{
											array_push ( $relOptsA, $relId ['relatedID'] );
										}
									}
									// display options with checked
									print ( '<div class="multiOptionsBox">' );
									foreach ( $struc as $item )
									{
										$chack = (in_array ( $item ['ID'], $relOptsA )) ? "checked='checked'" : '';
										print ( "<input name='productfield{$fieldID}_{$group}[]' id='productfield{$fieldID}_{$group}[]' type='checkbox' value='{$item['ID']}' $chack /> {$item['name']} <br />" );
									}
									print ( '</div>' );
								}
								else 
								{
									print ( "<select name='productfield{$fieldID}_{$group}'>\n" );
									$opDataList->DisplaySelect ( ' ... ', $value );
									print ( "</select>\n" );
								}
								break;
							case 3 : // int
								print ( "<input type='text' name='productfield{$fieldID}_{$group}' value='$value' size='10' />" );
								break;
							case 4 : // double
								print ( "<input type='text' name='productfield{$fieldID}_{$group}' id='productfield{$fieldID}_{$group}' value='$value' size='15' />" );
								break;
							case 5 : // char 255
								print ( "<input type='text' name='productfield{$fieldID}_{$group}' id='productfield{$fieldID}_{$group}' value='$value' size='30' />" );
								if ( $fieldID==42 || $fieldID==43 )
								{
									print ( "<input name='' type='button' id='dateBtn{$fieldID}_{$group}'>\n" );
									print ( '<script type="text/javascript">
												//<![CDATA[
												  var cal = Calendar.setup({
													  onSelect: function(cal) { cal.hide() }
												  });
												  cal.manageFields("dateBtn'.$fieldID.'_{$group}", "productfield'.$fieldID.'", "%Y-%m-%d");
												//]]>
											</script>' );
								}
								break;
							case 6 : // char 255 multi lang
								print ( "<input type='text' name='productfield{$fieldID}_{$group}' value='$value' size='30' />" );
								break;
							case 7 : // textarea
								if ( $field->GetEditor() )
								{
									print ( "<textarea name='productfield{$fieldID}_{$group}' cols='50' rows='7'>$value</textarea>" );
								}
								else 
								{
									print ( "<textarea name='productfield{$fieldID}_{$group}' cols='50' rows='7' class='mceNoEditor'>$value</textarea>" );
								}
								break;
						}
						
						print ( '<td></tr>' );
					}
				}
			}
			
			print ( '</table>' );
			print ( '<br /><br />' );
			$this->DisplayFormFooter ();
			if ( $showMainMap )
			{
				print ( '<br class="clearFloat" />
					</td>
					<td width="400" valign="top"><iframe src="geo.php?id='.$this->m_pr_main->GetID().'" width="400" height="350" frameborder="0"></iframe></td>
			          </tr>
			        </table>' );
			}
			else 
			{
				print ( '<br class="clearFloat" /></td></tr></table>' );
			}
		}
		
		// display relations form
		$tempPrType = new Pr_type($this->m_pr_main->GetTypeID());
		$prTypeRel = $tempPrType->GetPTRelations();
		if ( is_array($prTypeRel) && count($prTypeRel)>0 )
		{
			$relations = $this->m_pr_main->GetRelations();
			print ( "<div class='someGTitleBox'>Relations</div>" );
			print ( "<div class='iconBox'>" );
			$productList = new ProductList();
			$allPT = $productList->GetProductTypes("`ID`", "asc");
			
			$this->DisplayFormHeadr ( 'updateRelations' );
			$this->DisplayHidden ( $overLink, $ID );
			?>
			<select name="relations[]" multiple size="15">
				<?php
				for ( $i=0; $i<count($allPT); $i++ )
				{
					if ( in_array($allPT[$i]["ID"], $prTypeRel) )
					{
						?>
						<option value="" disabled><?php echo $allPT[$i]["name"];?></option>
						<?php
						$allP = $productList->GetAllProducts($allPT[$i]["ID"], 1000, "`typeID`", "asc", "all");
						for ( $j=0; $j<count($allP); $j++ )
						{
							if ( $allP[$j]["ID"]!=$this->m_pr_main->GetID())
							{
								?>
								<option value="<?php echo $allP[$j]["ID"];?>"<?php echo (in_array($allP[$j]["ID"], $relations) ? " selected": "");?>>&nbsp;&nbsp;&nbsp;<?php echo $allP[$j]["name"];?></option>
								<?php
							}
						}
					}
				}
				?>
			</select>
			<br />
			<br />
			<?php
			$this->DisplayFormFooter ( $doneS );
			print ( "</div>" );
		}
		
		// display Icon form
		$picture = $this->m_pr_main->GetPicture ();
		print ( "<div class='someGTitleBox'>Icon</div>" );
		print ( "<div class='iconBox'><img src='$picture?".time()."' />" );
		// dispkay remove icon form
		if ($picture != 'img/thumbnail.gif')
		{
			$this->DisplayFormHeadr ( 'deleteIcon' );
			$this->DisplayHidden ( $overLink, $ID );
			$this->DisplayFormFooter ( 'Remove icon' );
		}
		print ( '</div>' );
		
		// display Cover form
		$cover = $this->m_pr_main->GetCover();
		print ( "<div class='someGTitleBox'>Cover</div>" );
		if ( $cover=="img/cover.gif" )
		{
			print ( "<div class='iconBox'><img src='$cover' />" );
		}
		else 
		{
			print ( "<div class='iconBox'><img src='$cover?".time()."' width='1000' />" );
		}
		// dispkay remove icon form
		if ($cover != 'img/cover.gif')
		{
			$this->DisplayFormHeadr ( 'deleteCover' );
			$this->DisplayHidden ( $overLink, $ID );
			$this->DisplayFormFooter ( 'Remove cover' );
		}
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return ActionResult
	 */
	function Delete()
	{
		return $this->m_pr_main->Delete ();
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Update()
	{
		$ID = DB::REQUEST ( 'pr_mainID' );
		$this->m_pr_main = new Pr_main ( $ID );
	}
	
	/**
	 * Create tables for this module
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$res = parent::CreateDB ();
		
		// create other tables
		$prInt = new Pr_intCMS ( );
		$res &= $prInt->CreateDB ();
		
		$prDouble = new Pr_doubleCMS ( );
		$res &= $prDouble->CreateDB ();
		
		$prChar = new Pr_charCMS ( );
		$res &= $prChar->CreateDB ();
		
		$prText = new Pr_textCMS ( );
		$res &= $prText->CreateDB ();
		
		$prPicture = new Pr_pictureCMS ( );
		$res &= $prPicture->CreateDB ();
		
		$prOpt = new Pr_optionsCMS ( );
		$res &= $prOpt->CreateDB ();
		
		return $res;
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$langs = $this->m_site->GetLanguages ();
		
		$nameS = '';
		
		foreach ( $langs as $lang )
		{
			$nameS .= "`name_$lang` varchar(255) NOT NULL,
					  `desc_$lang` text,
					  `short_desc_$lang` text,";
		}
		
		$sql = "CREATE TABLE `pr_main` (
			  `ID` int(10) unsigned NOT NULL auto_increment,
			  `typeID` smallint(5) unsigned NOT NULL,
			  `random` varchar(16) default NULL,
			  $nameS
			  `picture` varchar(255) default 'img/thumbnail.gif',
			  `controller` tinyint(1) NOT NULL default '0',
  			  `date` date NOT NULL default '2010-01-01',
			  PRIMARY KEY  (`ID`)
			)DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$res = parent::SelfTest();
		
		// create other tables
		$prInt = new Pr_intCMS ( );
		$res &= $prInt->SelfTest ();
		
		$prDouble = new Pr_doubleCMS ( );
		$res &= $prDouble->SelfTest ();
		
		$prChar = new Pr_charCMS ( );
		$res &= $prChar->SelfTest ();
		
		$prText = new Pr_textCMS ( );
		$res &= $prText->SelfTest ();
		
		$prPicture = new Pr_pictureCMS ( );
		$res &= $prPicture->SelfTest ();
		
		$prOpt = new Pr_optionsCMS ( );
		$res &= $prOpt->SelfTest ();
		
		return $res;
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		$res = array ('pr_main' );
		
		// create other tables
		$prInt = new Pr_intCMS ( );
		$prIntA = $prInt->GetObjectTables ();
		
		$prDouble = new Pr_doubleCMS ( );
		$prDoubleA = $prDouble->GetObjectTables ();
		
		$prChar = new Pr_charCMS ( );
		$prCharA = $prChar->GetObjectTables ();
		
		$prText = new Pr_textCMS ( );
		$prTextA = $prText->GetObjectTables ();
		
		$prPicture = new Pr_pictureCMS ( );
		$prPictureA = $prPicture->GetObjectTables ();
		
		$prOpt = new Pr_optionsCMS ( );
		$prOptA = $prOpt->GetObjectTables ();
		
		return array_merge ( $res, $prIntA, $prDoubleA, $prCharA, $prTextA, $prPictureA, $prOptA );
	}
	
	function extract_keywords($str, $minWordLen = 6, $minWordOccurrences = 1, $asArray = false, $maxWords = 8)
	{
		$str = strip_tags($str);
		$str = preg_replace('/\s\s+/i', '', $str); // replace whitespace
		$str = explode(" ", $str);
		for ( $i=0; $i<count($str); $i++ )
		{
			$tempStr = strtolower($str[$i]);
			$tempStr = str_replace(array("?", "!", ";", "(", ")", ":", "[", "]", ",", "."), " ", $tempStr);
			if ( strlen($tempStr)>= $minWordLen )
			{
				$strArr[] = $tempStr;
			}
		}
		
		$strArr = array_count_values($strArr);
		arsort($strArr);
		
		$keysArr = array();
		if ( count($strArr)>$maxWords )
		{
			$count = 0;
			foreach ( $strArr as $key=>$value )
			{
				if ( $count<$maxWords )
				{
					$keysArr[] = $key;
				}
				else 
				{
					break;
				}
				$count++;
			}
		}
		else 
		{
			$keysArr = $strArr;
		}
		
		return $asArray ? $keysArr : implode(', ', $keysArr);
	}
}
?>