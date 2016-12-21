<?php
/**
 * This class provide all functionality for working with site structure.
 * It has functions for adding, deleting, moving, repositioning, changing visibility status for items of structure,
 * and functions for setting title, keywords and description for itme of structure  in different languages.
 * They are used for setting meta tags information in site.
 * It also has particular features like redirection function.
 * 
 * @package Structure
 */
class Category extends NamedRecord
{
    /**
     * Parent category ID
     *
     * @protected int
     */
    protected $m_parentID;
    
    /**
     * If true you can delete this category
     *
     * @protected bool
     */
    protected $m_isdelete;
    
    /**
     * If true you can insert new sub category
     *
     * @protected bool
     */
    protected $m_isinsert;
    
    /**
     * If true you can move this category, and set new parent
     *
     * @protected bool
     */
    protected $m_ismove;

    /**
     * ID of menu 
     *
     * @protected int
     */
    protected $m_menuID;

    /**
     * If true then you can change status of visibility
     *
     * @protected bool
     */
    protected $m_isvisible;

    /**
     * If true then category is visible, other way it is invisible
     *
     * @protected bool
     */
    protected $m_visible;
    
    /**
     * Position of this categori in the parent category
     *
     * @protected int
     */
    protected $m_position;

    /**
     * Redirect
     *
     * @protected int
     */
    protected $m_redirect;

    /**
     * path cache control
     *
     * @protected bool
     */
    protected $m_ispathcache;

    /**
     * Name for category path
     * 
     * @param string
     */
    protected $m_pathname;

    /**
	 * category cached  path 
	 *
	 * @protected string
	 */
    protected $m_path;

    /**
     * Category keywords (all languages)
     *
     * @protected array
     */
    protected $m_key = array();
    
    /**
     * Category titles (all languages)
     *
     * @protected array
     */
    protected $m_title = array();
    
    /**
     * Category Descriptions (all languages)
     *
     * @protected array
     */
    protected $m_desc = array();

    /**
     * Count on contents, that can be related with category
     *
     * @protected int
     */
    protected $m_content_count;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_titleopt;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_descopt;
    
    /**
     * Enter description here...
     *
     * @protected bool
     */
    protected $m_keyopt;
    
    /**
     * Enter description here...
     *
     * @var bool
     */
    protected $m_security;
    
    /**
     * Enter description here...
     *
     * @var bool
     */
    protected $m_cache;
    
    /**
     * Enter description here...
     *
     * @var int
     */
    protected $m_template;
    
    /**
     * Category showcontenttitle
     *
     * @protected array
     */
    protected $m_showcontenttitle;
    
    /**
     * Category titles (all languages)
     *
     * @protected array
     */
    protected $m_contenttitle = array();
    
    /**
     * Category Descriptions (all languages)
     *
     * @protected array
     */
    protected $m_contenttext = array();
    
    /**
     * Category Picture URL
     *
     * @protected string
     */
    protected $m_picture;
    
    /**
     * Canstructor
     *
     * @param int $categoryID
     * @param string $path
     * @param bool $redirect
     * @return Category
     */
    function __construct($categoryID = null, $path = null, $redirect = false)
    {
        parent::__construct(1, 'structure');
        
        $this->m_ID   = $categoryID;
        $this->m_path = $path;

        $this->Update();

        // redirection
        if($redirect && $this->m_redirect > 0)
        {
            // if has sub categories
            $sIDs = array();
            $this->GetSubIDs($sIDs);
            $subCount = count($sIDs);
            if($subCount > 0)
            {
                $redID = 0;

                if($this->m_redirect == 1) // first in osition
                $redID = $sIDs[0];
                elseif ($this->m_redirect == 2) // last in ossition
                $redID = $sIDs[$subCount-1];

                $this->__construct($redID, $redirect);
            }
        }
    }

