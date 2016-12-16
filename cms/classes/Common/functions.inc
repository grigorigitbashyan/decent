<?php
/**
 * Return random code
 *
 * @param int $length
 * @return string
 */
function GetRandCode($length, $onlyNumbers = false) {
	$randStr = "";
	for($index = 0; $index < $length; $index ++) {
		$dc = rand ( 1, 10 );
		
		if ($dc % 2 == 0 || $onlyNumbers) {
			$randStr .= rand ( 1, 9 );
		} else {
			$randStr .= chr ( rand ( 65, 90 ) );
		}
	}
	
	return $randStr;
}

/**
 * @param $errno
 * @param $errstr
 * @param $errfile
 * @param $errline
 * @return string
 */
function ErrorHandler($errno, $errstr, $errfile, $errline) {
	// Any error must be written to the error log file, with time , user and error properties.
	// Also it can be mailed to some email which must be defined in error.xml

	$errorHandler = CMS::GetErrorHandler();
	$errorHandler->Add($errno, $errstr, $errfile, $errline);
	return;
	switch ($errno) {
		case E_USER_ERROR :
			echo "<b>My ERROR</b> [$errno] $errstr<br />\n";
			echo "  Fatal error on line $errline in file $errfile";
			echo ", PHP " . PHP_VERSION . " (" . PHP_OS . ")<br />\n";
			echo "Aborting...<br />\n";
			//exit ( 1 );
			break;
		
		case E_USER_WARNING :
			echo "<b>My WARNING</b> [$errno] $errstr<br />\n";
			break;
		
		case E_USER_NOTICE :
			echo "<b>My NOTICE</b> [$errno] $errstr<br />\n";
			break;
		
		default :
			echo "Unknown error type: [$errno] $errstr<br />\n";
			break;
	}
	
	/* Don't execute PHP internal error handler */
	return true;
}

function CutText($text, $sNum=100)
{
	if ( is_string($text) )
	{
		$text = str_replace("</p>", "</p> ", $text);
		$text = strip_tags($text);
		if ( strlen($text)>$sNum )
		{
			$text = substr($text, 0, strpos($text, " ", $sNum))."...";
		}
	}
	
	return $text;
}

function ComputeDate($pDate)
{
	global $monthArr;
	
	$dateDiff = time() - strtotime($pDate);
	if ( $dateDiff<61 )
	{
		$showDate = "just now";
	}
	elseif ( $dateDiff>=86400 )
	{
		$pDate = explode(" ", $pDate);
		$pTime = substr($pDate[1], 0, 5);
		
		$showDate = date("d M, Y", strtotime($pDate[0]));
		/*$pDate = explode("-", $pDate[0]);
		if ( $pDate[0]==date("Y") )
		{
			$showDate = $pDate[2]." ".$monthArr[$pDate[1]].", ".$pTime;
		}
		else 
		{
			$showDate = $pDate[2]." ".$monthArr[$pDate[1]]." ".$pDate[0].", ".$pTime;
		}*/
	}
	else 
	{
		$passedMins = ceil($dateDiff/60);
		if ( $passedMins>59 )
		{
			$passedHours = ceil($passedMins/60) - 1;
			$passedMins = $passedMins - $passedHours*60;
			
			$showDate = $passedHours."h. ".$passedMins."m.";
		}
		else 
		{
			$showDate = $passedMins."minutes";
		}
	}
	
	return $showDate;
}
?>