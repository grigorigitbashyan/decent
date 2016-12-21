<?php
// includes
$includes = array ('classes/Product/pr_int/Pr_int.class.inc', 
			'classes/Product/pr_int/Pr_ListMan.class.inc', 
			'classes/Product/pr_char/Pr_char.class.inc', 
			'classes/Product/pr_text/Pr_text.class.inc',
			'classes/Product/pr_double/Pr_double.class.inc', 
			'classes/Product/pr_main/Pr_options.class.inc', 
			'classes/Product/pr_field/Pr_field.class.inc', 
			'classes/ItemOptions/Optionsdata.class.inc',
			'classes/Product/pr_main/Pr_optionsList.class.inc', 
			'classes/Common/Image.class.inc' );

Site::IncludeFiles ( $includes );

/**
 * Enter class description here...
 *
 * @package Item
 */
class Pr_main extends NamedRecord
{
	/**
	 * Enter description here...
	 *
	 * @protected array
	 */
	protected $m_subtitle;
	
	/**
	 * Enter description here...
	 *
	 * @protected array
	 */
	protected $m_desc;
	
	/**
	 * Enter description here...
	 *
	 * @protected array
	 */
	protected $m_short_desc;
	
	/**
	 * Enter description here...
	 *
	 * @protected int
	 */
	protected $m_typeID;
	
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	protected $m_picture;
	
	/**
	 * Enter description here...
	 *
	 * @protected string
	 */
	protected $m_cover;
	
	/**
	 * Enter description here...
	 *
	 * @var date
	 */
	protected $m_date;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_random;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_viewCount;
	
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	protected $m_controller;
	
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	protected $m_special;
	
	/**
     * Category keywords (all languages)
     *
     * @protected array
     */
    protected $m_seokey = array();
    
    /**
     * Category titles (all languages)
     *
     * @protected array
     */
    protected $m_seotitle = array();
    
    /**
     * Category Descriptions (all languages)
     *
     * @protected array
     */
    protected $m_seodesc = array();
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_seokeyopt;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_seotitleopt;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_seodescopt;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_seokeyauto;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_seotitleauto;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_seodescauto;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_status;
    
    /**
     * Enter description here...
     *
     * @protected int
     */
    protected $m_userID;
    
    /**
     * Enter description here...
     *
     * @protected string
     */
    protected $m_pattern;

    /**
     * Enter description here...
     *
     * @protected string
     */
    protected $m_language;

    /**
     * Enter description here...
     *
     * @protected array
     */
    protected $m_lang;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @return Pr_main
	 */
	function __construct($ID = null)
	{
		$IDField = 'ID';
		
		if(is_numeric($ID))
		{
			$IDField = 'ID';	
		}
		elseif (is_string($ID))
		{
			$IDField = 'random';
		}
		
		parent::__construct ( $ID, 'pr_main', $IDField );
		
		$this->Read ();
	}
	
	/**
	 * Delete record from database
	 *
	 * @return bool
	 */
	function Delete($addToRecycle = false, $additionalStore = NULL)
	{
		// delete pictures
		$pcisListCMS = new Pr_pictureListCMS ( $this->m_ID );
		$picStoreValue = $pcisListCMS->Delete ( false );

		// delete ints
		$prList = new Pr_ListMan ( );
		$storeValue = $prList->DeleteProductInformation ( $this->m_ID );
		
		$toStroe = null;
		if ($storeValue)
		{
			$toStroe = array ();
			
			foreach ( $storeValue as $typeArray )
				foreach ( $typeArray as $valueArray )
				{
					array_push ( $toStroe, $valueArray );
				}
		}
		
		if ($picStoreValue)
		{
			if (! $toStroe)
			{
				$toStroe = array ();
			}
			
			foreach ( $picStoreValue as $typeArray )
				foreach ( $typeArray as $valueArray )
				{
					array_push ( $toStroe, $valueArray );
				}
		}
		// delete chars
		
		// remove folder 

		return $this->DeleteRecord ( true, $toStroe );
	}

