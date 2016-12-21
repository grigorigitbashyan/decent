<?php

/**
 * Enter description here...
 *
 * @package Main
 */
class CmsXmlParser
{
	
	/**
	 * Enter description here...
	 *
	 * @var srray
	 */
	protected $m_structure;
	
	protected $m_langs;
	
	protected $m_fileName;
	
	function __construct()
	{
		$this->m_fileName = 'config/cms.xml';
		$this->m_langs = array ();
		
		//		$this->ParseXML();
		$this->ParseXML_DOM ();
	}
	/**
	 * Enter description here...
	 *
	 * @return array;
	 */
	function ParseXMLSimple()
	{
		if (file_exists ( $this->m_fileName ))
		{
			$xml = simplexml_load_file ( $this->m_fileName );
			
			$this->m_langs = ( array ) $xml->languages;
			
			foreach ( $xml->modules->module as $m_module )
			{
				$m_name = ( array ) $m_module->name;
				$m_cmsName = trim(( string ) $m_module->cmsName);
				$m_linkName = trim(( string ) $m_module->linkName);
				$m_menuID = trim(( string ) $m_module->menuID);
				$m_filePath = trim(( string ) $m_module->filePath);
				
				$m_filePath = trim ( $m_filePath );
				
				if (! file_exists ( "classes/" . $m_filePath ))
				{
					echo "classes/$m_filePath doesn't exist, please check the name\n";
				}
				
				$m_javascripts = $m_module->javascripts;
				$m_javascriptsArr = array ();
				
				foreach ( $m_javascripts->jsfilePath as $m_jsFilePath )
				{
					$m_curJSFilePath = ( string ) $m_jsFilePath;
					if ($m_curJSFilePath == "null")
					{
						array_push ( $m_javascriptsArr, Null );
					}
					else
					{
						if (file_exists ( $m_curJSFilePath )) //checking of null must be added 
						{
							array_push ( $m_javascriptsArr, trim($m_curJSFilePath) );
						}
						else
						{
							echo $m_curJSFilePath . " file was not added to your file Paths, as it doesn't exist.Please First create it!!!";
						}
					}
				}
				$this->m_structure [strtolower ( ( string ) $m_module->linkName )] = array ('name' => $m_name, 'cmsname' => $m_cmsName, 'linkname' => $m_linkName, 'filePath' => $m_filePath, 'menuID' => $m_menuID, 'javascripts' => $m_javascriptsArr );
			
			}
		}
		else
		{
			exit ( 'Failed to open cms_config.xml.' );
		}
	
	}
	
	function ParseXML_DOM()
	{
		// create DOM document
		$dom = new DOMDocument ( );
		
		$dom->load ( $this->m_fileName, LIBXML_NOBLANKS | LIBXML_DTDLOAD | LIBXML_DTDATTR );
		
		$root = $dom->documentElement;
		
		if ($root->hasChildNodes ())
		{
			$children = $root->childNodes;
			
			foreach ( $children as $child )
			{
				
				switch ($child->nodeName)
				{
					case 'languages' :
						// get languages
						$langNodes = $child->childNodes;
						
						// loop with languages
						foreach ( $langNodes as $currLangNode )
						{
							// get attributes
							$attributes = $currLangNode->attributes;
							
							if ($currLangNode->nodeName == 'language')
							{
								$attr = $attributes->getNamedItem ( "id" );
								
								if ($attr)
								{
									$arValue = ( string ) $attr->nodeValue;
									$this->m_langs [$arValue] = ( string ) $currLangNode->nodeValue;
								}
							}
						}
						break;
					case 'modules' :
						// get languages
						$langNodes = $child->childNodes;
						
						// loop with languages
						foreach ( $langNodes as $currModuleNode )
						{
							// chack is module node
							if ($currModuleNode->nodeName == 'module')
							{
								// get attributes
								$attributes = $currModuleNode->attributes;
								
								// get menuId and linkID
								$menuID = $attributes->getNamedItem ( "menuID" );
								$linkID = $attributes->getNamedItem ( "linkID" );
								
								$classFilePath = null;
								$className = null;
								$moduleNames = array ();
								$jsFiles = array ();
								$helpPath = null;
								
								if ($menuID && $linkID && $currModuleNode->hasChildNodes ())
								{
									$menuID = ( int ) $menuID->nodeValue;
									$linkID = ( string ) $linkID->nodeValue;
									// get names , class name and file path
									$properties = $currModuleNode->childNodes;
									
									foreach ( $properties as $property )
									{
										switch ($property->nodeName)
										{
											case 'names' :
												{
													$moduleNameNodes = $property->childNodes;
													
													foreach ( $moduleNameNodes as $moduleNameNode )
													{
														if ($moduleNameNode->nodeName == 'name' && $moduleNameNode->hasAttributes ())
														{
															// get attribute
															$moduleAttributes = $moduleNameNode->attributes;
															
															$nameLang = $moduleAttributes->getNamedItem ( "langName" );
															
															if ($nameLang)
															{
																$langName = ( string ) $nameLang->nodeValue;
																$moduleNames [$langName] = ( string ) $moduleNameNode->nodeValue;
															}
														}
													}
													break;
												}
											case 'className' :
												{
													$className = ( string ) $property->nodeValue;
													break;
												}
											case 'filePath' :
												{
													$classFilePath = ( string ) $property->nodeValue;
													break;
												}
											case 'javascripts' :
												{
													$jsNodes = $property->childNodes;
													
													foreach ( $jsNodes as $jsNode )
													{
														if ($jsNode->nodeName == 'jsfilePath')
														{
															$filePath = ( string ) $jsNode->nodeValue;
															array_push ( $jsFiles, $filePath );
														}
													}
													break;
												}
											case 'helpPath':
												{
													$helpPath = ( string ) $property->nodeValue;
													break;
												}
											default :
												{
													continue;
												}
										}
									}
								}
								
								// insert module
								if ($menuID && $linkID && $classFilePath)
								{
									$this->m_structure [$linkID] = array ('name' => $moduleNames, 
																		  'cmsname' => $className, 
																		  'linkname' => $linkID, 
																		  'filePath' => $classFilePath, 
																		  'menuID' => $menuID, 
																		  'javascripts' => $jsFiles, 
																		  'helpPath' => $helpPath );
								}
							}
						
						}
				}
			}
		}
	}
	
	function GetStructure()
	{
		return $this->m_structure;
	}
	
	function GetLanguages()
	{
		return $this->m_langs;
	}

}
?>