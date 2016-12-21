<?php
$includes = array ('classes/Common/DB.class.inc', 'classes/Account/SiteUser/Site_user.class.inc', 'classes/Account/RegOption/RegOption.class.inc' );

Site::IncludeFiles ( $includes );
Site::IncludeFile ('classes/Session/Session.class.inc');
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
		parent::__construct ();
		
		$userID = null;
		// if site userhas already login
		$logCode = null;
		// site user log code
		$sulc = $this->m_site->GetValue ( 'sulc' );
		
		if ($sulc && strlen ( $sulc ) == 16)
		{
			$sql = "SELECT * FROM `site_user` WHERE `logcode` = '$sulc' ";
			$res = DB::QueryToArray ( $sql );
			if ($res)
			{
				// if user is finaly activated
				if ($res [0] ['status'] == 3)
				{
					$userID = $res [0] ['ID'];
					$this->m_user = new Site_user ( $userID );
					
					// if logout 
					if (isset ( $params [0] ) && $params [0] == 'logout')
					{
						$newCode = GetRandCode ( 16 );
						$this->m_user->SetLogCode ( $newCode );
						$this->m_site->SaveValue ( 'sulc', '' );
						$userID = null;
					}
				}
			}
		}
		
		// site usertrys to log in 
		if (isset ( $_POST ['email'], $_POST ['password'] ))
		{
			$email = trim ( DB::POST ( 'email' ) );
			$password = md5 ( trim ( $_POST ['password'] ) );
			
			$sql = "SELECT * FROM `site_user` WHERE `email` = '$email' AND `password` = '$password'";
			$res = DB::QueryToArray ( $sql );
			
			if ($res)
			{
				// if user is finaly activated
				if ($res [0] ['status'] == 3)
				{
					$userID = $res [0] ['ID'];
					$logCode = GetRandCode ( 16 );
				}
				elseif ($res [0] ['status'] == 1)
				{
					// wait until filnaly activation
					$this->m_message = 19;
				}
			}
			else
			{
				$sql = "SELECT * FROM `site_user` WHERE `email` = '$email'";
				$res = DB::QueryToArray ( $sql );
				
				// user doesn`t exist or wrong password
				$this->m_message = (! $res) ? 17 : 18;
			}
		}
		
		$this->m_user = new Site_user ( $userID );
		
		// after login 
		if ($logCode)
		{
			$this->m_user->SetLogCode ( $logCode );
			$this->m_site->SaveValue ( 'sulc', $logCode );
		}
		
		// read names
		$this->ReadNameStrings ( 'langs/', 'userForms.xml', $this->m_displayLang );
		
		// read configuration
		$confFilePath = $_SERVER ['DOCUMENT_ROOT'] . '/cms/config/userConfig.xml'; // Added '/qr' by Adriada
		
		$simpleXML = simplexml_load_file ( $confFilePath );
		
		$this->m_actStatus = ( int ) $simpleXML->activationType;
		$this->m_fromEmail = ( string ) $simpleXML->activatorEmail;
		$this->m_activatorName = ( string ) $simpleXML->activatorName;
	}

	
	/**
	 * Do actions
	 *
	 */
	function DoAction()
	{
		// $actiovationLink = $params;
		global $session;

		if (isset ( $_POST ['siteUserAction'] ))
		{
			switch ($_POST ['siteUserAction'])
			{
				case 'deactivation' :
					//delete user 
					$this->m_ar = $this->m_user->Delete ();
					
					// deactivation messages
					$this->m_message = ($this->m_ar) ? 23 : 24;
					break;
				case 'newuser' :
					// get parameters ///////////////////////////////
					// captcha
					if ( DB::POST('validImage')!="" ) {
						
						$_SESSION["CAPTCHADIVSHOW"] = 1;
						$captcha = DB::POST('validImage');
						
						// Get value of session rkey from the file
						$_SESSION['rkey'] = $session->GetRkey();
						
						if ($captcha !== $_SESSION['rkey'])
						{
							$this->m_ar = false;
								
							// display captcha error
							$this->m_message = 28;
							return false;
						}
					}
					
					// name
					$fullName = DB::POST ( 'fullName' );
					$firstName = DB::POST ( 'firstName' );
					
					if (! $fullName && $firstName)
					{
						$fullName = $firstName;
					}
					// other parametors
					$password = trim ( DB::POST ( 'rpassword' ) );
					$cpassford = trim ( DB::POST ( 'cpassword' ) );
					$email = trim ( DB::POST ( 'email' ) );
//					$aggoup = DB::POST ( 'aggoup' );
					
					// if some filed has not been filled then retusn false and display message
					//TODO: display corsponding filed names
					//TODO: check email validity
//					if (! $fullName || strlen ( $password ) == 0 || strlen ( $email ) == 0 || ! $aggoup)
					if (! $fullName || strlen ( $password ) == 0 || strlen ( $email ) == 0)
					{
						$this->m_ar = false;
						$this->m_message = $this->GetNameString ( 'warningAlert' );
					}
					else
					{
						// chack passwords equalment
						if (strcmp ( $password, $cpassford ) != 0)
						{
							$this->m_ar = false;
							
							// display change password unsuccessful content
							$this->m_message = 15;
						}
						elseif (strlen ( $password ) < 6 ||strlen ( $cpassford ) < 6)
						{
							$this->m_ar = false;
							
							// display change password unsuccessful content because of less amount of symbols ( < 6 )
							$this->m_message = 27;
						}
						else
						{
							// insert new user
							$newUser = new Site_user ( );
							$this->m_ar = $newUser->Insert ( $fullName, $password, $email, '', 1 );
							
							// case when email address has already exist
							if ($this->m_ar === null)
							{
								$this->m_message = 4;
							}
							elseif (! $this->m_ar)
							{
								// registration unsuccess message
								$this->m_message = 3;
							}
							else
							{
								// send message
								$activatinCode = $newUser->GetActivation_code ();
								$activationID = $newUser->GetID ();
								
								$siteRoot = $this->m_site->GetSiteURL ();
								
								// get activation path
								$catActivation = new Category ( 6 );
								$actPath = $catActivation->GetPath ();
								$activationLink = "{$siteRoot}{$this->m_displayLang}/{$actPath}{$activatinCode}{$activationID}/";
								
								// get registraton email content
								$regOptions = new RegOption ( 5 );
								$regOptionVal = $regOptions->GetValue ();
								$regEmailCont = new Content ( $regOptionVal );
								
								// make activation email content
								$activEmailTitle = $regEmailCont->GetName ( $this->m_displayLang );
								$activEmailContent = $regEmailCont->GetContent ( $this->m_displayLang );
								
								// insert user name
								if (strpos ( $activEmailContent, '@@name@@' ))
								{
									$activEmailContent = str_replace ( '@@name@@', $fullName, $activEmailContent );
								}
								
								if (strpos ( $activEmailContent, '@@firstName@@' ))
								{
									$activEmailContent = str_replace ( '@@firstName@@', $firstName, $activEmailContent );
								}
								
								//								if(strpos($activEmailContent, '@@lastName@@'))
								//								{
								//									$activEmailContent = str_replace('@@lastName@@', $lastName, $activEmailContent);
								//								}
								

								// make activation email content		
								$activEmailContent .= "<p><a href='$activationLink'>$activationLink</a></p>";
								
								// send registraion email
								$this->m_site->Mail ( $email, $activEmailTitle, $activEmailContent, $this->m_fromEmail, $this->m_activatorName );
								
								// registration has been successful
								$this->m_message = 2;
								
								// Set new captcha string								
								$rkey = GetRandCode ( 6 );	
								$session->SetRkey($rkey);
								$_SESSION ['rkey'] = $rkey;																
							}
						}
					}
					break;
				case 'newuserfirst' :
					// get parameters ///////////////////////////////
					
					// other parametors
					$email = trim ( DB::POST ( 'email' ) );
					
					$sql = "SELECT count(*) FROM `site_user` WHERE `email` = '$email'";
					$res = DB::QueryToValue($sql);
					
					// if some filed has not been filled then retusn false and display message
					//TODO: display corsponding filed names
					//TODO: check email validity
					if ( strlen ( $email ) == 0 )
					{
						$this->m_ar = false;
						$this->m_message = $this->GetNameString ( 'warningAlert' );
						
						$_POST["email"] = "";
					}
					elseif ( $res ) 
					{
						$this->m_ar = false;
						$this->m_message = 4;
						
						$_POST["email"] = "";
					}
					break;
				case 'sendChangePassword' :
					$email = DB::POST ( 'email' );
					
					// get user
					$sql = "SELECT * FROM `site_user` WHERE `email` = '$email'";
					$row = DB::QueryToRow ( $sql );
					
					if ($row)
					{
						$ID = $row ['ID'];
						$code = $row ['logcode'];
						
						$this->m_ar = true;
						
						// send change password email success content
						$this->m_message = 9;
						
						// get forget password link
						$siteRoot = $this->m_site->GetSiteURL ();
						
						// get forget category
						$forgetCat = new Category ( 9 );
						$forgetPath = $forgetCat->GetPath ();
						
						$contentLink = "{$siteRoot}{$this->m_displayLang}/{$forgetPath}{$code}{$ID}/";
						
						// get change password email content
						$changeOptions = new RegOption ( 12 );
						$chngOptionVal = $changeOptions->GetValue ();
						$chngEmailCont = new Content ( $chngOptionVal );
						
						$chngEmailTitle = $chngEmailCont->GetName ( $this->m_displayLang );
						$chngEmailArticle = $chngEmailCont->GetContent ( $this->m_displayLang );
						
						// replase name
						if (strpos ( $chngEmailArticle, '@@name@@' ))
						{
							$chngEmailArticle = str_replace ( '@@name@@', $row ['name'], $chngEmailArticle );
						}
						
						// remove all html hags
						$content = "$chngEmailArticle";
						$content .= "<p><a href='$contentLink'>$contentLink</a></p>";
					
						// send registraion email
						$res = $this->m_site->Mail ( $email, $chngEmailTitle, $content, $this->m_fromEmail, $this->m_activatorName );
						
						if (! $res)
						{
							// could not send email
							$this->m_message = 9;
						}
					}
					else
					{
						// the user has not been found 
						$this->m_message = 11;
					}
					break;
				case 'recoverPassword' :
					$rpassword = DB::POST ( 'rpassword' );
					$cpassword = DB::POST ( 'cpassword' );
					
					$userInfo = DB::POST ( 'ufi' );
					
					$userlogCode = substr ( $userInfo, 0, 16 );
					$userID = intval ( substr ( $userInfo, 16 ) );
		
					if (strcmp ( $rpassword, $cpassword ) == 0 && strlen ( $cpassword ) > 0 && (strlen ( $rpassword ) >= 6 ||strlen ( $cpassword ) >= 6))
					{
						// get user
						$sql = "SELECT * FROM `site_user` WHERE `ID` = $userID AND `logcode` = '$userlogCode'";
						$row = DB::QueryToRow ( $sql );
						
						if ($row)
						{
							$ID = $row ['ID'];
							$code = $row ['activation_code'];
							
							$siteUser = new Site_user ( $ID );
							$this->m_ar = $siteUser->SetPassword ( $rpassword );
							
							// display change password successful or unsuccessful content
							$this->m_message = ($this->m_ar) ? 14 : 15;
						}
						else
						{
							// display change password unsuccessful content
							$this->m_message = 15;
						}
					}
					else
					{
						// display change password unsuccessful content
						$this->m_message = 15;
					}
					break;
				case 'changeinfo' :
					$fname = trim ( DB::POST ( 'fullName' ) );
					
					if (strlen ( $fname ) == 0)
					{
						$this->m_ar = false;
						$this->m_message = $this->GetNameString ( 'warningAlert' );
					}
					else
					{
						//						$this->m_user->SetCompany ( $company );
						$this->m_user->SetFirstName ( $fname );
						//						$this->m_user->SetLastName ( $lname );
					}
					break;
				case 'changepasswordin' :
					$password = trim ( DB::POST ( 'rpassword' ) );
					$cpassword = trim ( DB::POST ( 'cpassword' ) );
					
					if (strlen ( $password ) == 0 || strlen ( $cpassword ) == 0)
					{
						$this->m_ar = false;
						$this->m_message = 'Please fill all fields!';
					}
					else
					{
						// chack passwords equalment
						if (strcmp ( $password, $cpassword ) != 0)
						{
							$this->m_ar = false;
							
							// display change password unsuccessful content
							$this->m_message = 15;
						}
						elseif (strlen ( $password ) < 6 ||strlen ( $cpassword ) < 6)
						{
							$this->m_ar = false;
							
							// display change password unsuccessful content because of less amount of symbols ( < 6 )
							$this->m_message = 27;
						}
						else
						{
							// display change password successful content
							$this->m_message = 14;
							
							$this->m_user->SetPassword ( $password );
						}
					}
					break;
				case 'changeemail' :
					$email = trim ( DB::POST ( 'email' ) );
					
					if (strlen ( $email ) == 0)
					{
						// change email unsuccessfuly
						$this->m_ar = false;
						$this->m_message = 21;
					}
					else
					{
						$this->m_ar = $this->m_user->SetEmail ( $email );
						
						// change email successfuly
						$this->m_message = ($this->m_ar) ? 20 : 21;
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
		print ( "<br /> addition information" );
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
		$this->DisplayMessage ();
		
		$this->ReadNameStrings ( 'langs/', 'subscribeForm.xml', $this->m_displayLang );
		
		if (! $this->m_user->GetID ())
		{
			if (! $this->m_ar)
			{
				// display registration content
				$this->DisplayContent ( 1 );
				
				// Get value of session rkey from the file
				$_SESSION['rkey'] = $session->GetRkey();
				
				if (! isset ( $_SESSION ['rkey'] ))
				{
					// Set new value of session rkey to the file					
					$rkey = GetRandCode ( 6 );
					$session->SetRkey($rkey);
					$_SESSION['rkey'] = $rkey;
				}
				// array for names
				$names = array ();
				
				$names ["nameLabel"] = $this->GetNameString ( 'nameLabel' );
				$names ["emailDesc"] = $this->GetNameString ( 'emailDesc' );
				$names ["emailLabel"] = $this->GetNameString ( 'emailLabel' );
				$names ["passwordDesc"] = $this->GetNameString ( 'passwordDesc' );
				$names ["passwordLabel"] = $this->GetNameString ( 'passwordLabel' );
				$names ["confirmPasswordLabel"] = $this->GetNameString ( 'confirmPasswordLabel' );
				$names ["captchaDesc"] = $this->GetNameString ( 'captchaDesc' );
				$names ["agreementLabel"] = $this->GetNameString ( 'agreementLabel' );
				$names ["agreementDesc"] = $this->GetNameString ( 'agreementDesc' );
				$names ["signUpBtn"] = $this->GetNameString ( 'signUpBtn' );
				$names ["subscribeBtnNext"] = $this->GetNameString ( 'subscribeBtnNext' );
				$names ["subscribeBtn"] = $this->GetNameString ( 'subscribeBtn' );
				
				// error message
				$errorMessageS = $this->GetNameString ( 'warningAlert' );
				?>
<script type='text/javascript'>
				<!--
				<?php
				if ( isset($_SESSION["CAPTCHADIVSHOW"]) && $_SESSION["CAPTCHADIVSHOW"] == 1 ) {?>
					
					var captcha = 1;
				<?php } else {?>
					
					var captcha = 0;
				<?php }?>
				function validation()
				{
					firstName = TextFildValid('fullName', null, 'fullName');
					email = TextFildValid('email', 'email', 'email');
					
					validImage = 1;
					if ( captcha == 1 ) {
						
						validImage = TextFildValid('validImage', null, 'validImage');
					}
					
					rpassword = TextFildValid('rpassword', null, 'rpassword');
					cpassword = TextFildValid('cpassword', null, 'cpassword');

					pass = false;
					
					if(rpassword && cpassword)
					{
						pass = isEqual('rpassword', 'cpassword', 'rpassword', 'cpassword');
						
						var pasLenght = GetFieldValueLenght('rpassword');
						if(pasLenght < 6)
						{
							document.getElementById('passLenght').className = 'wsuAlertActive';
						}
						else
						{
							document.getElementById('passLenght').className = '';
						}
					}
					res = (firstName && email && rpassword && cpassword && pass && validImage && (pasLenght > 5));
					
					//	if user first time enter incorrect data then captcha appier
					if ( !res ) {
						
						captcha = 1;
						document.getElementById("captchaDiv").style.display = "block";
					}
					
					// group validation
					SerErrorText(res, '<?php echo $errorMessageS?>');
					return res;
				}
				
				function validation1()
				{
					email = TextFildValid('email', 'email', 'email');
					// group validation
					SerErrorText(email, '<?php echo $errorMessageS?>');
					return email;
				}
				-->
				</script>
<form action="" method="post" name="registration" onsubmit="return <?php echo (isset($_POST["email"]) && $_POST["email"]!="" ? "validation": "validation1");?>();">

	<div class="wsuAlertMessage" id="errorDiv">&nbsp;</div>
	<?php
	if ( isset($_POST["email"]) && $_POST["email"]!="" ) {?>
		
		<input type="hidden" name="siteUserAction" value="newuser" />
		<input type="hidden" name="email" id="email" value="<?php echo $_POST["email"];?>" />
		
		<div class="wsuTextFieldLabel">
			<div id="fullNameHTML"><?php echo $names ['nameLabel']?></div>
		</div>
		<div class="wsuTextField"><input name="fullName" id="fullName" type="text" size="40" /></div>
		<br clear="left" />
		
		<div class="wsuTextFieldDesc">
			<div id="passLenghtHTML"><?php echo $names ['passwordDesc']?></div>
		</div>
		<div class="wsuTextFieldLabel">
			<div id="rpasswordHTML"><?php echo $names ['passwordLabel']?></div>
		</div>
		<div class="wsuTextField"><input name="rpassword" id="rpassword" type="password" size="20" /></div>
		<br clear="left" />
		
		<div class="wsuTextFieldLabel">
			<div id="cpasswordHTML"><?php echo $names ['confirmPasswordLabel']?></div>
		</div>
		<div class="wsuTextField"><input name="cpassword" id="cpassword" type="password" size="20" /></div>
		<br clear="left" />
		
		<div id="captchaDiv" style="display:<?php echo (isset($_SESSION["CAPTCHADIVSHOW"]) && $_SESSION["CAPTCHADIVSHOW"] == 1 ? "block": "none");?>;">
			<div class="wsuTextFieldDesc">
				<div id="validImageHTML"><?php echo $names ['captchaDesc']?></div>
			</div>
			<span class="wsuTextFieldLabel"><img src="files/imageT.php" width="122" height="22" alt=" " /></span>
			<span class="wsuTextField"><input name="validImage" id="validImage"	type="text" /></span>
		</div>
		<br clear="left" />
		
		<div class="wsuButton"><input name="" type="submit"	value="<?php echo $names ['subscribeBtn']?>" /></div>
	<?php } else { 
		
		$_SESSION["CAPTCHADIVSHOW"] = 0;
		?>
		<input type="hidden" name="siteUserAction" value="newuserfirst" />
		<div class="wsuTextFieldDesc"><?php echo $names ['emailDesc']?></div>
		<div class="wsuTextFieldLabel">
			<div id="emailHTML"><?php echo $names ['emailLabel']?></div>
		</div>
		<div class="wsuTextField"><input name="email" id="email" type="text" size="40" /></div>
		<br clear="left" />
		
		<div class="wsuButton"><input name="" type="submit"	value="<?php echo $names ['subscribeBtnNext']?>" /></div>
	<?php }?>
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
		$this->DisplayMessage ();
		
		$this->ReadNameStrings ( 'langs/', 'registrationForm.xml', $this->m_displayLang );
		
		if (! $this->m_user->GetID ())
		{
			if (! $this->m_ar)
			{
				// display registration content
				$this->DisplayContent ( 1 );
				
				// Get value of session rkey from the file
				$_SESSION['rkey'] = $session->GetRkey();
				
				if (! isset ( $_SESSION ['rkey'] ))
				{
					// Set new value of session rkey to the file					
					$rkey = GetRandCode ( 6 );
					$session->SetRkey($rkey);
					$_SESSION['rkey'] = $rkey;
				}
				// array for names
				$names = array ();
				
				$names ["nameLabel"] = $this->GetNameString ( 'nameLabel' );
				$names ["emailDesc"] = $this->GetNameString ( 'emailDesc' );
				$names ["emailLabel"] = $this->GetNameString ( 'emailLabel' );
				$names ["passwordDesc"] = $this->GetNameString ( 'passwordDesc' );
				$names ["passwordLabel"] = $this->GetNameString ( 'passwordLabel' );
				$names ["confirmPasswordLabel"] = $this->GetNameString ( 'confirmPasswordLabel' );
				$names ["captchaDesc"] = $this->GetNameString ( 'captchaDesc' );
				$names ["agreementLabel"] = $this->GetNameString ( 'agreementLabel' );
				$names ["agreementDesc"] = $this->GetNameString ( 'agreementDesc' );
				$names ["signUpBtn"] = $this->GetNameString ( 'signUpBtn' );
				
				// error message
				$errorMessageS = $this->GetNameString ( 'warningAlert' );
				?>
<script type='text/javascript'>
				<!--
				function validation()
				{
					firstName = TextFildValid('fullName', null, 'fullName');
					email = TextFildValid('email', 'email', 'email');
					validImage = TextFildValid('validImage', null, 'validImage');
					
					rpassword = TextFildValid('rpassword', null, 'rpassword');
					cpassword = TextFildValid('cpassword', null, 'cpassword');

					pass = false;
					
					if(rpassword && cpassword)
					{
						pass = isEqual('rpassword', 'cpassword', 'rpassword', 'cpassword');
						
						
						agg = IsChecked('agg', 'registration', 'agg');
						var pasLenght = GetFieldValueLenght('rpassword');
						if(pasLenght < 6)
						{
							document.getElementById('passLenght').className = 'wsuAlertActive';
						}
						else
						{
							document.getElementById('passLenght').className = '';
						}
					}
					res = (firstName && email && rpassword && cpassword && pass && validImage && agg && (pasLenght > 5));
					// group validation
					SerErrorText(res, '<?php echo $errorMessageS?>');
					return res;
				}
				-->
				</script>
<form action="" method="post" name="registration" onsubmit="return validation();">

<div class="wsuAlertMessage" id="errorDiv">&nbsp;</div>
<input type="hidden" name="siteUserAction" value="newuser" />
<div class="wsuTextFieldLabel">
	<div id="fullNameHTML"><?php echo $names ['nameLabel']?></div>
</div>
<div class="wsuTextField"><input name="fullName" id="fullName" type="text" size="40" /></div>
<br clear="left" />
<div class="wsuTextFieldDesc"><?php echo $names ['emailDesc']?></div>
<div class="wsuTextFieldLabel">
	<div id="emailHTML"><?php echo $names ['emailLabel']?></div>
</div>
<div class="wsuTextField"><input name="email" id="email" type="text" size="40" /></div>
<br clear="left" />
<div class="wsuTextFieldDesc">
	<div id="passLenghtHTML"><?php echo $names ['passwordDesc']?></div>
</div>
<div class="wsuTextFieldLabel">
	<div id="rpasswordHTML"><?php echo $names ['passwordLabel']?></div>
</div>
<div class="wsuTextField"><input name="rpassword" id="rpassword" type="password" size="20" /></div>
<br clear="left" />
<div class="wsuTextFieldLabel">
	<div id="cpasswordHTML"><?php echo $names ['confirmPasswordLabel']?></div>
</div>
<div class="wsuTextField"><input name="cpassword" id="cpassword" type="password" size="20" /></div>
<br clear="left" />
<div class="wsuTextFieldDesc">
	<div id="validImageHTML"><?php echo $names ['captchaDesc']?></div>
</div>
<span class="wsuTextFieldLabel"><img src="files/imageT.php" width="122" height="22" alt=" " /></span>
<span class="wsuTextField"><input name="validImage" id="validImage" type="text" /></span>
<br clear="left" />
<div class="wsuCheckboxDesc"><?php echo $names ['agreementDesc']?></div>
<div class="wsuCheckbox"><input name="aggoup" id="agg" type="checkbox" value="1" checked /></div>
<div class="wsuCheckboxLabel">
	<div id="aggHTML"><?php echo $names ['agreementLabel']?></div>
</div>
<br clear="left" />
<div class="wsuButton"><input name="" type="submit" value="<?php echo $names ['signUpBtn']?>" /></div>

</form>
<?php
			}
		}
		
		return false;
	}
	
	function DisplayProfile()
	{
		$this->DisplayMessage ();
		
		$this->ReadNameStrings ( 'langs/', 'profile.xml', $this->m_displayLang );
		
		if ($this->m_user->GetID ())
		{
			// display deactivisation
			$firstName = $this->m_user->GetFirstName ();
			
			$email = $this->m_user->GetEmail ();
			
			// get language strings
			$passwordLabelS = $this->GetNameString ( 'passwordLabel' );
			$confirmPasswordLabelS = $this->GetNameString ( 'confirmPasswordLabel' );
			$nameLabelS = $this->GetNameString ( 'nameLabel' );
			$changeBtnS = $this->GetNameString ( 'changeBtn' );
			$passwordDescS = $this->GetNameString ( 'passwordDesc' );
			$deactivationLinkS = $this->GetNameString ( 'deactivationLink' );
			
			$emailS = $this->GetNameString ( 'emailLabel' );
			$emailDescS = $this->GetNameString ( 'emailDesc' );
			// display information form
			

			// error message
			$errorMessageS = $this->GetNameString ( 'warningAlert' );
			?>
<script type='text/javascript'>
				<!--
				function validationInfo()
				{
					res = TextFildValid('fullName', null, 'fullName');
					SerErrorText(res, '<?php echo $errorMessageS?>', 'errorDiv1');
					return res;
				}

				function validationPass()
				{
					rpassword = TextFildValid('rpassword', null, 'rpassword');
					cpassword = TextFildValid('cpassword', null, 'cpassword');

					pass = false;
					
					if(rpassword && cpassword)
					{
						pass = isEqual('rpassword', 'cpassword', 'rpassword', 'cpassword');
	
						var pasLenght = GetFieldValueLenght('rpassword');
						if(pasLenght < 6)
						{
							document.getElementById('passLenght').className = 'wsuAlertActive';
							pass = false;
						}
						else
						{
							document.getElementById('passLenght').className = '';
						}
					}

					res = (rpassword && cpassword && pass);
					SerErrorText(res, '<?php echo $errorMessageS?>', 'errorDiv3');
					return  res;
				}
				
				function validationEmail()
				{
					res = TextFildValid('email', 'email', 'email')
					SerErrorText(res, '<?php echo $errorMessageS?>', 'errorDiv2');
					return res;
				}
				-->
				</script>
<!--profile-->
<div class="wsuFormArea"><!--name form-->
<div class="wsuAlertMessage" id="errorDiv1">&nbsp;</div>

<form action="" method="post" name="registration" onsubmit="return validationInfo();">
<input type="hidden" name="siteUserAction" value="changeinfo" />
<div class="wsuTextFieldLabel">
	<div id="fullNameHTML"><?php echo $nameLabelS?></div>
</div>
<div class="wsuTextField"><input name="fullName" type="text" id="fullName" value="<?php echo $firstName?>" size="40" /></div>
<br clear="left" />
<div class="wsuButton"><input name="" type="submit" value="<?php echo $changeBtnS?>" /></div>
</form>
<!--email form-->
<div class="wsuAlertMessage" id="errorDiv2">&nbsp;</div>

<form action="" method="post" name="registration" onsubmit="return validationEmail();">
<input type="hidden" name="siteUserAction" value="changeemail" />
<div class="wsuTextFieldDesc"><?php echo $emailDescS?></div>
<div class="wsuTextFieldLabel">
	<div id="emailHTML"><?php echo $emailS?></div>
</div>
<div class="wsuTextField"><input name="email" id="email" type="text" size="40" value="<?php echo $email?>" /></div>
<br clear="left" />
<div class="wsuButton"><input name="" type="submit" value="<?php echo $changeBtnS?>" /></div>
</form>
<!--password form-->
<div class="wsuAlertMessage" id="errorDiv3">&nbsp;</div>

<form action="" method="post" name="registration" onsubmit="return validationPass();">
<input type="hidden" name="siteUserAction" value="changepasswordin" />
<div class="wsuTextFieldDesc">
	<div id="passLenghtHTML"><?php echo $passwordDescS?></div>
</div>
<div class="wsuTextFieldLabel">
	<div id="rpasswordHTML"><?php echo $passwordLabelS?></div>
</div>
<div class="wsuTextField"><input name="rpassword" id="rpassword" type="password" size="20" /></div>
<br clear="left" />
<div class="wsuTextFieldLabel">
	<div id="cpasswordHTML"><?php echo $confirmPasswordLabelS?></div>
</div>
<div class="wsuTextField"><input name="cpassword" id="cpassword" type="password" size="20" /></div>
<br clear="left" />
<div class="wsuButton"><input name="" type="submit" value="<?php echo $changeBtnS?>" /></div>
</form>
</div>
<!--//profile-->
<?php
			print ( "<div class='wsuDeactivationLink'><a href='{$this->m_site->GetSiteURL()}{$this->m_displayLang}/deactivation/'>$deactivationLinkS</a></div>" );
		}
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayLogin($withForget = true)
	{
		$this->DisplayMessage ();
		
		$this->ReadNameStrings ( 'langs/', 'login.xml', $this->m_displayLang );
		
		// get language strings
		$passS = $this->GetNameString ( 'passwordLabel' );
		$signInBtnS = $this->GetNameString ( 'signInBtn' );
		
		$forgPassS = $this->GetNameString ( 'recoverLink' );
		
		$signUpLinkS = $this->GetNameString ( 'signUpLink' );
		
		$screenLabelS = $this->GetNameString ( 'screenLabel' );
		$screenDescS = $this->GetNameString ( 'screenDesc' );
		
		$loginNameDescS = $this->GetNameString ( 'loginNameDesc' );
		$passwordDescS = $this->GetNameString ( 'passwordDesc' );
		
		//
		$errorMessageS = $this->GetNameString ( 'warningAlert' );
		// display lingin content
		$this->DisplayContent ( 16 );
		
		// is login
		if (! $this->m_user->GetID ())
		{
			?>
<script type='text/javascript' language="javascript">
				<!--
				function validationLogin()
				{
					email = TextFildValid('email', 'email', 'email');
					password = TextFildValid('password', null, 'password');
					res = (password && email);
					SerErrorText(res, '<?php echo $errorMessageS?>');
					// group validation
					return res;
				}
				-->
				</script>
<!--authorization-->
<div class="wsuFormArea">
	<div class="wsuAlertMessage" id="errorDiv">&nbsp;</div>
	
	<form action="" method="post" onsubmit="return validationLogin();" name="login">
	<input type="hidden" name="siteUserAction" value="login" />
	<div class="wsuTextFieldDesc"><?php echo $screenDescS?></div>
	<div class="wsuRowLogin">
		<div class="wsuRowLeftLogin">
			<div class="wsuTextFieldLabelLogin">
				<div id="emailHTMLLogin"><b><?php echo $screenLabelS?></b></div>
				<div class="wsuTextFieldLogin"><input name="email" id="email" type="text" size="40"></div>
			</div>
			<div class="wsuTextFieldLabelULogin"><?php echo $loginNameDescS;?></div>
		</div>
		<div class="wsuRowRightLogin">
			<?php 
			print ( '<div class="wsuLink">' );
			// get registration category
			$registrationCat = new Category ( 5 );
			$regPath = $registrationCat->GetPath ();
			print ( "<a href='{$this->m_displayLang}/$regPath'>{$signUpLinkS}</a>" );
			print ( '</div>' );
			?>
		</div>
	</div>
	<br clear="left" />
	<br clear="all">
	
	<div class="wsuRowLogin">
		<div class="wsuRowLeftLogin">
			<div class="wsuTextFieldLabelLogin">
				<div id="passwordHTMLLogin"><b><?php echo $passS?></b></div>
				<div class="wsuTextFieldLogin"><input name="password" id="password" type="password" size="40" /></div>
			</div>
			<div class="wsuTextFieldLabelULogin"><?php echo $passwordDescS;?></div>
		</div>
		<div class="wsuRowRightLogin">
			<?php 
			if ($withForget)
			{
				print ( "<div class='wsuLink'>
				<a href='#' onclick='return ToggleArchaicTools(\"recover\");'>$forgPassS</a>
			   </div>" );
				print ( '<div style="display:none;" id="recover">' );
				$this->DisplayForgetEmail ();
				print ( '</div>' );
			}
			else
			{
				// get forget passford start page
				$cat = new Category ( 11 );
				$path = $cat->GetPath ();
				print ( "<div class='wsuLink'><a href='{$this->m_displayLang}/$path'>$forgPassS</a></div>" );
			}
			?>
		</div>
	</div>
	
	<br clear="left" />
	<br clear="all">
	<div class="wsuButton"><input type="submit" value="<?php echo $signInBtnS?>" onclick="return validationLogin();" /></div>
	</form>
	
</div>
<!--//authorization-->
<?php
			/*if ($withForget)
			{
				print ( "<div class='wsuLink'>
				<a href='#' onclick='return ToggleArchaicTools(\"recover\");'>$forgPassS</a>
			   </div>" );
				print ( '<div style="display:none;" id="recover">' );
				$this->DisplayForgetEmail ();
				print ( '</div>' );
			}
			else
			{
				// get forget passford start page
				$cat = new Category ( 11 );
				$path = $cat->GetPath ();
				print ( "<div class='wsuLink'><a href='{$this->m_displayLang}/$path'>$forgPassS</a></div>" );
			}
			
			print ( '<div class="wsuLink">' );
			// get registration category
			$registrationCat = new Category ( 5 );
			$regPath = $registrationCat->GetPath ();
			print ( "<a href='{$this->m_displayLang}/$regPath'>{$signUpLinkS}</a>" );
			print ( '</div>' );*/
		}
	}
	
	function DisplayLogout()
	{
		if ($this->m_user->GetID ())
		{
			$siteURL = $this->m_site->GetSiteURL ();
			
			print ( $this->m_user->GetFirstName () . ' ' );
			
			// display log out
			print ( "<a href='{$siteURL}{$this->m_displayLang}/logout/'>Log out</a>" );
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function IsLogin()
	{
		return ($this->m_user->GetID () > 0);
	}
	
	/**
	 * This function shows the form, which display form for recover forgotten password
	 *
	 * @param string $forgetInfo
	 */
	function DisplayForgetForm($forgetInfo = null)
	{
		// display registation successful or unsuccessful message
		$this->DisplayMessage ();
		
		$this->ReadNameStrings ( 'langs/', 'forgetForm.xml', $this->m_displayLang );
		
		// if forget user selected
		if (is_string ( $forgetInfo ) && strlen ( $forgetInfo ) > 16)
		{
			if (! $this->m_ar)
			{
				// display recover password form content
				$this->DisplayContent ( 13 );
				
				// get names
				$passwordDescS = $this->GetNameString ( 'passwordDesc' );
				$passwordLabelS = $this->GetNameString ( 'passwordLabel' );
				$confirmPasswordLabelS = $this->GetNameString ( 'confirmPasswordLabel' );
				$doneBtnS = $this->GetNameString ( 'doneBtn' );
				
				// error message
				$errorMessageS = $this->GetNameString ( 'warningAlert' );
				?>
<script type='text/javascript'>
				<!--
				function validationChange()
				{
					var rpassword = TextFildValid('rpassword', null, 'rpassword');
					var cpassword = TextFildValid('cpassword', null, 'cpassword');
					pass = isEqual('rpassword', 'cpassword', 'rpassword', 'cpassword');
					
					var pasLenght = GetFieldValueLenght('rpassword');
					if(pasLenght < 6)
					{
						document.getElementById('passLenght').className = 'wsuAlertActive';
					}
					else
					{
						document.getElementById('passLenght').className = '';
					}
					
					res = (pass && (pasLenght > 5));
					
					// group validation
					//TODO set message
					SerErrorText(res, '<?php echo $errorMessageS?>');
					return res;
				}
				-->
				</script>
<!--set new password-->
<div class="wsuFormArea">
<div class="wsuAlertMessage" id="errorDiv1">&nbsp;</div>

<form method="post" action="" name="newuserform" onsubmit="return validationChange();">
<input type="hidden" name="siteUserAction" value="recoverPassword" />
<input type="hidden" name="ufi" value="<?php echo $forgetInfo?>" />
<div class="wsuTextFieldDesc">
	<div id="passLenghtHTML"><?php echo $passwordDescS?></div>
</div>
<div class="wsuTextFieldLabel">
	<div id="rpasswordHTML"><?php echo $passwordLabelS?></div>
</div>
<div class="wsuTextField"><input type="password" name="rpassword" id="rpassword" size="20" /></div>
<br clear="left" />
<div class="wsuTextFieldLabel">
	<div id="cpasswordHTML"><?php echo $confirmPasswordLabelS?></div>
</div>
<div class="wsuTextField"><input type="password" name="cpassword" id="cpassword" /></div>
<br clear="left" />
<div class="wsuButton"><input type="submit" value="<?php echo $doneBtnS?>" /></div>
</form>
</div>
<!--//set new password-->
<?php
			}
		}
		else
		{
			if (! $this->m_ar)
			{
				//TODO add new content for wrong address
				echo 'Incorrect link!';
			}
		}
	}
	
	/**
	 * This function display deactivation form or action result content
	 *
	 */
	function DisplayDeactivationForm()
	{
		$this->DisplayMessage ();
		
		$this->ReadNameStrings ( 'langs/', 'deactivationForm.xml', $this->m_displayLang );
		// if there is not any action display deactivation form
		if (! $this->m_ar)
		{
			// set message for content of deactivation page
			if (! $this->m_message)
			{
				$this->m_message = 22;
				$this->DisplayMessage ();
			}
			
			// get language strings
			$deactivationBtnS = $this->GetNameString ( 'deactivationBtn' );
			
			// display form of deactivation
			print ( '<div class="wsuFormArea">
					<form method="post">
					<input type="hidden" name="siteUserAction" value="deactivation" /> 
					<div class="wsuButton">
						<input type="submit" value="' . $deactivationBtnS . '" />
					</div>
					</form>
					</div>' );
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $activationInfo
	 */
	function DisplayActivationForm($activationInfo = null)
	{
		$this->DisplayMessage ();
		
		// chack activation //////////////////////////////////////////////////////////////
		$ar = null;
		
		if (strlen ( $activationInfo ) > 32)
		{
			// get activation code and ID
			$activationCode = substr ( $activationInfo, 0, 32 );
			$activationID = intval ( substr ( $activationInfo, 32 ) );
			
			// if the conde and ID exist then try to activate
			if ($activationID && $activationCode)
			{
				if (is_int ( $activationID ) && strlen ( $activationCode ) > 0)
				{
					$aUser = new Site_user ( $activationID );
					$aCode = $aUser->GetActivation_code ();
					
					if (strcmp ( $activationCode, $aCode ) == 0)
					{
						$ar = $aUser->SetStatus ( $this->m_actStatus );
					}
				}
			}
		}
		
		if ($ar)
		{
			// display activation successful content
			$this->DisplayContent ( 6 );
		}
		else
		{
			// display activation unsuccessful content
			$this->DisplayContent ( 7 );
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
		if ($logcode && strlen ( $logcode ) == 16)
		{
			$sql = "SELECT * FROM `site_user` WHERE `logcode` = '$logcode' ";
			
			$res = DB::QueryToArray ( $sql );
			
			if ($res)
			{
				// if user is finaly activated
				if ($res [0] ['status'] == 3)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	function DisplayMessage()
	{
		if (is_int ( $this->m_message ))
		{
			$this->DisplayContent ( $this->m_message );
		}
		elseif ($this->m_message)
		{
			print ( "<p> $this->m_message </p>" );
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
		$regOptions = new RegOption ( $optionsID );
		$regOptionVal = $regOptions->GetValue ();
		
		// create content
		$content = new Content ( $regOptionVal );
		
		// get title
		$title = $content->GetName ( $this->m_displayLang );
		$showTitle = $content->GetTitleShow ();
		
		// get right content
		$article = $content->GetContent ( $this->m_displayLang );
		
		// display title
		if ($showTitle)
		{
			print ( "<h1>$title</h1>" );
		}
		
		// display content
		print ( "$article" );
	}
	
	/**
	 * This function display form to recover account.
	 * It contains email filed only.
	 *
	 */
	function DisplayForgetEmail()
	{
		$this->ReadNameStrings ( 'langs/', 'forgetEmail.xml', $this->m_displayLang );
		
		// get language strings
		$emailS = $this->GetNameString ( 'emailLabel' );
		$souldBeS = $this->GetNameString ( 'recoverEmailDesc' );
		$sendS = $this->GetNameString ( 'recoverBtn' );
		// error message
		$errorMessageS = $this->GetNameString ( 'warningAlert' );
		// display content
		$this->DisplayContent ( 8 );
		?>
<script type='text/javascript' language="javascript">
				<!--
				function validationForget()
				{
					res = TextFildValid('forgetEmail', 'email', 'forgetEmail');
					SerErrorText(res, '<?php echo $errorMessageS?>', 'errorDiv1');
					return res;
				}
				-->
				</script>
<!--recover account-->
<div class="wsuFormArea">
<div class="wsuAlertMessage" id="errorDiv1">&nbsp;</div>

<form action='' method='post' name='forget' style='margin: 0px;' onsubmit="return validationForget();">
<input id="siteUserAction" name="siteUserAction" value="sendChangePassword" type="hidden" />
<div class="wsuTextFieldDesc"><?php echo $souldBeS?></div>
	<div class="wsuTextFieldLabel">
<div id="forgetEmailHTML"><?php echo $emailS?></div>
</div>
<div class="wsuTextField"><input name="email" type="text" size="40" id="forgetEmail"></div>
<br clear="left" />
<div class="wsuButton"><input name="" type="submit" value="<?php echo $sendS?>" /></div>
</form>
</div>
<!--//recover account-->
<?php
	}
}
?>