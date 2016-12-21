<?php
/*
 * This class can be used to perform operations with file. 
 */
class FolderXP
{
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	protected static $ftpConnectin = null;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected static $ftpRoot = null;
	
	static function SetFtpConnection($ftpConn)
	{
		FolderXP::$ftpConnectin = $ftpConn;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ftpRoot
	 */
	static function SetFtpRoot($ftpRoot)
	{
		FolderXP::$ftpRoot = $ftpRoot;
	}
	
	/**
	 * Delete file
	 *
	 * @param string $fileName
	 * @return bool
	 */
	static function Unlink($fileName)
	{
		$res = true;
		
		if (file_exists ( $fileName ))
		{
			// ftp
			if (FolderXP::$ftpConnectin)
			{
				$res = ftp_delete ( FolderXP::$ftpConnectin, $fileName );
			}
			// local
			else
			{
				$res = unlink ( $fileName );
			}
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $sourseFile
	 * @param string $destFile
	 * @param bool $owerite
	 * @return bool
	 */
	static function Rename($sourseFile, $destFile, $owerite = true)
	{
		$res = true;
		
		if ($owerite && file_exists ( $destFile ))
		{
			$res = FolderXP::Unlink ( $destFile );
		}
		
		// FTP
		try
		{
			if (FolderXP::$ftpConnectin)
			{
				$res = ftp_rename ( FolderXP::$ftpConnectin, $sourseFile, $destFile );
				if (!$res)
				{
					throw new Exception ( 'File cannot be renamed!' );
				}
			}
			// local
			else
			{
				$res = rename ( $sourseFile, $destFile );
			}
		}
		catch ( Exception $e )
		{
			echo $e->getMessage ();
		}
		
		return $res;
	}
	
	static function Upload($sourceFile, $destFile, $ftpUpType = FTP_BINARY)
	{
		$res = true;
		// uploading
		try
		{
			// ftp
			if (FolderXP::$ftpConnectin)
			{
				$res = ftp_put ( self::$ftpConnectin, $destFile, $sourceFile, $ftpUpType );
			}
			// local
			else
			{
				$res = copy ( $sourceFile, $destFile );
			}
			
			if (! $res)
			{
				throw new Exception ( 'Cann not be uploaded!' );
			}
		}
		catch ( Exception $e )
		{
			$msg = $e->getMessage ();
			$msg = "Caught exception: $msg";
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $fullName
	 * @param string $access
	 * @return bool
	 */
	static function Mkdir($fullName, $access = 0753)
	{
		$res = null;
		
		if (FolderXP::$ftpConnectin)
		{
			// change to htp path
			$res = ftp_mkdir ( FolderXP::$ftpConnectin, $fullName );
			
			if (! $res)
			{
				echo "Current dir: " . ftp_pwd(self::$ftpConnectin);
				echo "<br />The directoy <b>$fullName</b> cannot be created!";
			}
		}
		else
		{
			$res = mkdir ( $fullName, $access );
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $fullName
	 * @return bool
	 */
	static public function Rmdir($fullName)
	{
		$res = null;
		
		if (FolderXP::$ftpConnectin)
		{
			// change to htp path
			$res = ftp_rmdir( FolderXP::$ftpConnectin, $fullName );
			
			if (! $res)
			{
				echo "Current dir: " . ftp_pwd(self::$ftpConnectin);
				echo "<br />The directoy <b>$fullName</b> cannot be created!";
			}
		}
		else
		{
			$res = rmdir( $fullName );
		}
		
		return $res;
	}
	/**
	 * Enter description here...
	 *
	 * @param string $dir
	 * @param bool $addRoot
	 */
	static public function Chdir($dir, $addRoot = true)
	{
		if (self::$ftpConnectin)
		{
			$dirrectory = ($addRoot) ? self::$ftpRoot . $dir : $dir;
			ftp_chdir ( self::$ftpConnectin, $dirrectory );
		}
	}
}
?>