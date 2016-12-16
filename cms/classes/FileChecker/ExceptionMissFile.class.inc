<?php
class ExceptionMissFile extends Exception
{
	static protected $missFiles = array();
	
	/**
	 * 
	 * @param string $missFile
	 * @param string $message
	 * @param int $code
	 */
	function __construct($missFile, $message = '', $code = null)
	{
		parent::__construct($message, $code);

		// add exception to static array
		if(!in_array($missFile, ExceptionMissFile::$missFiles))
		{
			ExceptionMissFile::$missFiles[] = $missFile;
		}
	}
	
	/**
	 * return array
	 */
	static public function GetMissFiles()
	{
		return ExceptionMissFile::$missFiles;
	} 
}