<?php
Site::IncludeFile('classes/Common/AddExtension.class.inc');

Site::IncludeFile('classes/Category/CategoryCMS.class.inc');

class ExCategoryCMS extends AddExtension 
{
	function __construct()
	{
		parent::__construct();
		
		$this->AddMainModule(new CategoryCMS());
		$this->ReadExtFromXML('config/structure.xml');
	}
}
?>