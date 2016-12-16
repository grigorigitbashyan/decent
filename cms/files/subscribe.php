<?php
// unsucrcribe url ///////////
// use @@code@@ in template
$unSubsMess = 'to unsubcribe just click on link <a href="http://www.tufenkianheritage.com/en/unsub/@@code@@/">http://www.tufenkianheritage.com/en/unsub/@@code@@/</a>';
//////////////////////////////
	// access only logined users
	include_once("../classes/Common/Site.class.inc");
	
	Site::LoadStatics();
	
	
	include_once("../classes/Common/NamedRecord.class.inc");
	include_once("../classes/UserManagement/UserManagement.class.inc");
	include_once("db.inc");
	
	$um = new UserManagement();
	$login = $um->CheckLog();
    
    if(!$login)
    {
    	die('Please Log in!');
    }
    
// include content
Site::IncludeFile('classes/Content/Content.class.inc' );

// include subscribe
Site::IncludeFile ( 'classes/Subscribe/Subscribe.class.inc' );
Site::IncludeFile ( 'classes/Subscribe/SURelation.class.inc' );
// this is a optional
//Site::IncludeFile ( 'classes/Acount/SiteUser/SiteUserList.class.inc' );

// create site object
$site = new Site();
$site->LoadConfiguration();

// load sbscribe configuration
$filePath = '../config/subscribe.xml';

if(!file_exists($filePath))
{
	die("The file $filePath does not exist!");
}

$xmlDoc = simplexml_load_file($filePath); 

$userCount = (int) (isset($xmlDoc->usersPerTik)) ? $xmlDoc->usersPerTik : 5;
$refreshTime = (int) (isset($xmlDoc->refreshTime)) ? $xmlDoc->refreshTime : 5;
$subscribeEmail = (string) $xmlDoc->email;

//////////////////////////////////////////////
$subscribeID = DB::GET ('subscribeID');

$subscribe = new Subscribe ( $subscribeID );
$subscribeName = $subscribe->GetName ();

// get subscribe content
// select language
$contentID = $subscribe->GetContentID ();
$content = new Content ( $contentID );

$defLang = $site->GetDefaultLanguage();

$article = $content->GetContent ($defLang, true);
$title = $content->GetName($defLang);

// get subscribe users (limit 100 user)
$users = array ();

$lastUserID = $subscribe->GetLastUserID ();

$subUsers = new SURelation ( $subscribeID );
$users = $subUsers->GetRemainingUsers ( $userCount );

// send emails
if(count($users) > 0)
{
	foreach ( $users as $user )
	{
		$currUser = $article . str_replace('@@code@@', $user['code'], $unSubsMess); 
		$currUser = str_replace('@@name@@', $user['name'], $currUser);
		
		$site->Mail($user ['email'], $title, $currUser, $subscribeEmail);
		$lastUserID = $user ['ID'];
	}
}

$subscribe->SetLastUserID ( $lastUserID );
// calculate procent
$newSubUsers = new SURelation ( $subscribeID );
$procent = $newSubUsers->GetSubscribeProcent ();
// condition for refresh
$isReffresh = ($procent < 100);

if($procent >= 100)
{
	$subscribe->SetContentID(0);
	$subscribe->SetStatus(0);
	$subscribe->SetLastUserID(0);
}
?>
<html>
<head>

<?php
if ($isReffresh)
{
	print ( "<title>Subscribe '$subscribeName' - $procent%</title>" );
	print ( "<meta http-equiv='refresh' content='{$refreshTime}'>" );
}
else
{
	print ( '<title>Subscribe Complite</title>' );
}
?>

</head>
<body>
<?php
if ($isReffresh)
{
	print ( "Subscribe '$subscribeName' - $procent%" );
}
else
{
	print ( 'Subscribe Complite' );
}
?>
</body>
</html>