<?php
/** 
 * This class for image control, change image size, chenge size in box
 *
 * @version 1.0
 * @package Main
 */
class Image
{
	// change image size
	function ChangeImageSize($filename, $newwidth, $newheight, $newFileName = null)
	{
		if (! $newFileName)
		{
			$newFileName = $filename;
		}
		
		$path_parts = pathinfo ( $filename );
		
		$filetype = $path_parts ['extension'];
		$filetype = strtolower ( $filetype );
		
		list ( $width, $height ) = getimagesize ( $filename );
		
		// Load
		

		$thumb = imagecreatetruecolor ( $newwidth, $newheight );
		
		$source = NULL;
		
		switch ($filetype)
		{
			case "gif" :
				$source = imagecreatefromgif ( $filename );
				break;
			case "jpeg" :
			case "jpg" :
				$source = imagecreatefromjpeg ( $filename );
				break;
			case "png" :
				$source = imagecreatefrompng ( $filename );
				break;
			default :
				$source = imagecreatefromgif ( $filename );
				break;
		}
		
		// Resize
		imagecopyresampled ( $thumb, $source, 0, 0, 0, 0, $newwidth, $newheight, $width, $height );
		
		// Output
		switch ($filetype)
		{
			case "gif" :
				imagegif ( $thumb, $newFileName );
				break;
			case "jpeg" :
			case "jpg" :
				imagejpeg ( $thumb, $newFileName, 90 );
				break;
			case "png" :
				imagepng ( $thumb, $newFileName );
				break;
		}
		
		return true;
	}
	
	////
	function ChangeSizeInBox($filename, $max_width, $max_height = null, $newFileName = null)
	{
		if (! $newFileName)
		{
			$newFileName = $filename;
		}
		
		if (! file_exists ( $filename ))
		{
			//return false;
		}
		
		list ( $width, $height ) = getimagesize ( $filename );
		
		$proc_width = $width / $max_width;
		
		$proc = $proc_width;
		
		if ($max_height)
		{
			$proc_height = $height / $max_height;
			$proc = ($proc_width > $proc_height) ? $proc_width : $proc_height;
		}
		
		$newwidth = floor ( $width / $proc );
		$newheight = floor ( $height / $proc );
		
		if ($proc >= 1)
		{
			$this->ChangeImageSize ( $filename, $newwidth, $newheight, $newFileName );
		}
		else
		{
			$this->ChangeImageSize ( $filename, $width, $height, $newFileName );
		}
		
		return true;
	}
	
	function GetImageSourceInBox($filename, $max_width, $max_height, &$imgSource)
	{
		
		list ( $width, $height ) = getimagesize ( $filename );
		
		$proc_width = $width / $max_width;
		$proc_height = $height / $max_height;
		
		$proc = ($proc_width > $proc_height) ? $proc_width : $proc_height;
		
		$newwidth = floor ( $width / $proc );
		$newheight = floor ( $height / $proc );
		
		if ($proc > 1)
		{
			$this->GetImageSource ( $filename, $newwidth, $newheight, $imgSource );
		}
	}
	
	// change image size
	function GetImageSource($filename, $newwidth, $newheight, $imgSource)
	{
		$newwidth = intval ( $newwidth );
		$newheight = intval ( $newheight );
		
		$path_parts = pathinfo ( $filename );
		
		$filetype = $path_parts ['extension'];
		$filetype = strtolower ( $filetype );
		
		list ( $width, $height ) = getimagesize ( $filename );
		
		// Load
		

		$source = NULL;
		
		switch ($filetype)
		{
			case "gif" :
				$source = imagecreatefromgif ( $filename );
				break;
			case "jpeg" :
			case "jpg" :
				$source = imagecreatefromjpeg ( $filename );
				break;
			case "png" :
				$source = imagecreatefrompng ( $filename );
				break;
			default :
				$source = imagecreatefrompng ( $filename );
				break;
		}
		
		$thumb = @imagecreatetruecolor ( $newwidth, $newheight );
		// Resize
		imagecopyresampled ( $thumb, $source, 0, 0, 0, 0, $newwidth, $newheight, $width, $height );
		
		$imgSource = $thumb;
		return $imgSource;
	}
}
?>