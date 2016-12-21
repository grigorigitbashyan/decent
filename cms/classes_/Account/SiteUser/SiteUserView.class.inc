<?php
$includes = array(
    'classes/Common/DB.class.inc',
    'classes/Account/SiteUser/Site_user.class.inc',
    'classes/Account/RegOption/RegOption.class.inc',
    'classes/ItemOptions/OptionsdataList.class.inc',
    'classes/Product/pr_main/Pr_main.class.inc',
    'classes/Product/pr_main/Pr_mainList.class.inc',
    'classes/Product/pr_main/Pr_options.class.inc',
    'classes/Product/pr_int/Pr_int.class.inc',
    'classes/Product/pr_double/Pr_double.class.inc',
    'classes/Product/pr_char/Pr_char.class.inc',
    'classes/Product/pr_picture/Pr_picture.class.inc',
    'classes/Product/pr_video/Pr_video.class.inc',
    'classes/Common/Image.class.inc',
    'classes/Folder/Folder.class.inc',
    'classes/Folder/FolderStructure.class.inc',
    'classes/Category/Category.class.inc'
);

Site::IncludeFiles($includes);
Site::IncludeFile('classes/Session/Session.class.inc');
////////////////////////////////////////////////////////////
global $session;

/**
 * Enter description here...
 *
 * @package Account
 */
class SiteUserView extends View
{
    /**
     * Enter description here...
     *
     * @var Site_user
     */
    protected $m_user;

    /**
     * Enter description here...
     *
     * @var int
     */
    protected $m_actStatus;

    /**
     * Enter description here...
     *
     * @var bool
     */
    protected $m_ar;

    /**
     * Enter description here...
     *
     * @var string|int
     */
    protected $m_message;

    /**
     * Registration suppory email, all email from this
     *
     * @var string
     */
    protected $m_fromEmail;

    protected $m_activatorName;

    /**
     * Enter description here...
     *
     * @param int $userID
     */
    function __construct($params = null)
    {
        parent::__construct();

        $userID = null;
        // if site userhas already login
        $logCode = null;
        // site user log code
        $sulc = $this->m_site->GetValue('sulc');

        if ( $sulc && strlen($sulc)==16 )
        {
            $sql = "SELECT * FROM `site_user` WHERE `logcode` = '$sulc' ";
            $res = DB::QueryToArray($sql);
            if ( $res )
            {
                // if user is finaly activated
                if ( /*$res[0]['status']==3*/ 1 ) {
                    $userID = $res[0]['ID'];
                    $this->m_user = new Site_user ($userID);

                    // if logout
                    if ( isset($params[0]) && $params[0] == 'logout' )
                    {
                        $newCode = GetRandCode(16);
                        $this->m_user->SetLogCode($newCode);
                        $this->m_site->SaveValue('sulc', '');
                        $this->m_site->SaveValue('decentuserid', '0');
                        $userID = null;
                    }
                }
            }
        }

        // site usertrys to log in
        if ( isset($_POST['email'], $_POST['password']) )
        {
            $email = trim(DB::POST('email'));
            $password = md5(trim($_POST ['password']));

            $sql = "SELECT * FROM `site_user` WHERE `email` = '$email' AND `password` = '$password'";
            $res = DB::QueryToArray($sql);
            if ( $res )
            {
                $userID = $res [0]['ID'];
                $logCode = GetRandCode(16);
            } 
            else 
            {
				$this->m_ar = false;
				
                $resContent = new Content(23);
                $this->m_message = $resContent->GetName($this->m_displayLang);
            }
        }

        $this->m_user = new Site_user($userID);

        //	new addition
        $tempUserID = $userID;
        if ( !$tempUserID )
        {
            $tempUserID = intval($this->m_site->GetValue('decentuserid'));
            if ( !$tempUserID )
            {
                $tempUserID = GetRandCode(6, true);
            }
        }
        $this->m_site->SaveValue('decentuserid', $tempUserID, true, true);

        // after login
        if ( $logCode )
        {
            $this->m_user->SetLogCode($logCode);
            if ( isset($_POST["remember"]) && $_POST["remember"]=="on" )
            {
                $this->m_site->SaveValue('sulc', $logCode, true, true);
            } 
            else 
            {
                $this->m_site->SaveValue('sulc', $logCode);
            }

            header("location:".$this->m_site->GetSiteURL());
            exit();
        }

        // read names
        $this->ReadNameStrings('langs/', 'userForms.xml', $this->m_displayLang);

        // read configuration
        $confFilePath = $_SERVER ['DOCUMENT_ROOT'] . '/cms/config/userConfig.xml'; // Added '/qr' by Adriada

        $simpleXML = simplexml_load_file($confFilePath);

        $this->m_actStatus = ( int )$simpleXML->activationType;
        $this->m_fromEmail = ( string )$simpleXML->activatorEmail;
        $this->m_activatorName = ( string )$simpleXML->activatorName;
    }


