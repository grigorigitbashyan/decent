<?php
	include_once 'db.inc';
	
	include_once('../cms/classes/Common/Site.class.inc');
		
	Site::LoadStatics ();
	
	include_once ('../cms/classes/Common/NamedRecord.class.inc');
	include_once('../cms/classes/Account/SiteUser/SiteUserView.class.inc');
	
	// get site and site URL
	$site = new Site();
	$siteURL = $site->GetSiteURL();
	
	$code = $_GET['code'];
	
	if(strlen($code) < 17)
	{
		exit();
	}
	// site user /////////////////////////////////
	$siteUser = new SiteUserView();
	$isLogin = $siteUser->IsLoginUser(substr($code, 0, 16));

	if(!$isLogin)
	{
		print("Please log in!");
		exit();
	}
	
	////////////////////////////////////////////////////////////
	// get file 
	$fileID = substr($code, 16);

	include_once '../cms/classes/Folder/Folder.class.inc';
	$fileObj = new Folder($fileID);
	
	
	$file = $fileObj->GetFolderPath();
	$file = '../' . $file;
    /////////////////////////////////////////////////
	
	
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
      case "inc":
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