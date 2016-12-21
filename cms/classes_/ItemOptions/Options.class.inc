<?php
/**
 * Enter class description here...
 *
 * @package ItemOptions
 */
class Options extends NamedRecord
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected $m_category_id;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 * @return Options
	 */
	function __construct($ID = null)
	{
		parent::__construct ( $ID, 'options' );
		$this->Read ();
	}
	
	/**
	 * Update record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Update($properties)
	{
		return $this->UpdateValues ( $properties );
	}
	/**
	 * Insert new record
	 *
	 * @param array $properties
	 * @return bool
	 */
	function Insert($properties = null)
	{
		return $this->InsertRecord ( $properties );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool|string
	 */
	function Delete()
	{
		// delete all items
		$additional = array ();
		
		// get items list
		$listOfOptions = new OptionsdataList ( $this->m_ID );
		$items = $listOfOptions->GetStructure ();
		
		if ($items)
		{
			foreach ( $items as $itemA )
			{
				$item = new Optionsdata ( $itemA ['ID'] );
				$resArray = $item->Delete ();
				array_push ( $additional, $resArray [0] );
			}
		}
		else 
		{
			$additional = null;
		}
		
		// delete
		return parent::Delete ( true, $additional );
	}
	
	/**
	* Return value of m_category_id
	*
	* @return int
	*/
	function GetCategoryID()
	{
		return $this->m_category_id;
	}
	
	/**
	* Set value to m_category_id  
	*
	* @param int $value
	* @param bool $dbupdate
	* @return bool
	*/
	function SetCategoryID($value, $dbupdate = true)
	{
		return $this->UpdateValue('category_id', $value, $dbupdate);
	}
}
?>