    /**
      * Insert new sub category and return ID of new crated category or false if its can`t insert
      * (for example if depth of menu is < then new inserted category depth)
      *
      * @param string $name
      * @param bool $isdelete
      * @param bool $isinsert
      * @param bool $ismove
      * @param int $menuID
      * @return int 
      */
    function InsertSub($name, $isdelete=true, $isinsert=true, $ismove=true, $isvisible=true, $menuID=-1) //tested
    {
        if(!$this->m_isinsert)
            return false;
            
        $name = trim($name);

        // if categorry name is empty return false;
        if(strlen($name) < 1)
        {
            // The Name of Category cann`t be empty
            return false;
        }

        // get menu depth and check that new sub category most <= maximu depth of menu
        $menu = new Menu($menuID, false);
        $depth = $this->GetDepth();

        if($depth >= $menu->GetDepth())
        {
            // The Maximum Depth Of Corrent Menu Is
            return false;
        }
        
        // create new category, set parent this category, return ID
        $site = View::GetSite();
        $langs = $site->GetLanguages();

        // get sub categoryes for positioning
        $subs = array();
        $this->GetSubIDs($subs);
        $position = count($subs) + 1;

        // if parent is root, then need enter menu ID
        $mID = ($this->m_ID == 1) ? $menuID : $this->m_menuID;// root

        $params['parentID'] = $this->m_ID;
        $params['isdelete'] = $isdelete;
        $params['isinsert'] = $isinsert;
        $params['ismove'] = $ismove;
        $params['isvisible'] = $isvisible;
        $params['menuID'] = $mID;
        $params['position'] = $position;
        $params['content_count'] = ($this->m_content_count != 0) ? $this->m_content_count : 1;
        
        foreach ($langs as $language)
        {
            $params["name_$language"] = $name;
        }

        $res = $this->InsertRecord($params);

        // calculate path vor new inserted category
        if($res)
        {
            $newName = $this->m_ID;
            $this->SetPathName($newName);
            $this->SavePath(false);
        }

        return $this->m_ID;
    }

    /**
      * Get depth of category
      *
      * @return int
      */
    public function GetDepth()
    {
    	if($this->m_ID == 1)
    	{
    		return 0;	
    	}
    	
    	$pathID = array();
        $this->GetPathCategories($pathID);;
        return count($pathID);
    }
    
    /**
      * Delete category and sub categoryes if $allDelete is true,
      * if $allDelete is false and category has sub categoryes, 
      * then return flase, else delete category and return true.
      *
      * @param bool $allDelete
      * @return bool
      */
    public function Delete($allDelete = false) //tested
    {
        if(!$this->m_isdelete)
            return false;
            
        // get all sub and delete all of them, this is recurtiv function
        $IDs = array();
        $this->GetSubIDs($IDs);

        // if alldelete is true, then delete all sub categoryes and contents of this category and this categry return true;
        if($allDelete)
        {
            foreach ($IDs as $ID)
            {
                $category = new Category($ID);
                $category->Delete(true);
            }

            return $this->DeleteRecord();
        }
        else // if alldelete is false
        {
            if(count($IDs) > 0) // if category has sub categories then return flase;
            {
                return false;
            }
            else // if category hasn`t sub categories then delete category and relation from struct_content return true;
            {
                return $this->DeleteRecord();
            }
        }
    }

    /**
      * set new position in parent category return true;
      *
      * @param int $newPosi
      * @return bool
      */
    public function Reposition($newPosi)
    {
    	$actionResult = $this->GetActionResult();
    	
        // if new positin is same old position
        if($newPosi == $this->m_position)
        {
        	$actionResult->SetStatus(4);        	
        	return $actionResult;
        }
        // get all categories from parent category with out current
        $colegieIDs = $this->GetColegaIDs();

        // get count of colegas
        $count = count($colegieIDs);

        if($newPosi > $count)
        {
            $newPosi = $count+1;
        }

        // set new positions for colegas of corrent category
        $subindex = 1;
        for($index = 0; $index < $count; $index++)
        {
            if($subindex == $newPosi) $subindex++; // $newPosi position for corrent category

            $sql = "UPDATE `structure` SET `position` = $subindex WHERE ID = {$colegieIDs[$index]}";
            DB::Query($sql);

            $subindex++;
        }

        // set new position for corrent category
        return $this->UpdateValue('position', $newPosi);
    }

