<?php
/**
 * This class provide all functionality for dsplaying trees and lists
 *
 * @package Main
 */
class TreeView extends View
{
	/**
	 * array of table names, which contains query
	 *
	 * @var array
	 */
	protected $m_tableNames = null;
	/**
	 * structure
	 *
	 * @protected array 
	 */
	protected $m_struct;
	
	/**
	 * current ID
	 *
	 * @private int
	 */
	private $m_currID;
	
	/**
	 * current ID
	 *
	 * @private int
	 */
	private $m_currSubID;
	
	/**
	 * this is string withis is overloading for linking
	 *
	 * @protected string
	 */
	protected $m_linkIDName;
	
	/**
	 * this is display string
	 *
	 * @private string
	 */
	private $m_linkName;
	
	/**
	 * this property continas tree perimeters
	 *
	 * @private array
	 */
	protected $m_treeParams;
	
	/**
	 * Enter description here...
	 *
	 * @var query string
	 */
	protected $m_queryString;
	
	protected $m_additionPath;
	
	protected $m_additionPathSelected;
	/**
	 * if $structure is string then do sql query and fill structure, if $structure is array then set it as structure
	 *
	 * @param array|string $structure
	 * @param string $linkIDName
	 * @param string $linkName
	 * @param int $currID
	 * @param array $tableNames table names which contains sql query 
	 */
	function __construct($structure = null, $linkIDName = null, $linkName = 'name', $currID = null, $tableNames = null)
	{
		parent::__construct ();
		
		$this->m_tableNames = $tableNames;
		$this->m_additionPath = null;
		$this->m_additionPathSelected = null;
		
		// if $structure is string then do query
		if ($structure)
		{
			if (is_string ( $structure ))
			{
				// bad way, it can be tree, not list
				$this->SetQueryString ( $structure );
			}
			else
			{
				// copy structure 
				$this->m_struct = $structure;
			}
		}
		
		if ($linkIDName == null)
		{
			$className = get_class ( $this );
			$linkIDName = $className . 'ID';
		}
		
		// detect current ID
		if ($currID === null)
		{
			$currID = DB::GET_POST ( $linkIDName );
		}
		
		// detect qorrent sub ID
		$this->m_currSubID = DB::GET_POST ( "sub$linkIDName" );
		
		$this->m_currID = $currID;
		$this->m_linkIDName = $linkIDName;
		$this->m_linkName = $linkName;
		$this->m_treeParams = null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	public function GetLinkIDName()
	{
		return $this->m_linkIDName;
	}
	
	/**
	 * Draw tree or list to select input
	 *
	 * @param array $struct
	 * @param int $currDepth
	 */
	protected function DisplayAddvancSelect(&$struct, $selectName = null, $currID = null)
	{
		print $this->SourceAddvancSelect ( $struct, $selectName, $currID );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $struct
	 * @param string $selectName null ID name
	 * @param int $currID current ID to be selected
	 * @param int $currDepth this for calculations and default is 1
	 * @return string
	 */
	function SourceAddvancSelect(&$struct, $selectName = null, $currID = null, $currDepth = 1)
	{
		$res = '';
		// display empty option
		if ($selectName)
		{
			if ($currID)
				$res .= "<option value=''>$selectName</option>\n";
			else
				$res .= "<option value='' selected>$selectName</option>\n";
		
		}
		
		if ($struct)
		{
			foreach ( $struct as $category )
			{
				// draw category
				// space
				$bspace = "";
				
				if ($currDepth > 1)
					$bspace = str_pad ( $bspace, 3 * 6 * ($currDepth - 1), "&nbsp;" ); // 3 is count of space
				

				// display name
				$drawName = $bspace . $category [$this->m_linkName];
				
				if ($category ['ID'] == $currID || (is_array ( $currID ) && in_array ( $category ['ID'], $currID )))
				{
					$res .= "<option value='{$category['ID']}' selected>$drawName</option>\n";
				}
				else
				{
					$res .= "<option value='{$category['ID']}'>$drawName</option>\n";
				}
				
				// draw sub categoryes
				if (isset ( $category ['ref'] ) && $category ['ref'] != null)
				{
					$res .= $this->SourceAddvancSelect ( $category ['ref'], $selectName, $currID, $currDepth + 1 );
				}
			}
		}
		
		return $res;
	}
	/**
	 * Enter description here...
	 *
	 * @param array $categoryis
	 * @param array $styles
	 * @param array $selectedButLink
	 * @param int $maxDepth
	 * @param int $currDepth
	 */
	protected function DisplayAddvancTree(&$categoryis, $styles = null, $selectLinkStyle = null, $selectedButLink = null, $maxDepth = 3, $currDepth = 1)
	{
		if (($categoryis == null) || ($currDepth > $maxDepth))
			return;
			
		// get syles
		$st = (isset ( $styles [$currDepth] )) ? $styles [$currDepth] : $styles [count ( $styles )];
		$sst = (isset ( $selectLinkStyle [$currDepth] )) ? $selectLinkStyle [$currDepth] : $selectLinkStyle [count ( $styles )];
		
		foreach ( $categoryis as $category )
		{
			// draw category
			print ( "\t<div class='$st'>" );
			
			$catName = $category [$this->m_linkName];
			
			// remove slashes
			$catName = $catName;
			
			if ($this->m_currID == $category ['ID'])
			{
				if ($selectedButLink)
				{
					$sbst = (isset ( $selectedButLink [$currDepth] )) ? $selectedButLink [$currDepth] : $selectedButLink [count ( $styles )];
					$categoryPath = "?{$this->m_linkIDName}={$category['ID']}{$this->m_additionPath}{$this->m_additionPathSelected}";
					print ( "<a href='{$categoryPath}' class='$sbst'>$catName</a>\n" );
				}
				else
				{
					print "$catName\n";
				}
			}
			else
			{
				$categoryPath = "?{$this->m_linkIDName}={$category['ID']}{$this->m_additionPath}";
				
				if ($this->m_currSubID == $category ['ID'])
				{
					$categoryPath .= $this->m_additionPathSelected;
					print ( "<a href='{$categoryPath}' class='$sst'>$catName</a>\n" );
				}
				else
				{
					print ( "<a href='{$categoryPath}'>$catName</a>\n" );
				}
			}
			
			print ( "</div>\n" );
			
			if (isset ( $category ['ref'] ) && $category ['ref'] != null)
			{
				$this->DisplayAddvancTree ( $category ['ref'], $styles, $selectLinkStyle, $selectLinkStyle, $maxDepth, $currDepth + 1 );
			}
		}
		
		return;
	}
	
	/**
	 * This function display list by styles.
	 * First stile is style for div, which contain the list item.
	 * Second styles is style for selected items, but needed to be link (link style)
	 * In case if list item has been selected , but need to display as link, then use $selectedButLink 
	 * not null value.
	 * 
	 * @param string $style
	 * @param string $selectedStyle
	 * @param string $selectedButLink
	 * 
	 */
	public function DisplayList($style = null, $selectedStyle = null, $selectedButLink = null)
	{
		$selectedButLinkA = null;
		
		if ($selectedButLink)
		{
			$selectedButLinkA = array ();
			$selectedButLinkA [1] = $selectedButLink;
		}
		
		$this->DisplayAddvancTree ( $this->m_struct, array ('1' => $style ), array ('1' => $selectedStyle ), $selectedButLinkA, 1 );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $styles
	 * @param array $selLink
	 * @param int $maxDepth
	 */
	public function DisplayTree($styles = null, $selLink = 'selLink', $maxDepth = 3)
	{
		$this->DisplayAddvancTree ( $this->m_struct, $styles, $selLink, null, $maxDepth );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	public function DisplaySelect($selectName = null, $currID = null)
	{
		if ($currID == null)
		{
			$currID = $this->m_currID;
		}
		
		$this->DisplayAddvancSelect ( $this->m_struct, $selectName, $currID );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $selectName
	 * @param int $currID
	 * @return string
	 */
	function SourceSelect($selectName, $currID = null)
	{
		if ($currID == null)
		{
			$currID = $this->m_currID;
		}
		
		return $this->SourceAddvancSelect ( $this->m_struct, $selectName, $currID );
	}
	
	/**
	 * Display list. use @@ vor replase string in patern
	 *
	 * @param string $pattern
	 * @param string|array $styles
	 * @param string $clear add this value to the end of each item, usually '<br clear="all" />'
	 */
	public function DisplayListDiv($pattern, $styles = null, $clear = null)
	{
		print $this->GetPatternDesign ( $pattern, $styles, 'div', $clear );
	}
	
	/**
	 * Return patterned string. Replase values to pattern and group (if groupNumber is great then 2).
	 *
	 * @param string $pattern
	 * @param string|array $styles
	 * @param string $tag
	 * @param string $clear
	 * @param int $groupNumber number of items that must be grouped together
	 * @param string|array $groupStlye group style(s) like item $style
	 * @param string $groupTag tag for group like item $tab
	 * @param string $groupClear clear control for group like item clear
	 * @return string
	 */
	public function GetPatternDesign($pattern, $styles = null, $tag = 'div', $clear = null, $groupNumber = 0, $groupStlye = null, $groupTag = 'div', $groupAppend = null)
	{
		$str = '';
		// get struct fields
		if (! isset ( $this->m_struct [0] ))
			return $str;
		
		$allKeys = array_keys ( $this->m_struct [0] );
		
		// detect keys in patern
		$keys = array ();
		
		foreach ( $allKeys as $tryKey )
		{
			if (strpos ( $pattern, $tryKey ) !== null)
			{
				array_push ( $keys, $tryKey );
			}
		}
		// detect item styles
		$styleCount = 0;
		
		if (is_string ( $styles ))
		{
			$styleCount = 1;
		}
		elseif (is_array ( $styles ))
		{
			$styleCount = count ( $styles );
		}
		
		// detect group styles
		$styleGrCount = 0;
		
		if (is_string ( $groupStlye ))
		{
			$styleGrCount = 1;
		}
		elseif (is_array ( $groupStlye ))
		{
			$styleGrCount = count ( $groupStlye );
		}
		
		$currentStyleIndex = 0;
		$currentGrStyleIndex = 0;
		$index = 0;
		$itemCount = count ( $this->m_struct );
		
		// display dives
		foreach ( $this->m_struct as $structItem )
		{
			// group start part ///////////////////////////
			if ($groupNumber > 2 && $index % $groupNumber == 0)
			{
				$classGroup = null;
				// get right style for item
				if ($styleGrCount == 1)
				{
					$classGroup = " class='$groupStlye'";
				}
				elseif ($styleGrCount > 1)
				{
					$classGroup = " class='{$groupStlye[$currentGrStyleIndex++]}'";
					
					if ($currentGrStyleIndex >= $styleGrCount)
					{
						$currentGrStyleIndex = 0;
					}
				}
				
				if ($classGroup !== null)
				{
					$str .= "<{$groupTag}$classGroup>\n";
				}
			}
			// end group //////////////////////////////////
			$patternCopy = $pattern;
			// replase
			foreach ( $keys as $ckey )
			{
				if ( $ckey=="iconPath" && ($structItem[$ckey]=="" || $structItem[$ckey]==null) )
				{
					$patternCopy = str_replace ( "@@$ckey@@", "{$structItem["path"]}", $patternCopy );
				}
				elseif ( $ckey=="single" )
				{
					$replaceWith = "";
					if ( $structItem["fieldType"]==1 )
					{
						if ( $structItem["single"]==1 )
						{
							$replaceWith = " (single)";
						}
						else 
						{
							$replaceWith = " (multiple)";
						}
					}
					$patternCopy = str_replace ( "@@$ckey@@", "$replaceWith", $patternCopy );
				}
				else 
				{
					$patternCopy = str_replace ( "@@$ckey@@", "{$structItem[$ckey]}", $patternCopy );
				}
			}
			
			if ( strpos($patternCopy, "videoCodeImg") )
			{
				if ( $structItem["videoCode"]!="" && getimagesize("http://i1.ytimg.com/vi/".$structItem["videoCode"]."/default.jpg") )
				{
					$patternCopy = str_replace ( "@@videoCodeImg@@", "http://i1.ytimg.com/vi/".$structItem["videoCode"]."/default.jpg", $patternCopy );
				}
				else 
				{
					$patternCopy = str_replace ( "@@videoCodeImg@@", "img/thumbnail.gif", $patternCopy );
				}
			}
			
			$class = null;
			// get right style for item
			if ($styleCount == 1)
			{
				$class = " class='$styles'";
			}
			elseif ($styleCount > 1)
			{
				$class = " class='{$styles[$currentStyleIndex++]}'";
				
				if ($currentStyleIndex >= $styleCount)
				{
					$currentStyleIndex = 0;
				}
			}
			
			if ($class !== null)
			{
				$str .= "<{$tag}$class>$patternCopy</{$tag}>\n";
			
			}
			else
			{
				$str .= "$patternCopy\n";
			}
			
			if ($clear)
			{
				$str .= $clear;
			}
			
			$index ++;
			
			if ($groupNumber > 2 && ($index % $groupNumber == 0 || $index == $itemCount))
			{
				$str .= "</$groupTag>\n";
				
				if ($groupAppend)
				{
					$str .= $groupAppend;
				}
			}
		}
		
		return $str;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	public function GetCurrentID()
	{
		if ($this->m_currID !== null)
			return $this->m_currID;
		
		if ($this->m_currSubID !== null)
			return $this->m_currSubID;
		
		return null;
	}
	
	/**
	 * Return count of items in first level of structure
	 *
	 * @return int
	 */
	public function GetItemCount()
	{
		return count ( $this->m_struct );
	}
	
	/**
	 * return structure
	 *
	 * @return array
	 */
	public function GetStructure()
	{
		return $this->m_struct;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $structure
	 */
	function SetStructure($structure)
	{
		$this->m_struct = $structure;
	}
	
	/**
	 * Update query string
	 *
	 * @param string $query
	 */
	function SetQueryString($query, $tableNames = null)
	{
		$this->m_queryString = $query;
		$this->m_tableNames = $tableNames;
		$this->Update ();
	}
	
	/**
	 * Update tree query perimeters
	 *
	 * @param string $tableName table name
	 * @param string $relCollName related column name (parentID)
	 * @param string $conditions SQL string in WHERE section
	 * @param string $cols colume names or null for all columes (*)
	 * @param int $rootElem parent element ID
	 * @param int $currentDepth
	 * @param int $maxdDepth
	 */
	function SetQueryStringTree($tableName, $relCollName, $conditions = null, $cols = null, $rootElem = 1, $withRoot = false, $currentDepth = 1, $maxdDepth = null)
	{
		$this->m_treeParams = array ();
		$this->m_treeParams ['tableName'] = $tableName;
		$this->m_treeParams ['relCollName'] = $relCollName;
		$this->m_treeParams ['conditions'] = $conditions;
		$this->m_treeParams ['cols'] = $cols;
		$this->m_treeParams ['rootElem'] = $rootElem;
		$this->m_treeParams ['withRoot'] = $withRoot;
		$this->m_treeParams ['currentDepth'] = $currentDepth;
		$this->m_treeParams ['maxdDepth'] = $maxdDepth;
		$this->Update ();
	}
	
	/**
	 * This function search Element by ID in tree.
	 * Return tree of path. Result is reversed.
	 *
	 * @param int $lookinForID
	 * @param array $structure
	 * @return array|false
	 */
	function FindPathInTree($lookinForID, $structure = null)
	{
		$finalRes = false;
		
		if (! $structure)
		{
			$structure = $this->m_struct;
		}
		
		if ($structure)
		{
			foreach ( $structure as $item )
			{
				if ($item ['ID'] == $lookinForID)
				{
					$item ['ref'] = null;
					$finalRes = array ($item );
					break;
				}
				
				if ($item ['ref'] != null)
				{
					$finalRes = $this->FindPathInTree ( $lookinForID, $item ['ref'] );
					
					if ($finalRes)
					{
						$item ['ref'] = null;
						array_push ( $finalRes, $item );
						break;
					}
				}
			}
		}
		
		return $finalRes;
	}
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function Update()
	{
		// tree case
		if ($this->m_treeParams)
		{
			// controller for read
			if (! $this->IsReadable ( $this->m_treeParams ['tableName'] ))
			{
				return false;
			}
			
			// read structure
			$this->m_struct = DB::QueryToTree ( $this->m_treeParams ['tableName'], $this->m_treeParams ['relCollName'], $this->m_treeParams ['conditions'], $this->m_treeParams ['cols'], $this->m_treeParams ['rootElem'], $this->m_treeParams ['withRoot'], $this->m_treeParams ['currentDepth'], $this->m_treeParams ['maxdDepth'] );
		}
		// list case
		else
		{
			// controller for read
			if (self::HasController ())
			{
				if (! $this->m_tableNames)
				{
					$this->m_tableNames = DBxp::GetSQLQueryTables ( $this->m_queryString );
				}
				
				// chack is any of tables is protected
				foreach ( $this->m_tableNames as $table )
				{
					if (! $this->IsReadable ( $table ))
					{
						return false;
					}
				}
			}
			
			// read structure
			$this->m_struct = DB::QueryToArray ( $this->m_queryString );
		}
		
		return true;
	}
	
	function SetAdditionalPath($path)
	{
		$this->m_additionPath = $path;
	}
	
	function GetAdditionalPath()
	{
		return $this->m_additionPath;
	}
	
	function SetAdditionalPathSelected($path)
	{
		$this->m_additionPathSelected = $path;
	}
	
	function GetAdditionalPathSelected()
	{
		return $this->m_additionPathSelected;
	}
}
?>