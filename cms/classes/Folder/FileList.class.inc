<?php
// includes
Site::IncludeFile('classes/Common/TreeView.class.inc');

/**
 * Enter description here...
 *
 * @package Folder
 */
class FileList extends TreeView
{
	/**
	 * Enter description here...
	 *
	 * @param unknown_type $folderID
	 */
	function __construct($folderID)
	{
		$sql = "SELECT * FROM `folder` WHERE `parentID` = $folderID AND `fileOrFolder` = 1 ORDER BY `ID` DESC ";
		parent::__construct($sql, 'fileID');
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function AddIcons()
	{
		if($this->m_struct)
		{
			foreach ($this->m_struct as $key => $value)
			{
				// get name
				$name = $value['name'];
				$pos = strrpos($name, '.');
				
				$ext = strtolower(substr($name, $pos + 1));
				// calc extantion
				$icon = null;
				switch ($ext)
				{
					case 'txt':
						$icon = 'txt96.gif';
						break;
					case 'pdf':
						$icon = 'pdf96.gif';
						break;
					case 'doc':
					case 'docx':
						$icon = 'word96.gif';
						break;
					case 'xls':
					case 'xlsx':
						$icon = 'xls96.gif';
						break;
					case 'exe':
						$icon = 'exe96.gif';
						break;
					case 'mp3':
						$icon = 'mp396.gif';
						break;
					case 'ppt':
						$icon = 'ppt96.gif';
						break;
					case 'swf':
						$icon = 'swf96.gif';
						break;
					case 'zip':
					case 'rar':
						$icon = 'zip96.gif';
						break;
					default:
						$icon = 'unknown96.gif';
						break;
						
				}

				$this->m_struct[$key]['icon'] = $icon;
			}
		}
	}
}
?>