<?php
// version 0.1
$max_width = $_GET['w'];
$max_height = $_GET['h'];
$filename = $_GET['p'];

$filename = str_replace(" ", "%20", $filename);
if ( strpos($filename, "media") ) {
	
	$filename = "../../".substr($filename, strpos($filename, "media"));
}
// Content type
$path_parts = pathinfo($filename);

$ext = strtolower($path_parts['extension']);

// if image functions are avalible then change size of image and display
if (function_exists('imagecreatetruecolor') && function_exists('imagecopyresampled')) 
{
	switch ($ext)
	{
		case "jpeg":
		case "jpg":
		case "gif":
		case "png":
			
			header("Content-type: image/$ext");
		
			// Get new dimensions
			list($width, $height) = getimagesize($filename);
			
			$proc_width = $max_width / $width;
			$proc_height = $max_height / $height;
		
			$percent = ($proc_width < $proc_height) ? $proc_width : $proc_height;
				
			$new_width = $width * $percent;
			$new_height = $height * $percent;
			
			// Resample
			$image_p = imagecreatetruecolor($new_width, $new_height);
			
			if($ext)
			{
				switch($ext)
				{
					case "gif":
						$image = imagecreatefromgif($filename);
						break;
					case "jpeg":
					case "jpg":
						$image = imagecreatefromjpeg($filename);
						break;
					case "png":
						imagealphablending($image_p, false);
						
						$image = imagecreatefrompng($filename);
						imagealphablending($image, false);
						imagesavealpha($image, true);
						break;
				}
				
			}
			
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $new_width, $new_height, $width, $height);
			
			// Output
			if($ext)
			{
				switch($ext)
				{
					case "gif":
						imagegif($image_p);
						break;
					case "jpeg":
					case "jpg":
						imagejpeg($image_p, null, 80);
						break;
					case "png":
						imagealphablending($image_p, false);
						imagesavealpha($image_p, true);
						imagepng($image_p);
						break;
				}
			}
		break;
	}
}
else
{
	header("Location: $filename");
}
?>