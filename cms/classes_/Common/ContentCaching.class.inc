<?php
/**
 * This class cache contents. 
 *
 * @package Main
 */
class ContentCaching
{
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_cacheRoot;
	
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	
	protected $m_cacheSeconds;
	
	/**
	 * Enter description here...
	 *
	 */
	function __construct($relRoot = null)
	{
		$filePath = ($relRoot) ? $relRoot.'cms/config/cache.xml' : 'cms/config/cache.xml'; 
		$xml = simplexml_load_file($filePath);
		
		$this->m_cacheRoot = (string)(($relRoot) ? ($relRoot) . $xml->path : $xml->path);
		$this->m_cacheSeconds = (int) $xml->seconds;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetCacheFolder()
	{
		return $this->m_cacheRoot;
	}
	/**
	 * This function display cached file if it is exist and return true,
	 * in other case return false. If the $fileName has not been set if 
	 * generates from $_GET
	 *
	 * @param string $fileName
	 * @return bool
	 */
	public function DisplayCachedContent($fileName = null)
	{
		// if $fileName in not set, then get it from $_GET
		if (! $fileName)
		{
			$fileName = $this->GetFileNameFromGet ();
		}
		
		// make file full path
		$filePath = $this->m_cacheRoot . $fileName;
		
		// if cached file exist then chake life time of it 
		if (file_exists ( $filePath ))
		{
			// chack the cached file life time
			if(time() - $this->m_cacheSeconds < filemtime($filePath))
			{
				include_once $filePath;
			}
			else 
			{
				// delete the cached file
				FolderXP::Unlink($filePath);
				return false;
			}
			
			return true;
		}
		
		// the cached file has not been found
		return false;
	}
	
	/**
	 * This function cache content. 
	 * If the $content attribute has been set, then put the put its content to file, 
	 * in other case get content from buffer (by 'ob_get_contents' function) and put it.  
	 *
	 * @param string $fileName
	 * @param string $content
	 * @return bool
	 */
	public function CacheContent($fileName = null, $content = null)
	{
		// if $fileName in not set, then get it from $_GET
		if (! $fileName)
		{
			$fileName = $this->GetFileNameFromGet ();
		}
		// tring to save the content to cached file 
		try
		{
			// open the file or throw to excaption
			$fp = fopen ( $this->m_cacheRoot . $fileName, 'w' );
			if (! $fp)
				throw new Exception ( 'Error has been generated during opening the cached file!' );
			
			if ($content)
			{
				// write content to file
				fwrite ( $fp, $content );
			}
			else
			{
				// get content from buffer and write it to file
				fwrite ( $fp, ob_get_contents () );
			}
			
			// close the cached file
			fclose ( $fp );
		}
		catch ( Exception $e )
		{
			print $e->getMessage ();
			return false;
		}
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	protected function GetFileNameFromGet()
	{
		$fileName = '';
		
		// get file name
		foreach ( $_GET as $key => $value )
		{
			$value = trim ( $value );
			
			if($key == 'lang' || (substr($key, 0, 1) == 'p' && is_numeric(substr($key, 1))))
			{
				if (strlen ( $value ) > 0)
				{
					$fileName .= $value;
				}
			}
		}

		if(strlen ( $fileName ) < 1) $fileName = 'home';
		
		$fileName = md5($fileName) . '.html';
		return strtolower($fileName);
	}
	
	/**
	 * This function delete all deleted files
	 *
	 */
	function DeleteOldFiles($tiemControl = true)
	{
		// scan dir
		
		$files = scandir($this->m_cacheRoot);
		foreach ($files as $file)
		{
			$filePath = $this->m_cacheRoot . $file;
			$path_parts = pathinfo($filePath);
			 
			
			// if is file and it is old then delte it
			if($path_parts['extension'] == 'html' )
			{
				$defTime = time() - $this->m_cacheSeconds;
				if(($tiemControl &&  $defTime < filemtime($filePath)))
				{
					continue;
				}
				
				// remove the file
				FolderXP::Unlink($filePath); 
			}
		}
	} 
}
?>