    /**
     * Do actions
     *
     */
    function DoAction()
    {
        // $actiovationLink = $params;
        global $session;

        if (isset($_POST['siteUserAction'])) {
            switch ($_POST['siteUserAction']) {
                case 'deactivation' :
                    //delete user
                    $this->m_ar = $this->m_user->Delete();

                    // deactivation messages
                    $this->m_message = ($this->m_ar) ? 23 : 24;
                    break;
                case 'newuser' :
                    $this->ReadNameStrings('langs/', 'registrationForm.xml', $this->m_displayLang);
                    
                    // personal data
                    $fullname = trim(DB::POST('fullname'));
                    if ( $fullname=="" ) 
                    {
                        $_SESSION["FORMERROR"][] = "fullname";
                    }
                    
                    $url = trim(DB::POST('url'));
                    
                    $email = trim(DB::POST('r_email'));
                    if ( strlen($email)<6 )
                    {
                        $_SESSION["FORMERROR"][] = "email";
                    }
                    
                    $password = trim(DB::POST('r_password'));
                    if ( strlen($password)<6 )
                    {
                        $_SESSION["FORMERROR"][] = "password";
                    }
                    $cpassword = trim(DB::POST('cpassword'));
                    if ( $cpassword!==$password )
                    {
                        $_SESSION["FORMERROR"][] = "cpassword";
                    }
					
                    if ( !$fullname || strlen($email)<6 || strlen($password)<6 || $password!==$cpassword )
                    {
                        $this->m_ar = false;
                        $this->m_message = "Please fill all required fields";
                    } 
                    else 
                    {
                        $this->m_ar = true;
                        
                        // insert new user
                        $newUser = new Site_user ();
                        $this->m_ar = $newUser->Insert($fullname, $url, $email, $password, 1);
						
                        // case when email address has already exist
                        if ( $this->m_ar===null )
                        {
                            $resContent = new Content(25);
	                        $this->m_message = $resContent->GetName($this->m_displayLang);
                        } 
                        elseif ( !$this->m_ar )
                        {
	                        $this->m_message = "Oops. something went wrong. Please try again later.";
                        } 
                        else 
                        {
                            // send message
                            $activatinCode = $newUser->GetActivation_code();
                            $activationID = $newUser->GetID();

                            $siteRoot = $this->m_site->GetSiteURL();

                            // get activation path
                            $catActivation = new Category (6);
                            $actPath = $catActivation->GetPath();
                            $activationLink = $siteRoot.$actPath.$activatinCode.$activationID;
							
                            // get registraton email content
                            $regEmailCont = new Content (127);
							
                            // make activation email content
                            $activEmailTitle = $regEmailCont->GetName($this->m_displayLang);
                            $activEmailContent = $regEmailCont->GetContent($this->m_displayLang);

                            $activEmailContent = str_replace('@@name@@', $fullname, $activEmailContent);
							$activEmailContent = str_replace('@@email@@', $email, $activEmailContent);
							$activEmailContent = str_replace('@@link@@', $activationLink, $activEmailContent);
							$activEmailContent = str_replace('@@siteLink@@', $siteRoot, $activEmailContent);
							
                            // send registraion email
                            require("phpmailer/class.phpmailer.php");
							$phpMailer = new PHPMailer();
							$phpMailer->CharSet = 'UTF-8';
							$phpMailer->SetFrom($this->m_fromEmail, $this->m_activatorName);
							$phpMailer->AddReplyTo($this->m_fromEmail, $this->m_activatorName);
							$phpMailer->AddAddress($email);
							$phpMailer->Subject = $activEmailTitle;
							$phpMailer->AltBody = "To view the message, please use an HTML compatible email viewer!";
							$phpMailer->MsgHTML($activEmailContent);
							$mailRes = $phpMailer->Send();
							
                            // registration has been successful
                            $resContent = new Content(37);
	                        $this->m_message = $resContent->GetContent($this->m_displayLang);
                        }
                    }
                    break;
                case 'newuserfirst' :
                    // get parameters

                    // other parametors
                    $email = trim(DB::POST('email'));

                    $sql = "SELECT count(*) FROM `site_user` WHERE `email` = '$email'";
                    $res = DB::QueryToValue($sql);

                    // if some filed has not been filled then retusn false and display message
                    //TODO: display corsponding filed names
                    //TODO: check email validity
                    if (strlen($email) == 0) {
                        $this->m_ar = false;
                        $this->m_message = $this->GetNameString('warningAlert');

                        $_POST["email"] = "";
                    } elseif ($res) {
                        $this->m_ar = false;
                        $this->m_message = 4;

                        $_POST["email"] = "";
                    }
                    break;
                case 'sendChangePassword' :
                    $email = DB::POST('email');

                    // get user
                    $sql = "SELECT * FROM `site_user` WHERE `email` = '$email'";
                    $row = DB::QueryToRow($sql);

                    if ( $row )
                    {
                        $ID = $row['ID'];
                        $code = $row['logcode'];
                        $name = $row['name'];

                        $this->m_ar = true;

                        // get forget password link
                        $siteRoot = $this->m_site->GetSiteURL();

                        // get forget category
                        $forgetCat = new Category (9);
                        $forgetPath = $forgetCat->GetPath();

                        $contentLink = $siteRoot.$forgetPath.$code.$ID;

                        // get change password email content
                        $chngEmailCont = new Content(129);
						$chngEmailTitle = $chngEmailCont->GetName($this->m_displayLang);
						$chngEmailArticle = $chngEmailCont->GetContent($this->m_displayLang);
						
						$chngEmailArticle = str_replace('@@name@@', $name, $chngEmailArticle);
						$chngEmailArticle = str_replace('@@link@@', $contentLink, $chngEmailArticle);
						$chngEmailArticle = str_replace('@@siteLink@@', $siteRoot, $chngEmailArticle);
						
						// send registraion email
						require("phpmailer/class.phpmailer.php");
						$phpMailer = new PHPMailer();
						$phpMailer->CharSet = 'UTF-8';
						$phpMailer->SetFrom($this->m_fromEmail, $this->m_activatorName);
						$phpMailer->AddReplyTo($this->m_fromEmail, $this->m_activatorName);
						$phpMailer->AddAddress($email);
						$phpMailer->Subject = $chngEmailTitle;
						$phpMailer->AltBody = "To view the message, please use an HTML compatible email viewer!";
						$phpMailer->MsgHTML($chngEmailArticle);
						$mailRes = $phpMailer->Send();
                        if ( !$mailRes )
                        {
                        	$this->m_ar = false;
                            $this->m_message = "Oops. something went wrong. Please try again later.";
                        }
                        else 
                        {
                        	$this->m_message = '<br /><p class="font-20">We sent you instructions on <strong>'.$email.'</strong> for changing your password. Please check your inbox.</p>';
                        }
                    }
                    else
                    {
                    	$this->m_ar = false;
                    	
                        // the user has not been found
                        $resContent = new Content(34);
                        $this->m_message = $resContent->GetName($this->m_displayLang);
                    }
                    break;
                case 'recoverPassword' :
                    $rpassword = DB::POST('password');
                    $cpassword = DB::POST('cpassword');
                    $userInfo = DB::POST('ufi');

                    $userlogCode = substr($userInfo, 0, 16);
                    $userID = intval(substr($userInfo, 16));

                    if ( strcmp($rpassword, $cpassword)==0 && strlen($cpassword)>0 && (strlen($rpassword)>=6 || strlen($cpassword)>=6) )
                    {
                        // get user
                        $sql = "SELECT * FROM `site_user` WHERE `ID` = $userID AND `logcode` = '$userlogCode'";
                        $row = DB::QueryToRow($sql);
                        if ( $row )
                        {
                            $ID = $row['ID'];
                            $code = $row['activation_code'];

                            $siteUser = new Site_user($ID);
                            $this->m_ar = $siteUser->SetPassword($rpassword);
                            if ( $this->m_ar )
                            {
                            	$resContent = new Content(20);
                            }
                            else 
                            {
                            	$resContent = new Content(21);
                            }
                        } 
                        else 
                        {
                            // display change password unsuccessful content
                            $resContent = new Content(21);
                        }
                    } 
                    else 
                    {
                        // display change password unsuccessful content
                        $resContent = new Content(21);
                    }
                    
                    $this->m_message = $resContent->GetContent($this->m_displayLang);
                    break;
                case 'changeinfo' :
                    $checked = true;

                    // personal data
                    $fullName = trim(DB::POST('fullName'));
                    if ($fullName == "") {
                        $_SESSION["FORMERROR"][] = "fullName";
                        $checked = false;
                    }

                    $nickName = trim(DB::POST('nickName'));
                    if ($nickName != "") {
                        $nickNameCheck = $this->m_user->CheckNickName($nickName);
                        if (!$nickNameCheck) {
                            $_SESSION["FORMERROR"][] = "nickName";
                            $checked = false;
                        }
                    }

                    $phone = trim(DB::POST('phone'));
                    if ($phone == "") {
                        $_SESSION["FORMERROR"][] = "phone";
                        $checked = false;
                    }

                    $gender = trim(DB::POST('gender'));
                    if ($gender == 100) {
                        $gender = "";
                    }

                    $password = trim(DB::POST('rpassword'));
                    if (strlen($password) > 0) {
                        if (strlen($password) < 6) {
                            $_SESSION["FORMERROR"][] = "rpassword";
                            $checked = false;
                        }

                        $cpassword = trim(DB::POST('cpassword'));
                        if ($cpassword !== $password) {
                            $_SESSION["FORMERROR"][] = "cpassword";
                            $checked = false;
                        }
                    }
                    $how_to_show = intval(DB::POST('how_to_show'));

                    if ($checked) {
                        $this->m_user->SetFirstName($fullName);
                        $this->m_user->SetNickName($nickName);
                        $this->m_user->SetPhone($phone);
                        $this->m_user->SetGender($gender);
                        if (strlen($password) > 0) {
                            $this->m_user->SetPassword($password);
                        }
                        $this->m_user->SetSiteNameOpt($how_to_show, $firstName, $lastName, $nickName);

                        $picture = $_FILES["image"];
                        if ($picture["size"] > 0) {
                            $path_parts = pathinfo($picture['name']);
                            $ext = $path_parts ['extension'];
                            $ext = strtolower($ext);

                            $imageName = $this->m_user->GetRandom() . "." . $ext;
                            $imageName_400 = "400_" . $imageName;
                            $imageName_120 = "120_" . $imageName;

                            $types = array('jpg', 'jpeg', 'png');
                            $res = array_search($ext, $types);
                            if (is_int($res)) {
                                if (copy($picture['tmp_name'], "media/images/users/" . $imageName) && copy($picture['tmp_name'], "media/images/users/" . $imageName_120) && copy($picture['tmp_name'], "media/images/users/" . $imageName_400)) {
                                    $this->m_user->SetImage("media/images/users/" . $imageName);
                                    $this->m_user->SetImage400("media/images/users/" . $imageName_400);
                                    $this->m_user->SetImage120("media/images/users/" . $imageName_120);

                                    //	resize image
                                    $filename = "media/images/users/" . $imageName_400;
                                    $path_parts = pathinfo($filename);
                                    $filetype = $path_parts ['extension'];
                                    $filetype = strtolower($filetype);

                                    $DPW = 400;
                                    $DPH = 400;
                                    $im = new Image ();
                                    $im->ChangeSizeInBox($filename, 1280, 1280, $filename);

                                    list ($width, $height) = getimagesize($filename);
                                    if ($width > $height) {
                                        $width = round($width * $DPH / $height);
                                        $height = $DPH;
                                        $ch = 0;
                                        $cW = ceil(($width - $DPW) / 2);
                                    } else {
                                        $height = round($height * $DPW / $width);
                                        $width = $DPW;
                                        $cW = 0;
                                        $ch = 0;
                                    }
                                    $im->ChangeImageSize($filename, $width, $height, $filename);

                                    $new = imagecreatetruecolor($DPW, $DPH);

                                    switch ($filetype) {
                                        case 'jpg':
                                        case 'jpeg':
                                            $source = imagecreatefromjpeg($filename);
                                            break;
                                        case 'png':
                                            imagealphablending($new, false);

                                            $source = imagecreatefrompng($filename);
                                            imagealphablending($source, false);
                                            imagesavealpha($source, true);
                                            break;
                                        case 'gif':
                                            $source = imagecreatefromgif($filename);
                                            break;
                                    }

                                    imagecopy($new, $source, 0, 0, $cW, $ch, $DPW, $DPH);
                                    switch ($filetype) {
                                        case 'jpg':
                                        case 'jpeg':
                                            imagejpeg($new, $filename, 99);
                                            break;
                                        case 'png':
                                            imagealphablending($new, false);
                                            imagesavealpha($new, true);
                                            imagepng($new, $filename, 9);
                                            break;
                                        case 'gif':
                                            imagegif($new, $filename, 99);
                                            break;
                                    }

                                    //	resize image
                                    $filename = "media/images/users/" . $imageName_120;
                                    $path_parts = pathinfo($filename);
                                    $filetype = $path_parts ['extension'];
                                    $filetype = strtolower($filetype);

                                    $DPW = 120;
                                    $DPH = 120;
                                    $im = new Image ();
                                    $im->ChangeSizeInBox($filename, 1280, 1280, $filename);

                                    list ($width, $height) = getimagesize($filename);
                                    if ($width > $height) {
                                        $width = round($width * $DPH / $height);
                                        $height = $DPH;
                                        $ch = 0;
                                        $cW = ceil(($width - $DPW) / 2);
                                    } else {
                                        $height = round($height * $DPW / $width);
                                        $width = $DPW;
                                        $cW = 0;
                                        $ch = 0;
                                    }
                                    $im->ChangeImageSize($filename, $width, $height, $filename);

                                    $new = imagecreatetruecolor($DPW, $DPH);

                                    switch ($filetype) {
                                        case 'jpg':
                                        case 'jpeg':
                                            $source = imagecreatefromjpeg($filename);
                                            break;
                                        case 'png':
                                            imagealphablending($new, false);

                                            $source = imagecreatefrompng($filename);
                                            imagealphablending($source, false);
                                            imagesavealpha($source, true);
                                            break;
                                        case 'gif':
                                            $source = imagecreatefromgif($filename);
                                            break;
                                    }

                                    imagecopy($new, $source, 0, 0, $cW, $ch, $DPW, $DPH);
                                    switch ($filetype) {
                                        case 'jpg':
                                        case 'jpeg':
                                            imagejpeg($new, $filename, 99);
                                            break;
                                        case 'png':
                                            imagealphablending($new, false);
                                            imagesavealpha($new, true);
                                            imagepng($new, $filename, 9);
                                            break;
                                        case 'gif':
                                            imagegif($new, $filename, 99);
                                            break;
                                    }
                                }
                            }
                        }

                        $this->m_message = "Изменения сохранены";
                    } else {
                        $this->m_message = $this->GetNameString('warningAlert');
                    }
                    break;
                case 'changeuserinfo':
                    $checked = true;

                    // personal data
                    $fullName = trim(DB::POST('fullName'));
                    if ($fullName == "") {
                        $_SESSION["FORMERROR"][] = "fullName";
                        $checked = false;
                    }

                    $gender = trim(DB::POST('gender'));
                    if ($gender == 100) {
                        $gender = "";
                    }

                    $password = trim(DB::POST('rpassword'));
                    if (strlen($password) > 0) {
                        if (strlen($password) < 6) {
                            $_SESSION["FORMERROR"][] = "rpassword";
                            $checked = false;
                        }

                        $cpassword = trim(DB::POST('cpassword'));
                        if ($cpassword !== $password) {
                            $_SESSION["FORMERROR"][] = "cpassword";
                            $checked = false;
                        }
                    }

                    if ($checked) {
                        $this->m_user->SetFirstName($fullName);
                        $this->m_user->SetGender($gender);
                        if (strlen($password) > 0) {
                            $this->m_user->SetPassword($password);
                        }

                        $picture = $_FILES["image"];
                        if ($picture["size"] > 0) {
                            $path_parts = pathinfo($picture['name']);
                            $ext = $path_parts ['extension'];
                            $ext = strtolower($ext);

                            $imageName = $this->m_user->GetRandom() . "." . $ext;
                            $imageName_400 = "400_" . $imageName;
                            $imageName_120 = "120_" . $imageName;

                            $types = array('jpg', 'jpeg', 'png');
                            $res = array_search($ext, $types);
                            if (is_int($res)) {
                                if (copy($picture['tmp_name'], "media/images/users/" . $imageName) && copy($picture['tmp_name'], "media/images/users/" . $imageName_120) && copy($picture['tmp_name'], "media/images/users/" . $imageName_400)) {
                                    $this->m_user->SetImage("media/images/users/" . $imageName);
                                    $this->m_user->SetImage400("media/images/users/" . $imageName_400);
                                    $this->m_user->SetImage120("media/images/users/" . $imageName_120);

                                    //	resize image
                                    $filename = "media/images/users/" . $imageName_400;
                                    $path_parts = pathinfo($filename);
                                    $filetype = $path_parts ['extension'];
                                    $filetype = strtolower($filetype);

                                    $DPW = 400;
                                    $DPH = 400;
                                    $im = new Image ();
                                    $im->ChangeSizeInBox($filename, 1280, 1280, $filename);

                                    list ($width, $height) = getimagesize($filename);
                                    if ($width > $height) {
                                        $width = round($width * $DPH / $height);
                                        $height = $DPH;
                                        $ch = 0;
                                        $cW = ceil(($width - $DPW) / 2);
                                    } else {
                                        $height = round($height * $DPW / $width);
                                        $width = $DPW;
                                        $cW = 0;
                                        $ch = 0;
                                    }
                                    $im->ChangeImageSize($filename, $width, $height, $filename);

                                    $new = imagecreatetruecolor($DPW, $DPH);

                                    switch ($filetype) {
                                        case 'jpg':
                                        case 'jpeg':
                                            $source = imagecreatefromjpeg($filename);
                                            break;
                                        case 'png':
                                            imagealphablending($new, false);

                                            $source = imagecreatefrompng($filename);
                                            imagealphablending($source, false);
                                            imagesavealpha($source, true);
                                            break;
                                        case 'gif':
                                            $source = imagecreatefromgif($filename);
                                            break;
                                    }

                                    imagecopy($new, $source, 0, 0, $cW, $ch, $DPW, $DPH);
                                    switch ($filetype) {
                                        case 'jpg':
                                        case 'jpeg':
                                            imagejpeg($new, $filename, 99);
                                            break;
                                        case 'png':
                                            imagealphablending($new, false);
                                            imagesavealpha($new, true);
                                            imagepng($new, $filename, 9);
                                            break;
                                        case 'gif':
                                            imagegif($new, $filename, 99);
                                            break;
                                    }

                                    //	resize image
                                    $filename = "media/images/users/" . $imageName_120;
                                    $path_parts = pathinfo($filename);
                                    $filetype = $path_parts ['extension'];
                                    $filetype = strtolower($filetype);

                                    $DPW = 120;
                                    $DPH = 120;
                                    $im = new Image ();
                                    $im->ChangeSizeInBox($filename, 1280, 1280, $filename);

                                    list ($width, $height) = getimagesize($filename);
                                    if ($width > $height) {
                                        $width = round($width * $DPH / $height);
                                        $height = $DPH;
                                        $ch = 0;
                                        $cW = ceil(($width - $DPW) / 2);
                                    } else {
                                        $height = round($height * $DPW / $width);
                                        $width = $DPW;
                                        $cW = 0;
                                        $ch = 0;
                                    }
                                    $im->ChangeImageSize($filename, $width, $height, $filename);

                                    $new = imagecreatetruecolor($DPW, $DPH);

                                    switch ($filetype) {
                                        case 'jpg':
                                        case 'jpeg':
                                            $source = imagecreatefromjpeg($filename);
                                            break;
                                        case 'png':
                                            imagealphablending($new, false);

                                            $source = imagecreatefrompng($filename);
                                            imagealphablending($source, false);
                                            imagesavealpha($source, true);
                                            break;
                                        case 'gif':
                                            $source = imagecreatefromgif($filename);
                                            break;
                                    }

                                    imagecopy($new, $source, 0, 0, $cW, $ch, $DPW, $DPH);
                                    switch ($filetype) {
                                        case 'jpg':
                                        case 'jpeg':
                                            imagejpeg($new, $filename, 99);
                                            break;
                                        case 'png':
                                            imagealphablending($new, false);
                                            imagesavealpha($new, true);
                                            imagepng($new, $filename, 9);
                                            break;
                                        case 'gif':
                                            imagegif($new, $filename, 99);
                                            break;
                                    }
                                }
                            }
                        }

                        $this->m_message = "Изменения сохранены";
                    } else {
                        $this->m_message = $this->GetNameString('warningAlert');
                    }
                    break;
                case 'changeuserinfo_old':
                    $checked = true;

                    // personal data
                    $lastName = trim(DB::POST('lastName'));
                    if ($lastName == "") {
                        $_SESSION["FORMERROR"][] = "lastname";
                        $checked = false;
                    }

                    $firstName = trim(DB::POST('firstName'));
                    if ($firstName == "") {
                        $_SESSION["FORMERROR"][] = "firstname";
                        $checked = false;
                    }

                    $password = trim(DB::POST('rpassword'));
                    if (strlen($password) > 0) {
                        if (strlen($password) < 6) {
                            $_SESSION["FORMERROR"][] = "rpassword";
                            $checked = false;
                        }

                        $cpassword = trim(DB::POST('cpassword'));
                        if ($cpassword !== $password) {
                            $_SESSION["FORMERROR"][] = "cpassword";
                            $checked = false;
                        }
                    }

                    if ($checked) {
                        $this->m_user->SetFirstName($firstName);
                        $this->m_user->SetLastName($lastName);
                        if (strlen($password) > 0) {
                            $this->m_user->SetPassword($password);
                        }

                        $this->m_message = "Изменения сохранены";
                    } else {
                        $this->m_message = $this->GetNameString('warningAlert');
                    }
                    break;
                case 'changepasswordin' :
                    $password = trim(DB::POST('rpassword'));
                    $cpassword = trim(DB::POST('cpassword'));

                    if (strlen($password) == 0 || strlen($cpassword) == 0) {
                        $this->m_ar = false;
                        $this->m_message = 'Please fill all fields!';
                    } else {
                        // chack passwords equalment
                        if (strcmp($password, $cpassword) != 0) {
                            $this->m_ar = false;

                            // display change password unsuccessful content
                            $this->m_message = 15;
                        } elseif (strlen($password) < 6 || strlen($cpassword) < 6) {
                            $this->m_ar = false;

                            // display change password unsuccessful content because of less amount of symbols ( < 6 )
                            $this->m_message = 27;
                        } else {
                            // display change password successful content
                            $this->m_message = 14;

                            $this->m_user->SetPassword($password);
                        }
                    }
                    break;
                case 'addarticle':
                	ini_set('memory_limit', '128M');
                    if ( $this->m_user->GetID() )
                    {
                        $this->m_ar = true;

                        $title = DB::POST('title');
                        if ( $title=="" )
                        {
                            $this->m_ar = false;
                            $this->m_message = "Type your title<br />";
                        }

                        $content = trim(DB::POST('content'));
                        if ( $content=="" )
                        {
                            $this->m_ar = false;
                            $this->m_message = "Tell your story<br />";
                        }

                        $tags = trim($_POST["tags"]);
                        $tags = json_decode($tags, true);
                        if ( count($tags)==0 )
                        {
                            $this->m_ar = false;
                            $this->m_message = "Add a tag<br />";
                        }
                        
                        $rights = intval($_POST["rights"]);
                        if ( $rights==0 )
                        {
                        	$this->m_ar = false;
                            $this->m_message = "Choose license<br />";
                        }
                        
                        $private = isset($_POST["private"]) ? 1: 0;
                        
                        $free = isset($_POST["free"]) ? 1: 0;
                        $paid = isset($_POST["paid"]) ? 1: 0;
                        $priceUnit = trim(DB::POST('priceUnit'));
                        $price = trim(DB::POST('price'));
                        if ( $free )
                        {
                        	$payment = 0;
                        	$priceUnit = "";
                        	$price = 0;
                        }
                        elseif ( $paid ) 
                        {
                        	$payment = 1;
                        }
                        else 
                        {
                        	$this->m_ar = false;
                            $this->m_message = "Choose payment type<br />";
                        }
                        
                        if ( $this->m_ar )
                        {
                            if ( $this->m_ar )
                            {
                                $newPr_main = new Pr_main ();

                                $params = array('typeID'=>1, "userID"=>$this->m_user->GetID());
                                $params["name_en"] = $title;
                                $params["desc_en"] = $content;
                                $params['date'] = date("Y-m-d");
                                $params['created_date'] = date("Y-m-d H:i:s");
                                $params['published_date'] = date("Y-m-d H:i:s");
                                $params['updated_date'] = date("Y-m-d H:i:s");
                                $params['status'] = 1;
                                $params['payment'] = $payment;
                                $params['price'] = $price;
                                $params['priceUnit'] = $priceUnit;
                                $params['private'] = $private;

                                $newPr_main->Insert($params);
                                $productID = $newPr_main->GetID();

                                for ( $i=0; $i<count($tags); $i++ )
                                {
                                	$tag = $tags[$i];
                                	$tag = str_replace(array("'", "&", ",", "|", "{", "}", "[", "]", "/", "\\"), array("", "", "", "", "", "", "", "", "", ""), $tag);
                                	
                                	//	check if tag exist
                                    $query = "select `ID` from `optionsdata` where `optionsID` = 1 and `name_en` = '$tag'";
                                    $optiondataID = DB::QueryToValue($query);
                                    if ( !$optiondataID )
                                    {
                                        $optiondata = new Optionsdata();
                                        $props = "";
                                        $props['optionsID'] = 1;
                                        $props['name_en'] = $tag;
                                        $props['key'] = strtolower(str_replace(" ", "-", $tag));
                                        $optiondata->Insert($props);

                                        $optiondataID = $optiondata->GetID();
                                    }
                                    
                                    $props = "";
                                    $productOptions = new Pr_options(null, $productID, 71);
                                    $props ['productID'] = $productID;
                                    $props ['fieldID'] = 1;
                                    $props ['relatedID'] = $optiondataID;
                                    $productOptions->Insert($props);
                                }
                                
                                //	license
                                $props = "";
                                $productOptions = new Pr_options(null, $productID, 71);
                                $props ['productID'] = $productID;
                                $props ['fieldID'] = 2;
                                $props ['relatedID'] = $rights;
                                $productOptions->Insert($props);

                                //	images
                                $prodFolderPath = $this->m_site->GetProImageRoot(true, false) . $productID . "/";
                                $PFP = $this->m_site->GetProImageRoot(false) . $productID . "/";

                                //create forlder
                                $res = FolderXP::Mkdir("$prodFolderPath", 0777);
                                $defPicID = 0;
                                $defCoverID = 0;

                                $photo = $_FILES['image'];
                                if ( is_array($photo) )
                                {
                                    // get upload file properties
                                    $name = str_replace(" ", "", $photo['name']);
                                    $size = str_replace(" ", "", $photo['size']);
                                    if ( $size>1 )
                                    {
                                        // get uploaded file extension
                                        $path_parts = pathinfo($photo['name']);
                                        $ext = $path_parts['extension'];
                                        $ext = strtolower($ext);

                                        // replase uper chares to lower
                                        $pos = strripos($name, $ext);
                                        $name = GetRandCode(10, true).".".$ext;

                                        // all posible extentions
                                        $types = array('jpg', 'jpeg', 'png', 'gif');
                                        $res = array_search($ext, $types);
                                        if ( is_int($res) )
                                        {
                                            //	get file dimensions and check min requirements
                                            $image_info = getimagesize($photo['tmp_name']);
                                            if ( $image_info[0]>700 && $image_info[1]>400 )
                                            {
                                                $fileAddress = $prodFolderPath . $name;
                                                if ( !file_exists($fileAddress) )
                                                {
                                                    // insert new pr_picture with name
                                                    $newPr_picture = new Pr_picture();
                                                    $fullPath = $PFP.$name;
                                                    $params = array('productID'=>$productID, 'path'=>$fullPath);
                                                    $ar = $newPr_picture->Insert($params);

                                                    // uploading
                                                    $res = FolderXP::Upload($photo['tmp_name'], $fileAddress);

                                                    // if file is image and must be resized, create Image class item and resize image
                                                    $maxWidth = $this->m_site->GetMaxImageWidth();
                                                    $image = new Image();
                                                    $image->ChangeSizeInBox($fileAddress, $maxWidth, null, null, "");

                                                    $defPicID = $newPr_picture->GetID();
                                                    $defCoverID = $newPr_picture->GetID();

                                                    //	create icon
                                                    $path = $name;
                                                    $pos = strrpos($path, ".");
                                                    $defPath = substr($path, 0, $pos)."-icon".substr($path, $pos);

                                                    $prLocalRoot = $this->m_site->GetProImageRoot(true).$productID.'/'.$defPath;
                                                    $prGlobalRoot = $this->m_site->GetProImageRoot(false).$productID.'/'.$defPath;

                                                    $im = new Image ();
                                                    $im->ChangeSizeInBox($this->m_site->GetProImageRoot(true).$productID.'/'.$path, 2560, 2560, $prLocalRoot);

                                                    list ($width, $height) = getimagesize($prLocalRoot);
                                                    if ( $image_info[1]>170 )
                                                    {
                                                        $DPH = 170;
                                                        $DPW = 170 * $width / $height;
                                                    } 
                                                    else 
                                                    {
                                                        $DPH = $height;
                                                        $DPW = $width;
                                                    }
                                                    $im->ChangeImageSize($prLocalRoot, $DPW, $DPH, $prLocalRoot);

                                                    $newPr_picture->SetIconPath($prGlobalRoot);
                                                }
                                            }
                                        }
                                    }

                                    if ( $defPicID )
                                    {
                                        $pic = new Pr_picture($defPicID);

                                        $defPath = $pic->GetPath();
                                        $pos = strrpos($defPath, '.');
                                        $esxt = substr($defPath, $pos + 1);

                                        $prID = $productID;

                                        $prodFolderPath = $this->m_site->GetProImageRoot(true, false) . $productID . "/";
                                        $PFP = $this->m_site->GetProImageRoot(false) . $productID . "/";

                                        $prLocalRootMain = $this->m_site->GetProImageRoot(true, false);
                                        $prGlobalRootMain = $this->m_site->GetProImageRoot(false);


                                        //	BIG ICON 700x400
                                        $prLocalRoot = $prLocalRootMain . $prID . '-big.' . $esxt;
                                        $prGlobalRoot = $prGlobalRootMain . $prID . '-big.' . $esxt;
                                        $iconW = 700;
                                        $iconH = 400;
                                        $im = new Image ();
                                        $im->ChangeSizeInBox($defPath, 2560, 2560, $prLocalRoot, "");
                                        list ($width, $height) = getimagesize($prLocalRoot);
                                        if ($width * $iconH > $height * $iconW) {
                                            $width = round($width * $iconH / $height);
                                            $height = $iconH;
                                            $ch = 0;
                                            $cW = ceil(($width - $iconW) / 2);
                                        } else {
                                            $height = round($height * $iconW / $width);
                                            $width = $iconW;
                                            $cW = 0;
                                            $ch = 0;
                                        }
                                        $im->ChangeImageSize($prLocalRoot, $width, $height, $prLocalRoot);
                                        switch ($esxt) {
                                            case 'jpg':
                                            case 'jpeg':
                                                $source = imagecreatefromjpeg($prLocalRoot);
                                                break;
                                            case 'png':
                                                $source = imagecreatefrompng($prLocalRoot);
                                                break;
                                            case 'gif':
                                                $source = imagecreatefromgif($prLocalRoot);
                                                break;
                                        }
                                        $new = imagecreatetruecolor($iconW, $iconH);
                                        imagecopy($new, $source, 0, 0, $cW, $ch, $iconW, $iconH);
                                        switch ($esxt) {
                                            case 'jpg':
                                            case 'jpeg':
                                                imagejpeg($new, $prLocalRoot, 99);
                                                break;
                                            case 'png':
                                                imagepng($new, $prLocalRoot, 9);
                                                break;
                                            case 'gif':
                                                imagegif($new, $prLocalRoot, 99);
                                                break;
                                        }
                                        $newPr_main->SetPicture($prGlobalRoot);

                                        //	SMALL ICON 200x170
                                        $prLocalRoot = $prLocalRootMain . $prID . '-small.' . $esxt;
                                        $prGlobalRoot = $prGlobalRootMain . $prID . '-small.' . $esxt;
                                        $iconW = 200;
                                        $iconH = 170;
                                        $im = new Image ();
                                        $im->ChangeSizeInBox($defPath, 2560, 2560, $prLocalRoot, "");
                                        list ($width, $height) = getimagesize($prLocalRoot);
                                        if ($width * $iconH > $height * $iconW) {
                                            $width = round($width * $iconH / $height);
                                            $height = $iconH;
                                            $ch = 0;
                                            $cW = ceil(($width - $iconW) / 2);
                                        } else {
                                            $height = round($height * $iconW / $width);
                                            $width = $iconW;
                                            $cW = 0;
                                            $ch = 0;
                                        }
                                        $im->ChangeImageSize($prLocalRoot, $width, $height, $prLocalRoot);
                                        switch ($esxt) {
                                            case 'jpg':
                                            case 'jpeg':
                                                $source = imagecreatefromjpeg($prLocalRoot);
                                                break;
                                            case 'png':
                                                $source = imagecreatefrompng($prLocalRoot);
                                                break;
                                            case 'gif':
                                                $source = imagecreatefromgif($prLocalRoot);
                                                break;
                                        }
                                        $new = imagecreatetruecolor($iconW, $iconH);
                                        imagecopy($new, $source, 0, 0, $cW, $ch, $iconW, $iconH);
                                        switch ($esxt) {
                                            case 'jpg':
                                            case 'jpeg':
                                                imagejpeg($new, $prLocalRoot, 99);
                                                break;
                                            case 'png':
                                                imagepng($new, $prLocalRoot, 9);
                                                break;
                                            case 'gif':
                                                imagegif($new, $prLocalRoot, 99);
                                                break;
                                        }
                                        $newPr_main->SetPicture1($prGlobalRoot);
                                    }
                                    if ( $defCoverID )
                                    {
                                        $pic = new Pr_picture ($defCoverID);

                                        $defPath = $pic->GetPath();
                                        $pos = strrpos($defPath, '.');
                                        $esxt = substr($defPath, $pos + 1);

                                        $prID = $productID;

                                        $prodFolderPath = $this->m_site->GetProImageRoot(true, false) . $productID . "/";
                                        $PFP = $this->m_site->GetProImageRoot(false) . $productID . "/";

                                        $prLocalRoot = $this->m_site->GetProImageRoot(true, false);
                                        $prGlobalRoot = $this->m_site->GetProImageRoot(false);
                                        $prLocalRoot .= $prID . '_cover.' . $esxt;
                                        $prGlobalRoot .= $prID . '_cover.' . $esxt;

                                        list ($width, $height) = getimagesize($pic->GetPath());
                                        $coverW = 1400;
                                        $coverH = 1400 * $height / $width;
                                        if ( $coverH<600 )
                                        {
                                            $coverH = 600;
                                            $coverW = 600 * $width / $height;
                                        }

                                        $im = new Image ();
                                        $im->ChangeSizeInBox($defPath, 2560, 2560, $prLocalRoot, "");
                                        list ($width, $height) = getimagesize($prLocalRoot);
                                        if ($width * 600 > $height * 1400) {
                                            $width = round($width * 600 / $height);
                                            $height = 600;
                                            $ch = 0;
                                            $cW = ceil(($width - 1400) / 2);
                                        } else {
                                            $height = round($height * 1400 / $width);
                                            $width = $coverW;
                                            $cW = 0;
                                            $ch = 0;
                                        }
                                        $im->ChangeImageSize($prLocalRoot, $coverW, $coverH, $prLocalRoot);
                                        switch ($esxt) {
                                            case 'jpg':
                                            case 'jpeg':
                                                $source = imagecreatefromjpeg($prLocalRoot);
                                                break;
                                            case 'png':
                                                $source = imagecreatefrompng($prLocalRoot);
                                                break;
                                            case 'gif':
                                                $source = imagecreatefromgif($prLocalRoot);
                                                break;
                                        }
                                        $new = imagecreatetruecolor(1400, 600);
                                        imagecopy($new, $source, 0, 0, $cW, $ch, 1400, 600);
                                        switch ($esxt) {
                                            case 'jpg':
                                            case 'jpeg':
                                                imagejpeg($new, $prLocalRoot, 99);
                                                break;
                                            case 'png':
                                                imagepng($new, $prLocalRoot, 9);
                                                break;
                                            case 'gif':
                                                imagegif($new, $prLocalRoot, 99);
                                                break;
                                        }
                                        $newPr_main->SetCover($prGlobalRoot);
                                    }
                                }

                                $toCat = new Category(321);
                                header("location:".$this->m_site->GetSiteURL().$toCat->GetPath());
                                exit();
                            }
                        }
                    }

                    break;
                case 'editarticle':
                    if ($this->m_user->GetID()) {
                        $this->m_ar = true;

                        $prID = $productID = DB::POST("articleID");
                        $newPr = new Pr_main($prID);
                        $prUserID = $newPr->GetUserID();

                        if ($prUserID == $this->m_user->GetID()) {
                            if ($newPr->GetStatus()) {
                                $createIcon = false;

                                $newPr->SetUpdatedDate(date("Y-m-d H:i:s"));

                                $content = $_POST["content"];
                                if ($content != "") {
                                    $mainContent = $newPr->GetDesc($this->m_displayLang);
                                    $mainContent = $mainContent . "<hr>" . $content;

                                    $newPr->SetDesc($mainContent, $this->m_displayLang);
                                }

                                $tag2 = trim(DB::POST('tag2'));
                                $tag3 = trim(DB::POST('tag3'));
                                if ($tag2 != "") {
                                    //	check if tag exist
                                    $query = "select `ID` from `optionsdata` where `optionsID` = 28 and `name_ru` = '$tag2'";
                                    $optiondataID = DB::QueryToValue($query);
                                    if (!$optiondataID) {
                                        $optiondata = new Optionsdata();
                                        $props = "";
                                        $props['optionsID'] = 28;
                                        $props['name_ru'] = $tag2;
                                        $props['key'] = str_replace(" ", "_", $tag2);
                                        $optiondata->Insert($props);

                                        $optiondataID = $optiondata->GetID();
                                    }

                                    $props = "";
                                    $productOptions = new Pr_options(null, $productID, 71);
                                    $props ['productID'] = $productID;
                                    $props ['fieldID'] = 71;
                                    $props ['relatedID'] = $optiondataID;
                                    $productOptions->Insert($props);
                                }
                                if ($tag3 != "") {
                                    //	check if tag exist
                                    $query = "select `ID` from `optionsdata` where `optionsID` = 28 and `name_ru` = '$tag3'";
                                    $optiondataID = DB::QueryToValue($query);
                                    if (!$optiondataID) {
                                        $optiondata = new Optionsdata();
                                        $props = "";
                                        $props['optionsID'] = 28;
                                        $props['name_ru'] = $tag3;
                                        $props['key'] = str_replace(" ", "_", $tag3);
                                        $optiondata->Insert($props);

                                        $optiondataID = $optiondata->GetID();
                                    }

                                    $props = "";
                                    $productOptions = new Pr_options(null, $productID, 71);
                                    $props ['productID'] = $productID;
                                    $props ['fieldID'] = 71;
                                    $props ['relatedID'] = $optiondataID;
                                    $productOptions->Insert($props);
                                }

                                $source = DB::POST('source');
                                if ($source != "") {
                                    $props = "";
                                    $productOptions = new Pr_char(null, $productID, 70);
                                    $props ['productID'] = $productID;
                                    $props ['fieldID'] = 70;
                                    $props ['value'] = $source;
                                    $productOptions->Insert($props);
                                }
                            } else {
                                $createIcon = true;

                                $title = DB::POST('title');
                                if ($title == "") {
                                    $this->m_ar = false;
                                    $this->m_message = "Заполните Название";
                                }

                                $content = $_POST["content"];
                                if ($content == "") {
                                    $this->m_ar = false;
                                    $this->m_message = "Заполните Текст";
                                }

                                $tag1 = trim(DB::POST('tag1'));
                                $tag2 = trim(DB::POST('tag2'));
                                $tag3 = trim(DB::POST('tag3'));
                                if ($tag1 == "" && $tag2 == "" && $tag3 == "") {
                                    $this->m_ar = false;
                                    $this->m_message = "Один Тэг объязателен";
                                }

                                $source = DB::POST('source');

                                if ($this->m_ar) {
                                    if (isset($_POST["publish"])) {
                                        $newPr->SetPublishedDate(date("Y-m-d H:i:s"));
                                        $newPr->SetUpdatedDate(date("Y-m-d H:i:s"));
                                        $newPr->SetStatus(1);
                                    }
                                    $newPr->SetName($title, $this->m_displayLang);
                                    $newPr->SetDesc($content, $this->m_displayLang);

                                    if ($tag1 != "") {
                                        //	check if tag exist
                                        $query = "select `ID` from `optionsdata` where `optionsID` = 28 and `name_ru` = '$tag1'";
                                        $optiondataID = DB::QueryToValue($query);
                                        if (!$optiondataID) {
                                            $optiondata = new Optionsdata();
                                            $props = "";
                                            $props['optionsID'] = 28;
                                            $props['name_ru'] = $tag1;
                                            $props['key'] = $tag1;
                                            $optiondata->Insert($props);

                                            $optiondataID = $optiondata->GetID();
                                        }

                                        $props = "";
                                        $prOptions = new Pr_options(null, $prID, 71, 0, $optiondataID);
                                        if ($prOptions->GetID()) {
                                            $prOptions->SetRelatedID($optiondataID);
                                        } else {
                                            $props ['productID'] = $prID;
                                            $props ['fieldID'] = 71;
                                            $props ['relatedID'] = $optiondataID;
                                            $productOptions->Insert($props);
                                        }
                                    }
                                    if ($tag2 != "") {
                                        //	check if tag exist
                                        $query = "select `ID` from `optionsdata` where `optionsID` = 28 and `name_ru` = '$tag2'";
                                        $optiondataID = DB::QueryToValue($query);
                                        if (!$optiondataID) {
                                            $optiondata = new Optionsdata();
                                            $props = "";
                                            $props['optionsID'] = 28;
                                            $props['name_ru'] = $tag2;
                                            $props['key'] = $tag1;
                                            $optiondata->Insert($props);

                                            $optiondataID = $optiondata->GetID();
                                        }

                                        $props = "";
                                        $prOptions = new Pr_options(null, $prID, 71, 0, $optiondataID);
                                        if ($prOptions->GetID()) {
                                            $prOptions->SetRelatedID($optiondataID);
                                        } else {
                                            $props ['productID'] = $prID;
                                            $props ['fieldID'] = 71;
                                            $props ['relatedID'] = $optiondataID;
                                            $productOptions->Insert($props);
                                        }
                                    }
                                    if ($tag3 != "") {
                                        //	check if tag exist
                                        $query = "select `ID` from `optionsdata` where `optionsID` = 28 and `name_ru` = '$tag3'";
                                        $optiondataID = DB::QueryToValue($query);
                                        if (!$optiondataID) {
                                            $optiondata = new Optionsdata();
                                            $props = "";
                                            $props['optionsID'] = 28;
                                            $props['name_ru'] = $tag3;
                                            $props['key'] = $tag1;
                                            $optiondata->Insert($props);

                                            $optiondataID = $optiondata->GetID();
                                        }

                                        $props = "";
                                        $prOptions = new Pr_options(null, $prID, 71, 0, $optiondataID);
                                        if ($prOptions->GetID()) {
                                            $prOptions->SetRelatedID($optiondataID);
                                        } else {
                                            $props ['productID'] = $prID;
                                            $props ['fieldID'] = 71;
                                            $props ['relatedID'] = $optiondataID;
                                            $productOptions->Insert($props);
                                        }
                                    }

                                    $props = "";
                                    $prChar = new Pr_char(null, $prID, 70);
                                    if ($prChar->GetID()) {
                                        $prChar->SetValue($source);
                                    } else {
                                        $props ['productID'] = $prID;
                                        $props ['subID'] = 0;
                                        $props ['fieldID'] = 70;
                                        $props ['value'] = $source;
                                        $prChar->Insert($props);
                                    }
                                }
                            }

                            if ($this->m_ar) {
                                $photoDel = DB::POST("delImg");
                                if ($photoDel[0] != "") {
                                    $newPr->SetDefPicNull();
                                    $newPr->SetCoverPicNull();

                                    for ($p = 0; $p < count($photoDel); $p++) {
                                        $newPrPic = new Pr_picture($photoDel[$p]);
                                        $picPath = $newPrPic->GetPath();
                                        $query = "delete from `pr_picture` where `ID` = " . $newPrPic->GetID();
                                        DB::Query($query);

                                        if (strpos($picPath, "media")) {
                                            $picPath = substr($picPath, strpos($picPath, "media"));
                                        }
                                        if (file_exists($picPath)) {
                                            unlink($picPath);
                                        }
                                    }

                                    $pictures = new Pr_pictureList($prID);
                                    $picturesList = $pictures->GetStructure();
                                    if ($picturesList[0] != "") {
                                        $defPicID = $picturesList[0]["ID"];
                                    }

                                    $createIcon = true;
                                }


                                $productID = $prID;
                                //	images
                                $prodFolderPath = $this->m_site->GetProImageRoot(true, false) . $productID . "/";
                                $PFP = $this->m_site->GetProImageRoot(false) . $productID . "/";

                                //create forlder
                                $res = FolderXP::Mkdir("$prodFolderPath", 0777);
                                $defPicID = 0;

                                $photo = $_FILES['photo'];

                                if (is_array($photo)) {
                                    $photoCount = count($photo["name"]);

                                    for ($i = 0; $i < $photoCount; $i++) {
                                        // get upload file properties
                                        $name = str_replace(" ", "", $photo['name'][$i]);
                                        $size = str_replace(" ", "", $photo['size'][$i]);
                                        if ($size > 1) {
                                            // get uploaded file extension
                                            $path_parts = pathinfo($photo['name'][$i]);
                                            $ext = $path_parts['extension'];
                                            $ext = strtolower($ext);

                                            // replase uper chares to lower
                                            $pos = strripos($name, $ext);
                                            $name = GetRandCode(10, true) . "." . $ext;

                                            // all posible extentions
                                            $types = array('jpg', 'jpeg', 'png', 'gif');
                                            $res = array_search($ext, $types);
                                            if (is_int($res)) {
                                                $fileAddress = $prodFolderPath . $name;
                                                if (!file_exists($fileAddress)) {
                                                    // insert new pr_picture with name
                                                    $newPr_picture = new Pr_picture();
                                                    $fullPath = $PFP . $name;
                                                    $params = array('productID' => $productID, 'path' => $fullPath);
                                                    $ar = $newPr_picture->Insert($params);

                                                    // uploading
                                                    $res = FolderXP::Upload($photo['tmp_name'][$i], $fileAddress);

                                                    // if file is image and must be resized, create Image class item and resize image
                                                    $maxWidth = $this->m_site->GetMaxImageWidth();
                                                    $image = new Image();
                                                    $image->ChangeSizeInBox($fileAddress, $maxWidth, null, null, "");

                                                    if ($defPicID == 0) {
                                                        $defPicID = $newPr_picture->GetID();
                                                    }

                                                    //	create icon
                                                    $path = $name;
                                                    $pos = strrpos($path, ".");
                                                    $defPath = substr($path, 0, $pos) . "-icon" . substr($path, $pos);

                                                    $prLocalRoot = $this->m_site->GetProImageRoot(true) . $productID . '/' . $defPath;
                                                    $prGlobalRoot = $this->m_site->GetProImageRoot(false) . $productID . '/' . $defPath;

                                                    $im = new Image ();
                                                    $im->ChangeSizeInBox($this->m_site->GetProImageRoot(true) . $productID . '/' . $path, 1280, 1280, $prLocalRoot);

                                                    list ($width, $height) = getimagesize($prLocalRoot);
                                                    if ($width * 300 > $height * 400) {
                                                        $width = round($width * 300 / $height);
                                                        $height = 300;
                                                        $ch = 0;
                                                        $cW = ceil(($width - 400) / 2);
                                                    } else {
                                                        $height = round($height * 400 / $width);
                                                        $width = 400;
                                                        $cW = 0;
                                                        $ch = 0;
                                                    }
                                                    $im->ChangeImageSize($prLocalRoot, $width, $height, $prLocalRoot);

                                                    switch ($ext) {
                                                        case 'jpg':
                                                        case 'jpeg':
                                                            $source = imagecreatefromjpeg($prLocalRoot);
                                                            break;
                                                        case 'png':
                                                            $source = imagecreatefrompng($prLocalRoot);
                                                            break;
                                                        case 'gif':
                                                            $source = imagecreatefromgif($prLocalRoot);
                                                            break;
                                                    }
                                                    $new = imagecreatetruecolor(400, 300);

                                                    imagecopy($new, $source, 0, 0, $cW, $ch, 400, 300);
                                                    switch ($ext) {
                                                        case 'jpg':
                                                        case 'jpeg':
                                                            imagejpeg($new, $prLocalRoot, 99);
                                                            break;
                                                        case 'png':
                                                            imagepng($new, $prLocalRoot, 9);
                                                            break;
                                                        case 'gif':
                                                            imagegif($new, $prLocalRoot, 99);
                                                            break;
                                                    }

                                                    $newPr_picture->SetIconPath($prGlobalRoot);
                                                }
                                            }
                                        }
                                    }

                                    if ($createIcon) {
                                        if ($defPicID) {
                                            $pic = new Pr_picture ($defPicID);

                                            $defPath = $pic->GetPath();
                                            $pos = strrpos($defPath, '.');
                                            $esxt = substr($defPath, $pos + 1);

                                            $prID = $productID;

                                            $prodFolderPath = $this->m_site->GetProImageRoot(true, false) . $productID . "/";
                                            $PFP = $this->m_site->GetProImageRoot(false) . $productID . "/";

                                            $prLocalRoot = $this->m_site->GetProImageRoot(true, false);
                                            $prGlobalRoot = $this->m_site->GetProImageRoot(false);

                                            $prLocalRoot .= $prID . '.' . $esxt;
                                            $prGlobalRoot .= $prID . '.' . $esxt;

                                            // create icon
                                            $im = new Image ();
                                            $im->ChangeSizeInBox($defPath, 800, 800, $prLocalRoot, "");

                                            //	added by Grigor		start
                                            list ($width, $height) = getimagesize($prLocalRoot);
                                            if ($width * 300 > $height * 400) {
                                                $width = round($width * 300 / $height);
                                                $height = 300;
                                                $ch = 0;
                                                $cW = ceil(($width - 400) / 2);
                                            } else {
                                                $height = round($height * 400 / $width);
                                                $width = 400;
                                                $cW = 0;
                                                $ch = 0;
                                            }
                                            $im->ChangeImageSize($prLocalRoot, $width, $height, $prLocalRoot);

                                            switch ($esxt) {
                                                case 'jpg':
                                                case 'jpeg':
                                                    $source = imagecreatefromjpeg($prLocalRoot);
                                                    break;
                                                case 'png':
                                                    $source = imagecreatefrompng($prLocalRoot);
                                                    break;
                                                case 'gif':
                                                    $source = imagecreatefromgif($prLocalRoot);
                                                    break;
                                            }
                                            $new = imagecreatetruecolor(400, 300);

                                            // Copy
                                            imagecopy($new, $source, 0, 0, $cW, $ch, 400, 300);
                                            switch ($esxt) {
                                                case 'jpg':
                                                case 'jpeg':
                                                    imagejpeg($new, $prLocalRoot, 99);
                                                    break;
                                                case 'png':
                                                    imagepng($new, $prLocalRoot, 9);
                                                    break;
                                                case 'gif':
                                                    imagegif($new, $prLocalRoot, 99);
                                                    break;
                                            }
                                            //	added by Grigor		end

                                            $newPr->SetPicture($prGlobalRoot);
                                        }
                                        if ($defPicID) {
                                            $pic = new Pr_picture ($defPicID);

                                            $defPath = $pic->GetPath();
                                            $pos = strrpos($defPath, '.');
                                            $esxt = substr($defPath, $pos + 1);

                                            $prID = $productID;

                                            $prodFolderPath = $this->m_site->GetProImageRoot(true, false) . $productID . "/";
                                            $PFP = $this->m_site->GetProImageRoot(false) . $productID . "/";

                                            $prLocalRoot = $this->m_site->GetProImageRoot(true, false);
                                            $prGlobalRoot = $this->m_site->GetProImageRoot(false);

                                            $prLocalRoot .= $prID . '_cover.' . $esxt;
                                            $prGlobalRoot .= $prID . '_cover.' . $esxt;

                                            // create icon
                                            $im = new Image ();
                                            $im->ChangeSizeInBox($defPath, 1200, 1200, $prLocalRoot, "");

                                            //	added by Grigor		start
                                            list ($width, $height) = getimagesize($prLocalRoot);
                                            if ($width * 450 > $height * 800) {
                                                $width = round($width * 450 / $height);
                                                $height = 450;
                                                $ch = 0;
                                                $cW = ceil(($width - 800) / 2);
                                            } else {
                                                $height = round($height * 800 / $width);
                                                $width = 800;
                                                $cW = 0;
                                                $ch = 0;
                                            }
                                            $im->ChangeImageSize($prLocalRoot, $width, $height, $prLocalRoot);

                                            switch ($esxt) {
                                                case 'jpg':
                                                case 'jpeg':
                                                    $source = imagecreatefromjpeg($prLocalRoot);
                                                    break;
                                                case 'png':
                                                    $source = imagecreatefrompng($prLocalRoot);
                                                    break;
                                                case 'gif':
                                                    $source = imagecreatefromgif($prLocalRoot);
                                                    break;
                                            }
                                            $new = imagecreatetruecolor(800, 450);

                                            // Copy
                                            imagecopy($new, $source, 0, 0, $cW, $ch, 800, 450);
                                            switch ($esxt) {
                                                case 'jpg':
                                                case 'jpeg':
                                                    imagejpeg($new, $prLocalRoot, 99);
                                                    break;
                                                case 'png':
                                                    imagepng($new, $prLocalRoot, 9);
                                                    break;
                                                case 'gif':
                                                    imagegif($new, $prLocalRoot, 99);
                                                    break;
                                            }
                                            //	added by Grigor		end

                                            $newPr->SetCover($prGlobalRoot);
                                        }
                                    }
                                }

                                header("location:" . $this->m_site->GetSiteURL() . $this->m_displayLang . "/profile/my-articles/");
                                exit();
                            }
                        }
                    }

                    break;
            }
        }
    }

