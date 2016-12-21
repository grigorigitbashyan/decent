<?php
// includes
Site::IncludeFile ( 'classes/Box/Box.class.inc' );
/**
 * Enter description here...
 *
 * @package Box
 */
class BoxView extends View
{
	/**
	 * Enter description here...
	 *
	 * @var Box
	 */
	protected $m_box;
	
	/**
	 * Enter description here...
	 *
	 * @param int $ID
	 */
	function __construct($ID)
	{
		parent::__construct();
		$this->m_box = new Box ( $ID );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function Display()
	{
		// get related content ID
		$param = $this->m_box->GetParam ();
					
		switch ($this->m_box->GetType ())
		{
			case 1 : // content type

					// create content 
					$content = new Content ( $param );
					
					// get content article with replased URLs
					$article = $content->GetContent ( $this->m_displayLang );
					
					// display article
					print ( $article );
				break;
			case 2 : // file type
				
				if ( $param!==0 ) {
					
					$s2file = $this->m_site->GetStructureToFileRoot();
					$filePath = $_SERVER['DOCUMENT_ROOT'] . '/'. $s2file . $param;
					
					if(file_exists($filePath))
					{
						include "$filePath";
					}
					else 
					{
						print("The file $filePath does not exist!");
					}
				}
				break;
		}
	}
}
?>