<?php
Site::IncludeFile('classes/Subscribe/Subscriber/SubscriberList.class.inc');
Site::IncludeFile('classes/Subscribe/Subscriber/Subscriber.class.inc');

/**
 * Enter class description
 */
class SubscriberListCMS extends CMSObject
{
	/**
	 * Enter description here...
	 *
	 * @var SubscriberList
	 */
	protected $m_list;
	/**
	 * Enter description here...
	 */
	function __construct()
	{
		parent::__construct ();
		$this->ReadNameStrings ( 'classes/Subscribe/langs/', 'subscriber.xml' );
		
		$this->Update();
	}
	/**
	* All actions must be done hear
	*
	* @return bool
	*/
	function DoAction()
	{
		$res = false;
		
		switch ($this->GetFormAction())
		{
			case 'subcribersAction':
				$IDs = DB::POST('subscriberIDs');
				if($IDs)
				{
					foreach ($IDs as $ID)
					{
						$suber = new Subscriber($ID);
						$res = $suber->Delete();
					}
				}
				break;
		}
		
		if($res)
		{
			$this->Update();
		}
	}

	function Update()
	{
		$this->m_list = new SubscriberList();
	}
	/**
	* Display menu
	*
	*/
	function DisplayMenu()
	{
		$moduleNameS = 'Subscriber';//$this->GetNameString ( 'moduleName' );
		print("<div class='someWTitleBox'><a href='?SubscribeListID=1'>$moduleNameS</a></div>");
	}

	/**
	* Display features
	*
	*/
	function DisplayFeatures()
	{
		if($this->m_list->GetItemCount())
		{
			$allS = $this->GetNameString ( 'all' );
				$deleteS = $this->GetNameString ( 'delete' );
				$noneS = $this->GetNameString ( 'none' );
				$selectS = $this->GetNameString ( 'select' );
				$noSelectedS = $this->GetNameString ( 'noselected' );
				$sureDeleteS = $this->GetNameString ( 'sureDelete' );
		
			// display form header
			$this->DisplayFormHeadr('subcribersAction', 'subscribersForm');
			
			print('<div class="titleBox">
				<div class="titleColR">
					<span style="padding-right: 20px"><strong>'.$selectS.'</strong>
						<a href="#"	onclick="chekeBoxes(\'subscribersForm\', \'subscriberIDs[]\', true); return false">'.$allS.'</a>,
						<a href="#"	onclick="chekeBoxes(\'subscribersForm\', \'subscriberIDs[]\', false); return false">'.$noneS.'</a>
					</span>');
			
			
					if ($this->IsDeletable ( 'subscriber' ))
					{
						print ( "<input name=\"delete\" type=\"submit\" value=\"$deleteS\" onclick=\"return ChecksFormDelete('subscribersForm', 'subscriberIDs[]', '$noSelectedS', '$sureDeleteS');\" />" );
					}

			print('</div><br clear="all" /></div>');
			// display list
			print('<div class="someTableArea">');
			
			$pattern = "<div class=\"tableCHB\">
	                    	<input name='subscriberIDs[]' id='subscriberIDs[]' type='checkbox' value='@@ID@@' />
	                	</div>
	                    <div class=\"tableT\">
	                    	@@name@@ 
		                </div>
		                <div class=\"tableT\">
	                    	@@email@@ 
		                </div>";
			
			$this->m_list->DisplayListDiv ( $pattern, array ('tableUneven', 'tableEven' ), '<br clear="all" />' );
			
			print('</div><br />');
			
			// display form footer
			print('</form>');
			
			// display navigation
			$this->m_list->DisplayNavigation();
		}
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `subscriber` (
				  `ID` int(11) NOT NULL auto_increment,
				  `code` varchar(10) character set latin1 collate latin1_bin NOT NULL,
				  `name` varchar(128) NOT NULL,
				  `email` varchar(64) character set latin1 collate latin1_bin NOT NULL,
				  `statistic` varchar(2) character set latin1 collate latin1_bin default NULL,
				  `additional` varchar(256) default NULL,
				  UNIQUE KEY `email` (`email`),
				  KEY `ID` (`ID`)
				) ENGINE=MyISAM  DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>