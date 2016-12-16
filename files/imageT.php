<?php
	session_start();

	// this script can be used to dissplay random string+number on image
	header("Content-type: image/png");
	$string = $_SESSION['rkey'];
	$im = imagecreate(100, 20); //imagecreatefrompng("images/button1.png");
	
	$with = imagecolorallocate($im, 255, 255, 255);
	imagefill($im, 100, 20, $with);
	
	
	// draw lines
	for($index = 0; $index < 5; $index++){
		$x1 = rand(2, 40);
		$x2 = rand(60, 100);
		$y1 = rand(1 , 20);
		$y2 = rand(1, 20);
		
		$newcolor = imagecolorallocate($im,rand(128, 255), rand(128, 255), 255);
		Imageline ($im, $x1, $y1, $x2, $y2, $newcolor);
	}
	
	// print text
	$orange = imagecolorallocate($im, 0, 0, 0);
	$px     = (imagesx($im) - 7.5 * strlen($string)) / 2;
	for ($index = 0; $index < strlen($string); $index++)
	{
		
		$font = rand(3, 5);
		$px = 10+10*$index;
		imagestring($im, $font, $px, 3, $string[$index], $orange);
	}
	imagepng($im);
	imagedestroy($im);
?> 