<?php
/**
 * Enter description here...
 *
 * @package Structure
 */
class SCRelation
{
	function __construct()
	{
		
	}
	
	/**
	 * Insert or add new Relation
	 *
	 * @param int $catID
	 * @param int $contID
	 * @return bool
	 */
	function Add($catID, $contID)
	{
		// get category
		$category = new Category ( $catID );
		//if category type is one, then category has only one relation
		

		$contCount = $category->GetRelContentCount ();
		if ($contCount > 0) // if category can has content
		{
			if ($contCount == 1)
			{
				
				$sql = "SELECT count(*) FROM `structure_content` WHERE `structureID` = $catID";
				$count = DB::QueryToValue($sql);
				
				if ($count > 0) // if relation has olredy exist then update it set new contentID
				{
					$sql = "UPDATE `structure_content` SET `contentID` = $contID WHERE `structure_content`.`structureID` =$catID LIMIT 1 ;";
					$res = DB::Query( $sql );
					return $res;
				}
				else // else inser new relation
				{
					$sql = "INSERT INTO `structure_content` (`structureID`, `contentID`) VALUES ('$catID', '$contID');";
					$res = DB::Query( $sql );
					return $res;
				}
			}
			else //else insert new relatin if sampe relation doesnt exist  
			{
				$sql = "INSERT INTO `structure_content` (`structureID`, `contentID`) VALUES ('$catID', '$contID');";
				$res = DB::Query( $sql );
				return $res;
			}
		}
		
		return false;
	}
	
	/**
	 * Delete relation
	 *
	 * @param int $catID
	 * @param int $contID
	 */
	function Delete($catID, $contID)
	{
		$sql = "DELETE FROM `structure_content` WHERE `structureID`= $catID AND `contentID` = $contID";
		$res = DB::Query( $sql );
		return $res;
	}
	
	/**
	 * Return all rilations with $catID category (ID, title), or null. IF $bAll is true then return article and date.
	 * If $sortBY is null, then not sorted, true sorting by title, flase by date.
	 * @param int $catID
	 * @param string $lang
	 * @param bool $sortBY
	 * @return array
	 */
	function GetCatRelations($catID, $lang, $sortBY = true)
	{
		$title = "name_$lang";
		
		$sortT = "";
		
		if ($sortBY == true)
			$sortT = " ORDER BY $title ";
		elseif ($sortBY == false)
			$sortT = " ORDER BY `art_date` DESC ";
		
		$sql = "SELECT `ID`, `$title` as `title`, `art_date` as `date`, `purpose`, `shortArticle_$lang` as `shortArticle`  FROM `content` inner join `structure_content` ON `content`.`ID` = `structure_content`.`contentID`  WHERE `structure_content`.`structureID` = $catID $sortT";
		
		return DB::QueryToArray ( $sql );
	}
	
	/**
	 * Search in contents nd return results of content and categori ID-s wich content is related
	 *
	 * @param string $key
	 * @param string $lang
	 * @param bool $inSecur
	 * @return array
	 */
	function Search($key, $lang = 'en', $title = true, $article = true, $inSecur = false)
	{
		$array = array ();
		
		$key = trim ( $key );
		
		//////////////////////////////////////
		if (strlen ( $key ) > 0)
		{
			// get site supported languages
			$site = View::GetSite ();
			$languages = $site->GetLanguages ();
			
			// title sql
			$sqlTitle = "";
			$titleArr = array ();
			
			if ($title)
			{
				foreach ( $languages as $language )
				{
					array_push ( $titleArr, "`content`.`name_$language` LIKE '%$key%'" );
				}
				
				$sqlTitle = join ( ' OR ', $titleArr );
			}
			
			// article sql
			$sqlArticle = "";
			$articleArr = array ();
			if ($article)
			{
				foreach ( $languages as $language )
					array_push ( $articleArr, "`content`.`article_$language` LIKE '%$key%'" );
				
				$sqlArticle = join ( ' OR ', $articleArr );
			}
			// join sqls
			$jsql = "";
			
			if ($title && $article)
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
			
			//////////////////////////
			$secur = " AND `structure`.`security` = 0 ";
			if ($inSecur)
			{
				$secur = '';
			}
			
			$sql = "SELECT `structure`.`ID` as `structureID`, `structure`.`path` as `path`, `structure`.`ispathcache` as `ispath`, `content`.`ID` as `contentID`, `content`.`name_$lang` as `name` 
                        FROM 
                        `content` inner join `structure_content` inner join `structure` 
                            ON `content`.`ID` = `structure_content`.`contentID` AND `structure_content`.`structureID` = `structure`.`ID`  
                        WHERE ($jsql) $secur";
			
			$array = DB::QueryToArray($sql); 
		}
		
		return $array;
	}
}
?>