<?php
Site::IncludeFile ( 'classes/Common/TreeViewNavigation.class.inc' );
/**
 * @version 1.0
 * 
 * This class provide working with contnets. It provide fitures to geting 
 * all no related contents, and all related
 *
 * @package Content
 */
class ContentListView extends TreeViewNavigation
{
	protected $m_contentType;
	
	/**
	 * Enter description here...
	 *
	 * @param int $contentType
	 * @param int $limit
	 * @param int $Related
	 * @param bool $isDelete
	 * @param bool $withSort
	 * @param bool $withCont
	 */
	function __construct($contentType = 1, $limit = 20, $Related = 0, $isDelete = null, $withSort = false, $withCont = false, $sortBy = 'art_date', $lang = null)
	{
		parent::__construct ( null, 'contID', $limit, null, 'title', null, array('sortby'=>$sortBy) );
		
		$this->m_contentType = $contentType;
		$this->m_sortBy = $sortBy;		
		$cond = null;
		
		if(!$lang)
		{
			$lang = $this->m_displayLang;
		}		
		
		switch ($Related)
		{
			// only related
			case 1 :
				$cond = " `ID` IN (SELECT `contentID` FROM `structure_content`)  AND ";
				break;
			// onli no related
			case 2 :
				$cond = " `ID` NOT IN (SELECT `contentID` FROM `structure_content`) AND ";
				break;
		}
		
		$delS = null;
		
		if ($isDelete)
		{
			$delS = "`isdelete` = $isDelete AND";
		}
		
		// list with short content
		$shortS = ($withSort)?"`shortArticle_{$lang}` as `shortArticle`, ":'';
		// list with contents
		$contS = ($withCont)?"`article_{$lang}` as `article`, ":'';
		
		$sql = "SELECT `ID`, `name_{$lang}` as `title`, $shortS $contS `art_date`  FROM `content` WHERE $cond $delS  `purpose` = $contentType";
		
		$this->SetQuery($sql, $limit);
	}
	
	/**
	 * Get random content by selected purpose 
	 *
	 * @param int $purpos
	 * @return Content
	 */
	function GetRandContent($purpose = 2)
	{
		$sql = "SELECT `ID` FROM `content` WHERE `purpose` = $purpose ORDER BY RAND() LIMIT 1";
		$contentID = DB::QueryToValue($sql);
		
		if ($contentID)
		{
			$content = new Content ( $contentID );
			return $content;
		}
		// return result
		return null;
	}
	
	/**
	 * Return last few sorted contents from content type.
	 * The result array contains rows of content and each row contains anly 
	 * `ID`, `name`, `shortArticle` and `date`.
	 *
	 * @param int $count
	 * @return array
	 */
	function GetLastContents($count = 3)
	{
		// query string
		$sql = "SELECT `ID`, `name_{$this->m_displayLang}` as `name`, `shortArticle_{$this->m_displayLang}` as `shortArticle`, `art_date` as `date` 
				FROM `content` 
				WHERE `purpose` = {$this->m_contentType} ORDER BY `art_date` DESC LIMIT $count";

		return DB::QueryToArray($sql);
	}
}
?>