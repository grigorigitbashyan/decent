<?php
// inculdes
Site::IncludeFile ( 'classes/Box/Box.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxCMS.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxList.class.inc' );

/**
 * Enter description here...
 *
 * @package Box
 */
class BoxListCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var BoxList
	 */
	protected $m_boxList;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct ();
		
		$this->m_boxList = new BoxList ( );
		
		$this->ReadNameStrings ( 'classes/Box/langs/', 'box.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function DoAction()
	{
		// action result
		$ar = null;
		
		// call DoAction of BoxCMS
		

		$overLink = $this->m_boxList->GetLinkIDName ();
		if (($postAction = DB::POST($overLink)))
		{
			$boxCMS = new BoxCMS ( $postAction );
			$ar = $boxCMS->DoAction ();
		}
		
		// do actions for Box list
		switch ($this->GetFormAction ())
		{
			case 'insert' :
				
				$name = DB::POST ('name');
				$type = DB::POST ('boxtype');
				if ($name && $type )
				{
					if (strlen ( trim ( $name ) ) > 0)
					{
						// insert new box with name
						$newBox = new Box ( );
						$params = array ('name' => $name, 'type' => $type );
						$ar = $newBox->Insert ( $params );
					}
				}
				break;
		}
		
		// if there is a any action then update object  
		if ($ar)
		{
			$this->__construct ();
		}
		
		return $ar;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayMenu()
	{
		$currID = $this->m_boxList->GetCurrentID ();
		
		$boxS = $this->GetNameString ( 'box' );
		
		if ($currID)
		{
			$linkName = $this->m_boxList->GetLinkIDName ();
			print ( "<div class='someWTitleBox'><a href='?$linkName='>$boxS</a></div>" );
		}
		else
		{
			print ( "<div class='someWTitleBox'>$boxS</div>" );
		}
		print ( '<div class="someWBox">' );
		
		$this->m_boxList->DisplayList ( 'directoryMenu1', 'selLink' );
		
		print ( '</div>' );
		print ( '</div>' );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DisplayFeatures()
	{
		$currentBoxID = $this->m_boxList->GetCurrentID ();
		
		// get cooretn box
		$box = new Box ( $currentBoxID );
		
		$currentBoxID = $box->GetID ();
		// if is selected box, then diplay box features
		if ($currentBoxID)
		{
			$boxCMS = new BoxCMS ( $currentBoxID );
			$boxCMS->DisplayFeatures ();
		
		} // diplay new box form
		else
		{
			$newBoxS = $this->GetNameString ( 'newBox' );
			$newBoxNameS = $this->GetNameString ( 'newBoxName' );
			$doneS = $this->GetNameString ( 'done' );
			$noEmptyS = $this->GetNameString('noempty');
			
			// display form title
			$this->DisplayTitle ( $newBoxS );
			
			// display insert form ///////////////////////////////////////////////////////////////
			$insertJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
			$this->DisplayFormHeadr ( 'insert', 'insertBobForm', 'post', $insertJS );
			print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox"><tr><td><div class="titleColL">' );
			// display label
			print ( "<label><strong>$newBoxNameS</strong></label>\n" );
			print ( "<input type='text' name='name' id='name' size='64' maxlength='256' style='font-size:14px' />\n" );
			print('<label>Content: </label><input name="boxtype" type="radio" value="1" checked="checked" />');
			print('<label>File: </label><input name="boxtype" type="radio" value="2" />');
			print ( '</div><div class="titleColR"><input name="" type="submit" value="' . $doneS . '" /></div>
			         <br class="clearFloat" /></td></tr></table></form>' );
			// end new box form //////////////////////////////////////////////////////////////////
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$boxCMS = new BoxCMS ( );
		return $boxCMS->CreateDB ();
	}
	
/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		$boxCMS = new BoxCMS ( );
		return $boxCMS->SelfTest();
	}
}
?>