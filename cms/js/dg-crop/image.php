<?
$dstWidth = intval($_POST["width"]);
$dstHeight = intval($_POST["height"]);
$dstTop = intval($_POST["top"]);
$dstLeft = intval($_POST["left"]);

// Original image
$filename = 'sex.jpg';
 
// Get dimensions of the original image
list($current_width, $current_height) = getimagesize($filename);
 
// The x and y coordinates on the original image where we
// will begin cropping the image
$left = $dstLeft;
$top = $dstTop;
 
// This will be the final size of the image (e.g. how many pixels
// left and down we will be going)
$crop_width = $dstWidth;
$crop_height = $dstHeight;
 
// Resample the image
$canvas = imagecreatetruecolor($crop_width, $crop_height);
$current_image = imagecreatefromjpeg($filename);
imagecopy($canvas, $current_image, 0, 0, $left, $top, $current_width, $current_height);
imagejpeg($canvas, "aaa.jpg", 100);
?>