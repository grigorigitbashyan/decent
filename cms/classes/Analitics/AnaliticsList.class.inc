<?php
//includes
Site::IncludeFile ( 'classes/Common/TreeView.class.inc' );

/**
 * Enter class description here...
 *
 * @package List
 */
class AnaliticsList extends TreeView
{
	/**
	 * Enter description here...
	 *
	 * @param int $limit
	 * @return Analitics
	 */
	function __construct($limit=10, $place=0)
	{
		if ( $place>0 )
		{
			$query = "select * from `analitics` where `place` = $place order by `ID` asc limit 0, ".$limit;
		}
		else 
		{
			$query = "select * from `analitics` order by `ID` asc limit 0, ".$limit;
		}
		parent::__construct ( $query, 'analiticsID', 'title' );
	}
}
?>