<?php
session_start();
error_reporting(0);
set_include_path('cms/classes/');

include_once('Common/ContentCaching.class.inc');
include_once('Folder/FolderXP.class.inc');

$cache = new ContentCaching ();

if ( /*!$cache->DisplayCachedContent()*/ 1 ) {
    include_once('Common/DB.class.inc');
    include_once('files/db.inc');

    include_once('Common/Site.class.inc');
    include_once('Common/HTML.class.inc');

    Site::LoadStatics();

    include_once('Common/View.class.inc');

    include_once('Common/NamedRecord.class.inc');
    include_once('Content/Content.class.inc');

    include_once('Category/CategoryView.class.inc');
    include_once('Menu/MenuViewSite.class.inc');
    include_once('Gallery/GallerySite.class.inc');

    include_once('Common/functions.inc');
    include_once('Box/BoxCollectionViewSite.class.inc');

    include_once('Product/ProductViewSite.class.inc');
    include_once('Product/ProductAll.class.inc');
    include_once('Product/pr_main/ShoppingCartView.class.inc');

    include_once('Product/ProductSearch.class.inc');

    //SESSION//
    include_once('Session/Session.class.inc');
    $session = new Session (session_id());

    include_once('Account/SiteUser/SiteUserView.class.inc');
    include_once('RSS/RSSList.class.inc');
    include_once('RSS/RSS.class.inc');

    Site::ChackOffline();

    // get site and site URL
    $site = View::GetSite();

    // get language categoryID and other parametors
    $catID = 0;
    $lang = "";
    $params = array();

    $site->GetRequestParams($lang, $catID, $params);

    // registration SiteUser
    $regToSend = (!empty ($params)) ? $params : null; //Maybe null -> $catPath
    $siteUser = new SiteUserView($regToSend);
    $siteUser->DoAction();

    $siteUserCode = null;
    $isLogin = $siteUser->IsLogin();
    if (!$isLogin) {
        $issecurCat = new Category($catID);
        $catSecur = $issecurCat->GetSecurity();
        if ($catSecur) {
            $catID = 7;
        }
    } else {
        $siteUserObj = $siteUser->GetSiteUser();
        $siteUserCode = $siteUserObj->GetLogcode();
        if ($catID == 7) {
            $catID = 39;
        }
    }

    //	get currnet category
    $catDraw = new CategoryView ($catID, true);

    $catPtr = $catDraw->GetCategoryPtr();
    $catPath = $catPtr->GetPath();
    $catPathName = $catPtr->GetMainPathName($lang);
    $catID = $catPtr->GetID();
    $currMenuID = $catPtr->GetMenuID();
    $siteURL = $site->GetSiteURL();

    $pathArray = array();
    $catPtr->CalculatePath($pathArray);
    //	checking if in products 
    $inProd = false;
    for ($i = 0; $i < count($pathArray) && $pathArray[$i] != ""; $i++) {
        if ($pathArray[$i][0] == 103) {
            $inProd = true;
        }
    }

    $catPathTitles = $catPtr->GetPathTitles($pathArray, $lang);

    $pathArrayPr = array();
    $catPtr->CalculatePath($pathArrayPr);
    $catPathPrTitles = $catPtr->GetPathTitles($pathArrayPr, $lang);

    // set dsplay language
    View::SetDisplayLang($lang);

    // box view
    $boxCollectionViewSite = new BoxCollectionViewSite ($catID);

    // menu view
    $menuViewSite = new MenuViewSite ($catPtr, $lang, $params);

    // gallery 
    $galleryViewSite = new GallerySite($catID);

    $catFullPath = $siteURL . $catPath;
//	$prView = new ProductViewSite ( $catID, $catFullPath );
    $prView = new ProductViewSite ($catID, $siteURL . "$lang/");
    $prAll = new ProductAll();

    $productCountInCurrentPage = $prView->GetProductCountInCurrentPage($params);

    //	get SEO info
    $prName = "";
    $primg = "http://decent.ctrlq.com/img/cover.jpg";
    $prDesc = "";
    $prDescOpt = $prKeywordsOpt = 0;
    $prKeywords = "";
    $prAuthor = "ArattaUna";
    if ($prView->GetProductID($params)) {
        $prName = $prView->GetProductSEOTitle($params);
        if (!$prName) {
            $prName = $prView->GetProductName($params);
        }

        $primg = $prView->GetProductCover($params);
        if ($primg == "" || $primg == "img/cover.gif") {
            $primg = $prView->GetProductPicture($params);
        }

        $prNameOpt = $prView->GetProductSEOTitleOpt($params);
        $prDesc = $prView->GetProductSEODesc($params);
        if (!$prDesc) {
            $prDesc = $prView->GetProductDesc($params);
            $prDesc = strip_tags($prDesc);
            if (strlen($prDesc) > 300 && strpos($prDesc, " ", 300) !== false) {
                $prDesc = substr($prDesc, 0, strpos($prDesc, " ", 300)) . "...";
            }
        }

        $prDescOpt = $prView->GetProductSEODescOpt($params);
        $prKeywords = $prView->GetProductSEOKeywords($params);
        $prKeywordsOpt = $prView->GetProductSEOKeywordsOpt($params);

        $prAuthor = $prView->GetProductAuthor($params);
        $prPdate = $prView->GetPublishedDate($params);
        $prMdate = $prView->GetUpdatedDate($params);
        $prTags = $prView->GetProductTags($params);
    } else {
        $catPathWithParams = $catPath;
    }

    $parCount = count($params);
    if ($parCount > 0) {
        foreach ($params as $param)
            if (strlen($param) > 0)
                $catPathWithParams .= $param . "/";
    }

    // cache status
    $allowCache = $catPtr->GetCache();

    if ($allowCache) {
        ob_start();
    }

    $aaa = file_get_contents("http://www.geoplugin.net/json.gp?ip=" . $_SERVER['REMOTE_ADDR']);
    $aaa = json_decode($aaa, true);
    if ($aaa["geoplugin_countryCode"] == "CN") {
        $mapType = "bing";
    } else {
        $mapType = "google";
    }

    $currCat = new Category($catID);
    ?>

    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="utf-8">
        <title>DECENT</title>
    </head>
    <body>

    <div style="margin:300px auto;width:350px;text-align: center;">
        <h1>Welcome to DECENT</h1>
        <p>Opening soon</p>
    </div>

    </body>
    </html>

    <?php
    DB::ChangeConnection('admin');

    // Session
    $session->DeleteOldSessions();
    $session->UpdateSessionDataArray($_SESSION);
    DB::ChangeConnection('admin');
    if (0) {
        $cache->CacheContent();
        ob_flush();
    }
}
?>