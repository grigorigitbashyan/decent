<?php
	// version 0.1
	
	// access only logined users
	include_once("../classes/Common/Site.class.inc");
	Site::LoadStatics ();

	include_once '../classes/Common/Record.class.inc';
	include_once '../classes/Common/NamedRecord.class.inc';
	include_once("../classes/UserManagement/UserManagement.class.inc");
	include_once("db.inc");
	
	$userID = $_GET['uid'];
	
	$login = UserManagement::IsUserLogin($userID);
    
    if(!$login)
    {
    	print('Please Log in!');
    	
    	exit();
    }
    
    /////////////////////////////////////////////////
	$file = $_GET['p'];
	
	//First, see if the file exists
    if (!is_file($file)) { die("<b>404 File not found!</b>"); }

    //Gather relevent info about file
    $len = filesize($file);
    $filename = basename($file);
    $file_extension = strtolower(substr(strrchr($filename,"."),1));

    //This will set the Content-Type to the appropriate setting for the file
    switch( $file_extension ) {
      case "pdf": $ctype="application/pdf"; break;
      case "exe": $ctype="application/octet-stream"; break;
      case "zip": $ctype="application/zip"; break;
      case "doc": $ctype="application/msword"; break;
      case "xls": $ctype="application/vnd.ms-excel"; break;
      case "ppt": $ctype="application/vnd.ms-powerpoint"; break;
      case "gif": $ctype="image/gif"; break;
      case "png": $ctype="image/png"; break;
      case "jpeg":
      case "jpg": $ctype="image/jpg"; break;
      case "mp3": $ctype="audio/mpeg"; break;
      case "wav": $ctype="audio/x-wav"; break;
      case "mpeg":
      case "mpg":
      case "mpe": $ctype="video/mpeg"; break;
      case "mov": $ctype="video/quicktime"; break;
      case "avi": $ctype="video/x-msvideo"; break;

      //The following are for extensions that shouldn't be downloaded (sensitive stuff, like php files)
      case "php":
      case "htm":
      case "html":
      case "txt": die("<b>Cannot be used for ". $file_extension ." files!</b>"); break;

      default: $ctype="application/force-download";
    }

    //Begin writing headers
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Cache-Control: public");
    header("Content-Description: File Transfer");
   
    //Use the switch-generated Content-Type
    header("Content-Type: $ctype");

    //Force the download
    $header="Content-Disposition: attachment; filename=".$filename.";";
    header($header );
    header("Content-Transfer-Encoding: binary");
    header("Content-Length: ".$len);
    @readfile($file);
    exit;
?>