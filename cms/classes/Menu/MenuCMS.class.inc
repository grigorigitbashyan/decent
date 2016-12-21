<?php
/**
 * Enter description here...
 *
 * @package Menu
 */
class MenuCMS extends CMSObject implements CMSInterface
{
	/**
	 * Enter description here...
	 *
	 * @var Menu
	 */
	protected $m_menu;
	
	/**
	 * Enter description here...
	 *
	 * @param int $menuID
	 */
	function __construct($menuID = null)
	{
		parent::__construct ();
		$this->m_menu = new Menu ( $menuID );
		
		$this->ReadNameStrings ( 'classes/Menu/langs/', 'menu.xml' );
	}
	/**
	 * @see CMSInterface::DisplayFeatures()
	 *
	 */
	function DisplayFeatures()
	{
		if ($this->m_menu->GetID ())
		{
			$name = $this->m_menu->GetName ();
			$depth = $this->m_menu->GetDepth ();
			$doneS = $this->GetNameString ( 'done' );
			
			// get strucutre
			$structure = $this->m_menu->GetStructure ();
			$form = null;
			
			if (count ( $structure ) == 0)
			{
				// menu delete controll
				if ($this->IsDeletable ( 'menu' ))
				{
					$deleteS = $this->GetNameString ( 'delete' );
					$sureDeleteS = $this->GetNameString ( 'sureDelete' );
					$actionName = $this->GetFormActionName ();
					
					$form = "<div class='titleColR'>
						 <form method='post' class='clearPD'> 
						 <input type='hidden' name='$actionName' value='delete' />
						 <input type='hidden' name='menuID' value='{$this->m_menu->GetID()}' />
						 <input type='submit' onclick='return confirm(\"$sureDeleteS\");' value='$deleteS' />
						 </form></div>";
				}
			}
			
			$this->DisplayTitle ( $name, $form );
			
			// menu details and names
			$depthS = $this->GetNameString ( 'menuNewDepth' );
			$optionsS = $this->GetNameString ( 'options' );
			// display form
			$hid ['menuID'] = $this->m_menu->GetID ();
			
			// change depth
			$panelIndex = 0;
			
			print ( "<div class='someGTitleBox'>$optionsS</div>" );
			print ( '<div class="someGBox">' );
			
			$rename = $this->GetNameString ( 'rename' );
			$noEmptyS = $this->GetNameString ( 'noempty' );
			
			// rename
			// tab header
			$updating = CMSObject::IsRecUpdatable ( $this->m_menu, 'name' );
			$reading = CMSObject::IsRecReadable ( $this->m_menu, 'name' );
			
			if ($reading || $updating)
			{
				$this->DisplayTabHeader ( ++ $panelIndex, $rename );
				if ($updating)
				{
					$renameJS = "onsubmit='return SingleForm(\"name\", \"$noEmptyS\");'";
					$this->DisplayFormHeadr ( 'rename', 'renameForm', 'post', $renameJS );
					$this->DisplayHidden ( 'menuID', $hid ['menuID'] );
					print ( "<input type=\"text\" value=\"$name\" name=\"name\" id=\"name\" size=\"64\" maxlength=\"256\" />\n" );
					$this->DisplayFormFooter ( $doneS );
				}
				else
				{
					print ( $name );
				}
				// tab footer
				$this->DisplayTabFooter ();
			}
			
			// depth form
			$updating = $this->IsRecUpdatable ( $this->m_menu, 'name' );
			$reading = $this->IsRecReadable ( $this->m_menu, 'name' );
			
			if ($reading || $updating)
			{
				$this->DisplayTabHeader ( ++ $panelIndex, $depthS );
				
				if ($updating)
				{
					$this->DisplayFormHeadr ( 'changeDepth', 'renameForm', 'post', $renameJS );
					print ( "<input type=\"text\" value=\"$depth\" name=\"depth\" id=\"depth\" size=\"2\" maxlength=\"2\" />\n" );
					$this->DisplayFormFooter ( $doneS );
				}
				else
				{
					print ( $depth );
				}
				
				// tab footer
				$this->DisplayTabFooter ();
			}
			
			print ( '</div>' );
			// display javascript
			print ( '<script type="text/javascript"><!--' );
			
			for($index = 0; $index <= $panelIndex; $index ++)
				print ( "var CollapsiblePanel$index = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel$index\", {contentIsOpen:false});\n" );
			
			print ( '//--></script>' );
		}
		else
		{
			// insert control
			if (! $this->IsInsertable ( 'menu' ))
			{
				// do not have permition to add new menu
				return;
			}
			// getting names
			$doneS = $this->GetNameString ( 'done' );
			$menuNewS = $this->GetNameString ( 'menuNew' );
			$menuNameS = $this->GetNameString ( 'menuName' );
			$menuDepthS = $this->GetNameString ( 'menuDepth' );
			$noEmptyS = $this->GetNameString ( 'noempty' );
			
			// display title
			$this->DisplayTitle ( $menuNewS );
			
			// js for inserting
			//TODO complite validation
			$iJS = "onsubmit='return SingleForm(\"menuName\", \"$noEmptyS\")'";
			// display form
			print ( '<div class="someContent">' );
			$this->DisplayFormHeadr ( 'addmenu', 'newMenuForm', 'post', $iJS );
			print ( '<div class="metadataFormLabel"><label><strong>' . $menuNameS . '</strong></label></div>' );
			print ( '<div style="float:left"><input name="menuName" id="menuName" value="" type="text" size="30" maxlength="255" style="font-size:14px" /></div>' );
			print ( '<br class="clearFloat" />' );
			print ( '<div class="metadataFormLabel"><label><strong>' . $menuDepthS . '</strong></label></div>' );
			print ( '<div style="float:left"><input name="menuDepth" value="3" type="text" size="2" maxlength="2" style="font-size:14px" /></div>' );
			print ( '<br class="clearFloat" />' );
			print ( '<div class="metadataFormLabel"><label><strong>&nbsp;</strong></label></div>' );
			print ( '<div style="float:left"><input name="" type="submit" value="' . $doneS . '" /></div>' );
			print ( '<br class="clearFloat" />' );
			print ( '</form>' );
			print ( '</div>' );
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
		switch ($this->GetFormAction ())
		{
			case 'addmenu' :
				// get name and depth
				$name = DB::POST ( 'menuName' );
				$depth = DB::POST ( 'menuDepth' );
				
				$menu = new Menu ( );
				$menu->Insert ( $name, $depth, 1 );
				break;
			case 'rename' :
				$name = DB::POST ( 'name' );
				$this->m_menu->SetName ( $name );
				break;
			case 'delete' :
				$this->m_menu->Delete ();
				break;
			case 'changeDepth' :
				$depth = DB::POST ( 'depth' );
				if ($depth && is_int ( intval ( $depth ) ))
				{
					// change menu depth
					$newDepth = intval ( $depth );
					$this->m_menu->SetDepth ( $newDepth );
				}
				break;
		}
	}
	
	/**
	 * @see CMSObject::CreateDB()
	 *
	 * @return bool
	 */
	function CreateDB()
	{
		$res = parent::CreateDB ( );
		
		if($res)
		{
			$menu = new Menu();
			$res &= $menu->Insert("Additional menu", 1);
		}
		
		return $res;
	}
	
	/**
	 * @see CMSObject::CreateDBSQL()
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		$sql = "CREATE TABLE `menu` (
				  `ID` tinyint(3) unsigned NOT NULL auto_increment,
				  `name` varchar(255) NOT NULL default '',
				  `depth` tinyint(3) unsigned NOT NULL default '1',
				  PRIMARY KEY  (`ID`)
				)DEFAULT CHARSET=utf8;";
		
		return $sql;
	}
}
?>