    /**
     * Enter description here...
     *
     */
    function DisplayInfo()
    {
        print ("<br /> addition information");
    }

    /**
     * Enter description here...
     *
     * @param string $actiovationLink
     * @param int $activationID
     * @param string $activationCode
     * @return bool
     */
    function DisplaySubscribeForm()
    {
        global $session;
        $this->DisplayMessage();

        $this->ReadNameStrings('langs/', 'subscribeForm.xml', $this->m_displayLang);

        if (!$this->m_user->GetID()) {
            if (!$this->m_ar) {
                // display registration content
                $this->DisplayContent(1);

                // Get value of session rkey from the file
                $_SESSION['rkey'] = $session->GetRkey();

                if (!isset ($_SESSION ['rkey'])) {
                    // Set new value of session rkey to the file
                    $rkey = GetRandCode(6);
                    $session->SetRkey($rkey);
                    $_SESSION['rkey'] = $rkey;
                }
                // array for names
                $names = array();

                $names ["nameLabel"] = $this->GetNameString('nameLabel');
                $names ["emailDesc"] = $this->GetNameString('emailDesc');
                $names ["emailLabel"] = $this->GetNameString('emailLabel');
                $names ["passwordDesc"] = $this->GetNameString('passwordDesc');
                $names ["passwordLabel"] = $this->GetNameString('passwordLabel');
                $names ["confirmPasswordLabel"] = $this->GetNameString('confirmPasswordLabel');
                $names ["captchaDesc"] = $this->GetNameString('captchaDesc');
                $names ["agreementLabel"] = $this->GetNameString('agreementLabel');
                $names ["agreementDesc"] = $this->GetNameString('agreementDesc');
                $names ["signUpBtn"] = $this->GetNameString('signUpBtn');
                $names ["subscribeBtnNext"] = $this->GetNameString('subscribeBtnNext');
                $names ["subscribeBtn"] = $this->GetNameString('subscribeBtn');

                // error message
                $errorMessageS = $this->GetNameString('warningAlert');
                ?>
                <script type='text/javascript'>
                    <!--
                    <?php
                    if ( isset($_SESSION["CAPTCHADIVSHOW"]) && $_SESSION["CAPTCHADIVSHOW"] == 1 ) {?>

                    var captcha = 1;
                    <?php } else {?>

                    var captcha = 0;
                    <?php }?>
                    function validation() {
                        firstName = TextFildValid('fullName', null, 'fullName');
                        email = TextFildValid('email', 'email', 'email');

                        validImage = 1;
                        if (captcha == 1) {

                            validImage = TextFildValid('validImage', null, 'validImage');
                        }

                        rpassword = TextFildValid('rpassword', null, 'rpassword');
                        cpassword = TextFildValid('cpassword', null, 'cpassword');

                        pass = false;

                        if (rpassword && cpassword) {
                            pass = isEqual('rpassword', 'cpassword', 'rpassword', 'cpassword');

                            var pasLenght = GetFieldValueLenght('rpassword');
                            if (pasLenght < 6) {
                                document.getElementById('passLenght').className = 'wsuAlertActive';
                            }
                            else {
                                document.getElementById('passLenght').className = '';
                            }
                        }
                        res = (firstName && email && rpassword && cpassword && pass && validImage && (pasLenght > 5));

                        //	if user first time enter incorrect data then captcha appier
                        if (!res) {

                            captcha = 1;
                            document.getElementById("captchaDiv").style.display = "block";
                        }

                        // group validation
                        SerErrorText(res, '<?php echo $errorMessageS?>');
                        return res;
                    }

                    function validation1() {
                        email = TextFildValid('email', 'email', 'email');
                        // group validation
                        SerErrorText(email, '<?php echo $errorMessageS?>');
                        return email;
                    }
                    -->
                </script>
                <form action="" method="post" name="registration"
                      onsubmit="return <?php echo(isset($_POST["email"]) && $_POST["email"] != "" ? "validation" : "validation1"); ?>();">

                    <div class="wsuAlertMessage" id="errorDiv">&nbsp;</div>
                    <?php
                    if (isset($_POST["email"]) && $_POST["email"] != "") { ?>

                        <input type="hidden" name="siteUserAction" value="newuser"/>
                        <input type="hidden" name="email" id="email" value="<?php echo $_POST["email"]; ?>"/>

                        <div class="wsuTextFieldLabel">
                            <div id="fullNameHTML"><?php echo $names ['nameLabel'] ?></div>
                        </div>
                        <div class="wsuTextField"><input name="fullName" id="fullName" type="text" size="40"/></div>
                        <br clear="left"/>

                        <div class="wsuTextFieldDesc">
                            <div id="passLenghtHTML"><?php echo $names ['passwordDesc'] ?></div>
                        </div>
                        <div class="wsuTextFieldLabel">
                            <div id="rpasswordHTML"><?php echo $names ['passwordLabel'] ?></div>
                        </div>
                        <div class="wsuTextField"><input name="rpassword" id="rpassword" type="password" size="20"/>
                        </div>
                        <br clear="left"/>

                        <div class="wsuTextFieldLabel">
                            <div id="cpasswordHTML"><?php echo $names ['confirmPasswordLabel'] ?></div>
                        </div>
                        <div class="wsuTextField"><input name="cpassword" id="cpassword" type="password" size="20"/>
                        </div>
                        <br clear="left"/>

                        <div id="captchaDiv"
                             style="display:<?php echo(isset($_SESSION["CAPTCHADIVSHOW"]) && $_SESSION["CAPTCHADIVSHOW"] == 1 ? "block" : "none"); ?>;">
                            <div class="wsuTextFieldDesc">
                                <div id="validImageHTML"><?php echo $names ['captchaDesc'] ?></div>
                            </div>
                            <span class="wsuTextFieldLabel"><img src="files/imageT.php" width="122" height="22"
                                                                 alt=" "/></span>
                            <span class="wsuTextField"><input name="validImage" id="validImage" type="text"/></span>
                        </div>
                        <br clear="left"/>

                        <div class="wsuButton"><input name="" type="submit"
                                                      value="<?php echo $names ['subscribeBtn'] ?>"/></div>
                    <?php } else {

                        $_SESSION["CAPTCHADIVSHOW"] = 0;
                        ?>
                        <input type="hidden" name="siteUserAction" value="newuserfirst"/>
                        <div class="wsuTextFieldDesc"><?php echo $names ['emailDesc'] ?></div>
                        <div class="wsuTextFieldLabel">
                            <div id="emailHTML"><?php echo $names ['emailLabel'] ?></div>
                        </div>
                        <div class="wsuTextField"><input name="email" id="email" type="text" size="40"/></div>
                        <br clear="left"/>

                        <div class="wsuButton"><input name="" type="submit"
                                                      value="<?php echo $names ['subscribeBtnNext'] ?>"/></div>
                    <?php } ?>
                </form>

                <?php
            }
        }

        return false;
    }


