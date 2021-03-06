<?php
// includes
Site::IncludeFile ( 'classes/UserManagement/UserManagement.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserType.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserTypeList.class.inc' );
Site::IncludeFile ( 'classes/UserManagement/UserList.class.inc' );
Site::IncludeFile ( 'classes/Product/pr_main/Pr_main.class.inc' );

/**
 * This class provide actions with users(delete, add, change password) and drawing (user form) 
 *
 * @package CMSUser
 */
class UserStatisticsCMS extends CMSObject implements CMSInterface
{
	function __construct()
	{
		parent::__construct ();
		$this->ReadNameStrings ( 'classes/UserManagement/langs/', 'user.xml' );
	}
	/**
	 * This function doing actions with users, add, delete change password
	 *
	 * @return bool
	 */
	function DoAction()
	{
		$res = false;
		switch ( $this->GetFormAction() )
		{
			case 'modifyStats' :
				// get user articles
				$userarticles = DB::POST('userarticles');
				if ($userarticles)
				{
					foreach ( $userarticles as $userarticle )
					{
						$tempPr = new Pr_main($userarticle);
						if ( isset($_POST["setpaid"]) )
						{
							$tempPr->SetPaid(1);
						}
						elseif ( isset($_POST["setunpaid"]) )
						{
							$tempPr->SetPaid(0);
						}
					}
				}
				break;
			case 'addPaid':
				$userID = DB::GET("userID");
				$paid = DB::POST("paid");
				
				$user = new UserManagement($userID);
				$user->SetPaid($paid);
				break;
		}
		
		return $res;
	}
	
