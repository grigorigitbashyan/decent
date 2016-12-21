<?php
/**
* Enter class description here...
*
* @package Item
*/
class Pr_type extends NamedRecord
{
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_pattern;

	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternlist;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternlist1;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternlistheader;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternlistfooter;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patterngroup;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternsearch;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patterngallery;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternrelation;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternrelationheader;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternrelationfooter;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternother;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternotherheader;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternotherfooter;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternwishlist;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternwishlistheader;
	
	/**
	* Enter description here...
	*
	* @protected array
	*/
	protected $m_patternwishlistfooter;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_textprice;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_picprice;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_contactprice;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_bonus;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_mintextcount;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_minpiccount;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_mincontactcount;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_bonustextcount;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_bonuspiccount;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_bonuscontactcount;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_order;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_icon_size;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_cover_size;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_thumbnail_size;
	
	/**
	* Enter description here...
	*
	* @protected boolean
	*/
	protected $m_icon_crop;
	
	/**
	* Enter description here...
	*
	* @protected boolean
	*/
	protected $m_cover_crop;
	
	/**
	* Enter description here...
	*
	* @protected boolean
	*/
	protected $m_thumbnail_crop;
	
	/**
	* Enter description here...
	*
	* @protected int
	*/
	protected $m_structureID;
	
	/**
	* Enter description here...
	*
	* @protected string
	*/
	protected $m_tabs;
	
	/**
	* Enter description here...
	*
	* @param int $ID
	* @return Pr_type
	*/
	function __construct($ID = null)
	{
		 parent::__construct($ID, 'pr_type');

		 $this->Read();
	}
	
