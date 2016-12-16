<?php
error_reporting(0);
include_once ('classes/Common/CMS.class.inc');

// creating new CMS object
$CMS = new CMS ( );

// chaking login and logout
$res = $CMS->CheckLog ();

if (! $res)
{
	header ( "Location: login.php" );
}

// include controller
$cmsController = new CMSUserController ( );
ControllerBase::SetController ( $cmsController );

// change ftp root
FolderXP::Chdir('cms/');

//////////////////////////////
$CMS->IncludeModuleCMSfile ();

$res = $CMS->DoAction ();

// try ajax
include_once 'ajax/xajax_core/xajaxAIO.inc.php';

$xajax = new xajax ( );
$xajax->registerFunction ( "callAjax" );

function callAjax($arg, $FieldID, $destination = "innerHTML", $emptyIDs = null)
{
	// do some stuff based on $arg like query data from a database and
	// put it into a variable like $newContent
	global $CMS;
	$newContent = $CMS->GetAjax ( $arg );
	
	// Instantiate the xajaxResponse object
	$objResponse = new xajaxResponse ( );
	
	// add a command to the response to assign the innerHTML attribute of
	// the element with id="SomeElementId" to whatever the new content is
	$objResponse->assign ( $FieldID, $destination, $newContent );
	
	// if the content hass been changed and there are some HTML tags to be empty, then set null them
	

	if ($emptyIDs)
	{
		if (is_array ( $emptyIDs ))
		{
			foreach ( $emptyIDs as $emptyID )
			{
				$objResponse->assign ( $emptyID, $destination, '' );
			}
		}
		else
		{
			$objResponse->assign ( $emptyIDs, $destination, '' );
		}
	}
	//return the  xajaxResponse object
	return $objResponse;
}

$xajax->processRequest ();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Siteius Content Management System</title>
<link rel="shortcut icon" href="favicon.ico" />
<script src="js/mainJS.js" type="text/javascript"></script>
<script src="js/collapsiblePanel.js" type="text/javascript"></script>
<script src="js/tabbedPanels.js" type="text/javascript"></script>
<script src="js/validation.js" type="text/javascript"></script>
<script src="js/addAjax.js" type="text/javascript"></script>
<!-- Calendar -->
<script src="js/calendar/src/js/jscal2.js" type="text/javascript"></script>
<script src="js/calendar/src/js/lang/en.js" type="text/javascript"></script>
<!-- end Calindar -->
<?php
$CMS->IncludeJavaScripts ();

// include ajax
$xajax->printJavascript ();
?>
<!--calendar-->
<link rel="stylesheet" type="text/css" href="js/calendar/src/css/jscal2.css" />
<link rel="stylesheet" type="text/css" href="js/calendar/src/css/border-radius.css" />
<link rel="stylesheet" type="text/css" href="js/calendar/src/css/steel/steel.css" />
<!--main-->
<link href="css/ms.css" rel="stylesheet" type="text/css" />
<link href="css/collapsiblePanel.css" rel="stylesheet" type="text/css" />
<link href="css/tabbedPanels.css" rel="stylesheet" type="text/css" />
</head>
<body>
<table width="100%" border="0" cellspacing="0" cellpadding="0">
	<!--row 1 header-->
	<tr>
		<td class="logoBox"><img src="img/logo.gif" alt=" " /></td>
		<td class="headerBox">
			<div class="adminMenuBox">
				<?php
				$CMS->DisplayCommonMenu ();
				?>
			</div>
			<div class="versionBox">
				<div class="gradient1">
					
				</div>
			</div>
		</td>
	</tr>
	<!--end row 1 header-->
	<!--row 2-->
	<tr>
		<!--language menu-->
		<td class="languageBox">
		   	<?php
			$CMS->DisplayLanguages ();
			?>
    	</td>
		<!--end language menu-->
		<!--main menu-->
		<td class="mainMenuBox">
			<div class="mainMenu">
            	<?php
				$CMS->DisplayFeatureMenu ();
				?>
        	</div>
		</td>
		<!--end main menu-->
	</tr>
	<!--end row 2-->
	<!--row 3-->
	<tr>
		<!--navigation-->
		<td class="navigationBox">
    		<?php
			$CMS->DisplayLeftFeatures ();
			?>
    	</td>
		<!--end navigation-->
		<!--content-->
		<td class="contentBox">
    		<?php
			$CMS->DisplayMainFeatures ();
			?>
    	 </td>
		<!--end content-->
	</tr>
	<!--end row 3-->
	<!--row 4 footer-->
	<tr>
		<td height="32">&nbsp;</td>
		<td style="padding: 0px 15px 0px 15px">
    		<?php
			$CMS->DisplayCopyRight ();
			?>
    	</td>
	</tr>
	<!--end row 4 footer-->
</table>
</body>
</html>