	/**
	 * Update record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Update($properties)
	{
		return $this->UpdateValues ( $properties );
	}

	/**
	 * Insert new record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Insert($properties = null)
	{
		// generate random code
        $random = GetRandCode(3, true).GetRandCode(5);

        $query = "select * from `pr_main` where `random` = '$random'";
        $retArr = DB::QueryToRow($query);
        while ( is_array($retArr) )
        {
            $random = GetRandCode(3, true).GetRandCode(5);

            $query = "select * from `pr_main` where `random` = '$random'";
            $retArr = DB::QueryToRow($query);
        }

        if ( !isset($properties['random']) )
        {
            $properties['random'] = $random;
        }

		return $this->InsertRecord ( $properties );
	}
	
	/**
	 * Return value of m_subtitle
	 *
	 * @param string $lang
	 * @return string
	 */
	function GetSubtitle($lang)
	{
		return $this->m_subtitle[$lang];
	}
	
	/**
	 * Return value of m_desc
	 *
	 * @param string $lang
	 * @return string
	 */
	function GetDesc($lang)
	{
		return $this->m_desc [$lang];
	}
	
	/**
	 * Return value of m_short_desc
	 *
	 * @param string $lang
	 * @return string
	 */
	function GetDescShort($lang)
	{
		return $this->m_short_desc [$lang];
	}
	
	/**
	 * Return value of m_typeID
	 *
	 * @return int
	 */
	function GetTypeID()
	{
		return $this->m_typeID;
	}
	
	/**
	 * Return value of m_picture
	 *
	 * @return string
	 */
	function GetPicture()
	{
		return $this->m_picture;
	}
	
	/**
	 * Return value of m_cover
	 *
	 * @return string
	 */
	function GetCover()
	{
		return $this->m_cover;
	}
	