	function DisplayMenu()
	{
		// display profile
		// display users
		// get corrent user privilegy
		$currentUser = UserManagement::GetCurrentUser ();
		
		// get user type list that great then corrent users privilegy
		$usertypeList = new UserTypeList ( $currentUser->GetUserTypeID(), "usertypestat" );
		
		$styles = array ();
		$styles [1] = 'directoryMenu1';
		$styles [2] = 'directoryMenu2';
		$styles [3] = 'directoryMenu3';
		$styles [4] = 'directoryMenu4';
		
		$usertypeList->DisplayTree ( $styles, null );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		$usertypeID = DB::GET('usertypestat');
		$userID = DB::GET('userID');
		
		// create user list
		$userList = new UserList ( $usertypeID );
		$userNumber = $userList->GetItemCount ();
		$currentID = $userList->GetCurrentID ();
		$user = new UserManagement($currentID);
		$currentID = $user->GetID();
		
		// get user type list that great then corrent users privilegy
		$currentUser = UserManagement::GetCurrentUser ();
		$usertypeList = new UserTypeList ( $currentUser->GetUserTypeID () );
		$res = $usertypeList->FindPathInTree ( $usertypeID );
		
		//	if current user is not owner & administratior, when display his/her own stats
		if ( $currentUser->GetUserTypeID()>10 )
		{
			$user = new UserManagement ( $currentUser->GetID() );
			$this->DisplayUserStats ( $currentUser->GetID(), $currentUser->GetUserName () );
			
			return true;
		}
		
		// allow user info and co cousers
		$allowUser = (count ( $res ) > 0);
		$allowCouser = (count ( $res ) > 1);
		
		// if user type has been selected then display according info
		if ($usertypeID && $allowUser)
		{
			if ( $userID )
			{
				$user = new UserManagement ( $currentID );
				$this->DisplayUserStats ( $userID, $user->GetUserName () );
			}
			
			// display users
			if ($allowCouser && $userNumber)
			{
				print ( '<div class="titleBox"><div class="titleColR"></div><br class="clearFloat" /></div>' );
				print ( '<div class="someTableArea">' );
				
				$structure = $userList->GetStructure ();
				
				$index = 0;
				foreach ( $structure as $item )
				{
					//	get user articles count (published & not published)
					$query = "select COUNT(*) from `pr_main` where `userID` = ".$item['ID']." and `status` = 1";
					$item["published"] = DB::QueryToValue($query);
					$query = "select COUNT(*) from `pr_main` where `userID` = ".$item['ID']." and `status` = 0";
					$item["notpublished"] = DB::QueryToValue($query);
					
					$styale = (++ $index % 2 == 0) ? 'tableEven' : 'tableUneven';
					print ( "<div class=\"$styale\">" );
					
					$pattern = '
                		<div class="tableT"><a href="?userID=%d&usertypestat=%d">%s</a></div>
                		<div class="tableD" style="width:60px;">%s / %s</div>
                		<br class="clearFloat" />';
					
					printf ( $pattern, $item['ID'], $usertypeID, $item['username'], $item['published'], $item['notpublished'] );
					
					print ( '</div>' );
					print ( '<br class="clearFloat" />' );
				}
				
				print ( '</div>' );
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	public function DisplayUserStats($userID, $username)
	{
		$name = "";
		$email = "";
		
		// if it is not new user then get name and email
		if ( $username )
		{
			$usersRatings = $this->UsersRatings();
			$usersTotalRating = $this->UsersTotalRating();
			
			$user = new UserManagement ( $username, false );
			
			$name = $user->GetName ();
			$email = $user->GetEmail ();
			$typeID = $user->GetUserTypeID();
			
			// if it is not current user then create delete form
			
			// get current user name
			$curUser = UserManagement::GetCurrentUser ();
			$profileS = $this->GetNameString ( 'profile' );
			
			if ( $curUser->GetUserName()==$username )
			{
				$this->DisplayTitle ( "$profileS '$username'" );
			}
			else
			{
				// get language strings
				$deleteS = $this->GetNameString ( 'delete' );
				$sureS = $this->GetNameString ( 'sureDelete' );
				$formActionName = $this->GetFormActionName ();
				
				$deleteForm = "<div  class='titleColR'></div>";
				
				$this->DisplayTitle ( "Statistics for '$username'", $deleteForm );
				
				//	display user balance
				// display form header
				$this->DisplayFormHeadr('addPaid', 'paidForm', 'post', null, "index.php?userID=".$userID."&usertypestat=".$typeID);
				$paid = $user->GetPaid();
				$total = $user->GetTotal();
				$bonus = $user->GetBonus();
				$paymentHistory = $user->GetPaymentHistory();
				$stats = $user->GetStatistics();
				?>
				<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
					<tr>
						<td>
							<div class="titleColL" style="font-size:15px;">Total sum: <?php echo $total + $bonus;?></div>
							<div class="titleColL" style="font-size:15px;">Bonus: <?php echo $bonus;?></div>
							<div class="titleColL" style="font-size:15px;">Paid: <?php echo $paid;?></div>
							<div class="titleColL" style="font-size:15px;">Balance: <?php echo $total + $bonus - $paid;?></div>
							<div class="titleColR">
								<input type="text" name="paid" value=""> 
								<input type="submit" value="ADD">
							</div>
							<br class="clearFloat" />
						</td>
					</tr>
				</table>
				</form>
				<br />
				
				<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
					<tr>
						<td>
							<div class="titleColL" style="font-size:15px;"><a href="javascript:ShowHistory();">Payment history</a></div>
							<div class="titleColL" style="font-size:15px;"><a href="javascript:ShowStatistics();">User statistics</a></div>
							<div class="titleColR">&nbsp;</div>
							<br class="clearFloat" />
							<div id="history" style="display:none;">
								<?php
								if ( is_array($paymentHistory) )
								{
									?>
									<div class="tableEven" style="border-top:1px solid #777;">
										<div class="tableCHB" style="width:20px;">&nbsp;</div>
										<div class="tableT"><b>Sum</b></div>
										<div class="tableD" style="width:60px;"><b>Date</b></div>
									</div>
									<br class="clearFloat" />
									<?php
									for ( $i=0; $i<count($paymentHistory); $i++ )
									{
										if ( $i+1==count($paymentHistory) )
										{
											?>
											<div class="<?php echo ($i%2==0 ? "tableUneven": "tableEven");?>" style="border-bottom:0px;">
												<div class="tableCHB" style="width:20px;">&nbsp;</div>
												<div class="tableT"><?php echo $paymentHistory[$i]["sum"];?></div>
												<div class="tableD" style="width:60px;"><?php echo $paymentHistory[$i]["date"];?></div>
											</div>
											<br class="clearFloat" />
											<?php
										}
										else 
										{
											?>
											<div class="<?php echo ($i%2==0 ? "tableUneven": "tableEven");?>">
												<div class="tableCHB" style="width:20px;">&nbsp;</div>
												<div class="tableT"><?php echo $paymentHistory[$i]["sum"];?></div>
												<div class="tableD" style="width:60px;"><?php echo $paymentHistory[$i]["date"];?></div>
											</div>
											<br class="clearFloat" />
											<?php
										}
									}
									?>
									<script type="text/javascript">
									function ShowHistory()
									{
										$("#statistics").css("display", "none");
										$("#history").toggle(300);
									}
									</script>
									<?php
								}
								?>
							</div>
							
							<div id="statistics" style="display:none;">
								<?php
								if ( is_array($stats) )
								{
									$query = "select SUM(`viewCount`) as `av` from `pr_main` where `userID` = ".$userID;
									$articlesViews = DB::QueryToValue($query);
									
									$query = "select SUM(TIMESTAMPDIFF(DAY, `date`, NOW())) as `av` from `pr_main` where `userID` = ".$userID;
									$days = DB::QueryToValue($query);
									
									$authorRating = $articlesViews/$stats["totalArticles"];
									$authorRating = $authorRating/$days;
									$authorRating = ceil($authorRating*(($stats["totalArticles"] - $stats["rejectedArticles"])/$stats["totalArticles"])*1000000);
									?>
									<div class="tableEven" style="border-top:1px solid #777;">
										<div class="tableCHB" style="width:20px;">&nbsp;</div>
										<div class="tableT">Total articles count</div>
										<div class="tableD" style="width:100px;"><?php echo $stats["totalArticles"];?></div>
									</div>
									<br class="clearFloat" />
									<div class="tableUneven">
										<div class="tableCHB" style="width:20px;">&nbsp;</div>
										<div class="tableT">Approved/published articles count</div>
										<div class="tableD" style="width:100px;"><?php echo $stats["publishedArticles"];?></div>
									</div>
									<br class="clearFloat" />
									<div class="tableEven">
										<div class="tableCHB" style="width:20px;">&nbsp;</div>
										<div class="tableT">Rejected articles count</div>
										<div class="tableD" style="width:100px;"><?php echo $stats["rejectedArticles"];?></div>
									</div>
									<br class="clearFloat" />
									<div class="tableUneven" style="border-bottom:0px;">
										<div class="tableCHB" style="width:20px;">&nbsp;</div>
										<div class="tableT">Rating</div>
										<div class="tableD" style="width:100px;"><?php echo $authorRating;?> (<?php echo round($authorRating*100/$usersTotalRating, 2);?>%)</div>
									</div>
									<br class="clearFloat" />
									
									<script type="text/javascript">
									function ShowStatistics()
									{
										$("#history").css("display", "none");
										$("#statistics").toggle(300);
									}
									</script>
									<?php
								}
								?>
							</div>
						</td>
					</tr>
				</table>
				</form>
				<br />
				<?php
			}
		}
		
		$articleCountInPage = 30;
		
		//	check parameters
		$page = intval(DB::GET('page')) - 1;
		$pageStr = "";
		if ( $page>0 )
		{
			$pageStr = " limit ".($page*$articleCountInPage).", ".$articleCountInPage;
		}
		else 
		{
			$page = 0;
			$pageStr = " limit 0, ".$articleCountInPage;
		}
		
		$status = DB::GET('status');
		$statusStr = "";
		if ( $status=="0" || $status=="1" || $status=="2" || $status=="3" || $status=="4" )
		{
			$statusStr = " and `status` = ".intval($status);
		}
		else 
		{
			$status = "all";
		}
		
		//	get user articles
		$query = "select * from `pr_main` where `userID` = ".$userID.$statusStr;
		$userArticlesAll = DB::QueryToArray($query);
		
		$query = "select * from `pr_main` where `userID` = ".$userID.$statusStr.$pageStr;
		$userArticles = DB::QueryToArray($query);
		?>
		<script src="http://code.jquery.com/jquery-latest.min.js" type="text/javascript"></script>
		<script type="text/javascript">
		$(document).ready(function(){
			$("#status").change(function(){
				var statusVal = $("#status").val();
				document.location = 'index.php?userID=<?php echo $userID;?>&usertypestat=<?php echo $typeID;?>&status=' + statusVal;
			});
		});
		</script>
		<?php
		// display form header
		$this->DisplayFormHeadr('modifyStats', 'statsForm', 'post', null, "index.php?userID=".$userID."&usertypestat=".$typeID."&status=".$status);
		?>
		<div class="titleBox">
			<?php
			if ( count($userArticlesAll)>$articleCountInPage )
			{
				$pageCount = ceil(count($userArticlesAll)/$articleCountInPage);
				?>
				<div class="titleColL">
					Page: 
					<?php
					for ( $i=0; $i<$pageCount; $i++ )
					{
						if ( $page==$i )
						{
							?>
							<span><?php echo $i+1;?></span>
							<?php
						}
						else 
						{
							?>
							<span><a href="index.php?userID=<?php echo $userID;?>&usertypestat=<?php echo $typeID;?>&status=<?php echo $status;?>&page=<?php echo $i+1;?>"><?php echo $i+1;?></a></span>
							<?php
						}
					}
					?>
				</div>
				<?php
			}
			?>
			<div class="titleColR">
				Status 
				<select name="status" id="status">
					<option value="all">All</option>
					<option value="0"<?php echo ($status=="0" ? " selected": "");?>>Not Published</option>
					<option value="1"<?php echo ($status=="1" ? " selected": "");?>>Published</option>
					<option value="2"<?php echo ($status=="2" ? " selected": "");?>>Approved</option>
					<option value="3"<?php echo ($status=="3" ? " selected": "");?>>Rejected</option>
					<option value="4"<?php echo ($status=="4" ? " selected": "");?>>Updated</option>
				</select>
			</div>
			<br class="clearFloat" />
		</div>
		<div class="someTableArea">
			<div class="tableUneven">
				<div class="tableCHB" style="width:20px;">&nbsp;</div>
				<div class="tableT"><b>Title</b></div>
				<div class="tableD" style="width:60px;"><b>Date</b></div>
				<div class="tableD" style="width:60px;"><b>Status</b></div>
				<div class="tableD" style="width:200px;"><b>Structure</b></div>
			</div>
			<br class="clearFloat" />
			<?php
			for ( $i=0; $i<count($userArticles); $i++ )
			{
				$tempPr = new Pr_main($userArticles[$i]["ID"]);
				?>
				<div class="<?php echo ($i%2==0 ? "tableUneven": "tableEven");?>">
					<div class="tableCHB" style="width:20px;"><input name="userarticles[]" id="userarticles[]" type="checkbox" value="<?php echo $userArticles[$i]["ID"];?>" /></div>
					<div class="tableT"><?php echo $userArticles[$i]["name_ru"];?></div>
					<div class="tableD" style="width:60px;"><?php echo $userArticles[$i]["date"];?></div>
					<div class="tableD" style="width:60px;">
						<?php
						switch ( $userArticles[$i]["status"] )
						{
							case 1: echo "Published"; break;
							case 2: echo "Approved"; break;
							case 3: echo "Rejected"; break;
							case 4: echo "Updated"; break;
							default: echo "Not checked";
						}
						?>
					</div>
					<div class="tableD" style="width:200px;"><?php echo $tempPr->GetCategoryName();?></div>
				</div>
				<br class="clearFloat" />
				<?php
			}
			?>
		</div>
		</form>
		<br clear="all">
		<?php
	}
	
	public function UsersRatings()
	{
		$userList = new UserList(12);
		$userListArr = $userList->GetStructure();
		
		$retArr = array();
		for ( $i=0; $i<count($userListArr); $i++ )
		{
			$userID = $userListArr[$i]["ID"];
			$user = new UserManagement ( $userListArr[$i]["username"], false );
			
			$stats = $user->GetStatistics();
			
			$query = "select SUM(`viewCount`) as `av` from `pr_main` where `userID` = ".$userID;
			$articlesViews = DB::QueryToValue($query);
			
			$query = "select SUM(TIMESTAMPDIFF(DAY, `date`, NOW())) as `av` from `pr_main` where `userID` = ".$userID;
			$days = DB::QueryToValue($query);
			
			$authorRating = $articlesViews/$stats["totalArticles"];
			$authorRating = $authorRating/$days;
			$authorRating = ceil($authorRating*(($stats["totalArticles"] - $stats["rejectedArticles"])/$stats["totalArticles"])*1000000);
			
			$retArr[$userID] = $authorRating;
		}
		
		return $retArr;
	}
	public function UsersTotalRating()
	{
		$userList = new UserList(12);
		$userListArr = $userList->GetStructure();
		
		$retArr = 0;
		for ( $i=0; $i<count($userListArr); $i++ )
		{
			$userID = $userListArr[$i]["ID"];
			$user = new UserManagement ( $userListArr[$i]["username"], false );
			
			$stats = $user->GetStatistics();
			
			$query = "select SUM(`viewCount`) as `av` from `pr_main` where `userID` = ".$userID;
			$articlesViews = DB::QueryToValue($query);
			
			$query = "select SUM(TIMESTAMPDIFF(DAY, `date`, NOW())) as `av` from `pr_main` where `userID` = ".$userID;
			$days = DB::QueryToValue($query);
			
			$authorRating = $articlesViews/$stats["totalArticles"];
			$authorRating = $authorRating/$days;
			$authorRating = ceil($authorRating*(($stats["totalArticles"] - $stats["rejectedArticles"])/$stats["totalArticles"])*1000000);
			
			$retArr += $authorRating;
		}
		
		return $retArr;
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `user` (
				  `ID` smallint(5) unsigned NOT NULL auto_increment,
				  `name` varchar(32) NOT NULL default '',
				  `username` varchar(32) NOT NULL default '',
				  `password` varchar(64) NOT NULL default '',
				  `email` varchar(64) NOT NULL default '',
				  `cmsLang` varchar(2) default NULL,
				  `usertypeID` tinyint(3) unsigned NOT NULL default '0',
				  `last_log` date NOT NULL default '0000-00-00',
				  `cookie` varchar(64) NOT NULL default '',
				  PRIMARY KEY  (`ID`),
				  UNIQUE KEY `username` (`username`)
				) DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>