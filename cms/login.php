<?php
include_once 'classes/FileChecker/ExceptionMissFile.class.inc';
include_once ('classes/Common/Site.class.inc');

Site::LoadStatics ();

include_once 'classes/Common/DB.class.inc';
include_once ('files/db.inc');
include_once ('classes/Common/CMS.class.inc');
include_once ('classes/Common/functions.inc');
include_once ('classes/Common/NamedRecord.class.inc');
include_once ('classes/UserManagement/UserManagement.class.inc');

$username = DB::POST ( 'username' );
$password = DB::POST ( 'password' );

if (! ($username && $username))
{
	if (isset ( $_GET ['action'] ) && $_GET ['action'] == 'logout')
	{
		$user = UserManagement::GetCurrentUser ();
		if ($user->GetID ())
		{
			$res = $user->Logout ();
		}
		
		// delete cookies
		setcookie ( 'catID', '', time () - 3600 );
		setcookie ( 'menuID', '', time () - 3600 );
		setcookie ( 'contID', '', time () - 3600 );
		setcookie ( 'lang', '', time () - 3600 );
		setcookie ( 'section', '', time () - 3600 );
	}
}

$um = new UserManagement ( );

$msg = '';

if ($um->CheckLog ())
{
	header ( "Location: index.php" );
}
else
{
	if ($username && $password)
	{
		$log = $um->Login ( $username, $password );
		
		if ($log)
		{
			header ( "Location: index.php" );
		}
	}
}

