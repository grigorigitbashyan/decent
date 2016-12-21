<?php
/**
 * Enter class description
 */
class News extends NamedRecord
{
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	*/
	function __construct($ID = null)
	{
		parent::__construct($ID, 'news');
	
		$this->Read();
	}
	/**
	 * Insert News 
	* @return bool
	*/
	function Insert($properties)
	{
		// delete from content DB
		return $this->InsertRecord($properties);	}

	/**
	 * Delete News 
	* @return bool
	*/
	function Delete()
	{
		// delete from content DB
		return $this->DeleteRecord();	}

}
?>