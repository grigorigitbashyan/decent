<?php

Site::IncludeFile ( 'classes/Common/CMSListObject.class.inc' );
Site::IncludeFile ( 'classes/Subscribe/SubscribeCMS.class.inc' );
Site::IncludeFile ( 'classes/Subscribe/SubscribeList.class.inc' );

Site::IncludeFile ( 'classes/Subscribe/SURelationCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package Notification
 */
class SubscribeListCMS extends CMSListObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ( new SubscribeList ( ), 'Subscribe' );
		
		$this->ReadNameStrings ( 'classes/Subscribe/langs/', 'subscribe.xml' );
	}
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		// display current object if it is selected
		$isDisplayed = parent::DisplayFeatures ();
		
		// diplay new box form
		if (! $isDisplayed)
		{
			// display form title
			$newSubscribeS = $this->GetNameString ( 'newSubscribe' );
			$this->DisplayTitle ( $newSubscribeS );
			
			// display insert form
			$newSubscribeNameS = $this->GetNameString ( 'newSubscribeName' );
			$doneS = $this->GetNameString ( 'done' );
			$noEmptyS = $this->GetNameString ( 'noempty' );
			
			// display form
			$insertJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
			$this->DisplaySingleTextForm ( 'insert', $newSubscribeNameS, 'name', null, null, $doneS, $insertJS );
			// end new box form
		}
	}
	
	/**
	 * @see CMSInterface::DisplayMenu()
	 *
	 */
	function DisplayMenu()
	{
		$subscribeS = $this->GetNameString ( 'subscribe' );
		
		parent::DisplayMenu ( $subscribeS );
		
	// if there is a unfinished proccess then display link to continue
	//		$finished = true;
	//		$subscribes = $this->m_list->GetStructure();
	//		
	//		foreach ($subscribes as $subscribe)
	//		{
	//			if($subscribe['status'] == 1)
	//			{
	//				$finished = false;
	//				break;
	//			}
	//		}
	//		
	//		if(!$finished)
	//		{
	//			print('<a href="?">Continue</a>');	
	//		}
	}
	
	/**
	 * @see CMSInterface::DoAction()
	 *
	 */
	function DoAction()
	{
		// action result
		$ar = parent::DoAction ();
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$res = parent::CreateDB ();
		
		$su = new SURelationCMS ( );
		$res &= $su->CreateDB ();
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$res = parent::SelfTest ();
		
		$su = new SURelationCMS ( );
		$res &= $su->SelfTest ();
		
		// cache xml
		if (! file_exists ( 'config/subscribe.xml' ))
		{
			$res = false;
			print ( 'The config file <b>subscribe.xml</b> does not exist<br />' );
		}
		
		return $res;
	}
	
	/**
	 * This function returns object tables.
	 *
	 * @return array
	 */
	function GetObjectTables()
	{
		$res = parent::GetObjectTables ();
		
		$su = new SURelationCMS ( );
		$suR = $su->GetObjectTables ();
		
		$res = array_merge($res, $suR);
		// cache xml
				
		return array_unique($res);
	}
}
?>