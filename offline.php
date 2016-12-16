<?php
if ( isset($_POST["password"]) && $_POST["password"]=="01011970" )
{
	setcookie( "beta_login", "ok", NULL, "/" );
	header("location:http://decent.ctrlq.com");
	exit();
}
// get site and site URL
$site = View::GetSite ();

// get language categoryID and other parametors
$catID = 0;
$lang = "";
$params = array ();

$site->GetRequestParams ( $lang, $catID, $params );

$content = new Content(1);
$article = $content->GetContent($lang);
$articleTitle = $content->GetName($lang);

//	get currnet category
$catDraw = new CategoryView ( $catID, true );

header('Content-Type: text/html; charset=utf-8');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<?php
$catDraw->DrawHeaders ( $name );
?>
<meta name="classification" content="e-Learning" />
<meta name="copyright" content="&copy; 2016 >DECENT" />
<meta http-equiv="reply-to" content="info@ctrlq.com" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="resource-type" content="document" />
<meta name="revisit-after" content="1 Day" />
<meta name="distribution" content="GLOBAL" />
<meta name="rating" content="General" />
<meta name="doc-type" content="Web Page" />
<meta name="doc-class" content="Completed" />
<meta name="doc-rights" content="Copyrighted Work" />
<link rel="shortcut icon" href="favicon.ico" />
<link href="css/main.css" rel="stylesheet" type="text/css" />

</head>
<body>

<!--container-->
<div class="container" style="margin:150px 0 0 100px;">
	<!--content-->
    <div class="content">
    	<?php
    	echo "<h1>".$articleTitle."</h1>";
		echo $article;
		?>
		
		<!--authorization-->
		<form action="" method="post">
		<fieldset style="padding:0;">
			<dl>
				<dt style="margin:0 0 20px ;"><input name="password" id="password" type="password" style="width:200px;" /></dt>
				<dd style="margin:0;"><input type="submit" name="submit" value="Enter"></dd>
			</dl>
		</fieldset>
		</form>
		
		<!--//authorization-->
    </div>
</div>
</body>
</html>