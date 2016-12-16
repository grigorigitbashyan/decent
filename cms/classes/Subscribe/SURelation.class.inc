<?php


Site::IncludeFile('classes/Common/DB.class.inc');
Site::IncludeFile('classes/Account/SiteUser/SiteUserList.class.inc');
Site::IncludeFile('classes/Subscribe/Subscriber/SubscriberList.class.inc');

/**
 * Enter description here...
 *
 * @package Notification
 */
class SURelation
{
	/**
	 * Enter description here...
	 *
	 * @var Subscribe
	 */
	protected $m_subscribe;
	
	/**
	 * Enter description here...
	 *
	 * @param int $subscribeID
	 */
	function __construct($subscribeID = null)
	{
		$this->m_subscribe = new Subscribe ( $subscribeID );
	}
	
	/**
	 * Return procent of sendit users
	 * 
	 * @param bool $subORreg use users from subscribed or registrated users
	 * @return bool
	 */
	function GetSubscribeProcent($subORreg = true)
	{
		$allUsers = 1;
		$remUsers = 0;
		
		$subscribeID = $this->m_subscribe->GetID ();
		$lastUserID = $this->m_subscribe->GetLastUserID ();
		
		// notification, sending emial to all users
		if ($subscribeID == 1)
		{
			$uesrList = null;
			// read users from subscribed 
			if($subORreg)
			{
				$uesrList = new SubscriberList(  );
			}
			// read users from registrated
			else 
			{
				$uesrList = new SiteUserList ( 3 );
			}
			
			$allUsers = $uesrList->GetAllCount ();
			$remUsers = $uesrList->GetRemainingUsersCount ( $lastUserID );
		}
		else // subscribe , sending email from related database
		{
			// get all count
			$sql = "SELECT count(*) as `count` FROM `subscribe_user` WHERE `subscribeID` = $subscribeID";
			$res = DB::QueryToArray ( $sql );
			
			$allUsers = $res [0] ['count'];
			
			// get remainid count
			

			$sql = "SELECT count(*) as `count` FROM `subscribe_user` WHERE `subscribeID` = $subscribeID AND `userID` > $lastUserID";
			$res = DB::QueryToArray ( $sql );
			
			$remUsers = $res [0] ['count'];
		}
		
		if(!$allUsers)$allUsers = 1;
		
		return floor ( 100 * (100 * (1 - $remUsers / $allUsers)) ) / 100;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $limit
	 * @param bool $subORreg use users from subscribed or registrated users
	 * @return array
	 */
	function GetRemainingUsers($limit, $subORreg = true)
	{
		$users = null;
		
		// 1 is a all subscribers
		if ($this->m_subscribe->GetID () == 1)
		{
			$lastUserID = $this->m_subscribe->GetLastUserID ();
			$uesrList = null;
			// read users from subscribed 
			if($subORreg)
			{
				$uesrList = new SubscriberList(  );
			}
			// read users from registrated
			else 
			{
				$uesrList = new SiteUserList ( 3 );
			}
			
			$users = $uesrList->GetRemainingUsers ( $lastUserID, $limit );
		}
		else
		{
			$subscribeID = $this->m_subscribe->GetID ();
			$lastUserID = $this->m_subscribe->GetLastUserID ();
			
			$sql = "SELECT * FROM `site_user` 
					WHERE `ID` IN (SELECT `ID` FROM `subscribe_user` WHERE `subscribeID` = $subscribeID AND `userID` > $lastUserID LIMIT $limit)";
			
			$users = DB::QueryToArray ( $sql );
		}
		
		return $users;
	}
}
?>