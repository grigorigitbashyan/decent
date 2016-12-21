<?php
class RSS extends NamedRecord
{
	protected $m_url;
	/**
	 * Enter description here...
	 *
	 * @var simple_xml
	 */
	protected $m_xml;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_title;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_copyright;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_link;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_category;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_description;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_language;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_lastBuildDate;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_ttl;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_imageTitle;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_imageLink;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_imageURL;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_imageWidth;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_imageHeight;
	
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_item;
	
	
	protected $m_generator;
	
	protected $m_structure;
	protected $m_relations;
	protected $m_type;
	protected $m_value;
	protected $m_utype;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct($url)
	{
		parent::__construct($url, 'rss', 'rss');
		$this->Read();
		
		$this->m_url = $url;
		$this->m_generator = 'Siteius CMS Feed Generator 0.1';
	}
	
	/**
	* Insert new record
	*
	* @param array $properties
	* @return bool
	*/
	function Insert($properties = null)
	{
		 return $this->InsertRecord($properties);
	}
	
	function GetStruct()
	{
		return $this->m_structure;
	}
	
	function GetRelations()
	{
		return $this->m_relations;
	}
	
	function GetType()
	{
		return $this->m_type;
	}
	
	function GetUType()
	{
		return $this->m_utype;
	}
	
	function GetValue()
	{
		return $this->m_value;
	}
	
	function SetRelations($relations)
	{
		$this->m_relations = $relations;
		
		$sql = "update `rss` set `relations` = '$relations' where `rss` = '".$this->m_url."'";
		return DB::Query($sql);
		
//		return $this->UpdateValue("relations", $relations);
	}
	
	function SetStruct($structure)
	{
		$this->m_structure = $structure;
		
		$sql = "update `rss` set `structure` = '$structure' where `rss` = '".$this->m_url."'";
		return DB::Query($sql);
		
//		return $this->UpdateValue("relations", $relations);
	}
	
	function SetType($type)
	{
		$this->m_type = $type;
		
		$sql = "update `rss` set `type` = '$type' where `rss` = '".$this->m_url."'";
		return DB::Query($sql);
		
//		return $this->UpdateValue("type", $type);
	}
	
	function SetUType($utype)
	{
		$this->m_utype = $utype;
		
		$sql = "update `rss` set `utype` = '$utype' where `rss` = '".$this->m_url."'";
		return DB::Query($sql);
		
//		return $this->UpdateValue("type", $type);
	}
	
	function SetValue($value)
	{
		$this->m_value = $value;
		
		$sql = "update `rss` set `value` = '$value' where `rss` = '".$this->m_url."'";
		return DB::Query($sql);
		
//		return $this->UpdateValue("value", $value);
	}
	
	/**
	 * @return string
	 */
	final public function GetCategory()
	{
		return $this->m_category;
	}
	
	/**
	 * @return string
	 */
	final public function GetCopyright()
	{
		return $this->m_copyright;
	}
	
	/**
	 * @return string
	 */
	final public function GetDescription()
	{
		return $this->m_description;
	}
	
	/**
	 * @return string
	 */
	final public function GetImageTitle()
	{
		return $this->m_imageTitle;
	}
	
	/**
	 * @return string
	 */
	final public function GetImageLink()
	{
		return $this->m_imageLink;
	}
	
	/**
	 * @return string
	 */
	final public function GetImageURL()
	{
		return $this->m_imageURL;
	}
	
	/**
	 * @return int
	 */
	final public function GetImageWidth()
	{
		return $this->m_imageWidth;
	}
	
	/**
	 * @return int
	 */
	final public function GetImageHeight()
	{
		return $this->m_imageHeight;
	}
	
	/**
	 * @return array
	 */
	final public function GetItem()
	{
		return $this->m_item;
	}
	
	/**
	 * @return string
	 */
	final public function GetLanguage()
	{
		return $this->m_language;
	}
	
	/**
	 * @return string
	 */
	final public function GetLastBuildDate()
	{
		return $this->m_lastBuildDate;
	}
	
	/**
	 * @return string
	 */
	final public function GetLink()
	{
		return $this->m_link;
	}
	
	/**
	 * @return string
	 */
	final public function GetTitle()
	{
		return $this->m_title;
	}
	
	/**
	 * @return int
	 */
	final public function GetTtl()
	{
		return $this->m_ttl;
	}
	
	/**
	 * @return simple_xml
	 */
	final public function GetXml()
	{
		return $this->m_xml;
	}
	
	/**
	 * @param string $m_category
	 */
	final public function SetCategory($category)
	{
		$this->SetChanalPropery('category', $category);
	}
	
	/**
	 * @param string $m_copyright
	 */
	final public function SetCopyright($copyright)
	{
		$this->SetChanalPropery('copyright', $copyright);
	}
	
	/**
	 * @param string $m_description
	 */
	final public function SetDescription($description)
	{
		$this->SetChanalPropery('description', $description);
	}
	
