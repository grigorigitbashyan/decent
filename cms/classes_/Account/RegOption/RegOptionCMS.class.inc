<?php
Site::IncludeFile ( 'classes/Account/RegOption/RegOption.class.inc' );
Site::IncludeFile ( 'classes/Content/ContentTypeCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package Account
 */
class RegOptionCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var RegOption
	 */
	protected $m_regOption;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID = null)
	{
		parent::__construct ();
		
		$this->m_regOption = new RegOption ( $ID );
		
		$this->ReadNameStrings ( 'classes/Account/langs/', 'siteuser.xml' );
	}
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		// names
		$doneS = $this->GetNameString ( 'done' );
		/////////////////////////////
		$optID = $this->m_regOption->GetID ();
		$value = $this->m_regOption->GetValue ();
		
		// read name
		$regNameS = $this->GetNameString ( "regName{$optID}" );
		$this->DisplayTitle ( $regNameS, null, false );
		
		if ($optID == 0)
		{
			$activationTypeS = $this->GetNameString ( 'activationType' );
			
			print ( '<div class="someContent">' );
			$this->DisplayFormHeadr ( 'setActivation' );
			$this->DisplayHidden ( 'regOptID', 1 );
			
			print ( '<div class="metadataFormLabel"><label><strong>' . $activationTypeS . '</strong></label></div>' );
			print ( '<div style="float:left">' );
			
			//////////////////////////////////////////////////////////////
			

			if ($value == 2)
			{
				print ( '<input name="actType" id="actType" type="radio" value="2" checked="checked" />' );
			}
			else
			{
				print ( '<input name="actType" id="actType" type="radio" value="2" />' );
			}
			
			$actType2S = $this->GetNameString ( 'actType2' );
			print ( "<lable>$actType2S</lable><br />" );
			
			////////////////////////////////////////////////////////////
			

			if ($value == 3)
			{
				
				print ( '<input name="actType" id="actType" type="radio" value="3" checked="checked"  />' );
			
			}
			else
			{
				print ( '<input name="actType" id="actType" type="radio" value="3" />' );
			}
			
			$actType3S = $this->GetNameString ( 'actType3' );
			print ( "<lable>$actType3S</lable><br /><br />" );
			
			///////////////////////////////////////////////
			

			print ( '</div><br class="clearFloat" />' );
			print ( '<div class="metadataFormLabel">
				<label><strong>&nbsp;</strong></label></div>' );
			
			print ( '<div style="float:left">' );
			$this->DisplayFormFooter ( $doneS );
			print ( '</div>' );
		}
		else
		{
			$contentID = $this->m_regOption->GetValue ();
			$regID = $this->m_regOption->GetID ();
			$actionName = $this->GetFormActionName ();
			
			$contTypelist = new ContentTypeCMS ( );
			$contTypelist->DispalayContentRelation ( $contentID, 'regOptID', $regID, $actionName );
		}
	}
	
	/**
	 * @see CMSInterface::DisplayMenu()
	 *
	 */
	function DisplayMenu()
	{
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
			case 'addRelation' : // also delete, if paramValue is equal 0
				$paramValue = DB::POST ( 'paramValue' );
				if ($paramValue)
				{
					// insert new content section
					if ($paramValue == 0)
					{
						$contTypeCMS = new ContentTypeCMS ( );
						$newParamVal = $contTypeCMS->DoAction ();
						
						if (is_int ( intval ( $newParamVal ) ) && $newParamVal > 1)
						{
							$paramValue = $newParamVal;
						}
					}
					
					// rename
					$ar = $this->m_regOption->SetValue ( $paramValue );
				}
				break;
			case 'setActivation' :
				$actType = DB::POST ( 'actType' );
				if ($actType)
				{
					// rename
					$ar = $this->m_regOption->SetValue ( $actType );
				}
				break;
		}
		
		return $ar;
	}
}
?>