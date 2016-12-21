<?php

Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter description here...
 *
 * @package Main
 */
class TreeViewNavigation extends TreeView
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_limit;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_page;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_allResultesquery;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_sortBy;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_sortType = 'DESC';
	
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_externalParmas;
	
	/**
	 * Enter description here...
	 *
	 * @param string|array $structure
	 * @param string $linkIDName
	 * @param int $limit
	 * @param int $page
	 * @param string $linkName
	 * @param int $currID
	 * @param array $exParmas
	 * @param array $tableNames
	 */
	function __construct($structure = null, $linkIDName = null, $limit = null, $page = null, $linkName = 'name', $currID = null, $exParmas = null, $tableNames = null)
	{
		parent::__construct ( null, $linkIDName, $linkName, $currID, $tableNames );
		
		// set external parametores
		$this->m_externalParmas = $exParmas;
		
		$this->m_limit = $limit;
		$this->m_linkIDName = $linkIDName;
		
		$this->m_page = ($page) ? $page : $this->GetCurrentPage ();
		// sorting 		
		$this->m_sortBy = ($this->GetCurrentSort () !== 'e') ? $this->GetCurrentSort () : '';
		$this->m_sortType = ($this->GetCurrentSortType () !== 'e') ? $this->GetCurrentSortType () : '';
				
		// parse string
		if ($structure)
		{
			$this->SetQuery ( $structure, $limit );
		}
		
		$this->ReadNameStrings ( 'classes/Common/langs/', 'listNavigation.xml', $this->m_displayLang );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param unknown_type $structure
	 * @param int $limit
	 */
	function SetQuery($structure, $limit = null)
	{
		if ($limit)
		{
			$this->m_limit = $limit;
		}
		// parse string
		$pattern = '/[`\s]+FROM[\s]+/';
		$matches = preg_split ( $pattern, $structure, 2 );
		
		$this->m_allResultesquery = "SELECT count(*) as `count` FROM " . $matches [1];
		
		if ($this->m_sortBy)
		{
			$structure .= " ORDER BY {$this->m_sortBy} {$this->m_sortType} ";
		}
		
		// main query with limit
		if ($this->m_limit)
		{
			$start = ($this->m_page - 1) * $this->m_limit;
			$structure .= " LIMIT $start, {$this->m_limit}";
		}
		
		$this->SetQueryString ( $structure );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	function GetAllCount()
	{
		if (is_string ( $this->m_allResultesquery ))
		{
			return DB::QueryToValue ( $this->m_allResultesquery );
		}
		
		return 0;
	}
	
	/**
	 * This function displays navigation links.
	 * Set $ajaxFuStart and $ajaxFuEnd perimeters to display links by ajax functnion.
	 *
	 * @param string $additionalLink  The links must be added to the URL 
	 * @param bool $trueURL The control to display yre URL or by RL overloading
	 * @param string $ajaxFuStart ajax function start string 
	 * @param string $ajaxFuEnd ajax function end string
	 */
	function DisplayNavigation($additionalLink = null, $trueURL = false, $ajaxFuStart = null, $ajaxFuEnd = null, $tag='span', $tagClass=null, $afterLink="")
	{
		// get navigation sourse and print it
		print $this->GetNavigationSource($additionalLink, $trueURL, $ajaxFuStart, $ajaxFuEnd, $tag, $tagClass, $afterLink);
	}
	
	/**
	 * Return number of pages
	 *
	 * @return int
	 */
	function GetPageCount()
	{
		$countElements = $this->GetAllCount ();
		$pageCount = floor ( ($countElements - 1) / $this->m_limit ) + 1;
		
		return $pageCount;
	}
	/**
	 * This function return source code for navigation.
	 * Set $ajaxFuStart and $ajaxFuEnd perimeters to display links by ajax functnion.
	 *
	 * @param string $additionalLink  The links must be added to the URL 
	 * @param bool $trueURL The control to display yre URL or by RL overloading
	 * @param string $ajaxFuStart ajax function start string 
	 * @param string $ajaxFuEnd ajax function end string
	 * 
	 * @return string
	 */
	function GetNavigationSource($additionalLink = null, $trueURL = false, $ajaxFuStart = null, $ajaxFuEnd = null, $tag='span', $tagClass=null, $afterLink="")
	{
		// value to be returned
		$str = '';
		////////////////////////
		$countElements = $this->GetAllCount ();
		
		if ($countElements > $this->m_limit)
		{
			echo "<br clear='all'>";
			$tagClassS = ($tagClass) ? " class=\"$tagClass\"": '' ;
			$pageS = $this->GetNameString ( 'page' );
			
			// display 'page' world only in CMS
			if (! $this->m_externalParmas)
			{
				$str .= "<strong>$pageS</strong>";
			}
			
			$pageCount = floor ( ($countElements - 1) / $this->m_limit ) + 1;
			
			// pripearing tag start and end
			$tagStart = ($tag)?"<$tag{$tagClassS}>":'';
			$tagEnd = ($tag)?"</$tag>\n":'';
			
			// loop to display numbers
			for($index = 1; $index <= $pageCount; $index ++)
			{
				// add tag if it has been set
				$str .= $tagStart;
				
				// making link by true or false url
				$link = null;
				
				if ($trueURL)
				{
					$link = $additionalLink . "page{$index}/".$afterLink;
				}
				else
				{
					$link = "?page{$this->m_linkIDName}={$index}";
					
					if ($this->m_sortBy && $this->m_sortType)
					{
						$link .= "&sort{$this->m_linkIDName}=$this->m_sortBy&sortType=$this->m_sortType";
					}
					if ($additionalLink && strlen ( $additionalLink ) > 0)
					{
						$link .= '&' . $additionalLink;
					}
				}
				
				if ($this->m_page == $index)
				{
						$str .= "{$index}";
				}
				else
				{
					if ($ajaxFuStart && $ajaxFuEnd)
					{
						$str .= "<a href='{$ajaxFuStart}{$link}{$ajaxFuEnd}'>$index</a>";
					}
					else
					{
						$str .= "<a href='$link'>$index</a>";
					}
				}
				
				// add tag close
				$str .= $tagEnd;
			}
		}
		
		return $str;
	}
	
	/**
	 * If REQUEST contens page number return it, in other case return 1 
	 *
	 * @return int
	 */
	function GetCurrentPage()
	{
		// when list is used from Site and not from CMS
		if ($this->m_externalParmas && is_array ( $this->m_externalParmas ))
		{
			// looking for page param
			foreach ( $this->m_externalParmas as $parma )
			{
				// if param start with 'page'
				if (substr ( $parma, 0, 4 ) == 'page')
				{
					// if afther 'page' is number
					$pageIndex = intval ( substr ( $parma, 4 ) );
					if ($pageIndex)
					{
						return $pageIndex;
					}
				}
			}
		}
		
		//From CMS
		$res = DB::REQUEST ( "page{$this->m_linkIDName}" );
		
		if (! $res)
		{
			$res = 1;
		}
		return $res;
	}
	
	/**
	 * If REQUEST contens page number return it, in other case return 1 
	 *
	 * @return string
	 */
	function GetCurrentSort()
	{
		if (isset ( $this->m_externalParmas ['sortby'] ))
		{
			return $this->m_externalParmas ['sortby'];
		}
		//From CMS
		$res = $this->m_sortBy;
		
		if (($newRes = DB::REQUEST ( "sort{$this->m_linkIDName}" )))
		{
			$res = $newRes;
		}
		return $res;
	}
	
	/**
	 * If REQUEST contens page number return it, in other case return 1 
	 *
	 * @return string
	 */
	function GetCurrentSortType()
	{
		if (isset ( $this->m_externalParmas ['sorttype'] ))
		{
			return $this->m_externalParmas ['sorttype'];
		}
		
		$sortType = $this->m_sortType;
		if (($sortTypeNew = DB::REQUEST ( "sortType" )))
		{
			$sortType = $sortTypeNew;
		}
		
		return $sortType;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @param string $name
	 * @param string $addLink
	 */
	function DisplaySortLinks($key, $name, $addLink = null)
	{
		if ($addLink)
		{
			$addLink = '&' . $addLink;
		}
		
		$addLink .= "&page{$this->m_linkIDName}={$this->m_page}";
		
		if (count ( $this->m_struct ) > 0)
		{
			if ($this->m_sortBy == $key)
			{
				if ($this->m_sortType)
				{
					$sortType = ($this->m_sortType == 'ASC') ? 'DESC' : 'ASC';
				}
				else
				{
					$sortType = 'DESC';
				}
				print ( "<a href='?sort{$this->m_linkIDName}=$key&sortType={$sortType}{$addLink}'>$name</a>" );
			}
			else
			{
				print ( "<a href='?sort{$this->m_linkIDName}={$key}{$addLink}'>$name</a>" );
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	function GetLimit()
	{
		return $this->m_limit;
	}
	
	/**
	 * Return string that need to be added after structure link.
	 * This value can be used vor navigation purpose in list (sor, page, ...) 
	 *
	 * @param bool $withProduct
	 * @return string
	 */
	public function GetExternalLink($withProduct = false)
	{
		$additionLink = '';
		
		if ($this->m_externalParmas)
		{
			foreach ( $this->m_externalParmas as $key => $param )
			{
				
				if (strlen ( $param ) > 0)
				{
					// do not owerload the sort type and sort by properties
					if ($key == 'sortby' || $key == 'sorttype')
						continue;
					
					if ($withProduct)
					{
						$additionLink .= $param . '/';
					}
					else
					{
						if (! intval ( $param ))
						{
							$additionLink .= $param . '/';
						}
					}
				}
			}
		}
		return $additionLink;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $exParams
	 */
	function SetExternalParams($exParams)
	{
		$this->m_externalParmas = $exParams;
		
		if (isset ( $exParams ['sortby'] ))
		{
			$this->m_sortBy = $exParams ['sortby'];
		}
		
		if (isset ( $exParams ['sorttype'] ))
		{
			$this->m_sortType = $exParams ['sorttype'];
		}
		
		if (isset ( $exParams ['page'] ))
		{
			$this->m_page = $exParams ['page'];
		}
		
		if (isset ( $exParams ['0'] ) && (substr ( $exParams ['0'], 0, 4 ) == 'page'))
		{
			// if afther 'page' is number
			$pageIndex = intval ( substr ( $exParams ['0'], 4 ) );
			if ($pageIndex)
			{
				$this->m_page = $pageIndex;
			}
		}
	}
}
?>