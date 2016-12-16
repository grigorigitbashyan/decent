<?php
Site::IncludeFile ( 'classes/Common/TreeViewNavigation.class.inc' );

/**
 * Enter description here...
 *
 * @package Account
 */
class SiteUserList extends TreeViewNavigation
{
	/**
	 * Enter description here...
	 *
	 * @param int $status
	 * @param int $limit
	 * @param int $page
	 * @param int $currID
	 */
	function __construct($status = 1, $limit = 100, $page = null, $currID = null, $lang = 'en', $author = 'all')
	{
		if ( $status==3 )
		{
			if ( $author=="1" || $author=="0" )
			{
				$author = intval($author);
				$sql = "SELECT * FROM `site_user` WHERE `status` = $status and `author` = $author";
			}
			else 
			{
				$sql = "SELECT * FROM `site_user` WHERE `status` = $status";
			}
		}
		else 
		{
			$sql = "SELECT * FROM `site_user` WHERE `status` = $status";
		}
		
		parent::__construct ( $sql, 'siteuser', $limit, $page, 'username', $currID, $lang );
	}
	
	/**
	 * Return count of remaining users, whitch ID great then $userID.
	 * this function can by used in subscribe module to detect procent of sendit users.
	 *
	 * @param int $userID
	 * @return int
	 */
	function GetRemainingUsersCount($userID)
	{
		$sql = "SELECT count(*) as `count` FROM `site_user` WHERE `status` = 3 AND `ID` > $userID";
		$res = DB::QueryToArray ( $sql );
		
		return $res [0] ['count'];
	}
	
	/**
	 * Return remaining users, which ID's great then $userID.
	 * This function can be used in subscribe mudule to get users for sending email. 
	 *
	 * @param int $userID
	 * @param int $limit
	 * @return array
	 */
	function GetRemainingUsers($userID, $limit)
	{
		$sql = "SELECT * FROM `site_user` WHERE `status` = 3 AND ID > $userID ORDER BY `ID` LIMIT $limit";
		$res = DB::QueryToArray ( $sql );
		
		return $res;
	}
}
?>