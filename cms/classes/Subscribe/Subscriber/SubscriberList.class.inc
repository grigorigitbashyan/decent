<?php
Site::IncludeFile('classes/Common/TreeViewNavigation.class.inc');
/**
 * Enter class description
 */
class SubscriberList extends TreeViewNavigation
{
	/**
	 * Enter description here...
	*/
	function __construct()
	{
		parent::__construct ( null, 'subscriberID', 100);

		$sql = "SELECT * FROM `subscriber` ORDER BY `email`";
		$this->SetQueryString($sql);
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
		$sql = "SELECT * FROM `subscriber` WHERE `ID` > $userID ORDER BY `ID` LIMIT $limit";
		$res = DB::QueryToArray ( $sql );
		
		return $res;
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
		$sql = "SELECT count(*) as `count` FROM `subscriber` WHERE `ID` > $userID";
		$res = DB::QueryToArray ( $sql );
		
		return $res [0] ['count'];
	}
}
?>