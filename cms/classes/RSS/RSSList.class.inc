<?php
Site::IncludeFile('classes/Common/TreeView.class.inc');

class RSSList extends TreeView 
{
	function __construct()
	{
		$rsses = array();
		
		$dirfiles = scandir('rss/');
		 
		foreach ($dirfiles as $file)
		{
			$pathInfo = pathinfo($file);
			
			if($pathInfo['extension'] == 'xml')
			{
				array_push($rsses, array('ID' => $file, 'name' => $file));
			}
		}

		parent::__construct($rsses);
	}
}
?>