<?php
Site::IncludeFile ( 'classes/RecycleBin/RecycleBinList.class.inc' );
Site::IncludeFile ( 'classes/RecycleBin/RecycleBin.class.inc' );
Site::IncludeFile ( 'classes/Folder/Folder.class.inc' );

/**
 * Enter description here...
 *
 * @package RecycleBin
 */
class RecycleBinListCMS extends CMSObject implements CMSInterface
{
	
	/**
	 * Enter description here...
	 *
	 * @var RecycleBinList
	 */
	protected $m_recList;
	
	protected $m_recycleType;
	
	function __construct()
	{
		parent::__construct ();
		
		$this->Update ();
	}
	
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		// get names
		$seleteS = $this->GetNameString('select');
		$allS = $this->GetNameString('all');
		$noneS = $this->GetNameString('none');
		$deleteS = $this->GetNameString('delete');
		$sureDeleteS = $this->GetNameString('finalDelete');
		$noSelectedS = $this->GetNameString('noselected');
		
		// display product list
		$countrecycleBins = $this->m_recList->GetAllCount ();
		
		if ($countrecycleBins > 0)
		{
			$patern = "<div class='mediaImgBox'>
		                    <div class='mediaImgUnit'>
		        			<img src='img/recycle/@@type@@.gif' height='80' border='0' alt='image' /></div>
		                    <div class='mediaImgChackbox'>
		                    <input name='recycleItemIDs[]' id='recycleItemIDs[]' type='checkbox' value='@@ID@@' />
		                    </div> 
		                    <div class='mediaImgTitle'><span>@@name@@</span></div>
		                </div>";
			
			$this->DisplayFormHeadr ( 'groupAction', 'recycleBins' );
			?>
<br />
<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
	<tr>
		<td>
		<div class="titleColL"><span style="padding-right: 20px">
								 <?php
			$this->m_recList->DisplayNavigation ( "pr_typeID=$this->m_recycleType" );
			?>
						</div>
		<div class="titleColR"><span style="padding-right: 20px"> <strong><?php echo $seleteS?></strong> <a href="#"
			onclick="chekeBoxes('recycleBins', 'recycleItemIDs[]', true); return false"><?php echo $allS?></a>, <a href="#"
			onclick="chekeBoxes('recycleBins', 'recycleItemIDs[]', false); return false"><?php echo $noneS?></a> </span> 
			<input name="delete" type="submit"	value="<?php echo $deleteS?>"  onclick="return ChecksFormDelete('recycleBins', 'recycleItemIDs[]', '<?php echo $noSelectedS?>', '<?php echo $sureDeleteS?>');"  /> <input name="restore" type="submit" value="Restore" /></div>
		<br class="clearFloat" />
		</td>
	</tr>
</table>
<table width="100%" border="0" cellspacing="0" cellpadding="0"
	style="border-left: 1px solid #777777; border-right: 1px solid #777777; border-bottom: 1px solid #777777">
	<tr>
		<td>
		<?php
			
			$this->m_recList->DisplayListDiv ( $patern );
			print ( '</td></tr></table></form>' );
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
			case 'groupAction' :
				if (($IDs = DB::POST ('recycleItemIDs')))
				{
					// delete contents
					if (isset ( $_POST ['delete'] ))
					{
						foreach ( $IDs as $ID )
						{
							$recycleBin = new RecycleBin ( $ID );
							$ar = $recycleBin->Delete ();
						}
					}
					
					// move contents
					if (isset ( $_POST ['restore'] ))
					{
						foreach ( $IDs as $ID )
						{
							$recycleBin = new RecycleBin ( $ID );
							$ar = $recycleBin->Restore ();
							
							if ($ar)
							{
								$ar = $recycleBin->Delete ();
								$this->Update ();
							}
						}
					}
				}
				break;
		}
		
		return $ar;
	}
	
	function Update()
	{
		$this->m_recList = new RecycleBinList ( );
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		// get
		$site = View::GetSite ();
		$langs = $site->GetLanguages ();
		
		$langS = '';
		foreach ( $langs as $lang )
		{
			$langS .= "`name_$lang` varchar(255) NOT NULL,";
		}
		
		$sql = "CREATE TABLE `recycle_bin` (
				  `ID` int(10) unsigned NOT NULL auto_increment,
				  `type` varchar(64) NOT NULL,
				  $langS
				  `value` mediumtext NOT NULL,
				  PRIMARY KEY  (`ID`)
				)DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function SelfTest()
	{
		// test folder
		$resPath = Site::GetRecycleBinPath ();
		$res = true;
		
		// if the dir does not exist and it is allowed to make it then try to do that. 
		if(!file_exists($resPath) && $this->CanBeCorrected())
		{
			$res &= FolderXP::Mkdir($resPath);
		}

		// check is dir exist
		if(!file_exists($resPath))
		{
			$res = false;
			print("The folder <b>$resPath</b> does not exist!\n<br />");
		}
		// test database
		$res &= $this->TestTable();
		 
		return $res; 
	}
}
?>