    /**
     * Enter description here...
     *
     * @param string $actiovationLink
     * @param int $activationID
     * @param string $activationCode
     * @return bool
     */
    function DisplayRegistratinoForm()
    {
        global $session;
        
        $this->ReadNameStrings('langs/', 'registrationForm.xml', $this->m_displayLang);

        if ( !$this->m_user->GetID() )
        {
            if ( !$this->m_ar )
            {
                // array for names
                $names = array();

                $names ["nameLabel"] = $this->GetNameString('nameLabel');
                $names ["emailDesc"] = $this->GetNameString('emailDesc');
                $names ["emailLabel"] = $this->GetNameString('emailLabel');
                $names ["passwordDesc"] = $this->GetNameString('passwordDesc');
                $names ["passwordLabel"] = $this->GetNameString('passwordLabel');
                $names ["confirmPasswordLabel"] = $this->GetNameString('confirmPasswordLabel');
                $names ["captchaDesc"] = $this->GetNameString('captchaDesc');
                $names ["agreementLabel"] = $this->GetNameString('agreementLabel');
                $names ["agreementDesc"] = $this->GetNameString('agreementDesc');
                $names ["signUpBtn"] = $this->GetNameString('signUpBtn');

                // error message
                $errorMessageS = $this->GetNameString('warningAlert');
                
                $this->DisplayMessage();
                ?>
                <form action="" id="registration" method="post" name="registration" onsubmit="return validation();">
                    <input type="hidden" name="siteUserAction" value="newuser"/>

                    <div class="input-group name">
						<label>Full name</label>
						<input type="text" name="fullname" id="fullname" value="<?php echo $_POST["fullname"];?>" required />
					</div>
					<div class="input-group name">
						<label>URL</label>
						<input type="text" name="url" id="url" value="<?php echo $_POST["url"];?>" />
					</div>
					<div class="input-group name">
						<label>Email</label>
						<input type="email" name="r_email" id="email" value="<?php echo $_POST["email"];?>" required />
					</div>
					<div class="input-group password">
						<label>Password</label>
						<input type="password" name="r_password" id="password" pattern="^\S{6,}$" onchange="this.setCustomValidity(this.validity.patternMismatch ? 'Must have at least 6 characters' : ''); if(this.checkValidity()) form.cpassword.pattern = this.value;" required />
	
						<div class="password-strengh">
							Password strength:
							<span id="strength-area"></span>
						</div>
					</div>
					<div class="input-group password">
						<label>Confirm password</label>
						<input type="password" name="cpassword" id="cpassword" pattern="^\S{6,}$" onchange="this.setCustomValidity(this.validity.patternMismatch ? 'Please enter the same Password as above' : '');" required />
					</div>
					<button type="submit" class="btn btn-primary">Create an account</button>
					
					<?php
					$termsCat = new Category(335);
					$privacyCat = new Category(148);
					?>
					<p>By creating an account, I accept <br class="visible-xs">Descent’s <a href="<?php echo $termsCat->GetPath();?>"><?php echo $termsCat->GetName($this->m_displayLang);?></a> and <a href="<?php echo $privacyCat->GetPath();?>"><?php echo $privacyCat->GetName($this->m_displayLang);?></a></p>
                </form>
                
                <script type="text/javascript">
                $(document).ready(function(){
                	$("#password").keyup(function(){
                		if ( $(this).val()=="" )
                		{
                			$("#strength-area").removeAttr("class");
							$("#strength-area").text("");
                		}
                		else
                		{
                			checkPassStrength($(this).val());
                		}
                	});
                });
                
				function scorePassword(pass) {
					var score = 0;
					if ( !pass )
						return score;
					
					// award every unique letter until 5 repetitions
					var letters = new Object();
					for ( var i=0; i<pass.length; i++ )
					{
						letters[pass[i]] = (letters[pass[i]] || 0) + 1;
						score += 5.0 / letters[pass[i]];
					}
					
					// bonus points for mixing it up
					var variations = {
						digits: /\d/.test(pass),
						lower: /[a-z]/.test(pass),
						upper: /[A-Z]/.test(pass),
						nonWords: /\W/.test(pass),
					}
					
					variationCount = 0;
					for ( var check in variations ) {
						variationCount += (variations[check] == true) ? 1 : 0;
					}
					score += (variationCount - 1) * 10;
					
					return parseInt(score);
				}
				function checkPassStrength(pass) {
					var score = scorePassword(pass);
					
					if ( score>70 )
					{
						$("#strength-area").removeAttr("class");
						$("#strength-area").addClass("color-green");
						$("#strength-area").text("Strong");
						
					}
					else if ( score>40 )
					{
						$("#strength-area").removeAttr("class");
						$("#strength-area").addClass("color-blue");
						$("#strength-area").text("Good");
					}
					else
					{
						$("#strength-area").removeAttr("class");
						$("#strength-area").addClass("color-red");
						$("#strength-area").text("Week");
					}
					
					return "";
				}
                </script>
                <?php
                $_SESSION["FORMERROR"] = array();
            } 
            else 
            {
                $this->DisplayMessage();
            }
        }

        return false;
    }


    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplayProfile($catID, $prAll, $params)
    {
        global $session;
        
        $this->ReadNameStrings('langs/', 'registrationForm.xml', $this->m_displayLang);

        if ( $this->m_user->GetID() )
        {
            $fullname = $this->m_user->GetFullName();
            $url = $this->m_user->GetURL();
            $email = $this->m_user->GetEmail();
            $image = $this->m_user->GetImage();
            
            $notifications = $this->m_user->GetNotificationsSetting();
            $follow = $this->m_user->GetFollowSetting();
            $mention = $this->m_user->GetMentionSetting();
            $recommend = $this->m_user->GetRecommendSetting();
            $share = $this->m_user->GetShareSetting();

            // error message
            $errorMessageS = $this->GetNameString('warningAlert');
            ?>
			<div class="row">
				<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 text-center">
					<img src="images/avatar-large.png" alt="" class="photo-profile" width="300" height="300" />
					<h1 id="main-fullname-area"><?php echo $fullname;?></h1>
					<hr class="visible-xs"/>
				</div>
			</div>
			
			<div class="row">
				<div class="col-lg-8 col-lg-offset-2 col-md-10 col-md-offset-1 col-sm-10 col-sm-offset-1 col-xs-12 account blocks">
					<h2>Account</h2>
					<div class="row">
						<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 email relative">
							<label class="email pull-left text-label">Email:</label>
							<p id="email-area"><?php echo $email;?></p>
							<button type="button" class="btn btn-default pull-right" data-toggle="modal" data-target="#change-email">Change email</button>
						</div>
					</div>
					<div class="row">
						<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 name relative">
							<label class="email pull-left text-label">Name:</label>
							<p id="fullname-area"><?php echo $fullname;?></p>
							<button type="button" class="btn btn-default pull-right" data-toggle="modal" data-target="#change-name">Change name</button>
						</div>
					</div>
					<div class="row">
						<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 username relative">
							<label class="email pull-left text-label">URL:</label>
							<p id="url-area"><?php echo $url;?></p>
							<button type="button" class="btn btn-default pull-right" data-toggle="modal" data-target="#change-username">Change url</button>
						</div>
					</div>
					<div class="row">
						<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12 password relative">
							<label class="email pull-left text-label">Password:</label>
							<p>Password last changed over a month ago.</p>
							<button type="button" class="btn btn-default pull-right" data-toggle="modal" data-target="#change-password">Change password</button>
						</div>
					</div>
				</div>
			</div>
			
			<div class="row">
				<div class="col-lg-8 col-lg-offset-2 col-md-10 col-md-offset-1 col-sm-10 col-sm-offset-1 col-xs-12 email-settings blocks">
					<h2>Email settings</h2>
					<label>Email notifications:</label>
					<ul class="email-notifications">
						<li>We’ll email you when there’s notifications on your stories and publications. <input type="checkbox" id="notifications-checkbox" class="input-switcher"<?php echo ($notifications ? " checked": "");?> /></li>
						<li>We’ll email you when people follow you on Decent <input type="checkbox" id="follow-checkbox" class="input-switcher"<?php echo ($follow ? " checked": "");?> /></li>
						<li>We’ll email you when people mention you in their stories on Decent <input type="checkbox" id="mention-checkbox" class="input-switcher"<?php echo ($mention ? " checked": "");?> /></li>
						<li>We’ll email you with new stories written and recommended by people you follow on Decent <input type="checkbox" id="recommend-checkbox" class="input-switcher"<?php echo ($recommend ? " checked": "");?> /></li>
						<li>We’ll email you when we have exciting news to shareabout product features on Decent <input type="checkbox" id="share-checkbox" class="input-switcher"<?php echo ($share ? " checked": "");?> /></li>
					</ul>
				</div>
			</div>
			
			<?php
			$query = "select * from `user_tags` where `userID` = ".$this->m_user->GetID()." and `tagType` = 'author'";
			$retArr = DB::QueryToArray($query);
			if ( is_array($retArr) )
			{
				$authorCat = new Category(122);
				?>
				<div class="row">
					<div class="col-lg-8 col-lg-offset-2 col-md-10 col-md-offset-1 col-sm-10 col-sm-offset-1 col-xs-12 followed blocks">
						<h2>Followed</h2>
						<label>People:</label>
						<ul class="people">
							<?php
							for ( $i=0; $i<count($retArr); $i++ )
							{
								$tempU = new Site_user($retArr[$i]["tagID"]);
								?>
								<li>
									<img src="images/avatar-profile.png" alt="" class="" />
									<a href="<?php echo $authorCat->GetPath().$tempU->GetRandom();?>"><?php echo $tempU->GetFullName();?></a>
									<a id="unfollow" class="pull-right font-16" style="cursor:pointer;" data-id="<?php echo $tempU->GetID();?>">Un-follow</a>
								</li>
								<?php
							}
							?>
						</ul>
					</div>
				</div>
				<?php
			}
			?>
			
			<div class="row">
				<div class="col-lg-8 col-lg-offset-2 col-md-10 col-md-offset- col-sm-10 col-sm-offset-1 col-xs-12 connections blocks">
					<h2>Connections</h2>
					<label>Facebook:</label>
					<ul class="facebook">
						<li>Friends you have on Facebook who are also on Decent will become part of your Decent network. We will never post to Facebook or message your friends without your permission.</li>
						<li><a href="">Connect to Facebook</a></li>
					</ul>
					<label>Twitter:</label>
					<ul class="twitter">
						<li class="font-20">Connected <b><a href="" class="color-base">https://twitter.com/habitable_co</a></b></li>
						<li><a href="">Disconnect from Twitter</a></li>
					</ul>
				</div>
			</div>
            <?php
        }

        return false;
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplayUserProfile()
    {
        global $session;
        if (!isset($_SESSION["FORMERROR"])) {
            $_SESSION["FORMERROR"] = array();
        }

        $this->ReadNameStrings('langs/', 'registrationForm.xml', $this->m_displayLang);

        if ($this->m_user->GetID()) {
            $firstName = $this->m_user->GetFirstName();
            $lastName = $this->m_user->GetLastName();
            $nickName = $this->m_user->GetNickName();
            $phone = $this->m_user->GetPhone();
            $gender = $this->m_user->GetGender();
            $email = $this->m_user->GetEmail();
            $siteNameOpt = $this->m_user->GetSiteNameOpt();
            $image = $this->m_user->GetImage120();

            // error message
            $errorMessageS = $this->GetNameString('warningAlert');
            ?>
            <script type="text/javascript">
                $(document).ready(function () {
                    $("#registration").submit(function () {
                        var checking = true;

                        $("#fullName").removeAttr("style");
                        $("#rpassword").removeAttr("style");
                        $("#cpassword").removeAttr("style");

                        var fullName = $("#fullName").val();
                        if (fullName.length < 2) {
                            checking = false;
                            $("#fullName").css("border-bottom", "1px solid #a00");
                        }

                        var rpassword = $("#rpassword").val();
                        var cpassword = $("#cpassword").val();
                        if (rpassword.length > 0) {
                            if (rpassword.length < 6) {
                                checking = false;
                                $("#rpassword").css("border-bottom", "1px solid #a00");
                            }
                            else if (cpassword !== rpassword) {
                                checking = false;
                                $("#cpassword").css("border-bottom", "1px solid #a00");
                            }
                        }

                        return checking;
                    });
                });
            </script>

            <ul class="main-panel clearfix"><?php echo $this->DisplayMessage(); ?></ul>
            <ul class="main-container clearfix">
                <form class="pic-upload" action="" id="registration" method="post" name="registration"
                      enctype="multipart/form-data">
                    <input type="hidden" name="siteUserAction" value="changeuserinfo"/>
                    <div class="upload-img">
                        <div class="upload">
                            <label for="image" class="icon-camera">
                                <?php
                                if ($image) {
                                    ?>
                                    <img src="<?php echo $image; ?>" alt="">
                                    <?php
                                }
                                ?>
                            </label>
                            <input type="file" name="image" id="image">
                        </div>
                    </div>
                    <div class="profile-edit">
                        <label>
                            <span>Полное Имя</span>
                            <input type="text" name="fullName" id="fullName" class="account-inputs"
                                   value="<?php echo $firstName; ?>">
                        </label>
                        <label>
                            <h5 id="gender">Пол</h5>
                            <div class="radio">
                                <input id="radio-1" class="radio-custom" name="gender" type="radio"
                                       value="male"<?php echo($gender == "male" ? " checked" : ""); ?>>
                                <label for="radio-1" class="radio-custom-label">мужской</label>
                                <input id="radio-2" class="radio-custom" name="gender" type="radio"
                                       value="female"<?php echo($gender == "female" ? " checked" : ""); ?>>
                                <label for="radio-2" class="radio-custom-label">женский</label>
                            </div>
                        </label>
                        <label>
                            <span>Эл.почта</span>
                            <input type="email" name="email" id="email" class="account-inputs"
                                   value="<?php echo $email; ?>" readonly>
                        </label>
                        <label>
                            <span>Новый пароль</span>
                            <input type="password" name="rpassword" id="rpassword" class="account-inputs"
                                   data-placeholder="min 6 symbols">
                        </label>
                        <label>
                            <span>Подтверждение пароля</span>
                            <input type="password" name="cpassword" id="cpassword" class="account-inputs">
                        </label>
                        <button class="btn" value="">СОХРАНИТЬ</button>
                    </div>
                </form>
            </ul>
            <?php
            $_SESSION["FORMERROR"] = array();
        }

        return false;
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplayMyArticles($catID, $prAll, $params)
    {
        global $session;
        
        $this->ReadNameStrings('langs/', 'registrationForm.xml', $this->m_displayLang);

        if ( $this->m_user->GetID() )
        {
            $draftArticles = $this->m_user->GetMyArticles(0);
            $publicArticles = $this->m_user->GetMyArticles(1, 0);
            $privateArticles = $this->m_user->GetMyArticles(1, 1);
            ?>
			<img src="images/avatar-large.png" alt="" class="photo-profile" width="300" height="300" />
			<h1><?php echo $this->m_user->GetFullName();?></h1>
			
			<!-- Nav tabs -->
			<ul class="nav nav-tabs text-center" role="tablist">
				<li role="presentation"><a href="#drafts" aria-controls="drafts" role="tab" data-toggle="tab">Drafts<small> (<?php echo count($draftArticles);?>)</small></a></li>
				<li role="presentation" class="active"><a href="#public" aria-controls="public" role="tab" data-toggle="tab">Public<small> (<?php echo count($publicArticles);?>)</small></a></li>
				<li role="presentation"><a href="#private" aria-controls="private" role="tab" data-toggle="tab">Private<small> (<?php echo count($privateArticles);?>)</small></a></li>
			</ul>
			
			<!-- Tab panes -->
			<div class="tab-content">
				<div role="tabpanel" class="tab-pane" id="drafts">
					<?php
					for ( $i=0; $i<count($draftArticles); $i++ )
					{
						$tempPr = new Pr_main($draftArticles[$i]["ID"]);
						?>
						<div class="row">
							<div class="col-lg-2 col-md-2 col-sm-2 col-xs-12 info-article">
								<span class="city color-blue">Nevada Center</span>
								<i class="time"> - <?php echo ComputeDate($tempPr->GetPublishedDate());?> </i>
								<?php
								if ( $draftArticles[$i]["payment"]==0 )
								{
									?>
									<span class="btn btn-default">Free</span>
									<?php
								}
								?>
							</div>
							<div class="col-lg-10 col-md-10 col-sm-10 col-xs-12 title-article">
								<a href="<?php echo $tempPr->GetRandom();?>">
									<h2><?php echo $tempPr->GetName($this->m_displayLang);?></h2>
								</a>
								<?php
								if ( $tempPr->GetPicture1() )
								{
									?>
									<a href="<?php echo $tempPr->GetRandom();?>" class="title-img hidden-xs"><img src="<?php echo $tempPr->GetPicture1();?>" width="200" height="170"  class="pull-right"/></a>
									<?php
								}
								?>
							</div>
						</div>
						<?php
					}
					?>
				</div>
				<div role="tabpanel" class="tab-pane active" id="public">
					<?php
					for ( $i=0; $i<count($publicArticles); $i++ )
					{
						$tempPr = new Pr_main($publicArticles[$i]["ID"]);
						?>
						<div class="row">
							<div class="col-lg-2 col-md-2 col-sm-2 col-xs-12 info-article">
								<span class="city color-blue">Nevada Center</span>
								<i class="time"> - <?php echo ComputeDate($tempPr->GetPublishedDate());?> </i>
								<?php
								if ( $publicArticles[$i]["payment"]==0 )
								{
									?>
									<span class="btn btn-default">Free</span>
									<?php
								}
								?>
							</div>
							<div class="col-lg-10 col-md-10 col-sm-10 col-xs-12 title-article">
								<a href="<?php echo $tempPr->GetRandom();?>">
									<h2><?php echo $tempPr->GetName($this->m_displayLang);?></h2>
								</a>
								<?php
								if ( $tempPr->GetPicture1() )
								{
									?>
									<a href="<?php echo $tempPr->GetRandom();?>" class="title-img hidden-xs"><img src="<?php echo $tempPr->GetPicture1();?>" width="200" height="170"  class="pull-right"/></a>
									<?php
								}
								?>
							</div>
						</div>
						<?php
					}
					?>
				</div>
				<div role="tabpanel" class="tab-pane" id="private">
					<?php
					for ( $i=0; $i<count($privateArticles); $i++ )
					{
						$tempPr = new Pr_main($privateArticles[$i]["ID"]);
						?>
						<div class="row">
							<div class="col-lg-2 col-md-2 col-sm-2 col-xs-12 info-article">
								<span class="city color-blue">Nevada Center</span>
								<i class="time"> - <?php echo ComputeDate($tempPr->GetPublishedDate());?> </i>
								<?php
								if ( $privateArticles[$i]["payment"]==0 )
								{
									?>
									<span class="btn btn-default">Free</span>
									<?php
								}
								?>
							</div>
							<div class="col-lg-10 col-md-10 col-sm-10 col-xs-12 title-article">
								<a href="<?php echo $tempPr->GetRandom();?>">
									<h2><?php echo $tempPr->GetName($this->m_displayLang);?></h2>
								</a>
								<?php
								if ( $tempPr->GetPicture1() )
								{
									?>
									<a href="<?php echo $tempPr->GetRandom();?>" class="title-img hidden-xs"><img src="<?php echo $tempPr->GetPicture1();?>" width="200" height="170"  class="pull-right"/></a>
									<?php
								}
								?>
							</div>
						</div>
						<?php
					}
					?>
				</div>
			</div>
            <?php
        } 
        else 
        {
            header("location:".$this->m_site->GetSiteURL());
            exit();
        }

        return false;
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplayAddArticle($catID, $prAll, $params)
    {
        global $session;
        
        $this->ReadNameStrings('langs/', 'registrationForm.xml', $this->m_displayLang);

        if ( $this->m_user->GetID() )
        {
            if ( $this->m_user->GetStatus()==1 )
            {
            	?>
            	<div class="create-article">
            		<div class="row">
						<div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">
							<div class="box-notifications box-danger">
								<p>
									We sent a verification link to the email address <?php echo $this->m_user->GetEmail();?> you used to create the account.<br class="hidden-xs">
									If you don't verify your address, you may not be able to access certain Decent features.<br class="hidden-xs">
								</p>
								<a href="" class="block" id="resend">Re-send veritication link</a>
							</div>
						</div>
					</div>
            	</div>
            	<?php
                return null;
            }

            // error message
            $errorMessageS = $this->GetNameString('warningAlert');

            //	get all tags
            /*$optionsdatalist = new OptionsdataList(28);
            $optionsdatalistArr = $optionsdatalist->GetStructure();

            $optionsdatalistStr = "";
            if (is_array($optionsdatalistArr)) {
                $optionsdatalistArrNew = array();
                for ($i = 0; $i < count($optionsdatalistArr); $i++) {
                    $optionsdatalistArrNew[] = $optionsdatalistArr[$i]["name"];
                }

                $optionsdatalistStr = "'" . implode("', '", $optionsdatalistArrNew) . "'";
            }*/
            
            $licencelist = new OptionsdataList(2);
            $licencelistArr = $licencelist->GetStructure();
            ?>
			<div class="create-article">
				<form action="" method="POST" id="article" enctype="multipart/form-data">
					<input type="hidden" name="siteUserAction" value="addarticle"/>
					<input type="hidden" name="tags" id="tags" value="">
					<div class="row">
						<div class="col-lg-10 col-lg-offset-1 col-md-10 col-md-offset-1 col-sm-12 col-xs-12">
							<h2 class="h3"><strong>Title</strong></h2>
							<textarea rows="2" name="title" id="title" class="box" placeholder="type your title"></textarea>
						</div>
					</div>
					
					<div class="row">
						<div class="col-lg-10 col-lg-offset-1 col-md-10 col-md-offset-1 col-sm-12 col-xs-12">
							<h2 class="h3"><strong>Title image</strong></h2>
							<div class="article-image">
								<input type="file" name="image" id="image" />
								<button>Browse</button>
							</div>
						</div>
					</div>
					<script type="text/javascript">
					$(function(){
						// trigger open file upload
						$('.article-image button').on('click', function(){
							$('.article-image input').trigger('click');
						});
					});
					</script>
					
					<div class="row">
						<div class="col-lg-10 col-lg-offset-1 col-md-10 col-md-offset-1 col-sm-12 col-xs-12">
							<h2 class="h3"><strong>Tell your story</strong></h2>
							<div class="box">
								<textarea rows="15" name="content" id="content"></textarea>
							</div>
						</div>
					</div>
					
					<div class="article-settings">
						<div class="row">
							<div class="col-lg-10 col-lg-offset-1 col-md-10 col-md-offset-1 col-sm-12 col-xs-12">
								<div class="offset-side">
									<h2 class="h3"><strong>Article settings</strong></h2>
									<div class="row">
										<div class="col-lg-3 col-md-3 col-sm-3 col-xs-12">Author:</div>
										<div class="col-lg-8 col-md-8 col-sm-8 col-xs-12 small-xs"><?php echo $this->m_user->GetFullName();?> (you)</div>
									</div>
									<div class="row">
										<div class="col-lg-3 col-md-3 col-sm-3 col-xs-12" id="tagsTitle">Tags:</div>
										<div class="col-lg-7 col-md-7 col-sm-7 col-xs-12">
											<input type="text" data-role="tagsinput" placeholder="Add a tag..." />
											<p class="description">Add or change up to 3 tags so your story reaches more people.</p>
										</div>
									</div>
									<div class="row" id="visibility-area">
										<div class="col-lg-3 col-md-3 col-sm-3 col-xs-12">Visibility:</div>
										<div class="col-lg-7 col-md-7 col-sm-7 col-xs-9 small-xs">
											Private
											<p class="description">The story will only be visible to those with the link. It won’t be listed on Decent's public pages (your profile, homepage).</p>
										</div>
										<div class="col-lg-2 col-md-2 col-sm-2 col-xs-3 text-right">
											<input type="checkbox" class="input-switcher" name="private" id="private-checkbox" />
										</div>
									</div>
									<div class="row">
										<div class="col-lg-3 col-md-3 col-sm-3 col-xs-12" id="rightsTitle">License:</div>
										<div class="col-lg-7 col-md-7 col-sm-7 col-xs-12">
											<div class="radio-group">
												<?php
												for ( $i=0; $i<count($licencelistArr); $i++ )
												{
													?>
													<div><input type="radio" class="custom" name="rights" id="rights-<?php echo $licencelistArr[$i]["ID"];?>" value="<?php echo $licencelistArr[$i]["ID"];?>" /><label for="rights-<?php echo $licencelistArr[$i]["ID"];?>"><?php echo $licencelistArr[$i]["name"];?></label></div>
													<?php
												}
												?>
											</div>
											<p class="description">Others cannot copy, distribute, or perform your work without your permission (or as permitted by fair use).</p>
										</div>
									</div>
									<div class="row payment-method" id="payment-free-area">
										<div class="col-lg-3 col-md-3 col-sm-3 col-xs-12" id="paymentFreeTitle">Payment:</div>
										<div class="col-lg-7 col-md-7 col-sm-7 col-xs-12">
											Free
											<p class="description">Lorem ipsum dolor sit amet consectetur adipiscing elit dolomur consectetur lorem ipsum.</p>
										</div>
										<div class="col-lg-2 col-md-2 col-sm-2 col-xs-12 text-right position-top-100">
											<input type="checkbox" class="input-switcher" name="free" id="payment-free-checkbox" />
										</div>
									</div>
									<div class="row payment-method" id="payment-paid-area">
										<div class="col-lg-3 col-md-3 col-sm-3 col-xs-12" id="paymentPaidTitle">Payment:</div>
										<div class="col-lg-7 col-md-7 col-sm-7 col-xs-12 small-xs">
											Paid
											<div class="margin-top-20">
												<input type="text" name="price" id="price" class="width-130"/><select name="priceUnit" id="priceUnit" class="select2"><option val="usd">$</option><option val="euro">&euro;</option></select> <span>your fee</span>
											</div>
											<p class="description">Lorem ipsum dolor sit amet consectetur adipiscing elit dolomur consectetur lorem ipsum.</p>
										</div>
										<div class="col-lg-2 col-md-2 col-sm-2 col-xs-12 text-right position-top">
											<input type="checkbox" class="input-switcher" name="paid" id="payment-paid-checkbox" />
										</div>
									</div>
									<div class="submit">
										<input type="submit" class="btn btn-primary" value="Publish" />
										<span class="description">Saved to Drafts 2 minutes ago</span>
									</div>
								</div>
							</div>
						</div>
						<div class="text-center">
							<p>You will pay a 0,1 DCT transaction fee for publishing this content. <a href=""><small>What's this?</small></a></p>
						</div>
					</div>
				</form>
			</div>
            <?php
        }

        return false;
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplayEditArticle()
    {
        global $session;
        if (!isset($_SESSION["FORMERROR"])) {
            $_SESSION["FORMERROR"] = array();
        }

        $this->ReadNameStrings('langs/', 'registrationForm.xml', $this->m_displayLang);

        if ($this->m_user->GetID()) {
            if ($this->m_user->GetAuthor() == 0) {
                $settingsCat = new Category(354);
                header("location:" . $this->m_site->GetSiteURL() . $settingsCat->GetPath());
                exit();
            }

            $articleID = intval($_GET["id"]);
            $tempPr = new Pr_main($articleID);

            // error message
            $errorMessageS = $this->GetNameString('warningAlert');

            //	get all tags
            $optionsdatalist = new OptionsdataList(28);
            $optionsdatalistArr = $optionsdatalist->GetStructure();

            $optionsdatalistStr = "";
            if (is_array($optionsdatalistArr)) {
                $optionsdatalistArrNew = array();
                for ($i = 0; $i < count($optionsdatalistArr); $i++) {
                    $optionsdatalistArrNew[] = $optionsdatalistArr[$i]["name"];
                }
                $optionsdatalistStr = '"' . implode('", "', $optionsdatalistArrNew) . '"';
            }
            ?>
            <script type="text/javascript">
                var sampleTags = [<?php echo $optionsdatalistStr;?>];
                $(document).ready(function () {
                    if ($("#articleStatus").val() == 0) {
                        $("#publish-btn").fadeIn(500);
                    }
                });
            </script>
            <input type="hidden" name="articleRandom" id="articleRandom" value="<?php echo $tempPr->GetRandom(); ?>"/>
            <input type="hidden" name="articleStatus" id="articleStatus" value="<?php echo $tempPr->GetStatus(); ?>"/>
            <div class="cover clearfix">
                <div class="post col-lg-12 clearfix">
                    <div class="pic">
                        <span class="vh-centered">VP</span>
                    </div>
                    <div class="post-name">
                        <div class="vertical-center">
                            <h4><?php echo $this->m_user->GetFirstName(); ?></h4>
                            <div class="grey-line"></div>
                            <div id="title" contenteditable="true"
                                 placeholder="Заглавие"><?php echo $tempPr->GetName($this->m_displayLang); ?></div>
                            <div class="view-date">
                                <span class="date"><i
                                        class="icon-uploaded"></i><?php echo ComputeDate($tempPr->GetPublishedDate()); ?></span>
								<span class="view">
									<i class="icon-views"></i>
									<span><?php echo $tempPr->GetGoogleUniqueViewCount(); ?></span>
								</span>
                            </div>
                            <?php
                            $tags = $tempPr->GetFieldValue(71, $this->m_displayLang, true);
                            ?>
                            <ul id="removeConfirmationTags">
                                <?php
                                for ($i = 0; $i < count($tags); $i++) {
                                    ?>
                                    <li><?php echo $tags[$i]; ?></li>
                                    <?php
                                }
                                ?>
                            </ul>
                        </div>
                    </div>
                    <div class="edit">
                        <div class="previous-btn" title="Previous">
                            <img src="profilepage/img/icos/arrow-left.svg" alt="">
                        </div>
                        <div class="next-btn" title="Next">
                            <img src="profilepage/img/icos/arrow-right.svg" alt="">
                        </div>
                        <div class="upload">
                            <i class="icon-camera"></i>
                            <div>
                                <input class="upload-cover" type='file' name="coverImage"/>
                            </div>
                        </div>
                        <div class="switch">
                            <input id="switch-1" type="checkbox" disabled class="switch-input">
                            <label for="switch-1" class="switch-label"></label>
                        </div>
                    </div>
                </div>
            </div>
            <div class="carousel">
                <div id="owl-cover" class="owl-carousel">
                    <?php
                    if ($tempPr->GetCover() && $tempPr->GetCover() != "img/cover.gif") {
                        ?>
                        <div class="item"
                             style="background: transparent url('<?php echo $tempPr->GetCover(); ?>') no-repeat scroll center center;"
                             data-cover=""></div>
                        <?php
                    }
                    ?>
                </div>
            </div>
            <?php
            $desc = $tempPr->GetDesc($this->m_displayLang);
            while ($gallPos = strpos($desc, "<!--gallery")) {
                $gallPosEnd = strpos($desc, "-->", $gallPos);
                $gallGroup = intval(substr($desc, $gallPos + 11, $gallPosEnd));

                $gallery = new Pr_pictureList($tempPr->GetID(), $gallGroup);
                $galleryArr = $gallery->GetStructure();
                $toReplace = '';
                for ($i = 0; $i < count($galleryArr); $i++) {
                    $toReplace .= '
						<div class="medium-insert-images" style="">
							<figure contenteditable="false" style="">
								<img alt="" src="' . $galleryArr[$i]["path"] . '" data-image="' . $galleryArr[$i]["ID"] . '">
								<div class="medium-insert-images-progress"></div>
							</figure>
						</div>';
                }

                $desc = str_replace("<!--gallery$gallGroup-->", $toReplace, $desc);
            }

            while ($videoPos = strpos($desc, "<!--video")) {
                $videoPosEnd = strpos($desc, "-->", $videoPos);
                $videoGroup = intval(substr($desc, $videoPos + 11, $videoPosEnd));

                $video = new Pr_videoList($tempPr->GetID(), $videoGroup);
                $videoArr = $video->GetStructure();
                $toReplace = '';
                for ($i = 0; $i < count($videoArr); $i++) {
                    $toReplace .= '
						<div contenteditable="false" class="medium-insert-embeds">
							<figure>
								<div class="medium-insert-embed">
									<div style="max-width:320px;margin:auto;">
										<div>
											<div style="left: 0px; width: 100%; height: 0px; position: relative; padding-bottom: 56.2493%;">
												<iframe frameborder="0" style="top: 0px; left: 0px; width: 100%; height: 100%; position: absolute;" mozallowfullscreen="true" webkitallowfullscreen="true" allowfullscreen="true" src="' . $videoArr[$i]["link"] . '?wmode=transparent&amp;rel=0&amp;autohide=1&amp;showinfo=0&amp;enablejsapi=1"></iframe>
											</div>
										</div>
									</div>
								</div>
							</figure>
							<div class="medium-insert-embeds-overlay"></div>
						</div>';
                }

                $desc = str_replace("<!--video$videoGroup-->", $toReplace, $desc);
            }
            ?>
            <div class="content col-lg-12 clearfix">
                <div class="type-text col-lg-8 col-md-8">
                    <div class="description" contenteditable="true"
                         placeholder="Краткое описание"><?php echo $tempPr->GetDescShort($this->m_displayLang); ?></div>
                    <div class="editable" data-placeholder="Текст" placeholder="Текст">
                        <?php echo $desc; ?>
                    </div>
                </div>
                <div class="anons col-lg-4 col-md-4">
                    <?php
                    if ($tempPr->GetPicture2() && $tempPr->GetPicture2() != "profilepage/img/lion.png") {
                        $subscriptionHasNoImage = "none";
                        $subscriptionHasImage = "block";
                    } else {
                        $subscriptionHasNoImage = "block";
                        $subscriptionHasImage = "none";
                    }
                    ?>
                    <div class="clearfix">
                        <div>Анонс материала в списках</div>
                        <div class="edit">
                            <div class="switch-anons">
                                <input id="switch-2" type="checkbox"
                                       class="switch-input" <?php echo($subscriptionHasImage == "block" ? "checked" : "disabled"); ?>>
                                <label for="switch-2" class="switch-label"></label>
                            </div>
                            <div class="upload">
                                <i class="icon-camera"></i>
                                <div>
                                    <input class="upload-anons" type='file'/>
                                </div>
                            </div>
                            <div class="next-btn-anons" title="Next">
                                <img src="profilepage/img/icos/arrow-right.svg" alt="">
                            </div>
                            <div class="previous-btn-anons" title="Previous">
                                <img src="profilepage/img/icos/arrow-left.svg" alt="">
                            </div>
                        </div>
                    </div>
                    <div class="subscription-has-no-image" style="display:<?php echo $subscriptionHasNoImage; ?>;">
                        <p><span class="tagit-p">Tema</span></p>
                        <span class="circle-img">
                            <img src="profilepage/img/lion.png" class="img-circle no-pic" alt="no-img">
                            <span class="single"></span>
                        </span>
                        <h4><?php echo $tempPr->GetName($this->m_displayLang); ?></h4>
                        <span>
                            <a href="">
                                <span>PV</span>
                            </a>
                            <a class="author" href="#"><?php echo $this->m_user->GetFirstName(); ?></a>
                        </span>
                        <div>
                            <span class="date"><i class="icon-uploaded"></i>Дата публикации</span>
							<span class="view">
								<i class="icon-views"></i>
								<span>0</span>
							</span>
                        </div>
                    </div>
                    <div class="subscription-has-image" style="display:<?php echo $subscriptionHasImage; ?>;">
                        <div class="view-date">
                            <span class="date"><i class="icon-uploaded"></i>26 Октябрь 2015</span>
							<span class="view">
								<i class="icon-views"></i>
								<span>1234</span>
							</span>
                        </div>
                        <div class="image">
                            <div class="overlay-subscription-img"></div>
                            <div id="owl-demo" class="owl-carousel">
                                <?php
                                if ($tempPr->GetPicture2() && $tempPr->GetPicture2() != "profilepage/img/lion.png") {
                                    ?>
                                    <div><img src="<?php echo $tempPr->GetPicture2(); ?>" alt="" data-picture="">
                                    </div>
                                    <?php
                                }
                                ?>
                            </div>
                        </div>
                    </div>
                    <div class="subscription-title clearfix" style="display:<?php echo $subscriptionHasImage; ?>;">
                        <p><span class="tagit-p">ТЕМА</span></p>
                        <h4><?php echo $tempPr->GetName($this->m_displayLang); ?></h4>

                        <div class="pic">
                            <a href="#">
                                <span>PV</span>
                            </a>
                        </div>
                        <div class="author-name">
                            <span>
                                <a class="author" href="#"><?php echo $this->m_user->GetFirstName(); ?></a>
                            </span>
                        </div>
                    </div>
                    <div class="sourse" contenteditable="true"
                         placeholder="Источник материала"><?php echo $tempPr->GetFieldValue(70, $this->m_displayLang, true); ?></div>
                    <div class="link" contenteditable="true" placeholder="Ссылка"></div>
                    <div class="status">
                        <div class="switch-status">
                            <input id="switch-3" type="checkbox" class="switch-input">
                            <label for="switch-3" class="switch-label"></label>
                        </div>
                        <div>Материал для взрослых</div>
                    </div>
                </div>
            </div>
            <div style="display:none;">
                <input class="upload-image" type='file'/>
            </div>
            <?php
        }

        return false;
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplaySubscription($catID, $prAll, $params)
    {
        if ($this->m_user->GetID()) {
            //	display menu
            $mySubscriptionCat = new Category(347);
            $mySubscriptionSubCat = array();
            $mySubscriptionCat->GetSubIDs($mySubscriptionSubCat);
            ?>
            <style type="text/css">
                .main-panel li a {
                    cursor: pointer;
                }
            </style>
            <div class="main-panel-border">
                <ul class="main-panel clearfix">
                    <?php
                    for ($i = 0; $i < count($mySubscriptionSubCat); $i++) {
                        $tempCat = new Category($mySubscriptionSubCat[$i]);
                        $activeClass = '';
                        if ($i == 0) {
                            $activeClass = ' class="activ-panel"';
                        }

                        if ($this->m_user->GetAuthor() || $mySubscriptionSubCat[$i] != 349) {
                            ?>
                            <li><a <?php echo $activeClass; ?>
                                    data-path="<?php echo $tempCat->GetPathName(); ?>"><?php echo $tempCat->GetName($this->m_displayLang); ?></a>
                            </li>
                            <?php
                        }
                    }
                    ?>
                </ul>
                <div class="search">
                    <i class="icon-search"></i>
                    <i class="icon-close"></i>
                    <input type="text" id="search-field" name="search" placeholder="Поиск...">
                </div>
            </div>
            <?php
            $articlesCat = new Category(124);
            $authorCat = new Category(122);

            $mySubscribtionArticles = $this->m_user->GetMyFollowsArticles();
            $mySubscribtionArticlesCount = count($this->m_user->GetMyFollowsArticles("0, 100000"));
            $mySubscribers = $this->m_user->GetMyFollowers();
            $mySubscribtion = $this->m_user->GetMyFollows(null, "author");

            $tempCat = new Category($mySubscriptionSubCat[0]);
            ?>
            <div class="main-container clearfix" id="<?php echo $tempCat->GetPathName(); ?>">
                <?php
                if (is_array($mySubscribtionArticles)) {
                    ?>
                    <ul class="grid effect-2" id="grid">
                        <?php
                        for ($i = 0; $i < count($mySubscribtionArticles); $i++) {
                            $tempPr = new Pr_main($mySubscribtionArticles[$i]["ID"]);
                            $image = $tempPr->GetPicture2();
                            if ($image == "") {
                                $image = $tempPr->GetPicture();
                            }

                            $tags = $tempPr->GetFieldValue(71, $this->m_displayLang, false);
                            $valueT = $tags;
                            $tags = '';
                            foreach ($valueT as $val) {
                                $tempOD = new Optionsdata($val);
                                $tags .= '<a href="' . $siteURL . $articlesCat->GetPath() . $tempOD->GetKey() . '">' . $tempOD->GetName($this->m_displayLang) . ',</a> ';
                            }
                            $tags = substr($tags, 0, -1);

                            $authorLink = "<a class='author' href='" . $siteURL . $authorCat->GetPath() . $tempPr->GetAuthorRandom() . "'>" . $tempPr->GetAuthorSiteName() . "</a>";
                            $authorName = $tempPr->GetAuthorSiteName();
                            $authorName = explode(" ", $authorName);
                            $anFirstLetters = substr($authorName[0], 0, 1);
                            if ($authorName[1]) {
                                $anFirstLetters .= substr($authorName[1], 0, 1);
                            }

                            if ($image == "" || $image == "profilepage/img/lion.png") {
                                ?>
                                <li class="grid-item">
                                    <div class="subscription-has-no-image">
                                        <p><?php echo $tags; ?></p>
                                        <a href="<?php echo $tempPr->GetRandom(); ?>">
											<span class="circle-img">
												<img src="profilepage/img/lion.png" class="no-pic" alt="no-img">
												<span class="single"></span>
											</span>
                                        </a>
                                        <h4>
                                            <a href="<?php echo $tempPr->GetRandom(); ?>"><?php echo $tempPr->GetName($this->m_displayLang); ?></a>
                                        </h4>
                                        <span><a
                                                href=""><span><?php echo $anFirstLetters; ?></span></a><?php echo $authorLink; ?></span>
                                        <div>
                                            <span class="date"><i
                                                    class="icon-uploaded"></i><?php echo ComputeDate($tempPr->GetPublishedDate()); ?></span>
                                            <span class="view"><i
                                                    class="icon-views"></i><span><?php echo $tempPr->GetGoogleUniqueViewCount(); ?></span></span>
                                        </div>
                                    </div>
                                </li>
                                <?php
                            } else {
                                ?>
                                <li class="grid-item">
                                    <div class="subscription-has-image">
                                        <div class="view-date">
                                            <span class="date"><i
                                                    class="icon-uploaded"></i><?php echo ComputeDate($tempPr->GetPublishedDate()); ?></span>
                                            <span class="view"><i
                                                    class="icon-views"></i><span><?php echo $tempPr->GetGoogleUniqueViewCount(); ?></span></span>
                                        </div>
                                        <div class="image">
                                            <div class="overlay-subscription-img"></div>
                                            <a href="<?php echo $tempPr->GetRandom(); ?>"><img
                                                    src="<?php echo $image; ?>"></a>
                                        </div>
                                    </div>
                                    <div class="subscription-title">
                                        <p><?php echo $tags; ?></p>
                                        <h4>
                                            <a href="<?php echo $tempPr->GetRandom(); ?>"><?php echo $tempPr->GetName($this->m_displayLang); ?></a>
                                        </h4>
                                        <div class="pic">
                                            <a href="<?php echo $tempPr->GetRandom(); ?>"><span><?php echo $anFirstLetters; ?></span></a>
                                        </div>
                                        <div class="author-name">
                                            <span><?php echo $authorLink; ?></span>
                                        </div>
                                    </div>
                                </li>
                                <?php
                            }
                        }
                        ?>
                        <li class="gutter-sizer"></li>
                    </ul>

                    <div class="more" style="display:none;">
                        <a data-maxpage="<?php echo ceil($mySubscribtionArticlesCount / count($mySubscribtionArticles)); ?>"
                           data-page="1" style="cursor:pointer;">ЕЩЕ</a>
                    </div>
                    <?php
                } else {
                    ?>
                    <div class="empty">
                        <img src="profilepage/img/no-materials.svg" alt="">
                        <p>Материалов нет</p>
                    </div>
                    <?php
                }
                ?>
            </div>

            <?php
            $tempCat = new Category($mySubscriptionSubCat[1]);
            ?>
            <ul class="main-container clearfix" id="<?php echo $tempCat->GetPathName(); ?>" style="display:none;">
                <?php
                if (count($mySubscribers) > 0) {
                    for ($i = 0; $i < count($mySubscribers); $i++) {
                        $tempUser = new Site_user($mySubscribers[$i]["userID"]);
                        ?>
                        <li class="col-sm-6 sub">
                            <div class="post closed clearfix">
                                <div class="pic">
                                    <img src="profilepage/img/lion.png" alt="" class="no-pic">
                                    <span class="single">a</span>
                                </div>
                                <div class="post-name ">
                                    <div class="vertical-center">
                                        <h4><a href="#"><?php echo $tempUser->GetFirstName(); ?></a></h4>
                                        <span class="date">инфо о ирине</span>
                                    </div>
                                </div>
                                <div class="edit">

                                </div>
                            </div>
                        </li>
                        <?php
                    }
                } else {
                    ?>
                    <div class="empty">
                        <img src="profilepage/img/no-subscibes.svg" alt="">
                        <p>У вас нет подписчиков</p>
                    </div>
                    <?php
                }
                ?>
            </ul>

            <?php
            $tempCat = new Category($mySubscriptionSubCat[2]);
            ?>
            <ul class="main-container clearfix" id="<?php echo $tempCat->GetPathName(); ?>" style="display:none;">
                <?php
                if (count($mySubscribtion) > 0) {
                    for ($i = 0; $i < count($mySubscribtion); $i++) {
                        $tempUser = new Site_user($mySubscribtion[$i]["tagID"]);
                        $image = $tempUser->GetImage120();
                        if (!$image) {
                            $image = "profilepage/img/lion.png";
                        }
                        ?>
                        <li class="col-sm-6 sub">
                            <div class="post closed clearfix">
                                <div class="pic">
                                    <img src="<?php echo $image; ?>" alt="" class="no-pic"/>
                                    <span class="single">a</span>
                                </div>
                                <div class="post-name ">
                                    <div class="vertical-center">
                                        <h4><a href="#"><?php echo $tempUser->GetFirstName(); ?></a></h4>
                                        <span class="date">инфо о ирине</span>
                                    </div>
                                </div>
                                <div class="edit">
                                    <a href="#" class="delete-item"><i class="icon-delete"></i></a>
                                </div>
                            </div>
                        </li>
                        <?php
                    }
                } else {
                    ?>
                    <div class="empty">
                        <img src="profilepage/img/no-subscibes.svg" alt="">
                        <p>У вас нет подписок</p>
                    </div>
                    <?php
                }
                ?>
            </ul>

            <script type="text/javascript">
                $(document).ready(function () {
                    var conteiner;
                    $(".main-panel li a").click(function () {
                        $(".main-panel li a").removeClass("activ-panel");
                        $(this).addClass("activ-panel");

                        var ulID = $(this).data("path");

                        $(".main-container").fadeOut(0);
                        if ($("#" + ulID).length > 0) {
                            $("#" + ulID).fadeIn(200);
                        }

                        $("#search-field").val('');

                        if (conteiner) {
                            $(conteiner).find('li').fadeIn(0);
                        }
                    });

                    $.expr[':'].icontains = function (a, i, m) {
                        return jQuery(a).text().toUpperCase()
                                .indexOf(m[3].toUpperCase()) >= 0;
                    };

                    $("#search-field").keyup(function () {
                        var searchText = $(this).val();
                        var items = [];

                        //	get active conteiner
                        $(".main-container").each(function () {
                            if ($(this).css('display') == "block") {
                                conteiner = $(this);
                            }
                        });

                        if (searchText == "") {
                            $(conteiner).find('li').fadeIn(0);
                        }
                        else {
                            var next = 0;
                            $(conteiner).find('li').each(function () {
                                if ($(this).find(":icontains('" + searchText + "')").length > 0) {
                                    $(this).fadeIn(0);
                                }
                                else {
                                    $(this).fadeOut(0);
                                }
                            });
                        }

                        if ($(conteiner).attr('id') == 'news-feed') {
                            $grid.masonry('reloadItems');
                            $grid.masonry('layout');
                        }
                    });

                    if ($(".more a").data("maxpage") > 1) {
                        $(".more").fadeIn(0);
                    }
                });
            </script>
            <?php
        } else {
            header("location:" . $this->m_site->GetSiteURL());
            exit();
        }
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplayStatistics($catID, $prAll, $params)
    {
        if ( $this->m_user->GetID() )
        {
            if ( $this->m_user->GetAuthor()==0 )
            {
                $settingsCat = new Category(354);
                header("location:" . $this->m_site->GetSiteURL() . $settingsCat->GetPath());
                exit();
            }
            
            $rating = $this->m_user->GetMyRating();
            $share = $this->m_user->GetMyShare();
            $followers = $this->m_user->GetMyFollowers();
            $articles = $this->m_user->GetMyArticles(1, "0, 3");
            $dayFrom = time() - 7*86400;
            $statistics = $this->m_user->GetMyStats($dayFrom);
            
            $statDateFrom = 0;
            $statDateTo = 0;
            if ( is_array($statistics) )
            {
            	$statDateFrom = $statistics[0]["date"];
            	$statDateTo = $statistics[count($statistics)-1]["date"];
            }
            ?>
			<div class="main-container row">
				<div class="col-sm-6">
					<div id="my-chart1" class="graphics">
						<div>
							<div><a href="#">Доля</a> <span><?php echo $share;?></span></div>
							<div><span><span><?php echo date("d", $statDateFrom);?></span> <span><?php echo date("M", $statDateFrom);?></span></span> - <span><?php echo date("d", $statDateTo);?></span> <span><?php echo date("M", $statDateTo);?></span></div>
						</div>
					</div>
				</div>
				<div class="col-sm-6">
					<div id="my-chart2" class="graphics">
						<div>
							<div><a href="#">Рейтинг</a> <span><?php echo $rating;?></span></div>
							<div><span><span><?php echo date("d", $statDateFrom);?></span> <span><?php echo date("M", $statDateFrom);?></span></span> - <span><?php echo date("d", $statDateTo);?></span> <span><?php echo date("M", $statDateTo);?></span></div>
						</div>
					</div>
				</div>
				<div class="col-sm-4">
					<div class="account-materials">
						<h3>Материалы</h3>
						<div class="scroll" data-simplebar-direction="vertical">
							<?php
							for ( $i=0; $i<count($articles); $i++ )
							{
								$tempPr = new Pr_main($articles[$i]["ID"]);
								?>
								<div class="account-item">
									<h3><a href="<?php echo $tempPr->GetRandom();?>"><?php echo $tempPr->GetName($this->m_displayLang);?></a></h3>
									<div class="view-date">
										<span class="date">13 сентября, 2016</span>
										<span class="view">
											<i class="icon-views"></i>
											<span><i class="icont-uploaded"></i><?php echo $tempPr->GetGoogleUniqueViewCount();?></span>
										</span>
									</div>
								</div>
								<?php
							}
							?>
						</div>
					</div>
				</div>
				<div class="col-sm-4">
					<div class="account-notifications">
						<h3>Уведомления <span>+1</span></h3>
						<div class="scroll" data-simplebar-direction="vertical">
							<div class="account-item">
								<p class="unread">Речь идет, в первую очередь, о двенадцати добровольцах, которые принимали участие в боевых действиях в Донбассе, но</p>
								<div class="view-date">
									<span class="date">13 сентября, 2016</span>
								</div>
							</div>
							<div class="account-item">
								<p>Речь идет, в первую очередь, о двенадцати добров участие в боевых действиях в Донбассе, но</p>
								<div class="view-date">
									<span class="date">13 сентября, 2016</span>
								</div>
							</div>
						</div>
					</div>
				</div>
				<div class="col-sm-4">
					<div class="account-subsqribers">
						<h3>Подписчики</h3>
						<div class="scroll" data-simplebar-direction="vertical">
							<?php
							for ( $i=0; $i<count($followers); $i++ )
							{
								$tempU = new Site_user($followers[$i]["userID"]);
								$followerImage = "profilepage/img/zara.jpg";
								if ( $tempU->GetImage() )
								{
									$followerImage = $tempU->GetImage();
								}
								?>
								<div class="account-item">
									<div><img src="<?php echo $followerImage;?>" alt="<?php echo $tempU->GetFirstName();?>"></div>
									<h3><a href=""><?php echo $tempU->GetFirstName();?></a></h3>
								</div>
								<?php
							}
							?>
						</div>
					</div>
				</div>
			</div>

			<?php
			$ratingsStat = array();
			$shareStat = array();
			$datesStat = array();
			for ( $i=0; $i<count($statistics); $i++ )
			{
				$ratingsStat[] = $statistics[$i]["rating"];
				$shareStat[] = $statistics[$i]["percentage"];
				$datesStat[] = date("d M", $statistics[$i]["date"]);
			}
			
			$ratingsStatImplode = "'".implode("', '", $ratingsStat)."'";
			$shareStatImplode = "'".implode("', '", $shareStat)."'";
			$datesStatImplode = "'".implode("', '", $datesStat)."'";
			?>
			<script type="text/javascript" src="profilepage/lib/simpleScroll/simplebar.min.js"></script>
			<script type="text/javascript" src="profilepage/lib/chartist/chartist.js"></script>
			<script type="text/javascript">
			    var data1 = {
			        labels: [<?php echo $datesStatImplode;?>],
			        series: [
			            {
			                data: [<?php echo $shareStatImplode;?>]
			            }
			        ]
			    };
			    var data2 = {
			        labels: [<?php echo $datesStatImplode;?>],
			        series: [
			            {
			                data: [<?php echo $ratingsStatImplode;?>]
			            }
			        ]
			    };
			
			    /* Set some base options (settings will override the default settings in Chartist.js *see default settings*). We are adding a basic label interpolation function for the xAxis labels. */
			    var options = {
			        height: '400px',
			        axisX: {
			            labelInterpolationFnc: function (value) {
			                return value;
			            }
			        },
			        axisY: {
			            scaleMinSpace: 50
			        },
			        chartPadding: {
			            top: 80,
			            right: 40,
			            bottom: 10,
			            left: 10
			        },
			        fullWidth: true
			    };
			
			    /* Now we can specify multiple responsive settings that will override the base settings based on order and if the media queries match. In this example we are changing the visibility of dots and lines as well as use different label interpolations for space reasons. */
			    var responsiveOptions = [
			        ['screen and (min-width: 641px) and (max-width: 1024px)', {
			            showPoint: false,
			            axisX: {
			                labelInterpolationFnc: function (value) {
			                    return 'Week ' + value;
			                }
			            }
			        }],
			        ['screen and (max-width: 640px)', {
			            showLine: false,
			            axisX: {
			                labelInterpolationFnc: function (value) {
			                    return 'W' + value;
			                }
			            }
			        }]
			    ];
				
			    /* Initialize the chart with the above settings */
			    new Chartist.Line('#my-chart1', data1, options, responsiveOptions);
			    new Chartist.Line('#my-chart2', data2, options, responsiveOptions);
			</script>
            <?php
        } 
        else
        {
            header("location:" . $this->m_site->GetSiteURL());
            exit();
        }
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplayNotifications($catID, $prAll, $params)
    {
        if ($this->m_user->GetID()) {
            //	display menu
            $mySubscriptionCat = new Category(342);
            $mySubscriptionSubCat = array();
            $mySubscriptionCat->GetSubIDs($mySubscriptionSubCat);
            ?>
            <style type="text/css">
                .main-panel li a {
                    cursor: pointer;
                }
            </style>
            <div class="main-panel-border">
                <ul class="main-panel clearfix">
                    <?php
                    for ($i = 0; $i < count($mySubscriptionSubCat); $i++) {
                        $tempCat = new Category($mySubscriptionSubCat[$i]);
                        $activeClass = '';
                        if ($i == 0) {
                            $activeClass = ' class="activ-panel"';
                        }
                        ?>
                        <li><a <?php echo $activeClass; ?>
                                data-path="<?php echo $tempCat->GetPathName(); ?>"><?php echo $tempCat->GetName($this->m_displayLang); ?></a>
                        </li>
                        <?php
                    }
                    ?>
                </ul>
                <div class="search">
                    <i class="icon-search"></i>
                    <i class="icon-close"></i>
                    <input type="text" id="search-field" name="search" placeholder="Поиск...">
                </div>
            </div>

            <script type="text/javascript">
                $(document).ready(function () {
                    $(".main-panel li a").click(function () {
                        $(".main-panel li a").removeClass("activ-panel");
                        $(this).addClass("activ-panel");
                    });
                });
            </script>
            <?php
        } else {
            header("location:" . $this->m_site->GetSiteURL());
            exit();
        }
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function DisplayDashbord($catID, $prAll, $params)
    {
        if ($this->m_user->GetID()) {
            if ($this->m_user->GetAuthor() == 0) {
                $settingsCat = new Category(354);
                header("location:" . $this->m_site->GetSiteURL() . $settingsCat->GetPath());
                exit();
            }

            //	display menu
//			$this->DisplayAuthorMenu($catID);
            $articlesArr1 = $this->m_user->GetMyArticles(1);

            $authorName = $this->m_user->GetFirstName() . " " . $this->m_user->GetLastName();
            $authorName = explode(" ", $authorName);
            $anFirstLetters = substr($authorName[0], 0, 1);
            if ($authorName[1]) {
                $anFirstLetters .= substr($authorName[1], 0, 1);
            }

            $articlesCat = new Category(124);
            ?>
            <div class="main-container clearfix">
                <ul class="grid effect-2" id="grid">
                    <?php
                    if (count($articlesArr1) > 0) {
                        for ($i = 0; $i < count($articlesArr1); $i++) {
                            $tempPr = new Pr_main($articlesArr1[$i]["ID"]);
                            $tags = $tempPr->GetFieldValue(71, $this->m_displayLang, true);

                            $pDate = $tempPr->GetPublishedDate();

                            $tags = $tempPr->GetFieldValue(71, $lang, false);
                            $valueT = $tags;
                            $tags = '';
                            foreach ($valueT as $val) {
                                $tempOD = new Optionsdata($val);
                                $tags .= '<a href="' . $siteURL . $articlesCat->GetPath() . $tempOD->GetKey() . '">' . $tempOD->GetName($this->m_displayLang) . '</a>, ';
                            }
                            $tags = substr($tags, 0, -2);

                            if ($tempPr->GetPicture()) {
                                ?>
                                <li class="grid-item">
                                    <div class="subscription-has-image">
                                        <div class="view-date">
                                            <span class="date"><i
                                                    class="icon-uploaded"></i><?php echo ComputeDate($pDate); ?></span>
										<span class="view">
											<i class="icon-views"></i>
											<span><?php echo $tempPr->GetGoogleUniqueViewCount(); ?></span>
										</span>
                                        </div>
                                        <div class="image">
                                            <div class="overlay-subscription-img"></div>
                                            <a href="#"><img src="<?php echo $tempPr->GetPicture(); ?>"></a>
                                        </div>
                                    </div>
                                    <div class="subscription-title">
                                        <p><?php echo $tags; ?></p>
                                        <h4>
                                            <a href="<?php echo $tempPr->GetRandom(); ?>"><?php echo $tempPr->GetName($this->m_displayLang); ?></a>
                                        </h4>
                                        <div class="clearfix"></div>
                                    </div>
                                </li>
                                <?php
                            } else {
                                ?>
                                <li class="grid-item">
                                    <div class="subscription-has-no-image">
                                        <p><?php echo $tags; ?></p>
                                        <a href="#">
										<span class="circle-img">
											<img src="profilepage/img/lion.png" class="no-pic" alt="no-img">
											<span class="single"></span>
										</span>
                                        </a>
                                        <h4>
                                            <a href="<?php echo $tempPr->GetRandom(); ?>"><?php echo $tempPr->GetName($this->m_displayLang); ?></a>
                                        </h4>
										<span>
											<a href=""><span><?php echo $anFirstLetters; ?></span></a>
											<a class="author"
                                               href="#"><?php echo $this->m_user->GetFirstName() . " " . $this->m_user->GetLastName(); ?></a>
										</span>
                                        <div>
                                            <span class="date"><i
                                                    class="icon-uploaded"></i><?php echo ComputeDate($pDate); ?></span>
											<span class="view">
												<i class="icon-views"></i>
												<span><?php echo $tempPr->GetGoogleUniqueViewCount(); ?></span>
											</span>
                                        </div>
                                    </div>
                                </li>
                                <?php
                            }
                        }
                        ?>
                        <li class="gutter-sizer"></li>
                        <?php
                    } else {
                        ?>
                        <div class="empty">
                            <img src="profilepage/img/no-materials.svg" alt="">
                            <p>У вас нет материалов</p>
                        </div>
                        <?php
                    }
                    ?>
                </ul>
            </div>
            <?php
        }
    }


    /**
     * Enter description here...
     *
     */
    function DisplayLogin($withForget = true)
    {
        $this->ReadNameStrings('langs/', 'login.xml', $this->m_displayLang);

        // get language strings
        $passS = $this->GetNameString('passwordLabel');
        $signInBtnS = $this->GetNameString('signInBtn');

        $forgPassS = $this->GetNameString('recoverLink');

        $signUpLinkS = $this->GetNameString('signUpLink');

        $loginNameDescS = $this->GetNameString('loginNameDesc');
        $passwordDescS = $this->GetNameString('passwordDesc');

        $errorMessageS = $this->GetNameString('warningAlert');

        // is login
        if ( !$this->m_user->GetID() )
        {
        	$forgotCat = new Category(11);
        	$this->DisplayMessage();
            ?>
			<form action="" method="post" name="login">
				<input type="hidden" name="siteUserAction" value="login"/>
	            
				<div class="input-group name">
					<label>Email</label>
					<input type="email" name="email" id="email" required />
				</div>
	            <div class="input-group password">
					<label>Password</label>
					<input type="password" name="password" id="password" required />
					<a href="<?php echo $forgotCat->GetPath();?>" class="forgot-password">Forgot it?</a>
				</div>
				<button type="submit" class="btn btn-primary">Sign In</button>
			</form>
            <?php
        } 
        else 
        {
            header("location:".$this->m_site->GetSiteURL());
            exit();
        }
    }

    function DisplayLogout()
    {
        if ($this->m_user->GetID()) {
            $siteURL = $this->m_site->GetSiteURL();

            print ($this->m_user->GetFirstName() . ' ');

            // display log out
            print ("<a href='{$siteURL}{$this->m_displayLang}/logout/'>Log out</a>");
        }
    }

    /**
     * Enter description here...
     *
     * @return bool
     */
    function IsLogin()
    {
        return ($this->m_user->GetID() > 0);
    }

    /**
     * This function shows the form, which display form for recover forgotten password
     *
     * @param string $forgetInfo
     */
    function DisplayForgetForm($forgetInfo = null)
    {
        // display registation successful or unsuccessful message
        $this->ReadNameStrings('langs/', 'forgetForm.xml', $this->m_displayLang);

        $userlogCode = substr($forgetInfo, 0, 16);
        $userID = intval(substr($forgetInfo, 16));
        $sql = "SELECT * FROM `site_user` WHERE `ID` = $userID AND `logcode` = '$userlogCode'";
        $row = DB::QueryToRow($sql);

        // if forget user selected
        if ( $row )
        {
            if ( !$this->m_ar )
            {
                // get names
                $passwordDescS = $this->GetNameString('passwordDesc');
                $passwordLabelS = $this->GetNameString('passwordLabel');
                $confirmPasswordLabelS = $this->GetNameString('confirmPasswordLabel');
                $doneBtnS = $this->GetNameString('doneBtn');
                ?>
                <form method="post" action="" name="newuserform" id="chngform">
                    <input type="hidden" name="siteUserAction" value="recoverPassword"/>
                    <input type="hidden" name="ufi" value="<?php echo $forgetInfo; ?>"/>

                    <div class="input-group password">
						<label>New password</label>
						<input type="password" name="password" id="password" pattern="^\S{6,}$" onchange="this.setCustomValidity(this.validity.patternMismatch ? 'Must have at least 6 characters' : ''); if(this.checkValidity()) form.cpassword.pattern = this.value;" required />
	
						<div class="password-strengh">
							Password strength:
							<span id="strength-area"></span>
						</div>
					</div>
					<div class="input-group password">
						<label>Confirm password</label>
						<input type="password" name="cpassword" id="cpassword" pattern="^\S{6,}$" onchange="this.setCustomValidity(this.validity.patternMismatch ? 'Please enter the same Password as above' : '');" required />
					</div>
					<button type="submit" class="btn btn-primary">Save</button>
                </form>
                
                <script type="text/javascript">
                $(document).ready(function(){
                	$("#password").keyup(function(){
                		if ( $(this).val()=="" )
                		{
                			$("#strength-area").removeAttr("class");
							$("#strength-area").text("");
                		}
                		else
                		{
                			checkPassStrength($(this).val());
                		}
                	});
                });
                
				function scorePassword(pass) {
					var score = 0;
					if ( !pass )
						return score;
					
					// award every unique letter until 5 repetitions
					var letters = new Object();
					for ( var i=0; i<pass.length; i++ )
					{
						letters[pass[i]] = (letters[pass[i]] || 0) + 1;
						score += 5.0 / letters[pass[i]];
					}
					
					// bonus points for mixing it up
					var variations = {
						digits: /\d/.test(pass),
						lower: /[a-z]/.test(pass),
						upper: /[A-Z]/.test(pass),
						nonWords: /\W/.test(pass),
					}
					
					variationCount = 0;
					for ( var check in variations ) {
						variationCount += (variations[check] == true) ? 1 : 0;
					}
					score += (variationCount - 1) * 10;
					
					return parseInt(score);
				}
				function checkPassStrength(pass) {
					var score = scorePassword(pass);
					
					if ( score>70 )
					{
						$("#strength-area").removeAttr("class");
						$("#strength-area").addClass("color-green");
						$("#strength-area").text("Strong");
						
					}
					else if ( score>40 )
					{
						$("#strength-area").removeAttr("class");
						$("#strength-area").addClass("color-blue");
						$("#strength-area").text("Good");
					}
					else
					{
						$("#strength-area").removeAttr("class");
						$("#strength-area").addClass("color-red");
						$("#strength-area").text("Week");
					}
					
					return "";
				}
                </script>
                <!--//set new password-->
                <?php
            }
            else 
            {
                echo $this->DisplayMessage();
            }
        } 
        else 
        {
            header('location:'.$this->m_site->GetSiteURL().'404.html');
            exit();
        }
    }

    /**
     * This function display deactivation form or action result content
     *
     */
    function DisplayDeactivationForm()
    {
        $this->DisplayMessage();

        $this->ReadNameStrings('langs/', 'deactivationForm.xml', $this->m_displayLang);
        // if there is not any action display deactivation form
        if (!$this->m_ar) {
            // set message for content of deactivation page
            if (!$this->m_message) {
                $this->m_message = 22;
                $this->DisplayMessage();
            }

            // get language strings
            $deactivationBtnS = $this->GetNameString('deactivationBtn');

            // display form of deactivation
            print ('<div class="wsuFormArea">
					<form method="post">
					<input type="hidden" name="siteUserAction" value="deactivation" /> 
					<div class="wsuButton">
						<input type="submit" value="' . $deactivationBtnS . '" />
					</div>
					</form>
					</div>');
        }
    }

    /**
     * Enter description here...
     *
     * @param string $activationInfo
     */
    function DisplayActivationForm($activationInfo = null)
    {
        $this->DisplayMessage();

        // chack activation //////////////////////////////////////////////////////////////
        $ar = null;

        if (strlen($activationInfo) > 32) {
            // get activation code and ID
            $activationCode = substr($activationInfo, 0, 32);
            $activationID = intval(substr($activationInfo, 32));

            // if the conde and ID exist then try to activate
            if ($activationID && $activationCode) {
                if (is_int($activationID) && strlen($activationCode) > 0) {
                    $aUser = new Site_user ($activationID);
                    $aCode = $aUser->GetActivation_code();

                    if (strcmp($activationCode, $aCode) == 0) {
                        $ar = $aUser->SetStatus($this->m_actStatus);
                    }
                }
            }
        }

        if ($ar) {
            // display activation successful content
            $this->DisplayContent(6);
        } else {
            // display activation unsuccessful content
            $this->DisplayContent(7);
        }
    }

    /**
     * Enter description here...
     *
     * @return Site_user
     */
    function GetSiteUser()
    {
        return $this->m_user;
    }


    function GetUserID()
    {
        return $this->m_user->GetID();
    }

    function DisplayUserName()
    {
        return $this->m_user->GetName();
    }

    function IsLoginUser($logcode)
    {
        if ($logcode && strlen($logcode) == 16) {
            $sql = "SELECT * FROM `site_user` WHERE `logcode` = '$logcode' ";

            $res = DB::QueryToArray($sql);

            if ($res) {
                // if user is finaly activated
                if ($res [0] ['status'] == 3) {
                    return true;
                }
            }
        }

        return false;
    }

    function DisplayMessage()
    {
        if ( is_int($this->m_message) )
        {
            $this->DisplayContent($this->m_message);
        } 
        elseif ( $this->m_message )
        {
            print $this->m_message;
        }
    }

    /**
     * Display according content
     *
     * @param int $ID
     */
    function DisplayContent($optionsID)
    {
        // get content ID
        $regOptions = new RegOption ($optionsID);
        $regOptionVal = $regOptions->GetValue();

        // create content
        $content = new Content ($regOptionVal);

        // get title
        $title = $content->GetName($this->m_displayLang);
        $showTitle = $content->GetTitleShow();

        // get right content
        $article = $content->GetContent($this->m_displayLang);

        // display title
        if ($showTitle) {
            print ("<h1>$title</h1>");
        }

        // display content
        print ("$article");
    }

    /**
     * This function display form to recover account.
     * It contains email filed only.
     *
     */
    function DisplayForgetEmail()
    {
        $this->ReadNameStrings('langs/', 'forgetEmail.xml', $this->m_displayLang);

        // get language strings
        $emailS = $this->GetNameString('emailLabel');
        $souldBeS = $this->GetNameString('recoverEmailDesc');
        $sendS = $this->GetNameString('recoverBtn');
        
        if ( !$this->m_user->GetID() )
        {
        	if ( !$this->m_ar )
        	{
        		$this->DisplayMessage();
        		?>
		        <form action='' method='post' name='forget'>
		            <input id="siteUserAction" name="siteUserAction" value="sendChangePassword" type="hidden" />
		            
		            <div class="input-group password">
						<label>Enter your email address</label>
						<input type="email" name="email" id="email" value=""/>
					</div>
					<button type="submit" class="btn btn-primary">Reset password</button>
					
					<p>We'll send instructions for changing your password in just a minute</p>
		        </form>
		        <?php
        	}
        	else 
        	{
        		$this->DisplayMessage();
        	}
        }
        
        return null;
    }


    private function CheckPlageat($text1m, $text2m)
    {
        $text1m = str_replace(array(".", ",", ":", "'"), array("", "", "", ""), $text1m);
        $text2m = str_replace(array(".", ",", ":", "'"), array("", "", "", ""), $text2m);

        $text1 = explode(" ", $text1m);
        $text2 = explode(" ", $text2m);

        $match = array();
        for ($i = 0; $i < count($text1); $i++) {
            if (in_array($text1[$i], $text2)) {
                $match[] = $text1[$i];
            }
        }
        $perc1 = ceil(count($match) * 100 / count($text1));

        $match = array();
        for ($i = 0; $i < count($text2); $i++) {
            if (in_array($text2[$i], $text1)) {
                $match[] = $text2[$i];
            }
        }
        $perc2 = ceil(count($match) * 100 / count($text2));

        if ($perc1 > 70 or $perc2 > 70) {
            return true;
        }

        $text1 = explode(".", $text1m);
        $text2 = explode(".", $text2m);

        $match = array();
        for ($i = 0; $i < count($text1); $i++) {
            if (in_array($text1[$i], $text2)) {
                $match[] = $text1[$i];
            }
        }
        $perc1 = ceil(count($match) * 100 / count($text1));

        $match = array();
        for ($i = 0; $i < count($text2); $i++) {
            if (in_array($text2[$i], $text1)) {
                $match[] = $text2[$i];
            }
        }
        $perc2 = ceil(count($match) * 100 / count($text2));

        if ($perc1 > 70 or $perc2 > 70) {
            return true;
        }

        return false;
    }

    private function DisplayAuthorMenu($catID = 0)
    {
        $profileCat = new Category(8);
        $profileCatPath = $profileCat->GetPath();
        $profileCatTitle = $profileCat->GetName($this->m_displayLang);

        $subCats = array();
        $profileCat->GetSubIDs($subCats);
        ?>
        <style type="text/css">
            .profile-nav {
                float: left;
                border: 1px solid #3366ff;
                border-radius: 5px;
                padding: 6px 12px;
                margin: 0 10px 0 0;
                text-transform: uppercase;
            }

            .profile-nav a {
                color: #3366ff;
            }

            .profile-nav:hover {
                float: left;
                border: 1px solid #3366ff;
                background-color: #3366ff;
                border-radius: 5px;
                padding: 6px 12px;
                margin: 0 10px 0 0;
                text-transform: uppercase;
            }

            .profile-nav:hover a {
                color: #fff;
                text-decoration: none;
            }

            .profile-nav-active {
                float: left;
                border: 1px solid #3366ff;
                background-color: #3366ff;
                border-radius: 5px;
                padding: 6px 12px;
                margin: 0 10px 0 0;
                text-transform: uppercase;
                color: #fff;
            }
        </style>
        <div style="margin:20px 0;font-size:12px;color:#f00;">
            <?php
            foreach ($subCats as $subCat) {
                $tempCat = new Category($subCat);
                if ($tempCat->GetVisible()) {
                    if ($catID == $subCat) {
                        ?>
                        <div class="profile-nav-active"><?php echo $tempCat->GetName($this->m_displayLang); ?></div>
                        <?php
                    } else {
                        ?>
                        <div class="profile-nav"><a
                                href="<?php echo $this->m_site->GetSiteURL() . $tempCat->GetPath(); ?>"><?php echo $tempCat->GetName($this->m_displayLang); ?></a>
                        </div>
                        <?php
                    }
                }
            }
            ?>
            <br clear="all">
        </div>
        <?php
    }

    private function ConvertPattern($pattern, $product, $productObj, $catID)
    {
        $datePattern = "d F, Y";

        while (($start = strpos($pattern, '@@')) !== false) {
            $end = strpos($pattern, '@@', $start + 1);

            if (!$end) {
                break;
            }

            $key = substr($pattern, $start, $end - $start + 2);
            $key = substr($key, 2, strlen($key) - 4);

            $subKey = $key;

            //TODO modified
            $value = null;
            if ($key == 'name' || $key == 'short_desc' || $key == 'picture' || $key == 'ID' || $key == 'updated_date' || $key == 'desc' || $key == 'viewCount') {
                $value = $product[$key];
                if ($key == 'date') {
                    $value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, $datePattern);
                }
            } elseif ($key == 'innerPath') {
                $currCat = new Category($catID);
                $value = $currCat->GetPath();
            } elseif ($key == 'random') {
                $value = $product["link"];
            } elseif (strpos($key, "desc") !== false && strpos($key, "desc") == 0 && strpos($key, "|") !== false) {
                $value = $product["desc"];
                $value = str_replace("</p>", "</p> ", $value);
                $value = strip_tags($value);
                $werbCount = intval(substr($key, strpos($key, "|") + 1));
                if (strlen($value) > $werbCount) {
                    $value = substr($value, 0, strpos($value, " ", $werbCount)) . "...";
                }
            } elseif ($key == 'date_day') {
                $date = $product["date"];
                $date = explode("-", $date);
                $value = $date[2];
            } elseif ($key == 'date_month') {
                $date = $product["date"];
                $value = $this->m_site->GetDateTransformation($date, $this->m_displayLang, "F");
            } elseif ($key == 'date_year') {
                $date = $product["date"];
                $date = explode("-", $date);
                $value = $date[0];
            } else {
                $pos = strpos($key, ':');
                $separator = null;
                $param = null;
                if ($pos > 0) {
                    $separator = $param = substr($key, $pos + 1);
                    $key = substr($key, 0, $pos);
                }

                foreach ($fields as $field) {
                    if (strcmp($field['name'], $key) == 0) {
                        if ($field['fieldID'] == 71) {
                            $value = $productObj->GetFieldValue($field['fieldID'], $this->m_displayLang, false, $param);
                        } else {
                            $value = $productObj->GetFieldValue($field['fieldID'], $this->m_displayLang, true, $param);
                        }

                        if (($field['fieldID'] == 42 || $field['fieldID'] == 43) && $value != "") {
                            $value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, $datePattern);
                        }
                        break;
                    }
                }

                if (is_array($value)) {
                    if ($field['fieldID'] == 71) {
                        $articlesCat = new Category(124);
                        $valueT = $value;
                        $value = '';
                        foreach ($valueT as $val) {
                            $tempOD = new Optionsdata($val);
                            $value .= '<a href="' . $this->m_site->GetSiteURL() . $articlesCat->GetPath() . $tempOD->GetKey() . '">' . $tempOD->GetName($this->m_displayLang) . '</a>' . $separator;
                        }
                        $value = substr($value, 0, strlen($value) - strlen($separator));
                    } else {
                        $value = implode($separator, $value);
                    }
                }
            }
            // replace
            $pattern = str_replace("@@$subKey@@", $value, $pattern);
        }

        return $pattern;
    }

    private function ConvertPatternSingle($pattern, $productObj)
    {
        $typeID = $productObj->GetTypeID();
        $prType = new Pr_type ($typeID);

        $gallPattern = $prType->GetPatternGallery($this->m_displayLang);

        while (($start = strpos($pattern, '@@')) !== false) {
            $end = strpos($pattern, '@@', $start + 1);

            if (!$end) {
                break;
            }

            $key = substr($pattern, $start, $end - $start + 2);
            $key = substr($key, 2, strlen($key) - 4);

            $subKey = $key;
            $pos = strpos($key, ':');
            $param = null;
            if ($pos > 0) {
                $subKey = substr($key, 0, $pos);
                $param = substr($key, $pos + 1);
            }
            $value = null;
            switch ($subKey) {
                case 'ID' :
                    $value = $productObj->GetID();
                    break;
                case 'typeID' :
                    $value = $productObj->GetTypeI();
                    break;
                case 'random' :
                    $value = $productObj->GetRandom();
                    break;
                case 'name' :
                    $value = $productObj->GetName($this->m_displayLang);
                    break;
                case 'desc' :
                    $value = $productObj->GetDesc($this->m_displayLang);
                    break;
                case 'picture' :
                    $value = $productObj->GetPicture();
                    break;
                case 'cover' :
                    $value = $productObj->GetCover();
                    if ($value == "img/cover.gif") {
                        $value = "";
                    }
                    break;
                case 'viewCount':
                    $value = $productObj->GetViewCount();
                    break;
                case 'short_desc' :
                    $value = $productObj->GetDescShort($this->m_displayLang);
                    break;
                case 'date':
                    $value = $productObj->GetDate();
                    $value = $this->m_site->GetDateTransformation($value, $this->m_displayLang, "d F, Y");
                    break;
                case 'date_day':
                    $date = $productObj->GetDate();
                    $date = explode("-", $date);
                    $value = $date[2];
                    break;
                case 'date_month':
                    $date = $productObj->GetDate();
                    $value = $this->m_site->GetDateTransformation($date, $this->m_displayLang, "F");
                    break;
                case 'date_year':
                    $date = $productObj->GetDate();
                    $date = explode("-", $date);
                    $value = $date[0];
                    break;
                case 'gallery' :
                    $gallery = new Pr_pictureList ($productObj->GetID());
                    $value = $gallery->GetPatternDesign($gallPattern);
                    break;
                case 'lang' :
                    $value = $this->m_displayLang;
                    break;
                case 'siteURL' :
                    $value = $this->m_site->GetSiteURL();
                    break;
                default :
                    $value = "";

                    // get from parimiter
                    foreach ($fields as $field) {
                        if (strcmp($field ['name'], $subKey) == 0) {
                            if ($field['fieldID'] == 71) {
                                $value = $this->m_product->GetFieldValue($field['fieldID'], $this->m_displayLang, false, $param);
                            } else {
                                $value = $this->m_product->GetFieldValue($field['fieldID'], $this->m_displayLang, true, $param);
                            }

                            break;
                        }
                    }

                    break;
            }

            if (is_array($value)) {
                $value = implode($param, $value);
            }

            // replace only current $key value ($key values can be more then one)
            $patPos = strpos($pattern, "@@$key@@");
            $pattern = substr_replace($pattern, $value, $patPos, strlen("@@$key@@"));
        }

        return $pattern;
    }
}

?>