	/**
	 * @param string $imageTitle
	 */
	final public function SetImageTitle($imageTitle)
	{
		$this->SetImagePropery('title', $imageTitle);
	}
	
	/**
	 * @param string $imageLink
	 */
	final public function SetImageLink($imageLink)
	{
		$this->SetImagePropery('link', $imageLink);
	}
	
	/**
	 * @param string $imageURL
	 */
	final public function SetImageURL($imageURL)
	{
		$this->SetImagePropery('url', $imageURL);
	}
	
	/**
	 * @param int $imageWidth
	 */
	final public function SetImageWidth($imageWidth)
	{
		$this->SetImagePropery('width', $imageWidth);
	}
	
	/**
	 * @param int $imageHeight
	 */
	final public function SetImageHeight($imageHeight)
	{
		$this->SetImagePropery('height', $imageHeight);
	}
	
	/**
	 * @param array $m_item
	 */
	final public function SetItem($m_item)
	{
		$this->m_item = $m_item;
	}
	
	/**
	 * @param string $m_language
	 */
	final public function SetLanguage($language)
	{
		$this->SetChanalPropery('language', $language);
	}
	
	/**
	 * @param string $m_lastBuildDate
	 */
	final public function SetLastBuildDate($lastBuildDate)
	{
		$this->SetChanalPropery('lastBuildDate', $lastBuildDate);
	}
	
	/**
	 * @param string $m_link
	 */
	final public function SetLink($link)
	{
		$this->SetChanalPropery('link', $link);
	}
	
	/**
	 * @param string $m_title
	 */
	final public function SetTitle($title)
	{
		$this->SetChanalPropery('title', $title);
	}
	
	/**
	 * @param int $m_ttl
	 */
	final public function SetTtl($ttl)
	{
		$this->SetChanalPropery('ttl', $ttl);
	}
	
