<?php
/**
* Enter class description here...
*
* @package Content
*/
class ContentType extends NamedRecord
{
	/**
	* Enter description here...
	*
	* @param int $ID
	* @return Contenttype
	*/
	function __construct($ID = null)
	{
		 parent::__construct($ID, 'contenttype');
		 
		 if($ID !== null)
		 {
		 	$this->Read();
		 }
	}

	/**
	* Delete record from database
	*
	* @return bool
	*/
	function Delete()
	{
		 return $this->DeleteRecord();
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
	* @param string $name
	* @return bool
	*/
	function Insert($name)
	{
        return $this->InsertRecord(array('name' => $name));
	}
}
?>