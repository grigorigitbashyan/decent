<?php
Site::IncludeFile('classes/Subscribe/SubscribeListCMS.class.inc');
Site::IncludeFile('classes/Subscribe/Subscriber/SubscriberListCMS.class.inc');
Site::IncludeFile('classes/Common/MergingModules.class.inc');

class MergeSubscribe extends MergingModules 
{
	function __construct()
	{
		parent::__construct();
		
		$this->AddModule(new SubscribeListCMS(), 'Subscribe');
		$this->AddModule(new SubscriberListCMS(), 'Subscribers');
	}
}
?>