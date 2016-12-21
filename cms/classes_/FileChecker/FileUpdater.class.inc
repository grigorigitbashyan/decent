<?php
class FileUpdater
{
	/**
	 * 
	 * @var string
	 */
	protected $m_server = 'http://localhost/siteiusupdate/index.php';
	
	function __construct()
	{
		
	}
	
	/**
	 * This function is used to upload and put the missed file.
	 * 
	 * @param string $fileName
	 */
	function PutMissedFile($fileName)
	{
		$fullPath = $this->m_server . '?file=files/'.$fileName;
		
		try 
		{
			$content = file_get_contents($fullPath);
			
			// file has not been found
			if(!trim($content))
				throw new Exception("the file has not found!");
			
			// try to create file and folders
			$dirname = dirname($fileName);
			if(!file_exists($dirname))
			{
				if(!mkdir($dirname, 0777, true))
					throw new Exception("The dir '$dirname' has not been created!");
			}
			
			// try to create file
			if(!file_put_contents($fileName, $content))
				throw new Exception("The file '$fileName' has not been created!");
				
			print("The file '<b>$fileName</b>' has been successfuly created!<br />\n");
		}
		catch(Exception $e)
		{
			$msg = $e->getMessage(); 
			print("Error during update: <b>$msg</b><br />");		
		}
	}
}