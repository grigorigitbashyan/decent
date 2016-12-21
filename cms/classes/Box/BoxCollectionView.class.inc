<?php
// includes
Site::IncludeFile ( 'classes/Box/BoxView.class.inc' );
Site::IncludeFile ( 'classes/Box/BoxCollectionBox.class.inc' );

/**
 * Enter description here...
 *
 * @package Box
 */
class BoxCollectionView extends View
{
	/**
	 * BoxCollection to Box relatino
	 *
	 * @var BoxCollectionBox
	 */
	protected $m_boxCollection;
	
	/**
	 * Contructore
	 *
	 * @param int $ID
	 */
	function __construct($ID)
	{
		$this->m_boxCollection = new BoxCollectionBox ( $ID );
	}
	
	/**
	 * Display all boxes in collection
	 *
	 * set $align to true for vertical displaying, and false for horizontal 
	 * @param $align bool
	 */
	function Display($divClass = null, $tag = 'div')
	{
		// get related boxes
		$boxes = $this->m_boxCollection->GetRelatedBoxList ();
		
		if (! $boxes)
			return;
		
		// div style
		$divStyle = ($divClass) ? "class='$divClass'" : '';
		
		foreach ( $boxes as $boxID )
		{
			// get next box
			$box = new BoxView ( $boxID ['boxID'] );
			
			// display next box
			print ( "<$tag $divStyle>" );
			$box->Display ();
			print ( "</$tag>" );
		}
	}
}
?>