	/**
	 * @param simple_xml $m_xml
	 */
	final public function SetXml($m_xml)
	{
		$this->m_xml = $m_xml;
	}

	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function Parse()
	{
		$this->m_xml = simplexml_load_file($this->m_url);
		
		if ($this->m_xml)
		{
			return $this->ReadChanal();
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function ReadChanal()
	{
		$channal = $this->m_xml->channel;
		
		$this->m_title = ($channal->title) ? (string) $channal->title : null;   
		$this->m_link = ($channal->link) ? (string) $channal->link : null;   
		$this->m_copyright = ($channal->copyright) ? (string) $channal->copyright : null;
		$this->m_category = ($channal->category) ? (string) $channal->category : null;
		$this->m_description = ($channal->description) ? (string) $channal->description : null;
		$this->m_language = ($channal->language) ? (string) $channal->language : null;
		$this->m_lastBuildDate = ($channal->lastBuildDate) ? (string) $channal->lastBuildDate : null;
		$this->m_ttl = ($channal->ttl) ? (string) $channal->ttl : null;
		
		$this->m_imageTitle = ($channal->image->title) ? (string) $channal->image->title : null;
		$this->m_imageLink = ($channal->image->link) ? (string) $channal->image->link: null;
		$this->m_imageURL = ($channal->image->url) ? (string) $channal->image->url: null;
		$this->m_imageWidth = ($channal->image->width) ? (string) $channal->image->width: null;
		$this->m_imageHeight = ($channal->image->height) ? (string) $channal->image->height : null;
		
		$this->m_item = array();
		
		if(isset($channal->item))
		{
			foreach ($channal->item as $item)
			{
				$itemArray = (array) $item;
				
				if(isset($itemArray->image))
				{
					unset($itemArray->image);
					$itemArray['image'] = (array) $item->image;
				}
				array_push($this->m_item, $itemArray);
			}
		}
		else 
		{
			$this->m_item = array();
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $property
	 * @param mixed $value
	 */
	final protected function SetChanalPropery($property, $value)
	{
		$prName = "m_" . $property;
		$this->$prName = $value;
		
		$this->m_xml->channel->$property = $value;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $property
	 * @param mixed $value
	 */
	final protected function SetImagePropery($property, $value)
	{
		$prName = "m_image" . $property;
		$this->$prName = $value;
		
		$this->m_xml->channel->image->$property = $value;
	}
	
	function Save($publish = false)
	{
		// un set items
		if(isset($this->m_xml->channel->item))
		{
			unset($this->m_xml->channel->item);
		}
		
		if(isset($this->m_item))
		{
			foreach ($this->m_item as $item)
			{
				$this->AddNewItem($item, false);
			}
		}
		
		//////////////////////////////////////////////
		
		if($publish)
		{
			$this->SetChanalPropery('lastBuildDate', date(DATE_RSS));
			$this->m_xml->asXML('../' . $this->m_url); 	
		}
		
		return $this->m_xml->asXML($this->m_url);
	}
	
	function AddNewItem($newItem, $addToArray = true)
	{
		$item = $this->m_xml->channel->addChild('item');
		
		if($newItem)
		{
			foreach ($newItem as $key => $value)
			{
				if($key == 'image')
				{
					$image = $item->addChild($key);
					if ( is_array($value) && count($value)>0 )
					{
						foreach ($value as $imageKey => $imageValue )
						{
							$image->addChild($imageKey, $imageValue);
						}
					}
				}
				else 
				{
					$item->addChild($key, htmlspecialchars($value));
				}
			}
		}

		if($addToArray)
		{
			array_push($this->m_item, $newItem);
		}
	}
	
	function DeleteItem($itemID)
	{
		if(isset($this->m_item[$itemID]))
		{
			unset($this->m_item[$itemID]);
		}
	}
	
	function DeleteAllItems()
	{
		unset($this->m_item);
		$this->m_item = array();
	}
	
	function UpdateItem($itemID, $newItem)
	{
		$this->m_item[$itemID] = $newItem;
	}
	
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		$res = true;
		// remove copy
		if(file_exists($this->m_url))
		{
			$res = unlink($this->m_url);
			
			$this->DeleteRecord ( true, $toStroe );
		}
		
		// remove rss
		if(file_exists('../' . $this->m_url))
		{
			$res &= unlink('../' . $this->m_url);
			
			$this->DeleteRecord ( true, $toStroe );
		}
		
		$sql = "delete from `rss` where `rss` = '".$this->m_url."'";
		DB::Query($sql);
		
		$this->m_url = "";
		
		return $res;
	}
	
	function Generate($siteURL, $lang="ru")
	{
		//	delete existing items
		$this->DeleteAllItems();
		
		//	get current rss file features
		$relations = $this->GetRelations();
		if ( $relations )
		{
			$relations = unserialize($relations);
		}
		
		$type = $this->GetType();
		$value = $this->GetValue();
		
		//	check relations
		if ( $relations )
		{
			//	get related articles
			if ( in_array(1, $relations) )
			{
				$relStr = " PM.`typeID` > 0";
			}
			else 
			{
				$relStr = "(PM.`typeID` = 0";
				for ( $i=0; $i<count($relations); $i++ )
				{
					$relStr .= " OR PM.`typeID` = ".$relations[$i];
				}
				$relStr .= ")";
			}
			
			if ( $type==1 )
			{
				$limitStr = " limit 0, ".$value;
				$dateStr = "";
			}
			elseif ( $type==2 )
			{
				$limitStr = "";
				$dateStr = " AND PM.`date` BETWEEN DATE_SUB(NOW(), INTERVAL ".$value." DAY) AND NOW()";
			}
			
			$query = "
				SELECT 
					PM.`ID`, 
					PM.`typeID`, 
					PM.`name_{$lang}` as `name`, 
					PM.`desc_{$lang}` as `desc`, 
					PM.`short_desc_{$lang}` as `short_desc`, 
					PM.`picture`, 
					PM.`date`, 
					PM.`random`, 
					S.`path` 
				FROM 
				  	`pr_main` PM 
				JOIN 
				  	`pr_options` PO ON (PM.`ID` = PO.`productID`) 
				JOIN 
				  	`pr_field` PF ON (PO.`fieldID` = PF.`ID`) 
				JOIN 
					`structure` S ON (S.`ID` = PO.`relatedID`)
				WHERE 
				  	(PF.`type` = 2 || PF.`type` = 8)
				  	AND 
				  	$relStr 
				  	$dateStr 
				ORDER BY 
					PM.`ID` desc 
				$limitStr
			";
			
			//	create items for rss file
			$retArr = DB::QueryToArray($query);
			for ( $i=0; $i<count($retArr); $i++ )
			{
				$title = html_entity_decode($retArr[$i]["name"], ENT_QUOTES, "utf-8");
				$title = str_replace("&", "and", $title);
				
				$link = $siteURL.$lang."/".$retArr[$i]["path"].$retArr[$i]["random"];
				
				$desc = strip_tags($retArr[$i]["desc"]);
				if ( strlen($desc)>500 && strpos($desc, " ", 500)!==false )
				{
					$desc = substr($desc, 0, strpos($desc, " ", 500))."...";
				}
				$desc .= " <a href='".$link."'>подробнее</a>";
				$desc = html_entity_decode($desc, ENT_QUOTES, "utf-8");
				
				$newItem = array ();
				$newItem ['title'] = $title;
				$newItem ['link'] = $link;
				$newItem ['description'] = "<img src='".$retArr[$i]["picture"]."' hspace='5' border='0' width='80' align='left'>".$desc;
				$newItem ['guid'] = $link;
				$newItem ['pubDate'] = date(DATE_RSS, strtotime($retArr[$i]["date"]));
				$newItem ['image']['title'] = $title;
				$newItem ['image']['link'] = $link;
				$newItem ['image']['url'] = $retArr[$i]["picture"];
				$newItem ['image']['width'] = 220;
				$newItem ['image']['height'] = 220;
				$this->AddNewItem ( $newItem );
			}
			
			$this->Save (true);
		}
	}
}
?>