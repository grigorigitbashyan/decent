<?php
Site::IncludeFile('classes/Box/SBRelation.class.inc');
Site::IncludeFile('classes/Box/BoxCollectionView.class.inc');

/**
 * Enter description here...
 *
 * @package Box
 */
class BoxCollectionViewSite
{
	/**
	 * Enter description here...
	 *
	 * @var SBRelation
	 */
	protected $m_sbRelation;
	
	function __construct($catID)
	{
		$this->m_sbRelation = new SBRelation ( $catID );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $align
	 * @return bool
	 */
	function Display($align, $divClass = '', $tag = 'div')
	{
		$boxCollID = $this->m_sbRelation->GetCatRelationID ( $align );
		if(!$boxCollID)
		{
			$boxCollID = $this->m_sbRelation->GetCatRelationID ( $align, true );
		}
		if ($boxCollID)
		{
			$boxColl = new BoxCollectionView ( $boxCollID );
			$boxColl->Display($divClass, $tag);
			
			return true;
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $align
	 * @return bool
	 */
	function HasBoxCollection($align)
	{
		$boxCollID = $this->m_sbRelation->GetCatRelationID ( $align );
		if(!$boxCollID)
		{
			$boxCollID = $this->m_sbRelation->GetCatRelationID ( $align, true );
		}

		return $boxCollID;
	}
}
?>