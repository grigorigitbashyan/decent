<?php
/** 
 * This class has more fitchures for working with content, it has featchures for languages,
 * 1. create db for content
 * 2. insert new content
 * 3. delete content
 * 4. update content
 * 5. search in content
 * 6. add isdelete fithure
 * 7. add purpose featchure
 * 
 * @package Content
 * @version 1.0
*/
class Content extends NamedRecord 
{
    /**
     * array of articles (all languages)
     *
     * @protected array
     */
    protected $m_article; // langucage suport
    
    /**
     * array of shot article
     *
     * @var array
     */
    protected $m_shortArticle; 
    /**
     * date, in defult value is date of creating, then you can change it (for example for news or events) 
     *
     * @protected date
     */
    protected $m_art_date;

    /**
     * 1 for structure, 2 for ...
     *
     * @protected int
     */
    protected $m_purpose;
    
    /**
     * Is content can be deleted
     *
     * @protected bool
     */
    
    protected $m_isdelete;
    
    /**
     * show or hide title in site
     *
     * @protected bool
     */
    protected $m_show_title;
    /**
     * constructor
     *
     * @param int $ID
     * @return Content
     */
    
    function __construct($ID = 0) // if null den insert new contnet
    {
        parent::__construct($ID, 'content');

       	$this->Read();
    }

    /**
     * create new content and set date, return new created ID
     *
     * @return int
     */
    function Create() // static function
    {
        // create new category by
        $property['date'] = date("Y-m-d");

        // return added ID
        return $this->InsertRecord($property);
    }

    /**
     * delete contnt and all referensis in structur_content
     * @return bool
     */
    function Delete($addToRecycle = false, $additionalStore = NULL)
    {
        if($this->m_isdelete == 0)
            return false;
            
        // delete from content DB
        return $this->DeleteRecord();
    }

    /**
      * Update content, $str is value for update
      * $bArtTitl is bool value for article or title update
      * $lang is language parametor
      * 
      * @param string $str
      * @param string $lang 
      * @return bool
      */
    function UpdateText($str, $lang)
    {
        return $this->UpdateValue("article_$lang", trim($str));
    }
    
	/**
	 * Update short article
	 *
	 * @param string $str
	 * @param string $lang
	 * @return bool
	 */
	function UpdateShortText($str, $lang)
    {
        return $this->UpdateValue("shortArticle_$lang", $str);
    }
    
    /**
     * Enter description here...
     *
     * @param string $date
     * @return bool
     */
    function UpdateDate($date)
    {
        return $this->UpdateValue('art_date', $date);
    }
	
    /**
     * Enter description here...
     *
     * @param bool $visibility
     * @return bool
     */
    function UpdateVisibility($visibility = true)
    {
        return $this->UpdateValue('show_title', $visibility);
    }
    
/**
     * Enter description here...
     *
     * @param int $purpose
     * @return bool
     */
    function UpdatePurpose($purpose)
    {
        return $this->UpdateValue('purpose', $purpose);
    }
    
    /**
     * Insert new contnet to the content DB, set today date, title and type of content
     *
     * @param string $title
     * @param int $type
     * 
     * @return bool
     */
    function Insert($title, $article = "", $purpose = 1)
    {
        $title = trim($title);
        
        if(strlen($title) < 1)
        {
            return false;
        }
        // get today
        
        $properies = array();
        
        $properies['art_date'] = date("Y-m-d");
        $properies['purpose'] = $purpose;
        
        // get site supported languages
        $site = View::GetSite();
        $languages = $site->GetLanguages();
        
        foreach ($languages as $language)
        {
            $properies["name_$language"] = $title; 
            $properies["article_$language"] = $article; 
        }

        // insert new contnet, and all titles and date (today)
        $res = $this->InsertRecord($properies);    
        
        // update
        if($res)
        {
            $this->Read();
        }
    
        return $res;    
    }

    /**
     * Search by kayword in titles and articles
     *
     * @param string $kword
     * @param bool $title
     * @param bool $article
     * @return array|null
     */
    function Search($kword, $title=true, $article=true) // static function
    {
        // reslts
        $kword = trim($kword);

        if(strlen($kword) < 1)
        	return null;
        // get site supported languages
        $site = new Site();
        $languages = $site->GetLanguages();

        // title sql
        $sqlTitle = "";
        $titleArr = array();
        if($title)
        {
            foreach ($languages as $language)
            array_push($titleArr, "`name_$language` LIKE '%$kword%'");

            $sqlTitle = join(" OR ", $titleArr);
        }

        // article sql
        $sqlArticle = "";
        $articleArr = array();
        if($article)
        {
            foreach ($languages as $language)
            array_push($articleArr, `article_$language`);

            $sqlArticle = join(" OR ", $articleArr);
        }
        // join sqls
        $jsql = "";
        if($title && $article)
        {
            $jsql = " $sqlTitle OR $sqlArticle ";
        }
        elseif ($title)
        {
            $jsql = " $sqlTitle ";
        }
        elseif ($article)
        {
            $jsql = " $sqlArticle ";
        }
        else
        {
            return null;
        }

        // sql query
        $sql = "SELECT `ID` FROM `content` WHERE $jsql;";
        return DB::QueryToVector($sql);
    }
    
    /**
     * Return replaced content (image, ...)
     *
     * @param string $content
     * @param string $lang
     * @param string $securityCode
     * @param string $shortContent
     * 
     * @return strig
     */
	function GetContent($lang = 'en', $replase=true, $securityCode = null, $secReplase = true, $shortContent = false)
    {
        // replaing
        $site = View::GetSite();
        
        $root = $site->GetSiteURL();
        
        // get long or short content
        $content = ($shortContent) ? $this->m_shortArticle[$lang] : $this->m_article[$lang];
        
        // replase images path
        if($replase)
        {
           $content = str_replace("\"../media", "\"{$root}media", $content);
        }    
        
        // replase security src
        if($securityCode)
        {
        	$content = str_replace("**securityfileID**", "{$root}files/FileMeakerSite.php?code=$securityCode", $content);
        }
        else 
        {
        	$sroot = $site->GetSFRoot();
        	if ($secReplase && $sroot)
        	{
        		// login category ID is 3
        		// Changed by Adriada: in 'structure' table of db login page has id = 7 
	        	$category = new Category(7);
	        	$loginPath = $category->GetPath();
	
	        	$content = str_replace("**securityfileID**", "{$root}$lang/$loginPath", $content);
        	}
        }

        return $content;
    }
    
    /**
     * Return content date
     *
     * @return string
     */
    function GetDate()
    {
        return $this->m_art_date;
    }
    
    /**
     * Return status for delete
     *
     * @return bool
     */
    function GetDeleteStatus()
    {
        return $this->m_isdelete;
    }
   
    /**
     * Return bool value of title showen status
     *
     * @return bool
     */
    function GetTitleShow()
    {
        return $this->m_show_title;    
    }
    
    /**
     * Return purpose
     *
     * @return int
     */
    function GetPurpose() 
    {
    	return $this->m_purpose;
    }
}
?>