	/**
	 * Return value of m_viewCount
	 *
	 * @return int
	 */
	function GetViewCount()
	{
		return $this->m_viewCount;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetDate()
	{
		return $this->m_date;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetRandom()
	{
		return $this->m_random;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function GetProductController()
	{
		return $this->m_controller;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function GetSpecial()
	{
		return $this->m_special;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function GetStatus()
	{
		return $this->m_status;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return int
	 */
	function GetUserID()
	{
		return $this->m_userID;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetPattern()
	{
		return $this->m_pattern;
	}

    /**
     * Enter description here...
     *
     * @return string
     */
    function GetLanguage()
    {
        return $this->m_language;
    }

    /**
     * Return value of m_lang
     *
     * @param string $lang
     * @return string
     */
    function GetLang($lang)
    {
        return $this->m_lang[$lang];
    }
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetCategoryID()
	{
		$query = "select S.`ID` from `structure` S join `pr_options` PO on (S.`ID` = PO.`relatedID`) join `pr_field` PF on (PF.`ID` = PO.`fieldID`) where PF.`type` = 2 and PO.`productID` = ".$this->GetID()."";
		$pathID = DB::QueryToValue($query);
		
		return $pathID;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetCategoryPath()
	{
		$query = "select S.`path` from `structure` S join `pr_options` PO on (S.`ID` = PO.`relatedID`) join `pr_field` PF on (PF.`ID` = PO.`fieldID`) where PF.`type` = 2 and PO.`productID` = ".$this->GetID()."";
		$path = DB::QueryToValue($query);
		
		if ( $path=="" )
		{
			$query = "
				select S.`path` 
				from `optionsdata` OD 
				join `options` O on (OD.`optionsID` = O.`ID` and O.`category_id` > 0) 
				join `pr_options` PO on (PO.`relatedID` = OD.`ID`) 
				join `pr_main` PM on (PO.`productID` = PM.`ID` and PM.`ID` = ".$this->GetID().") 
				join `structure` S on (S.`ID` = O.`category_id`) 
			";
			$path = DB::QueryToValue($query);
		}
		
		if ( $path=="" )
		{
			$query = "select S.`path` from `structure` S join `pr_type` PT on (PT.`structureID` = S.`ID`) join `pr_main` PM on (PM.`typeID` = PT.`ID` and PM.`ID` = ".$this->GetID().")";
			$path = DB::QueryToValue($query);
		}
		
		return $path;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetCategoryName($catID=0)
	{
		$query = "select S.`name_ru` from `structure` S join `pr_options` PO on (S.`ID` = PO.`relatedID`) join `pr_field` PF on (PF.`ID` = PO.`fieldID`) where PF.`type` = 2 and PO.`productID` = ".$this->GetID()."";
		$catName = DB::QueryToValue($query);
		
		if ( $catName=="" )
		{
			if ( $catID )
			{
				$query = "
					select OD.`name_ru` as `name` 
					from `optionsdata` OD 
					join `options` O on (OD.`optionsID` = O.`ID` and O.`category_id` = $catID) 
					join `pr_options` PO on (PO.`relatedID` = OD.`ID`) 
					join `pr_main` PM on (PO.`productID` = PM.`ID` and PM.`ID` = ".$this->GetID().") 
				";
			}
			else 
			{
				$query = "
					select OD.`name_ru` as `name` 
					from `optionsdata` OD 
					join `options` O on (OD.`optionsID` = O.`ID` and O.`category_id` > 0) 
					join `pr_options` PO on (PO.`relatedID` = OD.`ID`) 
					join `pr_main` PM on (PO.`productID` = PM.`ID` and PM.`ID` = ".$this->GetID().") 
				";
			}
			$catName = DB::QueryToValue($query);
		}
		
		return $catName;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetMainCategoryPath()
	{
		$query = "select S.`path`, S.`parentID` from `structure` S join `pr_options` PO on (S.`ID` = PO.`relatedID`) join `pr_field` PF on (PF.`ID` = PO.`fieldID`) where PF.`type` = 2 and PO.`productID` = ".$this->GetID()."";
		$retArr = DB::QueryToRow($query);
		
		if ( $retArr && $retArr["parentID"]>2 )
		{
			$query = "select `path` from `structure` where `ID` = ".$retArr["parentID"];
			$path = DB::QueryToValue($query);
			
			return $path;
		}
		else 
		{
			return $retArr["path"];
		}
	}
	
	/**
	 * Set value to m_subtitle
	 *
	 * @param string $subtitle
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetSubtitle($subtitle, $lang, $dbupdate = true)
	{
		return $this->UpdateValue ( "subtitle_$lang", $subtitle, $dbupdate );
	}
	
	/**
	 * Set value to m_desc  
	 *
	 * @param string $desc
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetDesc($desc, $lang, $dbupdate = true)
	{
		return $this->UpdateValue ( "desc_$lang", $desc, $dbupdate );
	}
	
	/**
	 * Set value to m_short_desc  
	 *
	 * @param string $desc
	 * @param string $lang
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetDescShort($desc, $lang, $dbupdate = true)
	{
		return $this->UpdateValue ( "short_desc_$lang", $desc, $dbupdate );
	}
	
	/**
	 * Set value to m_typeID  
	 *
	 * @param int $typeID
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetTypeID($typeID, $dbupdate = true)
	{
		return $this->UpdateValue ( 'typeID', $typeID, $dbupdate );
	}
	
	/**
	 * Set value to m_picture  
	 *
	 * @param string $picture
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetPicture($picture, $dbupdate = true)
	{
		return $this->UpdateValue ( 'picture', $picture, $dbupdate );
	}
	
	/**
	 * Set value to m_cover
	 *
	 * @param string $cover
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetCover($cover, $dbupdate = true)
	{
		return $this->UpdateValue ( 'cover', $cover, $dbupdate );
	}
	
	/**
	 * Set value to m_viewCount
	 *
	 * @param string $viewCount
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetViewCount($viewCount, $dbupdate = true)
	{
		return $this->UpdateValue ( 'viewCount', $viewCount, $dbupdate );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $fieldID
	 * @param string $lang
	 * @param $fromSite
	 * @param $param - secondry param
	 * @return mixed
	 * 
	 */
	function GetFieldValue($fieldID, $lang = 'en', $valueOrID = false, $param = null, $group = 0)
	{
		$field = new Pr_field ( $fieldID );
		$value = null;
		$fieldType = $field->GetType ();
		$fieldMultiple = $field->GetMultilang();
		switch ($fieldType)
		{
			case 1 :
			case 2 :
				if($fieldMultiple != 2) // not multiple
				{
					$options = new Pr_options ( null, $this->m_ID, $fieldID, $group );
					if ($options->GetID ())
					{
						$value = $options->GetRelatedID ();
						if ($valueOrID && $fieldType == 1)
						{
							// get options list
							
							$options = new Optionsdata ( $value );
							switch ($param)
							{
								case 'key':
									$value = $options->GetKey();
									break;
								default:
									$value = $options->GetName ( $lang );
							}
						}
						else
						{
							$value = $options->GetRelatedID ();
						}
					}
					
					// structure type, gt path , name, or ID
					if($fieldType == 2 && $param)
					{
						$cat = new Category($value);
						switch ($param)
						{
							case 'path':
								$value = $cat->GetPath();
								break;
							case 'name':
								$value = $cat->GetName($lang);
								break;
							case 'ID':
								$value = $cat->GetID();
								break;
						}
					}
				}
				else 
				{
					$pol = new Pr_optionsList($this->m_ID, $fieldID);
					$struct = $pol->GetStructure();
					
					$value = array();
					if ( is_array($struct) )
					{
						foreach ($struct as $item)
						{
							if($valueOrID)
							{
								$options = new Optionsdata ( $item['relatedID'] );
								$name = $options->GetName ( $lang );
								array_push($value, $name);
							}
							else 
							{
								array_push($value, $item['relatedID']);
							}
						}
					}
				}
				
				break;
			case 3 :
				$fieldInt = new Pr_int ( null, $this->m_ID, $fieldID, $group );
				$value = $fieldInt->GetValue ();
				break;
			case 4 :
				$productDouble = new Pr_double ( null, $this->m_ID, $fieldID, $group );
				if ($productDouble->GetID ())
				{
					$value = $productDouble->GetValue ();
				}
				break;
			case 5 :
			case 6 :
				$dlang = ($fieldType == 5) ? null : $lang;
				$productChar = new Pr_char ( null, $this->m_ID, $fieldID, $dlang, $group );
				if ($productChar->GetID ())
				{
					$value = $productChar->GetValue ();
				}
				break;
			case 7 :
				$productText = new Pr_text( null, $this->m_ID, $fieldID, $lang, $group );
				if ($productText->GetID ())
				{
					$value = $productText->GetValue ( false );
				}
				break;
			case 8 :
				if($fieldMultiple != 2) // not multiple
				{
					$options = new Pr_options ( null, $this->m_ID, $fieldID );
					
					if ($options->GetIDs ())
					{
						$value = $options->GetRelatedIDs ();
					}
					
					// structure type, gt path , name, or ID
					if($fieldType == 2 && $param)
					{
						$cat = new Category($value);
						switch ($param)
						{
							case 'path':
								$value = $cat->GetPath();
								break;
							case 'name':
								$value = $cat->GetName($lang);
								break;
							case 'ID':
								$value = $cat->GetID();
								break;
						}
					}
				}
				else 
				{
					$pol = new Pr_optionsList($this->m_ID, $fieldID);
					$struct = $pol->GetStructure();
					
					$value = array();
					foreach ($struct as $item)
					{
						if($valueOrID)
						{
							$options = new Optionsdata ( $item['relatedID'] );
							$name = $options->GetName ( $lang );
							array_push($value, $name);
						}
						else 
						{
							array_push($value, $item['relatedID']);
						}
					}
				}
				
				break;
		}
		
		return $value;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function SetDefPicNull()
	{
		$this->SetPicture ( 'img/thumbnail.gif' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function SetCoverPicNull()
	{
		$this->SetCover( 'img/cover.gif' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $date
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetDate($date, $dbupdate = true)
	{
		return $this->UpdateValue ( 'date', $date, $dbupdate );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $controller
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetProductController($controller, $dbupdate = true)
	{
		return $this->UpdateValue ( 'controller', $controller, $dbupdate );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $special
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetSpecial($special, $dbupdate = true)
	{
		return $this->UpdateValue ( 'special', $special, $dbupdate );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $status
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetStatus($status, $dbupdate = true)
	{
		return $this->UpdateValue ( 'status', $status, $dbupdate );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $userID
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetUserID($userID, $dbupdate = true)
	{
		return $this->UpdateValue ( 'userID', $userID, $dbupdate );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param bool $pattern
	 * @param bool $dbupdate
	 * @return bool
	 */
	function SetPattern($pattern, $dbupdate = true)
	{
		return $this->UpdateValue ( 'pattern', $pattern, $dbupdate );
	}

    /**
     * Enter description here...
     *
     * @param bool $language
     * @param bool $dbupdate
     * @return bool
     */
    function SetLanguage($language, $dbupdate = true)
    {
        return $this->UpdateValue ( 'language', $language, $dbupdate );
    }

    /**
     * Set value to m_lang
     *
     * @param string $value
     * @param string $lang
     * @param bool $dbupdate
     * @return bool
     */
    function SetLang($value, $lang, $dbupdate = true)
    {
        return $this->UpdateValue ( "lang_$lang", $value, $dbupdate );
    }
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function DeleteRelations()
	{
		$sql = "delete from `pr_relation` where `productID` = ".$this->GetID();
		DB::Query($sql);
		
		$sql = "delete from `pr_relation` where `relatedID` = ".$this->GetID();
		DB::Query($sql);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $relations
	 * @return bool
	 */
	function SetRelations($relations)
	{
		for ( $i=0; $i<count($relations); $i++ )
		{
			$sql = "insert into `pr_relation` (`productID`, `relatedID`) values (".$this->GetID().", ".$relations[$i].")";
			DB::Query($sql);
			
			$sql = "insert into `pr_relation` (`productID`, `relatedID`) values (".$relations[$i].", ".$this->GetID().")";
			DB::Query($sql);
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetRelations($limit = 1000, $relType=0, $relField=0)
	{
		if ( $relType )
		{
			if ( $relField )
			{
				$sql = "
					select 
						PM.`ID` as `relatedID` 
					from 
						`pr_main` PM 
					join 
						`pr_options` PO on (PM.`ID` = PO.`productID` and PO.`fieldID` = $relField)
					where 
						PM.`typeID` = $relType and PO.`relatedID` in 
							(
								select `relatedID` from `pr_options` where `productID` = ".$this->GetID()." and `fieldID` = $relField
							)";
			}
			else 
			{
				$sql = "
					select 
						PR.`relatedID` 
					from 
						`pr_relation` PR 
					join 
						`pr_main` PM on (PR.`relatedID` = PM.`ID` and PM.`typeID` = $relType)
					where 
						PR.`productID` = ".$this->GetID();
			}
		}
		else 
		{
			$sql = "select `relatedID` from `pr_relation` where `productID` = ".$this->GetID();
		}
		
		$sql .= " limit 0, ".$limit;
		
		$retArr = DB::QueryToArray($sql);
		
		$retNewArr = array();
		for ( $i=0; $i<count($retArr); $i++ )
		{
			$retNewArr[] = $retArr[$i]["relatedID"];
		}
		
		return $retNewArr;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetOthers($limit=5, $order="date", $exIds="", $selectCurrent=false)
	{
		$orderStr = "";
		switch ( $order )
		{
			case 'date': $orderStr = " order by `date` desc"; break;
			case 'random': $orderStr = " order by rand()"; break;
			case 'popular': $orderStr = " order by `viewCount` desc"; break;
		}
		
		$exIdsStr = "";
		if ( is_array($exIds) && count($exIds)>0 )
		{
			$exIdsStr = " and `ID` not in (".implode(", ", $exIds).")";
		}
		
		if ( $selectCurrent )
		{
			$sql = "
				select `ID` 
				from `pr_main` 
				where `status` = 1 and `typeID` = ".$this->GetTypeID().$exIdsStr.$orderStr." 
				limit 0, ".$limit;
		}
		else 
		{
			$sql = "
				select `ID` 
				from `pr_main` 
				where `status` = 1 and `typeID` = ".$this->GetTypeID()." and `ID` <> ".$this->GetID().$exIdsStr.$orderStr." 
				limit 0, ".$limit;
		}
		
		$retArr = DB::QueryToArray($sql);
		
		return $retArr;
	}
	
	/**
     * Return title
     *
     * @param string $lang
     * @return string
     */
    public function GetSEOTitle($lang = 'en')
    {
        return (isset($this->m_seotitle[$lang]))? $this->m_seotitle[$lang] : '';
    }
    
    /**
     * Return seotitle option
     *
     * @return bool
     */
    public function GetSEOTitleOpt()
    {
        return $this->m_seotitleopt;
    }
    
    /**
     * Return seotitle option
     *
     * @return bool
     */
    public function GetSEOTitleAuto()
    {
        return $this->m_seotitleauto;
    }
    
    /**
     * Return seodescription
     *
     * @param string $lang
     * @return string
     */
    public function GetSEODescription($lang = 'en')
    {
        return (isset($this->m_seodesc[$lang])) ? $this->m_seodesc[$lang] : '';
    }
    
     /**
     * Return seodescription option
     *
     * @return bool
     */
    public function GetSEODescriptionOpt()
    {
        return $this->m_seodescopt;
    }
    
    /**
     * Return seodescription option
     *
     * @return bool
     */
    public function GetSEODescriptionAuto()
    {
        return $this->m_seodescauto;
    }
    
    /**
     * Return seokeywords
     *
     * @param string $lang
     * @return string
     */
    public function GetSEOKeywords($lang = 'en')
    {
        return (isset($this->m_seokey[$lang]))? $this->m_seokey[$lang] : '';
    }
    
    /**
     * Return seokeywords options
     *
     * @return bool
     */
    public function GetSEOKeywordsOpt()
    {
        return $this->m_seokeyopt;
    }
    
    /**
     * Return seokeywords options
     *
     * @return bool
     */
    public function GetSEOKeywordsAuto()
    {
        return $this->m_seokeyauto;
    }
    
    /**
      * Rename category (select language), $type 1 for key, 2 for title and 3 for description
      * 
      * @param string $newName
      * @param string $lang
      * @param $type int
      * @return bool
      */
    function SetSEOKeywords($newName, $lang, $type=1) // tested
    {
        $key = "seokey";
        if($type == 2) $key = "seotitle";
        if($type == 3) $key = "seodesc";

        return $this->UpdateValue("{$key}_{$lang}", $newName);
    }
    
    /**
      * Rename category (select language), $type 1 for key, 2 for title and 3 for description
      * 
      * @param string $newName
      * @param $type int
      * @return bool
      */
    function SetSEOKeywordsOpt($value, $type=1) // tested
    {
        $key = "seokey";
        if($type == 2) $key = "seotitle";
        if($type == 3) $key = "seodesc";

        return $this->UpdateValue("{$key}opt", $value);
    }
    
    /**
      * Rename category (select language), $type 1 for key, 2 for title and 3 for description
      * 
      * @param string $newName
      * @param $type int
      * @return bool
      */
    function SetSEOKeywordsAuto($value, $type=1) // tested
    {
        $key = "seokey";
        if($type == 2) $key = "seotitle";
        if($type == 3) $key = "seodesc";

        return $this->UpdateValue("{$key}auto", $value);
    }
    
    /**
	 * This function returns current article's group field items count
	 *
	 * @return array|null
	 */
	function GetGFC($fieldID, $type)
	{
		switch ($type)
		{
			case 1: $table = "pr_options"; break;
			case 3: $table = "pr_int"; break;
			case 4: $table = "pr_double"; break;
			case 5: $table = "pr_char"; break;
			case 6: $table = "pr_char"; break;
			case 7: $table = "pr_text"; break;
			default: $table = "";
		}
		
		if ( $table!="" )
		{
			$query = "select `group` from `$table` where `productID` = ".$this->m_ID." and `group` > 0 group by `group` order by `group`";
			$retArr = DB::QueryToArray($query);
			if ( is_array($retArr) )
			{
				for ( $i=0; $i<count($retArr); $i++ )
				{
					$retArrNew[] = $retArr[$i]["group"];
				}
				
				return $retArrNew;
			}
			
			return null;
		}
		
		return null;
	}

    function GetAuthorSiteName()
    {
        $query = "select `name` from `site_user` where `ID` = ".$this->m_userID;
        return DB::QueryToValue($query);
    }

    function GetAuthorRandom()
    {
        $query = "select `random` from `site_user` where `ID` = ".$this->m_userID;
        return DB::QueryToValue($query);
    }
}
?>