    /**
      * set new parent ID
      *
      * @param int $parentID
      * @return bool
      */
    function Move($parentID, $newMenuID = null) // tested
    {
        $res = -1;
        
        // the category can movied onliy in salf menu
        $parent = new Category($parentID);
        $parentID = $parent->GetID();
        
        // if new parent is globals sub of corrent return flase or parent is corrent
        $bpar = $this->IsGlobalSub($parentID);

        if($bpar || $parentID == $this->m_ID)
        {
            // The Category Cann`t Moved To Childe Category
            return false;
        }

        if($this->m_menuID == $parent->GetMenuID() || $parentID == 1) // moving onliy in the menu
        {
            // get maximum sub depth and corrent depth and verifay that subDepth+corrDepth <= menuDepth

            // sub maximum depth
            $subDepth = $this->GetMaxSubDepth();

            // get parent depth
            $pDepth = $parent->GetDepth();

            // get menu depth
            $menu = new Menu($this->m_menuID, false);

            if(($subDepth + $pDepth) < $menu->GetDepth())
            {
                // move
                $res = $this->UpdateValue('parentID', $parentID);
            }
            else
            {
                // If Move, Then Menu Depth Will Be Big Then Maximum Depth!
                $res = false;
            }
        }

        if($res)
        {
            $this->SavePath(true, $newMenuID);
        }
                    
        // menu IDs doesn`t equal
        return $res;
    }

    /**
      * return array of sub category IDs
      * menuID need when root is HOME and home has more sub menus
      * @param array $IDs
      * @param int $menuID
      * @return bool
      */
    function GetSubIDs(&$IDs, $menuID=null)
    {
        if($this->m_ID == null)
        	return null;

        $IDs = array();

        // if parent is home, then it has more menus, you need set menuID
        if($menuID != null)
        {
        	$sql = "SELECT `ID` FROM `structure` WHERE `parentID`=$this->m_ID AND `menuID`=$menuID ORDER BY `position`";
        }
        else
        {
        	$sql = "SELECT `ID` FROM `structure` WHERE `parentID`=$this->m_ID ORDER BY `position`";
        }

        $IDs = DB::QueryToVector($sql);

        // add IDs into the array
        return ($IDs != null);
    }

    /**
      * This function answer is $catID category is sub category of this category or sub of eny sub category of this category
      * 
      * example  1->2->3   3 is global sub of 1
      *  
      * @param int $catID
      * @return bool
      */
    function IsGlobalSub($catID)
    {
        // get tring sub category
        $trSub = new Category($catID);
		
        // get tring sub category path
        $path = array(); 
        $trSub->GetPathCategories($path);

        // if in path has ID of corrent categorty return true
        foreach ($path as $cPath)
        {
        	if($cPath['ID'] == $this->m_ID)
        	{
        		return true;
        	}
        }
        
        return false;
    }

    /**
     * Return status for insert
     *
     * @return bool
     */
    public function IsInsert()
    {
        return $this->m_isinsert;
    }
    
    /**
     * Return status for move
     *
     * @return bool
     */
    public function IsMove()
    {
        return $this->m_ismove;
    }
    
    /**
     * Return status for visible
     *
     * @return bool
     */
    public function IsVsible()
    {
        return $this->m_isvisible;
    }
    
    /**
     * Return status for delete
     *
     * @return bool
     */
    public function IsDelete()
    {
        return $this->m_isdelete;
    }
    
    /**
      * Return Colega IDs
      *
      * @return array|null
      */
    function GetColegaIDs() // tested
    {
        // if parent is home, then it has more menus, you need set menuID
        $sql = "SELECT `ID` FROM `structure` WHERE `parentID`=$this->m_parentID AND `menuID`=$this->m_menuID AND `ID` <> $this->m_ID ORDER BY `position`";

        $IDs = DB::QueryToVector( $sql );

        // add IDs into the array
        return  $IDs;
    }

    /**
      * return path from category to home, you mast  reverce it if need display path
      *
      * @param array $path
      * @param int $ID
      */
    function CalculatePath(&$path, $ID=-1)// tested
    {
        // if ID  < 0 ith is corrent category ID

        if($ID == -1)
        $ID = $this->m_ID;

        // return path from `HOME` to this category, Home ID is 1
        if($ID == 1 || $ID == null)
        return;

        $parent = new Category($ID);
        array_push($path, array($ID , $parent->m_pathname));

        // get paretnt
        $sql = "SELECT `parentID` FROM `structure` WHERE `ID` = $ID";
        $parentID = DB::QueryToValue($sql);

        if( $parentID )
        {
            $this->CalculatePath($path, $parentID);
        }
    }
    
