<?php
/**
* Enter class description here...
*
* @package Box
*/
class BoxCollection extends NamedRecord
{
	/**
	* Enter description here...
	*
	* @param int $ID
	* @return BoxCollection
	*/
	function __construct($ID = null)
	{
		 parent::__construct($ID, 'boxcollection');

		 $this->Read();
	}

	/**
	* Update record
	*
	* @param array $properties
	* @return bool
	*/
	function Update($properties)
	{
		 return $this->UpdateValues($properties);
	}
	/**
	* Insert new record
	*
	* @param array $properties
	* @return bool
	*/
	function Insert($properties = null)
	{
		 return $this->InsertRecord($properties);
	}
}
?>