// send email
if (($email = DB::POST ( 'forgotpass' )))
{
	$res = $um->SetPassSendEmail ( $email );
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Siteius Content Management System</title>
<link rel="shortcut icon" href="favicon.ico">


<link href="css/ms.css" rel="stylesheet" type="text/css" />
<script src="js/mainJS.js" type="text/javascript"></script>

<script src="js/validation.js" type="text/javascript"></script>

<script src="js/collapsiblePanel.js" type="text/javascript"></script>
<script src="js/tabbedPanels.js" type="text/javascript"></script>
<link href="css/collapsiblePanel.css" rel="stylesheet" type="text/css" />
<link href="css/tabbedPanels.css" rel="stylesheet" type="text/css" />

<script type="text/javascript">

/***********************************************
* Open select links in new window script- � Dynamic Drive DHTML code library (www.dynamicdrive.com)
* This notice MUST stay intact for legal use
* Visit Dynamic Drive at http://www.dynamicdrive.com/ for full source code
***********************************************/

/*****************USAGE NOTES:*****************
Add: <form name="targetmain"><input type="checkbox" name="targetnew" checked onClick="applywindow(targetlinks)">Open designated links in new window</form>

anywhere on your page to automatically switch the script to manual mode, whereby only checking a checkbox will cause designated links to open in a new window. Customize the form as desired, but preserve key information such as the name attributes and onClick command. Remove form to switch back to auto mode.
***********************************************/

var linktarget="_blank" //Specify link target added to links when set to open in new window

var formcache=document.targetmain

function applywindow(){
if (typeof targetlinks=="undefined") return
if (!formcache || (formcache && formcache.targetnew.checked)){
for (i=0; i<=(targetlinks.length-1); i++)
targetlinks[i].target=linktarget
}
else
for (i=0; i<=(targetlinks.length-1); i++)
targetlinks[i].target=""
}


function collectElementbyClass(){
if (!document.all && !document.getElementById) return
var linksarray=new Array()
var inc=0
var alltags=document.all? document.all : document.getElementsByTagName("*")
for (i=0; i<alltags.length; i++){
if (alltags[i].className=="nwindow")
linksarray[inc++]=alltags[i]
if (alltags[i].className=="nwindowcontainer"){
var alldivlinks=document.all? alltags[i].all.tags("A") : alltags[i].getElementsByTagName("A")
for (t=0; t<alldivlinks.length; t++)
linksarray[inc++]=alldivlinks[t]
}
}
return linksarray
}
if (formcache && formcache.targetnew.checked) //overcome IE bug, manually check checkbox that has "checked" attribute
setTimeout("document.targetmain.targetnew.checked=true",100)
var targetlinks=collectElementbyClass()
applywindow()

// my script vor validation of forms
function LogValidation()
  {
        username = TextFildValid('username', null, 'usernameHTML');
        password = TextFildValid('password', null, 'passwordHTML');
        
        res = (username && password);
        SerErrorText(res);
        return res;
  }

function is_Mail()
{
	res = TextFildValid('forgotpass', 'email');
    SerErrorText(res);
    return res;
}
</script>
</head>
<body>
<table width="100%" border="0" cellspacing="0" cellpadding="0">
	<!--header-->
	<tr>
		<td class="headerBox">
			<?php
			$site = new Site ( );
			$site->LoadConfiguration ();
			$siteURL = $site->GetSiteURL ();
			?>
			<div class="adminMenuBox"><a href="<?php echo $siteURL?>">Launch the Site</a></div>
			<div class="versionBox">
				<div class="gradient1">
					
				</div>
			</div>
		</td>
	</tr>
	<!--end header-->
	<!--content-->
	<tr>
		<td align="center">
			<div style="padding: 20px 20px 0px 20px; color: #FF0000" align="center" id="errorDiv">
				<?php
				if ( $msg != null) print ( $msg );
				?>
				&nbsp;
			</div>
			<div style="padding: 40px 0px 60px 0px; width: 520px" align="left">
				<div class="someWTitleBox">
					Use a valid <strong>screen name</strong>and <strong>password</strong> to gain access to the Content Management System.
				</div>
				<table width="100%" border="0" cellspacing="0" cellpadding="0">
					<tr>
						<td width="250" bgcolor="#F9F9F9" style="border-top: 1px solid #777777; border-left: 1px solid #777777; border-right: 1px solid #777777">
							<div style="padding: 5px; width: 235px;" align="right">
								<form action="" method="post" name="log" style="margin-bottom: 0px"	onsubmit="return LogValidation();">
									<div style="padding: 5px">
										<label><strong>Screen name:</strong></label>
										<input name="username" id="username" type="text" />
									</div>
									<div style="padding: 5px">
										<label><strong>Password:</strong></label>
										<input name="password" id="password" type="password" />
									</div>
									<div style="padding: 5px; padding-left: 63px">
										<input value="Sign In" type="submit">
									</div>
								</form>
							</div>
						</td>
						<td style="border-top: 1px solid #777777; border-right: 1px solid #777777" align="center"><img src="img/logo.gif"></td>
					</tr>
				</table>
				<div id="CollapsiblePanel1" class="CollapsiblePanel">
					<div class="CollapsiblePanelTab" tabindex="0">Forget your password?</div>
					<div class="CollapsiblePanelContent">
						<div class="CollapsiblePanelContentPD">
							<form action="" method="post" name="forgpt" style="margin-bottom: 0px" onsubmit="return is_Mail();">
								<div style="padding: 5px">Please enter your e-mail address. You will receive a new password via e-mail.</div>
								<div style="padding: 5px">E-mail: <input name="forgotpass" id="forgotpass" type="text" /> <input name="" type="submit" value="Done" /></div>
							</form>
						</div>
					</div>
				</div>
			</div>
		</td>
	</tr>
	<!--end content-->
	<!--footer-->
	<tr>
		<td style="padding: 10px 0px 0px 0px;" align="center"><?php CMS::DisplayCopyRight ();?></td>
	</tr>
	<!--endfooter-->
</table>
<script type="text/javascript">
<!--
var CollapsiblePanel1 = new Spry.Widget.CollapsiblePanel("CollapsiblePanel1", {contentIsOpen:false});
//-->
</script>
</body>
</html>