    /**
     * Return path titles string 
     *
     * @return string
     */
    function GetPathTitles($pathArray, $lang)
    {
    	$newPathArr = array();
    	
    	$i = 0;
        foreach ( $pathArray as $key=>$value )
        {
        	$pathID = $value[0];
        	
        	$sql = "SELECT * FROM `structure` WHERE `ID` = $pathID";
        	$pathArr = DB::QueryToArray($sql);
        	
        	if ( is_array($pathArr) ) 
        	{
        		$newPathArr[$i]["path"] = $pathArr[0]["path"];
        		$newPathArr[$i]["name"] = $pathArr[0]["name_".$lang];
        		
        		$i++;
        	}
        }
        
        $sql = "SELECT * FROM `structure` WHERE `ID` = 2";
    	$pathArr = DB::QueryToArray($sql);
    	
    	if ( is_array($pathArr) ) 
    	{
    		$newPathArr[$i]["path"] = $pathArr[0]["path"];
    		$newPathArr[$i]["name"] = $pathArr[0]["name_".$lang];
    	}
        
        return $newPathArr;
    }

    /**
     * Return path string
     *
     * @return string
     */
    function GetPath()
    {
        // if path doesn`t cached calculate apth and save to DB
        if(!$this->m_ispathcache)
        	$this->SavePath();

        return $this->m_path;
    }
    
    /**
     * return path name
     *
     * @return string
     */
    public function GetPathName()
    {
        return $this->m_pathname;
    }
    
    /**
     * Calculate category path and save to database
     * @param bool $rekursia
     * @param int $newMenuID
     * @return bool
     */
    function SavePath($rekursia = true, $newMenuID = null)
    {
        // if external
        $res = true;
        
        // if external
        if(strpos($this->m_pathname, 'http://') === 0)
        {
        	$res = $this->UpdateValue('path', $this->m_pathname);
        }
        else 
        {
        	// join path names
        	 // geting path
	        $path = array();
	        $this->CalculatePath($path);
        
	        if(count($path) > 0)
	        {
	            $path = array_reverse($path);
	            $strPath = "";
	
	            foreach ($path as $subpath)
	            {
	                $strPath .= $subpath[1]."/";
	            }
	
	            $res = $this->UpdateValue('ispathcache', 1);
	            $res = $this->UpdateValue('path', $strPath);
	        }
	        else
	        {
	            $res = $this->UpdateValue('ispathcache', 0);
	            $res = $this->UpdateValue('path', '');
	        }
	        
		    // if the menuID has been set, then change che menu ID
	        if($newMenuID)
	        {
	        	$res = $this->UpdateValue('menuID', $newMenuID);
	        }
	
	        // rekursia
	        if($res && $rekursia)
	        {
	            $subIDs = array();
	            $this->GetSubIDs($subIDs);
	
	            // if has sub categories
	            if(count($subIDs) > 0)
	            {
	                foreach ($subIDs as $subID)
	                {
	                    $subCategory = new Category($subID);
	                    $subCategory->SavePath($rekursia, $newMenuID);
	                }
	            }
	        }
        }
    }

    /**
     * Return category path categories ID, title and path 
     *
     * @param array $patIDs
     * @param string $lang
     * @param int $cID
     */
    function GetPathCategories(&$patIDs, $lang = 'en', $cID = -1)
    {
        // if ID  < 0 ith is corrent category ID

        if($cID == -1)
        {
            $cID = $this->m_ID;
        }

        // return path from `HOME` to this category, Home ID is 1
        if($cID == 1 || $cID == null)
        {
            $patIDs = array_reverse($patIDs);
                return;
        }

        $parent = new Category($cID);

        $name = $parent->GetName($lang);
        $path =  $parent->GetPath();

        array_push($patIDs, array('ID' => $cID , 'name' => $name, 'path' => $path));

        // get paretnt
        $sql = "SELECT `parentID` FROM `structure` WHERE `ID` = $cID";
        $parentID = DB::QueryToValue($sql);

        if( $parentID )
        {
            $this->GetPathCategories($patIDs, $lang, $parentID);
        }
    }

    /**
     * Return title
     *
     * @param string $lang
     * @return string
     */
    public function GetTitle($lang = 'en')
    {
        return (isset($this->m_title[$lang]))? $this->m_title[$lang] : '';
    }
    
    /**
     * Return title option
     *
     * @return bool
     */
    public function GetTitleOpt()
    {
        return $this->m_titleopt;
    }
    
