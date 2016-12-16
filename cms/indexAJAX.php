<?php
    include_once("classes/Common/CMS.class.inc");
    
    // creating new CMS object
    $CMS = new CMS();
    
    // chaking login and logout
    $res = $CMS->CheckLog(2);
    
    if(!$res)
    {
    	header("Location: login.php");
    }
    
    $CMS->Includes();
                                                                                                      
    //$res = $CMS->DoAction();

	$acion = $_GET['acion'];
	$lang = $_GET['lang'];
	
	
	switch ($acion)
	{
		case 'getContent':
			$contID = $_GET['contID'];
			$content = new Content($contID);
			$aricle = $content->GetContent($lang, false);
			print($aricle);
			break;			
	}
	
?>