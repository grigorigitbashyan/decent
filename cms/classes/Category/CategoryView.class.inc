<?php
// includes
Site::IncludeFile ( 'classes/Category/Category.class.inc' );
Site::IncludeFile ( 'classes/Category/SFRelation.class.inc' );
Site::IncludeFile ( 'classes/Category/SCRelation.class.inc' );

/** Class CategoryAction
 * This class provide all actions with category
 * 1. Display fithures with catgory (forms)
 * 2. rename category 
 * 3. delete category (if isdelete is true)
 * 4. reposition category (its always allowned)
 * 5. move category (if is move value is true)
 * 6. insert new category and rename its all names with user typeing
 * 7. set visible or invisible
 * 
 * @package Structure
 */
class CategoryView extends View
{
	/**
	 * This is category, that need to View
	 *
	 * @var Category
	 */
	protected $m_category;
	
	/**
	 * Constructor
	 *
	 * @param int $ID
	 * @return CategoryAction
	 */
	function __construct($ID, $redirect = false)
	{
		parent::__construct ();
		
		$this->m_category = new Category ( $ID, null, $redirect );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return Category
	 */
	function GetCategoryPtr()
	{
		return $this->m_category;
	}
	/**
	 * Display to category related single or multiple contents
	 *
	 * @param int $contID
	 * @param bool $drawDate
	 * @param bool $drawTitle 
	 * @return bool
	 */
	function DisplayContent($contID = 0, $drawDate = false, $drawTitle = true, $siteUserCode = null)
	{
		$docRoot = $_SERVER ['DOCUMENT_ROOT'] . '/';
		
		// get content ID
		$catID = $this->m_category->GetID ();
		
		if ( $catID!=327 )
		{
			//	display main content
			if ( $this->m_category->GetShowContentTitle() )
			{
				if ( $this->m_category->GetContentTitle($this->m_displayLang) )
				{
					echo "<h1 class='heading'>".$this->m_category->GetContentTitle($this->m_displayLang)."</h1>";
				}
				else 
				{
					echo "<h1 class='heading'>".$this->m_category->GetName($this->m_displayLang)."</h1>";
				}
			}
			echo $this->m_category->GetContentText($this->m_displayLang);
		}
		
		// get related files and contents counts
		$contCount = $this->m_category->GetRelContentCount ();
		// draw before file
		// file draw result
		$beforResult = false;
		
		// get category related file list
		$sfRel = new SFRelation ( $catID );
		
		$fRelIDs = $sfRel->GetCatRelations ();
		$root = $this->m_site->GetStructureToFileRoot ();
		
		if ($fRelIDs)
		{
			foreach ( $fRelIDs as $elatFile )
			{
				
				$filePath = $root . $elatFile ['filename'];
				
				// show_type = 0 is before content file
				if ($elatFile ['show_type'] == 0)
				{
					include ($docRoot . $filePath);
					$beforResult = true;
				}
			}
			
			reset ( $fRelIDs );
		}
		
		$rel = new SCRelation ( );
		
		// get cactegory related content list
		$relIDs = $rel->GetCatRelations ( $catID, $this->m_displayLang, false );
		
		// get related contents count
		$countIDs = count ( $relIDs );
		
		// content draw result
		$contentResult = false;
		if ($contCount == 1) // if structure type is 1, simple type
		{
			// single content
			$contID = ($countIDs > 0) ? $relIDs [0] ['ID'] : 0;
			$contentResult = $this->DrawSingleContent ( $contID, $drawDate, $drawTitle, $siteUserCode );
		}
		elseif ($contCount > 1) // for news type, it has more contents
		{
			// multi content
			$contentResult = $this->DrawMultiContent ( $relIDs, $contID, $drawTitle, $siteUserCode );
		}
		
		// draw after file
		// file draw result
		$afterResult = false;
		
		if ($fRelIDs)
		{
			foreach ( $fRelIDs as $elatFile )
			{
				
				$filePath = $root . $elatFile ['filename'];
				
				// show_type = 0 is before content file
				if ($elatFile ['show_type'] == 1)
				{
					include ($docRoot . $filePath);
					$afterResult = true;
				}
			}
		}
		
		return ($beforResult || $contentResult || $afterResult);
	}
	
	/**
	 * Draw single content 1) title 2) article and 3) date if $drawDate variable is true
	 * (dont forget to create titleClass in css)
	 * 
	 * @param int $contID
	 * @param bool $drawDate
	 * @return bool
	 */
	function DrawSingleContent($contID = 0, $drawDate = false, $drawTitle = true, $siteUserCode = null)
	{
		if ($contID > 0)
		{
			// get related content
			$content = new Content ( $contID );
			
			// get article
			$article = $content->GetContent ( $this->m_displayLang, true, $siteUserCode );
			
			// draw title
			if ($drawTitle && $content->GetTitleShow ())
				print ( "<h1>{$content->GetName($this->m_displayLang)}</h1>" );
				// draw content
			print ( '<div style="padding-top:0px; padding-left:0; padding-right:0px">' );
			
			$drawResult = false;
			
			if (strlen ( $article ) > 0)
			{
				print ( $article );
				$drawResult = true;
			}
			
			// draw data
			if ($drawDate)
			{
				$date = $content->GetDate ();
				$date = $this->m_site->GetDateTransformation ( $date, $this->m_displayLang );
				print ( "<p align='right'>$date</p>" );
			}
			
			print ( '</div>' );
			
			// return result
			return $drawResult;
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $contIDs
	 * @param int $contID
	 * @param bool $drawTitle
	 * @param string $siteUserCode
	 * @param int $limit
	 * @return bool
	 */
	function DrawMultiContent($contIDs, $contID = 0, $drawTitle, $siteUserCode = null, $limit = 31)
	{
		$countentCount = count ( $contIDs );
		
		if ($countentCount < 1)
			return false;
			
		// chack $contID is contents in to the $contIDs
		$isSub = false;
		
		foreach ( $contIDs as $subs )
		{
			if ($contID == $subs ['ID'])
			{
				$isSub = true;
				break;
			}
		}
		
		// if content ID is null or not from real list
		if ($contID < 1 || ! $isSub)
			$contID = $contIDs [0] ['ID'];
			
		// if exist true $contID draw it
		$drawContent = false;
		if ($contID)
		{
			$drawContent = $this->DrawSingleContent ( $contID, true, $drawTitle, $siteUserCode );
			print ( '<hr>' );
		}
		
		// get site URL for drawing category links
		$siteURL = $this->m_site->GetSiteURL ();
		
		// draw colegas contents
		$drawContentsList = false;
		if ($countentCount > 0)
		{
			$drawContentsList = true;
			
			foreach ( $contIDs as $relCont )
			{
				$drawContentsList = true;
				print ( '<table>' );
				if ($relCont ['ID'] != $contID)
				{
					$path = $this->m_category->GetPath ();
					$date = $this->m_site->GetDateTransformation ( $relCont['date'], $this->m_displayLang );
					print ( '<tr>' );
					print ( "<td valign='top' style='width: 75px;'>{$date}</td>" );
					print ( '<td>' );
					print ( "<a href='{$siteURL}{$this->m_displayLang}/{$path}{$relCont['ID']}/'>{$relCont['title']}</a>" );
					
					if ($relCont ['shortArticle'])
					{
						// replace image links 
						$shortContent = str_replace ( '../media/', $siteURL . 'media/', $relCont ['shortArticle'] );
						print ( "<p>$shortContent</p>" );
					}
					print ( '</td>' );
					print ( '</tr>' );
				}
				print ( '</table>' );
			}
			
			// list link
			if ($countentCount > $limit)
			{
				// draw page lists
				print ( '' );
			}
		}
		
		return ($drawContent || $drawContentsList);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $class
	 * @param int $startDepth
	 * @param bool $drawHome
	 * @param bool $allLink if true then display all path items as link, this cam be used with product item
	 */
	function DrawPath($class = null, $startDepth = 1, $drawHome = false, $allLink = false)
	{
		// get path
		$pathCategories = array ();
		$this->m_category->GetPathCategories ( $pathCategories, $this->m_displayLang );
		
		$depth = count ( $pathCategories );
		
		$siteURL = $this->m_site->GetSiteURL ();
		
		if ($depth >= $startDepth)
		{
			// current ID
			$ID = $this->m_category->GetID ();
			// draw home
			if ($drawHome)
				print ( " <a href='{$siteURL}{$this->m_displayLang}/'><img src='{$siteURL}img/home.gif' alt='Home' border='0'></a> /" );
				// draw path
			foreach ( $pathCategories as $category )
			{
				if (($category ['ID'] == $ID))
				{
					if ($allLink == false)
					{
						if ($class)
						{
							print ( " <span class='$class'>{$category['name']}</span>" );
						}
						else
						{
							print ( " <span>{$category['name']}</span>" );
						}
					}
					else
					{
						print ( " <a href='{$siteURL}{$this->m_displayLang}/{$category['path']}'>{$category['name']}</a>" );
					}
				}
				else
				{
					print ( " <a href='{$siteURL}{$this->m_displayLang}/{$category['path']}'>{$category['name']}</a> /" );
				}
			}
		}
	}
	
	/**
	 * Draw Category metas ` Title, description and keywords
	 *
	 */
	function DrawHeaders($prTitle='', $prTitleOpt=0, $prDesc='', $prDescOpt=0, $prKeywords='', $prKeywordsOpt=0, $prImg='img/thumbnail.gif', $pagePath='')
	{
		print ( "<base href='{$this->m_site->GetSiteURL()}' />\n" );
		print ( "<meta name='author' content='ArattaDesign' />\n" );
		print ( "<meta name=\"language\" content=\"{$this->m_displayLang}\" />\n" );
		
		// meta options
		$titleOpt = $this->m_category->GetTitleOpt();
		$descsOpt = $this->m_category->GetDescriptionOpt();
		$keywsOpt = $this->m_category->GetKeywordsOpt();
		
		//	configure page title
		if ( $prTitleOpt==2 )	//	only item title
		{
			if ( $prTitle )
			{
				$pageTile = $prTitle;
			}
			elseif ( $this->m_category->GetTitle($this->m_displayLang) )
			{
				$pageTile = $this->m_category->GetTitle($this->m_displayLang);
			}
			else 
			{
				$pageTile = $this->m_site->GetDefaultTitle($this->m_displayLang);
			}
		}
		elseif ( $titleOpt )	//	page title + item title
		{
			if ( $prTitle )
			{
				if ( $this->m_category->GetTitle($this->m_displayLang) )
				{
					$pageTile = $this->m_category->GetTitle($this->m_displayLang)." / ".$prTitle;
				}
				else 
				{
					$pageTile = $prTitle;
				}
			}
			else 
			{
				if ( $this->m_category->GetTitle($this->m_displayLang) )
				{
					$pageTile = $this->m_category->GetTitle($this->m_displayLang);
				}
				else 
				{
					$pageTile = $this->m_site->GetDefaultTitle($this->m_displayLang);
				}
			}
		}
		else 					//	default title + page title + item title
		{
			if ( $this->m_category->GetTitle($this->m_displayLang) && $prTitle )
			{
				$pageTile = $this->m_site->GetDefaultTitle($this->m_displayLang)." / ".$this->m_category->GetTitle($this->m_displayLang)." / ".$prTitle;
			}
			elseif ( $this->m_category->GetTitle($this->m_displayLang) )
			{
				$pageTile = $this->m_site->GetDefaultTitle($this->m_displayLang)." / ".$this->m_category->GetTitle($this->m_displayLang);
			}
			elseif ( $prTitle )
			{
				$pageTile = $this->m_site->GetDefaultTitle($this->m_displayLang)." / ".$prTitle;
			}
			else 
			{
				$pageTile = $this->m_site->GetDefaultTitle($this->m_displayLang);
			}
		}
		
		//	configure page description
		if ( $prDescOpt==2 )	//	only item desc
		{
			$pageDesc = $prDesc;
		}
		elseif ( $descsOpt )	//	page desc + item desc
		{
			$pageDesc = $this->m_category->GetDescription($this->m_displayLang)." ".$prDesc;
		}
		else 					//	default desc + page desc + item desc
		{
			$pageDesc = $this->m_site->GetDefaultDescription($this->m_displayLang)." ".$this->m_category->GetDescription($this->m_displayLang)." ".$prDesc;
		}
		
		//	configure page keywords
		if ( $prKeywordsOpt==2 )	//	only item keywords
		{
			$pageKeywords = $prKeywords;
		}
		elseif ( $keywsOpt )	//	page keywords + item keywords
		{
			$pageKeywords = $this->m_category->GetKeywords($this->m_displayLang).", ".$prKeywords;
		}
		else 					//	default keywords + page keywords + item keywords
		{
			$pageKeywords = $this->m_site->GetDefaultKeywords($this->m_displayLang).", ".$this->m_category->GetKeywords($this->m_displayLang).", ".$prKeywords;
		}
		
		//	display meta tags
		print ( "<title>$pageTile</title>\n" );
		print ( "<meta name=\"Description\" content=\"$pageDesc\" />\n" );
//		print ( "<meta name=\"Keywords\" content=\"$pageKeywords\" />\n" );
		
		// display copyright
		$siteName = $this->m_site->GetSiteName ();
		print ( "<meta name=\"copyright\" content=\"&copy; ".date("Y")." $siteName\" />\n" );
		
		if ( $prImg=="" )
		{
			$prImg = $this->m_site->GetDefaultPicture();
			if ( $prImg=="" )
			{
				$prImg = $this->m_site->GetSiteURL()."images/social.jpg";
			}
			else 
			{
				$prImg = $this->m_site->GetSiteURL().$prImg;
			}
		}
		
		//	facebook tags
		print ("<meta property=\"og:title\" content=\"".$pageTile."\" />\n");
		print ("<meta property=\"og:type\" content=\"website\" />\n");
		print ("<meta property=\"og:image\" content=\"".$prImg."\" />\n");
		print ("<meta property=\"og:url\" content=\"".$pagePath."\" />\n");
		print ("<meta property=\"og:site_name\" content=\"Safronov Brothers\" />\n");
		print ("<meta property=\"og:description\" content=\"".$pageDesc."\" />\n");
	}
	
	/**
	 * Return current title, which must be displayed 
	 *
	 * @param int $contID
	 * @return string|null
	 */
	function GetTitle($contID = 0)
	{
		// get content ID
		$catID = $this->m_category->GetID ();
		
		// get related contents count
		$contCount = $this->m_category->GetRelContentCount ();
		
		///////////////////////////////////////////////////
		$rel = new SCRelation ( );
		
		// get cactegory related content list
		$relIDs = $rel->GetCatRelations ( $catID, $this->m_displayLang, false );
		
		// get related contents count
		$countIDs = count ( $relIDs );
		
		// content draw result
		if ($contCount == 1) // if structure type is 1, simple type
		{
			// single content
			$contID = ($countIDs > 0) ? $relIDs [0] ['ID'] : 0;
		}
		
		// get content title (name)
		if ($contID)
		{
			$content = new Content ( $contID );
			return $content->GetName ( $this->m_displayLang );
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $templateRoot
	 * @return bool
	 */
	function DisplayTemplate($templateRoot = null)
	{
		$templateID = $this->m_category->GetTemplate ();
		$templates = $this->m_sitesite->GetTemplates ();
		$fileName = $templates [$templateID];
		
		if ($templateRoot)
		{
			$fileName = $templateRoot . $fileName;
		}
		
		if (file_exists ( $fileName ))
		{
			include_once $fileName;
			return true;
		}
		
		return false;
	}
}
?>