    /**
     * Return visibility
     *
     * @return bool
     */
    public function GetVisible()
    {
        return $this->m_visible;
    } 
    
    /**
     * Return count of possible relations with contents
     *
     * @return int
     */
    public function GetRelContentCount()
    {
        return $this->m_content_count;
    }
    
    /**
     * Return description
     *
     * @param string $lang
     * @return string
     */
    public function GetDescription($lang = 'en')
    {
        return (isset($this->m_desc[$lang])) ? $this->m_desc[$lang] : '';
    }
    
     /**
     * Return description option
     *
     * @return bool
     */
    public function GetDescriptionOpt()
    {
        return $this->m_descopt;
    }
    
    /**
     * Return redirect status
     *
     * @return int
     */
    public function GetRedirect()
    {
        return $this->m_redirect;
    }
    
    /**
     * Return keywords
     *
     * @param string $lang
     * @return string
     */
    public function GetKeywords($lang = 'en')
    {
        return (isset($this->m_key[$lang]))? $this->m_key[$lang] : '';
    }
    
    /**
     * Return keywords options
     *
     * @return bool
     */
    public function GetKeywordsOpt()
    {
        return $this->m_keyopt;
    }
    
    /**
     * Return showcontenttitle
     *
     * @return tinyint
     */
    public function GetShowContentTitle()
    {
        return $this->m_showcontenttitle;
    }
    
    /**
     * Return contenttitle
     *
     * @param string $lang
     * @return string
     */
    public function GetContentTitle($lang = 'en')
    {
        return (isset($this->m_contenttitle[$lang]))? $this->m_contenttitle[$lang] : '';
    }
    
    /**
     * Return contenttext
     *
     * @param string $lang
     * @return string
     */
    public function GetContentText($lang = 'en')
    {
        return (isset($this->m_contenttext[$lang]))? $this->m_contenttext[$lang] : '';
    }
    
    /**
     * Return m_picture
     *
     * @return string
     */
    public function GetPicture()
    {
        return $this->m_picture;
    }
    
    /**
      * @param int $show
      * @return bool
      */
    function SetShowContentTitle($show)
    {
        return $this->UpdateValue("showcontenttitle", $show);
    }
    
    /**
      * @param string $title
      * @param string $lang
      * @return bool
      */
    function SetContentTitle($title, $lang)
    {
        return $this->UpdateValue("contenttitle_{$lang}", $title);
    }
    
    /**
      * @param string $text
      * @param string $lang
      * @return bool
      */
    function SetContentText($text, $lang)
    {
        return $this->UpdateValue("contenttext_{$lang}", $text);
    }
    
    /**
      * @param int $picture
      * @return bool
      */
    function SetPicture($picture)
    {
        return $this->UpdateValue("picture", $picture);
    }
    
    /**
     * Return menu ID
     *
     * @return int
     */
    function GetMenuID()
    {
        return $this->m_menuID;
    }
    
    /**
      * Return maxim depth from category
      *
      * @return int
      */
    function GetMaxSubDepth()//tested
    {
        $IDs = array();
        $this->GetSubIDs($IDs);

        if(count($IDs) == 0) // hasn`t sub categoryes
        {
            return 0;
        }

        $maxDepth = 0;

        // get all sub categoryes and return maximum depth of them depths
        foreach($IDs as $ID)
        {
            $cat = new Category($ID);
            $depth = $cat->GetMaxSubDepth();

            if($maxDepth < $depth)
            {
                $maxDepth = $depth;
            }
        }

        return 1+$maxDepth; // 1 is for corrent
    }

    /**
      * Rename category (select language), $type 1 for key, 2 for title and 3 for description
      * 
      * @param string $newName
      * @param string $lang
      * @param $type int
      * @return bool
      */
    function SetKeywords($newName, $lang, $type=1) // tested
    {
        $key = "key";
        if($type == 2) $key = "title";
        if($type == 3) $key = "desc";

        return $this->UpdateValue("{$key}_{$lang}", $newName);
    }
    
    /**
      * Rename category (select language), $type 1 for key, 2 for title and 3 for description
      * 
      * @param string $newName
      * @param $type int
      * @return bool
      */
    function SetKeywordsOpt($value, $type=1) // tested
    {
        $key = "key";
        if($type == 2) $key = "title";
        if($type == 3) $key = "desc";

        return $this->UpdateValue("{$key}opt", $value);
    }


