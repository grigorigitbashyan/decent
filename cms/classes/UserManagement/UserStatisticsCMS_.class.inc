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
		
		$paid = DB::GET('paid');
		$paidStr = "";
		if ( $paid=="yes" || $paid=="no" )
		{
			$paidStr = " and `paid` = ".($paid=="yes" ? 1: 0);
		}
		else 
		{
			$paid = "all";
		}
		
		//	get user articles
		$query = "select * from `pr_main` where `userID` = ".$userID.$statusStr.$paidStr;
		$userArticlesAll = DB::QueryToArray($query);
		
		$query = "select * from `pr_main` where `userID` = ".$userID.$statusStr.$paidStr.$pageStr;
		$userArticles = DB::QueryToArray($query);
		?>
		<script type="text/javascript" src="../tools/jquery/jquery-1.7.1.min.js"></script>
		<script type="text/javascript">
		$(document).ready(function(){
			$("#status").change(function(){
				var statusVal = $("#status").val();
				document.location = 'index.php?userID=<?php echo $userID;?>&usertypestat=<?php echo $typeID;?>&status=' + statusVal + '&paid=<?php echo $paid;?>';
			});
			$("#paid").change(function(){
				var paidVal = $("#paid").val();
				document.location = 'index.php?userID=<?php echo $userID;?>&usertypestat=<?php echo $typeID;?>&status=<?php echo $status;?>&paid=' + paidVal;
			});
		});
		</script>
		<?php
		// display form header
		$this->DisplayFormHeadr('modifyStats', 'statsForm', 'post', null, "index.php?userID=".$userID."&usertypestat=".$typeID."&status=".$status."&paid=".$paid);
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
							<span><a href="index.php?userID=<?php echo $userID;?>&usertypestat=<?php echo $typeID;?>&status=<?php echo $status;?>&paid=<?php echo $paid;?>&page=<?php echo $i+1;?>"><?php echo $i+1;?></a></span>
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
				&nbsp;&nbsp;Paid 
				<select name="paid" id="paid">
					<option value="all">All</option>
					<option value="yes"<?php echo ($paid=="yes" ? " selected": "");?>>Yes</option>
					<option value="no"<?php echo ($paid=="no" ? " selected": "");?>>No</option>
				</select>
				&nbsp;&nbsp;
				<input type="submit" name="setpaid" id="setpaid" value="Set paid">
				<input type="submit" name="setunpaid" id="setunpaid" value="Set unpaid">
			</div>
			<br class="clearFloat" />
		</div>
		<div class="someTableArea">
			<div class="tableUneven">
				<div class="tableCHB" style="width:20px;">&nbsp;</div>
				<div class="tableT"><b>Title</b></div>
				<div class="tableD" style="width:60px;"><b>Date</b></div>
				<div class="tableD" style="width:30px;"><b>Paid</b></div>
				<div class="tableD" style="width:60px;"><b>Status</b></div>
				<div class="tableD" style="width:190px;"><b>Structure</b></div>
			</div>
			<br class="clearFloat" />
			<?php
			for ( $i=0; $i<count($userArticles); $i++ )
			{
				$tempPr = new Pr_main($userArticles[$i]["ID"]);
				?>
				<div class="tableUneven">
					<div class="tableCHB" style="width:20px;"><input name="userarticles[]" id="userarticles[]" type="checkbox" value="<?php echo $userArticles[$i]["ID"];?>" /></div>
					<div class="tableT"><?php echo $userArticles[$i]["name_ru"];?></div>
					<div class="tableD" style="width:60px;"><?php echo $userArticles[$i]["date"];?></div>
					<div class="tableD" style="width:30px;"><?php echo ($userArticles[$i]["paid"] ? "yes": "no");?></div>
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