	/**
	* Update record
	*
	* @param array $properties
	* @return bool
	*/
	function Update($properties)
	{
		 return $this->UpdateValues($properties);
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

	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPattern($lang = 'en')
	{
		return $this->m_pattern[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternList($lang = 'en')
	{
		return $this->m_patternlist[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternList1($lang = 'en')
	{
		return $this->m_patternlist1[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternListHeader($lang = 'en')
	{
		return $this->m_patternlistheader[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternListFooter($lang = 'en')
	{
		return $this->m_patternlistfooter[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternGroup($lang = 'en')
	{
		return $this->m_patterngroup[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternSearch($lang = 'en')
	{
		return $this->m_patternsearch[$lang];
	}

	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternGallery($lang = 'en')
	{
		return $this->m_patterngallery[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternRelation($lang = 'en')
	{
		return $this->m_patternrelation[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternRelationHeader($lang = 'en')
	{
		return $this->m_patternrelationheader[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternRelationFooter($lang = 'en')
	{
		return $this->m_patternrelationfooter[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternOther($lang = 'en')
	{
		return $this->m_patternother[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternOtherHeader($lang = 'en')
	{
		return $this->m_patternotherheader[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternOtherFooter($lang = 'en')
	{
		return $this->m_patternotherfooter[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternWishlist($lang = 'en')
	{
		return $this->m_patternwishlist[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternWishlistHeader($lang = 'en')
	{
		return $this->m_patternwishlistheader[$lang];
	}
	
	/**
	* Return value of m_patern
	*
	* @return blob
	*/
	function GetPatternWishlistFooter($lang = 'en')
	{
		return $this->m_patternwishlistfooter[$lang];
	}
	
	/**
	* Return value of m_textprice
	*
	* @return int
	*/
	function GetTextPrice()
	{
		return $this->m_textprice;
	}
	
	/**
	* Return value of m_picprice
	*
	* @return int
	*/
	function GetPicPrice()
	{
		return $this->m_picprice;
	}
	
	/**
	* Return value of m_contactprice
	*
	* @return int
	*/
	function GetContactPrice()
	{
		return $this->m_contactprice;
	}
	
	/**
	* Return value of m_bonus
	*
	* @return int
	*/
	function GetBonus()
	{
		return $this->m_bonus;
	}
	
	/**
	* Return value of m_mintextcount
	*
	* @return int
	*/
	function GetMinTextCount()
	{
		return $this->m_mintextcount;
	}
	
	/**
	* Return value of m_minpiccount
	*
	* @return int
	*/
	function GetMinPicCount()
	{
		return $this->m_minpiccount;
	}
	
	/**
	* Return value of m_minpiccount
	*
	* @return int
	*/
	function GetMinContactCount()
	{
		return $this->m_mincontactcount;
	}
	
	/**
	* Return value of m_bonustextcount
	*
	* @return int
	*/
	function GetBonusTextCount()
	{
		return $this->m_bonustextcount;
	}
	
	/**
	* Return value of m_bonuspiccount
	*
	* @return int
	*/
	function GetBonusPicCount()
	{
		return $this->m_bonuspiccount;
	}
	
	/**
	* Return value of m_bonuspiccount
	*
	* @return int
	*/
	function GetBonusContactCount()
	{
		return $this->m_bonuscontactcount;
	}
	
	/**
	* Return value of m_order
	*
	* @return int
	*/
	function GetOrder()
	{
		return $this->m_order;
	}
	
	/**
	* Return value of m_icon_size
	*
	* @return string
	*/
	function GetIconSize()
	{
		return $this->m_icon_size;
	}
	
	/**
	* Return value of m_cover_size
	*
	* @return string
	*/
	function GetCoverSize()
	{
		return $this->m_cover_size;
	}
	
	/**
	* Return value of m_thumbnail_size
	*
	* @return string
	*/
	function GetThumbnailSize()
	{
		return $this->m_thumbnail_size;
	}
	
	/**
	* Return value of m_icon_crop
	*
	* @return string
	*/
	function GetIconCrop()
	{
		return $this->m_icon_crop;
	}
	
	/**
	* Return value of m_cover_crop
	*
	* @return string
	*/
	function GetCoverCrop()
	{
		return $this->m_cover_crop;
	}
	
	/**
	* Return value of m_thumbnail_crop
	*
	* @return string
	*/
	function GetThumbnailCrop()
	{
		return $this->m_thumbnail_crop;
	}
	
	/**
	* Return value of m_structureID
	*
	* @return int
	*/
	function GetStructureID()
	{
		return $this->m_structureID;
	}
	
	/**
	* Return value of m_tabs
	*
	* @return string
	*/
	function GetTabs()
	{
		return $this->m_tabs;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* Set value to m_patern  
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPattern($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("pattern_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternlist  
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternList($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternlist_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternlist1
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternList1($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternlist1_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternlistheader
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternListHeader($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternlistheader_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternlistfooter
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternListFooter($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternlistfooter_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paterngroup  
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternGroup($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patterngroup_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternsearch
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternSearch($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternsearch_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paterngallery  
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternGallery($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patterngallery_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternrelation
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternRelation($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternrelation_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternrelation
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternRelationHeader($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternrelationheader_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternrelation
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternRelationFooter($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternrelationfooter_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternrelation
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternOther($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternother_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternrelation
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternOtherHeader($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternotherheader_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_paternrelation
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternOtherFooter($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternotherfooter_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_patern  
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternWishlist($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternwishlist_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_patern  
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternWishlistHeader($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternwishlistheader_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_patern  
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPatternWishlistFooter($pattern, $lang, $dbupdate = true)
	{
		return $this->UpdateValue("patternwishlistfooter_$lang", $pattern, $dbupdate);
	}
	
	/**
	* Set value to m_textprice  
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetTextPrice($value, $dbupdate = true)
	{
		return $this->UpdateValue("textprice", $value, $dbupdate);
	}
	
	/**
	* Set value to m_picprice
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetPicPrice($value, $dbupdate = true)
	{
		return $this->UpdateValue("picprice", $value, $dbupdate);
	}
	
	/**
	* Set value to m_contactprice
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetContactPrice($value, $dbupdate = true)
	{
		return $this->UpdateValue("contactprice", $value, $dbupdate);
	}
	
	/**
	* Set value to m_bonus
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetBonus($value, $dbupdate = true)
	{
		return $this->UpdateValue("bonus", $value, $dbupdate);
	}
	
	/**
	* Set value to m_mintextcount
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetMinTextCount($value, $dbupdate = true)
	{
		return $this->UpdateValue("mintextcount", $value, $dbupdate);
	}
	
	/**
	* Set value to m_minpiccount
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetMinPicCount($value, $dbupdate = true)
	{
		return $this->UpdateValue("minpiccount", $value, $dbupdate);
	}
	
	/**
	* Set value to m_mincontactcount
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetMinContactCount($value, $dbupdate = true)
	{
		return $this->UpdateValue("mincontactcount", $value, $dbupdate);
	}
	
	/**
	* Set value to m_bonustextcount
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetBonusTextCount($value, $dbupdate = true)
	{
		return $this->UpdateValue("bonustextcount", $value, $dbupdate);
	}
	
	/**
	* Set value to m_bonuspiccount
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetBonusPicCount($value, $dbupdate = true)
	{
		return $this->UpdateValue("bonuspiccount", $value, $dbupdate);
	}
	
	/**
	* Set value to m_bonuscontactcount
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetBonusContactCount($value, $dbupdate = true)
	{
		return $this->UpdateValue("bonuscontactcount", $value, $dbupdate);
	}
	
	/**
	* Set value to m_order
	*
	* @param blob $patern
	* @param bool $dbupdate
	* @return bool
	*/
	function SetOrder($value, $dbupdate = true)
	{
		return $this->UpdateValue("order", $value, $dbupdate);
	}
	
	/**
	* Set value to m_icon_size
	*
	* @param blob $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetIconSize($value, $dbupdate = true)
	{
		return $this->UpdateValue("icon_size", $value, $dbupdate);
	}
	
	/**
	* Set value to m_cover_size
	*
	* @param blob $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetCoverSize($value, $dbupdate = true)
	{
		return $this->UpdateValue("cover_size", $value, $dbupdate);
	}
	
	/**
	* Set value to m_thumbnail_size
	*
	* @param blob $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetThumbnailSize($value, $dbupdate = true)
	{
		return $this->UpdateValue("thumbnail_size", $value, $dbupdate);
	}
	
	/**
	* Set value to m_icon_crop
	*
	* @param blob $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetIconCrop($value, $dbupdate = true)
	{
		return $this->UpdateValue("icon_crop", $value, $dbupdate);
	}
	
	/**
	* Set value to m_cover_crop
	*
	* @param blob $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetCoverCrop($value, $dbupdate = true)
	{
		return $this->UpdateValue("cover_crop", $value, $dbupdate);
	}
	
	/**
	* Set value to m_thumbnail_crop
	*
	* @param blob $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetThumbnailCrop($value, $dbupdate = true)
	{
		return $this->UpdateValue("thumbnail_crop", $value, $dbupdate);
	}
	
	/**
	* Set value to m_structureID
	*
	* @param blob $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetStructureID($value, $dbupdate = true)
	{
		return $this->UpdateValue("structureID", $value, $dbupdate);
	}
	
	/**
	* Set value to m_tabs
	*
	* @param blob $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetTabs($value, $dbupdate = true)
	{
		return $this->UpdateValue("tabs", $value, $dbupdate);
	}
	
	/**
	* Delete product types relations
	*/
	function DeletePTRelations()
	{
		$sql = "delete from `pr_typerelation` where `typeID1` = ".$this->m_ID." or `typeID2` = ".$this->m_ID;
		return DB::Query($sql);
	}
	
	/**
	* add product types relations
	* 
	* @param array $relations
	*/
	function AddPTRelations($relations)
	{
		for ( $i=0; $i<count($relations); $i++ )
		{
			$sql = "insert into `pr_typerelation` (`typeID1`, `typeID2`) values (".$this->m_ID.", ".$relations[$i].")";
			DB::Query($sql);
			
			$sql = "insert into `pr_typerelation` (`typeID2`, `typeID1`) values (".$this->m_ID.", ".$relations[$i].")";
			DB::Query($sql);
		}
		
	}
	
	/**
	* get product types relations
	* 
	* @return array
	*/
	function GetPTRelations()
	{
		$sql = "select `typeID2` from `pr_typerelation` where `typeID1` = ".$this->m_ID;
		$retArr = DB::QueryToArray($sql);
		
		$newArr = array();
		for ( $i=0; $i<count($retArr); $i++ )
		{
			$newArr[] = $retArr[$i]["typeID2"];
		}
		
		return $newArr;
	}
}
?>