    /**
     * Set category visibility status show or hidden
     *
     * @param bool $showhiden
     * @return bool
     */
    function Show($showhiden)
    {
        $showhiden = intval($showhiden);

        if($showhiden == 1 || $showhiden == 0)
        {
            return $this->UpdateValue("visible", $showhiden);
        }

        return false;
    }

    /**
     * Set redirect to DB
     *
     * @param int $type
     * @return bool
     */
    function SetRedirect($type)
    {
        return $this->UpdateValue("redirect", $type);
    }

    /**
     * Set content count to DB
     *
     * @param int $count
     * @return bool
     */
    function SetContentCount($count)
    {
        return $this->UpdateValue("content_count", $count);
    }
    /**
     * Update category information from DB
     *
     * @return bool
     */
    function Update() // tested
    {
        
        if($this->m_ID == null && $this->m_path == null)
            return false;

        if($this->m_ID == null && $this->m_path != null) //ID
        {
            $sql = "SELECT `ID` FROM `structure` WHERE `path`='{$this->m_path}'";
            $catID = DB::QueryToValue($sql);

            // if category isn`t exist
            if (! $catID) {
                $this->m_ID = null;
                return false;
            }
            else 
            {
                $this->m_ID = $catID;
            }
        }

        return $this->Read();
    }

    /**
     * Set path name
     *
     * @param string $pathName
     * @return bool
     */
    function SetPathName($pathName)
    {
        $pathName = trim($pathName);
        $pathName = str_replace(" ","", $pathName);

        if(strlen($pathName) > 0 && strcmp($pathName, $this->m_pathname) != 0)
        {
            return $this->UpdateValue('pathname', $pathName);
        }

        return false;
    }
    
/**
     * Enter description here...
     *
     * @return unknown
     */
    function GetParentID()
    {
    	return $this->m_parentID;
    }
    
    /**
     * Enter description here...
     *
     * @return bool
     */
    function GetSecurity()
    {
    	return $this->m_security;
    }
    
    /**
     * Enter description here...
     *
     * @return bool
     */
    function GetCache()
    {
    	return $this->m_cache;
    }
    
	/**
     * Enter description here...
     *
     * @return int
     */
    function GetTemplate()
    {
    	return $this->m_template;
    }
    
    /**
     * Enter description here...
     *
     * @param bool $security
     * @param bool $dbupdate
     * @return bool
     */
    function SetSecurity($security, $dbupdate = true)
    {
    	return $this->UpdateValue('security', $security, $dbupdate);
    }
    
    /**
     * Enter description here...
     *
     * @param bool $cache
     * @param bool $dbupdate
     * @return bool
     */
    function SetCache($cache, $dbupdate = true)
    {
    	return $this->UpdateValue('cache', $cache, $dbupdate);
    }
    
	/**
     * Enter description here...
     *
     * @param bool $cache
     * @param bool $dbupdate
     * @return bool
     */
    function SetTemplate($template, $dbupdate = true)
    {
    	return $this->UpdateValue('template', $template, $dbupdate);
    }
    
    /**
     * Enter description here...
     *
     * @param int $menuID
     * @param bool $dbupdate
     * @return bool
     */
    function SetMenuID($menuID, $dbupdate = true)
    {
    	return $this->UpdateValue('menuID', $menuID, $dbupdate);
    }
    
    /**
     * 
     * @param string $key
     * @param mixed $value
     */
    function SetValue($key, $value)
    {
    	return $this->UpdateValue($key, $value, true);
    }
    
    function GetMainPathName($lang)
    {
    	$parentID = $this->m_parentID;
    	$mainPID = $parentID;
    	if ( $parentID==1 )
    	{
    		$mainPID = $this->GetID();
    	}
    	
    	while ( $parentID>1 )
    	{
    		$sql = "select `parentID` from `structure` where `ID` = ".$parentID;
    		$parentID = DB::QueryToValue($sql);
    		if ( $parentID>1 )
    		{
    			$mainPID = $parentID;
    		}
    	}
    	
    	$sql = "select `name_$lang` from `structure` where `ID` = ".$mainPID;
    	$catName = DB::QueryToValue($sql);
    	
    	return $catName;
    }
}
?>