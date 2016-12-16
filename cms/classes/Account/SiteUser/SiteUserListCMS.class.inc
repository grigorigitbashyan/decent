<?php
Site::IncludeFile ( 'classes/Category/Category.class.inc' );
Site::IncludeFile ( 'classes/Account/SiteUser/SiteUserView.class.inc' );
Site::IncludeFile ( 'classes/Account/SiteUser/SiteUserList.class.inc' );

/**
 * Enter description here...
 *
 * @package Account
 */
class SiteUserListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_userTypeID;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_authorTypeID;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_typeLinkName;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_authorLinkName;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_fromEmail;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_activatorName;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_action_message;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->m_typeLinkName = 'siteusertypeID';
		$this->m_authorLinkName = 'siteauthorTypeID';
		$this->m_userTypeID = DB::REQUEST("{$this->m_typeLinkName}");
		$this->m_authorTypeID = DB::REQUEST("{$this->m_authorLinkName}");
		
		$this->ReadNameStrings ( 'classes/Account/langs/', 'siteuser.xml' );
		
		// read configuration
		$confFilePath = 'config/userConfig.xml';
		
		$simpleXML = simplexml_load_file ( $confFilePath );
		
		$this->m_actStatus = ( int ) $simpleXML->activationType;
		$this->m_fromEmail = ( string ) $simpleXML->activatorEmail;
		$this->m_activatorName = ( string ) $simpleXML->activatorName;
	}
	
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		if ($this->m_userTypeID > 0)
		{
			// display current user information
			$siteuserID = DB::REQUEST('siteuserID');
			if ($siteuserID)
			{
				$siteUserView = new SiteUserView ( $siteuserID);
				$siteUserView->DisplayInfo ();
			}
			else
			{
				$lang = CMS::GetCMSLang ();
				$userList = new SiteUserList ( $this->m_userTypeID, 20, null, null, $lang, $this->m_authorTypeID );
				$userCount = $userList->GetAllCount ();
				if ( $userCount>0 )
				{
					$additionLink = "siteusertypeID=$this->m_userTypeID";
					
					$this->DisplayFormHeadr ( 'update', 'siteusers' );
					// print header
					?>
					<table width="100%" border="0" cellspacing="0" cellpadding="0"	class="titleBox">
						<tr>
							<td>
								<div class="titleColL">
									<?php
									$userList->DisplayNavigation ( $additionLink );
									
									// names
									$allS = $this->GetNameString ( 'all' );
									$noneS = $this->GetNameString ( 'none' );
									$selectS = $this->GetNameString ( 'select' );
									$deleteS = $this->GetNameString ( 'delete' );
									$sureDeleteS = $this->GetNameString ( 'sureDelete' );
									$noSelectedS = $this->GetNameString ( 'noselected' );
									?>
								</div>
								<div class="titleColR">
									<span style="padding-right: 20px">
										<strong><?php echo $selectS?></strong>
										<a href="#" onclick="chekeBoxes('siteusers', 'siteuserIDs', true); return false"><?php echo $allS?></a>,
										<a href="#" onclick="chekeBoxes('siteusers', 'siteuserIDs', false); return false"><?php echo $noneS?></a>
									</span>
									<input name="delete" type="submit" value="<?php echo $deleteS?>" onclick="return ChecksFormDelete('siteusers', 'siteuserIDs', '<?php echo $noSelectedS?>', '<?php echo $sureDeleteS?>');" />
									<?php
									if ($this->m_userTypeID == 2) // activation
									{
										print ( '<input name="activate" type="submit" value="Activate" />' );
									}
									elseif ($this->m_userTypeID == 1)
									{
										?>
										<input name="notify" type="submit" value="Notify" onclick="return ChecksFormDelete('siteusers', 'siteuserIDs', '<?php echo $noSelectedS?>', 'Send notification?');" />
										<?php
									}
									?>
								</div>
								<br class="clearFloat" />
							</td>
						</tr>
					</table>
					
					<table width="100%" border="0" cellspacing="0" cellpadding="0" style="border-left: 1px solid #777777; border-right: 1px solid #777777;">
						<tr>
							<td>
							<?php
							if ($userCount > 0)
							{
								print ( "<div class='tableEven'><div class='tableCHB' style='width: 21px'></div>" );
								
								print ( "<div class='tableT' style='width: 180px'>" );
								$userList->DisplaySortLinks ( 'fullName', 'Name', $additionLink );
								print ( "</div>" );
								
								if ( $this->m_authorTypeID=="1" )
								{
									print ( "<div class='tableTE' style='width: 100px'>" );
									$userList->DisplaySortLinks ( 'nickName', 'Nickname', $additionLink );
									print ( "</div>" );
								}
								
								print ( "<div class='tableTE' style='width: 180px'>" );
								$userList->DisplaySortLinks ( 'email', 'E-mail', $additionLink );
								print ( "</div>" );
								
								if ( $this->m_authorTypeID=="1" )
								{
									print ( "<div class='tableTE'>" );
									$userList->DisplaySortLinks ( 'phone', 'Phone', $additionLink );
									print ( "</div>" );
								}
								
								print ( "<div class='tableD' style='width: 56px'>" );
								$userList->DisplaySortLinks ( 'date', 'Date', $additionLink );
								print ( "</div>" );
								
								print ( "<br class='clearFloat' /></div></div></div>" );
							}
							
							$patern = "
								<div class='tableCHB' style='width: 21px'><input name='siteuserIDs[]' id='siteuserIDs' type='checkbox' value='@@ID@@' /></div>
								<div class='tableT' style='width: 180px'>@@name@@ @@lname@@</div>
								<div class='tableTE' style='width: 180px'>@@email@@</div>
								<div class='tableD' style='width: 56px'>@@date@@</div>
								<br class='clearFloat' />
							";
							
							$userList->DisplayListDiv ( $patern, array ('tableEven', 'tableUneven' ) );
							print ( '</td></tr></table></form>' );
						}
					}
				}
				else
				{
					// display new user form
					$createUserS = $this->GetNameString ( 'createUser' );
					$this->DisplayTitle ( $createUserS );
					
					//	$this->DisplayFormHeadr('insert');
					?>
					<script type='text/javascript'>
						<!--
						function validation()
						{
							firstName = TextFildValid('fname', null, 'fnameHTML');
							lastName = TextFildValid('lname', null, 'lnameHTML');
							email = TextFildValid('email', 'email', 'emailHTML');
							company = TextFildValid('company', null, 'companyHTML');
		
							rpassword = TextFildValid('rpassword', null, 'rpasswordHTML');
							cpassword = TextFildValid('cpassword', null, 'cpasswordHTML');
		
							$pass = isEqual('rpassword', 'cpassword', 'rpasswordHTML', 'cpasswordHTML');
		
							var pasLenght = GetFieldValueLenght('rpassword');
							if(pasLenght < 6)
							{
								document.getElementById('passLenghtHTML').className = 'fRed';
							}
							else
							{
								document.getElementById('passLenghtHTML').className = '';
							}
							res = (firstName && lastName && email && company && rpassword && cpassword && $pass && (pasLenght > 5));
							// group validation
							SerErrorText(res);
							return res;
		
							return false;
						}
						-->
					</script>
					<div style="padding: 20px 0px 0px 0px">
						<form action="" method="post" name="registration"  onsubmit="return validation();">
							<?php
							$className = $this->GetFormActionName ();
							print ( "<input type='hidden' name='$className' value='insert'>\n" );
							?>
							<input type="hidden" name="siteUserAction" value="newuser" />
							<div style="float: left; width: 120px; padding-right: 5px;" align="right" id="fnameHTML">First Name:</div>
							<div style="float: left;"><input name="fname" type="text" id="fname" size="50"></div>
							<div style="clear: both; padding-bottom: 10px"></div>
							<div style="float: left; width: 120px; padding-right: 5px;" align="right" id="lnameHTML">Last Name:</div>
							<div style="float: left;"><input name="lname" type="text" size="50" id="lname"></div>
							<div style="clear: both; padding-bottom: 15px"></div>
							<!-- div style="float: left; width: 120px; padding-right: 5px;" align="right" id="companyHTML">Company /<br />Organization:</div>
							<div style="float: left; padding-top: 12px"><input name="company" id="company" type="text" size="30"></div>
							<div style="clear: both; padding-bottom: 15px"></div -->
							<div style="float: left; width: 120px; padding-right: 5px;" align="right" id="emailHTML">E-mail Address:</div>
							<div style="float: left;"><input name="email" id="email" type="text" size="50"></div>
							<div style="clear: both; padding-bottom: 15px"></div>
							<div style="float: left; width: 120px; padding-right: 5px;" align="right">&nbsp;</div>
							<div style="float: left; font-size: 10px" id="passLenghtHTML">Password	must be at least six characters long.</div>
							<div style="clear: both; padding-bottom: 5px"></div>
							<div style="float: left; width: 120px; padding-right: 5px;" align="right" id="rpasswordHTML">Password:</div>
							<div style="float: left;"><input name="rpassword" id="rpassword" type="password"></div>
							<div style="clear: both; padding-bottom: 10px"></div>
							<div style="float: left; width: 120px; padding-right: 5px;" align="right" id="cpasswordHTML">Confirm Password:</div>
							<div style="float: left;"><input name="cpassword" id="cpassword" type="password"></div>
							<div style="clear: both; padding-bottom: 20px"></div>
							<div style="float: left; width: 120px; padding-right: 5px;" align="right">&nbsp;</div>
							<div style="float: left;"><input name="" type="submit" value="Create My Account"></div>
							<div style="clear: both; padding-bottom: 10px"></div>
						</form>
					</div>
					<?php
				}
	}
	
	/**
	 * @see CMSInterface::DisplayMenu()
	 *
	 */
	function DisplayMenu()
	{
		$suteUserS = $this->GetNameString ( 'siteUser' );
		
		$userTypes ['1'] = $this->GetNameString ( 'userType1' );
		$userTypes ['2'] = $this->GetNameString ( 'userType2' );
		$userTypes ['3'] = $this->GetNameString ( 'userType3' );
		
		if ( $this->m_userTypeID )
		{
			print ( "<div class='someWTitleBox'><a href='?{$this->m_typeLinkName}'>$suteUserS</a></div>" );
		}
		else
		{
			print ( "<div class='someWTitleBox'>$suteUserS</div>" );
		}
		
		print ( '<div class="someWBox">' );
		
		foreach ( $userTypes as $key => $name )
		{
			$userList = new SiteUserList ( $key, 0 );
			$userCount = $userList->GetAllCount ();
			if ( $this->m_userTypeID==$key )
			{
				if ( /*$key==3*/ 0 )
				{
					if ( $this->m_authorTypeID=="all" )
					{
						print ( "<div class='directoryMenu1'>$name ($userCount) </div>" );
					}
					else 
					{
						print ( "<div class='directoryMenu1'><a href='?{$this->m_typeLinkName}=$key&{$this->m_authorLinkName}=all'>$name</a> ($userCount)</div>" );
					}
				}
				else 
				{
					print ( "<div class='directoryMenu1'>$name ($userCount) </div>" );
				}
				
				/*if ( $key==3 )
				{
					$userList = new SiteUserList ( $key, 0, null, null, 'en', 1 );
					$userCount = $userList->GetAllCount ();
					if ( $this->m_authorTypeID=="1" )
					{
						print ( "<div class='directoryMenu2'>Author ($userCount)</div>" );
					}
					else 
					{
						print ( "<div class='directoryMenu2'><a href='?{$this->m_typeLinkName}=$key&{$this->m_authorLinkName}=1'>Author</a> ($userCount)</div>" );
					}
					
					$userList = new SiteUserList ( $key, 0, null, null, 'en', 0 );
					$userCount = $userList->GetAllCount ();
					if ( $this->m_authorTypeID=="0" )
					{
						print ( "<div class='directoryMenu2'>Users ($userCount)</div>" );
					}
					else 
					{
						print ( "<div class='directoryMenu2'><a href='?{$this->m_typeLinkName}=$key&{$this->m_authorLinkName}=0'>Users</a> ($userCount)</div>" );
					}
				}*/
			}
			else
			{
				if ( $key==3 )
				{
					print ( "<div class='directoryMenu1'><a href='?{$this->m_typeLinkName}=$key&{$this->m_authorLinkName}=all'>$name</a> ($userCount)</div>" );
				}
				else 
				{
					print ( "<div class='directoryMenu1'><a href='?{$this->m_typeLinkName}=$key'>$name</a> ($userCount)</div>" );
				}
			}
		}
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * @see CMSInterface::DoAction()
	 *
	 */
	function DoAction()
	{
		$ar = null;
		
		switch ($this->GetFormAction ())
		{
			case 'update' :
				{
					$IDs = DB::POST ('siteuserIDs');
					
					if (count ( $IDs ) > 0)
					{
						$ar = true;
						
						if (isset ( $_POST ['delete'] ))
						{
							foreach ( $IDs as $ID )
							{
								$siteUser = new Site_user ( $ID );
								$ar &= $siteUser->Delete ();
							}
						}
						
						if (isset ( $_POST ['notify'] ))
						{
							require("../phpmailer/class.phpmailer.php");
							foreach ( $IDs as $ID )
							{
								$siteUser = new Site_user ( $ID );
								$email = $siteUser->GetEmail();
								$fullname = $siteUser->GetFullName();
								$activatinCode = $siteUser->GetActivation_code();
	                            $activationID = $siteUser->GetID();
								
								$catActivation = new Category (6);
	                            $actPath = $catActivation->GetPath();
								$activationLink = $this->m_site->GetSiteURL().$actPath.$activatinCode.$activationID;
								
								//	get email content
								$emailCont = new Content(127);
								$subject = $emailCont->GetName($this->m_displayLang);
								$content = $emailCont->GetContent($this->m_displayLang);
								
								$content = str_replace('@@name@@', $fullname, $content);
								$content = str_replace('@@email@@', $email, $content);
								$content = str_replace('@@link@@', $activationLink, $content);
								$content = str_replace('@@siteLink@@', $this->m_site->GetSiteURL(), $content);
								
								//	send email
								$phpMailer = new PHPMailer();
								$phpMailer->CharSet = 'UTF-8';
								$phpMailer->SetFrom($this->m_fromEmail, $this->m_activatorName);
								$phpMailer->AddReplyTo($this->m_fromEmail, $this->m_activatorName);
								$phpMailer->AddAddress($email);
								$phpMailer->Subject = $subject;
								$phpMailer->AltBody = "To view the message, please use an HTML compatible email viewer!";
								$phpMailer->MsgHTML($content);
								$mailRes = $phpMailer->Send();
							}
						}
						
						if (isset ( $_POST ['activate'] ))
						{
							foreach ( $IDs as $ID )
							{
								$siteUser = new Site_user ( $ID );
								$ar &= $siteUser->SetStatus ( 3 );
								
								$email = $siteUser->GetEmail ();
								
								$siteRoot = $this->m_site->GetSiteURL ();
								// get forget category
								$forgetCat = new Category ( 6 );
								$forgetPath = $forgetCat->GetPath ();
								
								// get las code
								$code = $siteUser->GetLogcode ();
								$contentLink = "{$siteRoot}{$this->m_displayLang}/{$forgetPath}{$code}{$ID}/";
								
								// get change password email content
								$changeOptions = new RegOption ( 26 );
								$chngOptionVal = $changeOptions->GetValue ();
								$chngEmailCont = new Content ( $chngOptionVal );
								
								$chngEmailHeader = $chngEmailCont->GetName ( $this->m_displayLang );
								$chngEmailArticle = $chngEmailCont->GetContent ( $this->m_displayLang );
								$chngEmailTitle = $chngEmailCont->GetContent ( $this->m_displayLang, true, null, true, true );
								
								// remove all html hags
								$chngEmailTitle = strip_tags ( $chngEmailTitle );
								
								$content = "<p>$chngEmailHeader </p>
											<p>$chngEmailArticle</p>
											<p><a href='$contentLink'>$contentLink</a></p>";
								
								// send registraion email
								$this->m_site->Mail ( $email, $chngEmailTitle, $content, $this->m_fromEmail, $this->m_activatorName );
							}
						}
					}
				}
				break;
			case 'insert' :
				// get parameters
				$fname = trim ( DB::POST ('fname') );
				$lname = trim ( DB::POST ('lname') );
				
				$password = trim ( DB::POST ('rpassword') );
				$cpassford = trim ( DB::POST ('cpassword') );
				$email = trim ( DB::POST ('email') );
				$company = trim ( DB::POST ('company') );
				
				$ar = false;
				
				if (strlen ( $fname ) == 0 || strlen ( $lname ) == 0 || strlen ( $password ) == 0 || strlen ( $email ) == 0 || strlen ( $company ) == 0)
				{
					$ar = false;
					$this->m_message = $this->GetNameString ( 'warningAlert' );
				}
				else
				{
					// chack passwords equalment
					if (strcmp ( $password, $cpassford ) != 0)
					{
						$ar = false;
						$this->m_message = 15;
					}
					elseif (strlen ( $password ) < 6 || strlen ( $cpassford ) < 6)
					{
						$this->m_ar = false;
						
						// display change password unsuccessful content because of less amount of symbols ( < 6 )
						$this->m_message = 27;
					}
					else
					{
						// insert new user
						$newUser = new Site_user ( );
						$ar = $newUser->Insert ( $fname.' '.$lname, $password, $email, $company, 3 );
						
						// case when email address has already exist
						if ($ar === null)
						{
							$this->m_message = 4;
						}
						elseif (! $ar)
						{
							// registration unsuccess message
							$this->m_message = 3;
						}
						else
						{
							$siteRoot = $this->m_site->GetSiteURL ();
							// get forget category
							$forgetCat = new Category ( 6 );
							$forgetPath = $forgetCat->GetPath ();
							
							// get activation code
							$activationCode = $newUser->GetActivation_code();
							$ID = $newUser->GetID();
							//TODO: Ask to Arman should i set status 3 when Admin registered and send activation link to user or username && password
							$contentLink = "{$siteRoot}{$this->m_displayLang}/{$forgetPath}{$activationCode}{$ID}/";
							
							// get change password email content
							$changeOptions = new RegOption ( 25 );
							$chngOptionVal = $changeOptions->GetValue ();
							$chngEmailCont = new Content ( $chngOptionVal );
							
							$chngEmailHeader = $chngEmailCont->GetName ( $this->m_displayLang );
							$chngEmailArticle = $chngEmailCont->GetContent ( $this->m_displayLang );
							$chngEmailTitle = $chngEmailCont->GetContent ( $this->m_displayLang, true, null, true, true );
							
							// remove all html hags
							$chngEmailTitle = strip_tags ( $chngEmailTitle );
							
							$content = "<p>$chngEmailHeader $fname $lname,</p>
											<p>$chngEmailArticle</p>
											<p><a href='$contentLink'>$contentLink</a></p>";
							
							// send registraion email
							$this->m_site->Mail ( $email, $chngEmailTitle, $content, $this->m_fromEmail, $this->m_activatorName );
						}
					}
				}
				break;
			case 'inviteuser':
				$email = trim ( DB::POST ('email') );
				
				$query = "select * from `site_user` where `email` = '$email'";
				$retArr = DB::QueryToRow($query);
				if ( is_array($retArr) )
				{
					$this->m_action_message = "Пользователь с таким адресом эл. почты уже зарегистрирован";
				}
				else 
				{
					$query = "select * from `invited_authors` where `email` = '$email'";
					$retArr = DB::QueryToRow($query);
					if ( is_array($retArr) )
					{
						$this->m_action_message = "Пользователь с таким адресом эл. почты уже приглашен";
					}
					else 
					{
						$code = GetRandCode(8);
						
						// get activation path
						$siteRoot = $this->m_site->GetSiteURL();
						$catActivation = new Category ( 330 );
						$actPath = $catActivation->GetPath ();
						$activationLink = $siteRoot.$actPath."?code=".$code;
						
						$invitationEmailContent = new Content(120);
						$invitationEmailSubject = $invitationEmailContent->GetName("ru");
						$invitationEmailMessage = $invitationEmailContent->GetContent("ru");
						
						$invitationEmailMessage = str_replace("@@link@@", $activationLink, $invitationEmailMessage);
						
						require("../phpmailer/class.phpmailer.php");
						$phpMailer = new PHPMailer();
						$phpMailer->CharSet = 'UTF-8';
						$phpMailer->SetFrom('no-reply@publiq360.com', 'Publiq');
						$phpMailer->AddReplyTo("no-reply@publiq360.com", "Publiq");
						$phpMailer->AddAddress($email);
						$phpMailer->Subject = $invitationEmailSubject;
						$phpMailer->AltBody = "To view the message, please use an HTML compatible email viewer!";
						$phpMailer->MsgHTML($invitationEmailMessage);
						$mailRes = $phpMailer->Send();
						if ( $mailRes )
						{
							$this->m_action_message = "Приглашение отправлено";
							
							$query = "insert into `invited_authors` (`email`, `code`, `date`) values ('$email', '$code', ".time().")";
							DB::Query($query);
						}
						else 
						{
							$this->m_action_message = "Системная ошибка: не удается отправить сообщение";
						}
					}
				}
				break;
		}
		
		if ($ar)
		{
			$this->Update ();
		}
	
	}
	
	function Update()
	{
	
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = 'CREATE TABLE `site_user` (
			  `ID` int(10) unsigned NOT NULL auto_increment,
			  `name` varchar(32) NOT NULL,
			  `lname` varchar(32) NOT NULL,
			  `password` varchar(32) NOT NULL,
			  `email` varchar(32) NOT NULL,
			  `activation_code` varchar(32) NOT NULL,
			  `date` date NOT NULL,
			  `status` tinyint(1) NOT NULL,
			  `logcode` varchar(16) default NULL,
			  PRIMARY KEY  (`ID`),
			  UNIQUE KEY `username` (`name`),
			  UNIQUE KEY `email` (`email`)
			) DEFAULT CHARSET=utf8  ;';
		
		return $sql;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		//chack dastabase
		$res = $this->TestTable ( );
		
		// chack files /////////////////////////////////

		// cache xml
		if (! file_exists ( 'config/userConfig.xml' ))
		{
			$res = false;
			print ( 'The config file <b>registration.xml</b> does not exist<br />' );
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		$tables = array('site_user');
		return $tables;
	}
	
	function GetTemplateValue($names)
	{
		$siteUser = new Site_user ( );
		
		$reg = '';
		
		switch ($names)
		{
			case 'fullname':
				$reg = $siteUser->GetFirstName();
				break;
			case 'company':
				$reg = $siteUser->GetCompany();
				break;
			case 'email':
				$reg = $siteUser->GetEmail();
				break;
			case 'password':
				$reg = $siteUser->GetPassword();
				break;
			case 'status':
				$status = $siteUser->GetPassword();
				switch ($status)
				{
					case '2':
						$reg = 'You need to activate your account';
						break;
					case '3':
						$reg = 'You are activated';
						break;
				}
				break;
		}
		
		return $reg;
	}
}
?>