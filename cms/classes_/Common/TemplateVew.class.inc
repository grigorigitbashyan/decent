<?php
/**
 * Enter description here...
 *
 */
class TemplateVew extends View 
{
	/**
	 * Enter description here...
	 *
	 */
	function __construct()
	{
		parent::__construct();
	}
	
	/**
	 * Put template on record
	 *
	 * @param RecordFlexible $record
	 * @param string $template
	 * 
	 * @return string
	 */
	function PutTemplate($record, $template)
	{
		$pattern = $template;
		
		// search and replase code
		while(false)
		{
			// looging for field
			$fieldFound = '';
			
			// getting field value
			$fieldValue = null;
			
			if($record->IsFieldMultiple($fieldFound))
			{
				$fieldValue = $record->GetFieldValue($fieldFound, $this->m_displayLang);
			}
			else 
			{
				$fieldValue = $record->GetFieldValue($fieldFound);
			}
			
			// replase field value
			
		}
		
		while ( ($start = strpos ( $pattern, '@@' )) !== false )
		{
			$end = strpos ( $pattern, '@@', $start + 1 );
			
			if (! $end)
			{
				break;
			}
			
			// get string between @@ and @@
			$key = substr ( $pattern, $start, $end - $start + 2 );
			$key = substr ( $key, 2, strlen ( $key ) - 4 );
			
			// get filed name and params
			$fieldFound = $key; 
			$pos = strpos($key, ':');
			// params can be used for addition information
			$param = null;
			
			if($pos > 0)
			{
				$fieldFound = substr($key, 0, $pos);
				$param = substr($key, $pos + 1);
			}
			
			// getting field value
			$fieldValue = null;
			
			if($record->IsFieldMultiple($fieldFound))
			{
				$fieldValue = $record->GetFieldValue($fieldFound, $this->m_displayLang);
			}
			else 
			{
				$fieldValue = $record->GetFieldValue($fieldFound);
			}
			
			// repalse patern value and destroy comments
			if($fieldValue)
			{
				$strPos = strpos($pattern, "@@$key@@");
				// start and end positions of comments
				$startPos = strrpos(substr($pattern, 0, $strPos), '<!--');
				$endPos = strpos( $pattern, '-->', $strPos);

				if($startPos !== false && $endPos !== false && $startPos < $strPos && $strPos < $endPos)
				{
					$beforeEndPos = strrpos( substr($pattern, 0, $strPos), '-->');
					
					// conditions for removing the comments
					if(!$beforeEndPos || $beforeEndPos < $startPos)
					{
						$pattern = substr_replace($pattern, '', $startPos, 4);
						$endPos -= 4;						
					}
					
					$afterStartPos = strpos( $pattern, '<!--', $strPos);
					if(!$afterStartPos || $afterStartPos > $endPos)
					{
						$pattern = substr_replace($pattern, '', $endPos, 3);
					}
				}
			}
			else
			{
				$fieldValue = '';
			}			
			
			// replase onlu current $key value ($key values can be more then one)
			$patPos = strpos($pattern, "@@$key@@");
			$pattern = substr_replace ( $pattern, $fieldValue, $patPos, strlen("@@$key@@"));
		}
		
		return $pattern;
	}
}
?>