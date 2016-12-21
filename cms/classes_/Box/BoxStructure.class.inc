<?php
Site::IncludeFile ( 'classes/Common/MergingModules.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxListCMS.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxCollectionListCMS.class.inc' );

/**
 * Enter description here...
 *
 * @package Box
 */
class BoxStructure extends MergingModules
{
	
	/**
	 * 
	 */
	public function __construct()
	{
		parent::__construct ( 'boxsub' );
		
		$this->ReadNameStrings ( 'classes/Box/langs/', 'box.xml' );
		
		$boxS = $this->GetNameString ( 'box' );
		$boxCollS = $this->GetNameString ( 'boxCollection' );
		
		$this->AddModule ( new BoxListCMS ( ), $boxS );
		$this->AddModule ( new BoxCollectionListCMS ( ), $boxCollS );
	}
}
?>