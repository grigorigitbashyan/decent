<?php
/*
 * This class can be used to pares 'MyScript' for templastes
 */
class MyScript
{
	/*
	 * Default template
	 * 
	 * @protected string
	 */
	protected $m_rawTemplate;
	
	/*
	 * Final template
	 * 
	 * @protected string
	 */
	protected $m_template;
	
	/*
	 * This propetrtie contains current script which has been retrived in GetCurrentScript function. 
	 * 
	 * @protected string 
	 */
	protected $m_tempScript;
	
	/*
	 * 
	 * protected TemplateView
	 */
	protected $m_object;
	
	/*
	 * Construct
	 * 
	 * @param string $template
	 * @param TemplateView|TemplateNamedView
	 */
	function __construct($template, $object = null)
	{
		if ($object)
		{
			try
			{
				if ($object instanceof TemplateView || $object instanceof TemplateNamedView)
				{
					$this->m_object = $object;
					$this->SetTemplate($template);
				}
				else
				{
					throw new Exception ( 'The object must be instance of TemplateView or TemplateNamedView' );
				}
			}
			catch ( Exception $e )
			{
				echo $e->getMessage ();
			}
		}
	}
	
	/**
	 * 
	 * @param string $template
	 */
	function SetTemplate($template)
	{
		$this->m_rawTemplate = $template;
		$this->m_template = $template;
	}
	
	/*
	 * This function return current script values or null.
	 * 
	 * @return array two keys name and value.
	 */
	function GetCurrentScript()
	{
		// finde current script
		$data = array ();
		preg_match ( '/@{2}([^@]*)@{2}/', $this->m_template, $data );
		
		// if script exiest
		try
		{
			if (count ( $data ) > 0)
			{
				$res = array ();
				
				// put current script to temp script
				$this->m_tempScript = $data [0];
				
				// search perimetor
				$pos = strpos ( $data [1], ':' );
				
				// parse script
				if ($pos > 0)
				{
					$res ['name'] = substr ( $data [1], 0, $pos );
					$param = substr ( $data [1], $pos + 1 );
					
					$pers = null;
					if ($param)
					{
						$pers = array ();
						
						$paramsSplited = explode ( '::', $param );
						
						foreach ( $paramsSplited as $currParam )
						{
							$paresd = explode ( '=', $currParam );
							$pers ["{$paresd [0]}"] = (isset ( $paresd [1] )) ? $paresd [1] : true;
						}
					}
					
					$res ['value'] = $pers;
				}
				else
				{
					$res ['name'] = $data [1];
				}
				
				return $res;
			}
		}
		catch ( Exception $e )
		{
			print $e->getMessage ();
		}
		
		// set temp script to null and return null
		$this->m_tempScript = null;
		return null;
	}
	
	/*
	 * 
	 * @param string $putContent
	 */
	function PutToCurrentPlase($putContent = '', $rmvCommens = false)
	{
		// remove HTML comments if content is not null
		if ($rmvCommens && $putContent)
		{
			//remove html comments
			// get replacement possition
			$repPos = 0;
			
			do
			{
				// get position of replacement string
				$repPos = strpos ( $this->m_template, $this->m_tempScript, $repPos );
				
				// find befor and after starts of comment
				// find befor and after ends of comment
				$comStartBefor = strrpos ( substr ( $this->m_template, 0, $repPos ), '<!--' );
				$comEndAfter = strpos ( $this->m_template, '-->', $repPos );
				
				// thre comments exist
				if ($comStartBefor !== false && $comEndAfter !== false && $comStartBefor < $repPos && $repPos < $comEndAfter)
				{
					$comEndBefor = strrpos ( substr ( $this->m_template, 0, $repPos ), '-->' );
					$comStartAfter = strpos ( $this->m_template, '<!--', $repPos );
					
					// conditions for removing the comments
					if (! $comEndBefor || $comEndBefor < $comStartBefor)
					{
						$this->m_template = substr_replace ( $this->m_template, '', $comStartBefor, 4 );
						$comEndAfter -= 4;
					}
					
					$comStartAfter = strpos ( $this->m_template, '<!--', $repPos );
					if (! $comStartAfter || $comStartAfter > $comEndAfter)
					{
						$this->m_template = substr_replace ( $this->m_template, '', $comEndAfter, 3 );
					}
				}
			}while ( $repPos++ );
		}
		
		$this->m_template = str_replace ( $this->m_tempScript, $putContent, $this->m_template );
	}
	
	/*
	 * This function reset template for secondery use.
	 */
	function ResetTemplate()
	{
		$this->m_template = $this->m_rawTemplate;
	}
	
	function GetTemplate()
	{
		return $this->m_template;
	}
	
	/**
	 * this function returns paresed template on object.
	 * It can be colled if object has been set.
	 *
	 * @param bool $rmvCommens rempve HTML commens it tey has been set
	 * @return string
	 */
	function GetParsedTemplate($rmvCommens = false, $resp = false)
	{
		// check is object has been set
		if (! $this->m_object)
		{
			return null;
		}
		
		while ( ($res = $this->GetCurrentScript ()) )
		{
			$isName = (strcmp ( $res ['name'], 'name' ) == 0);
			// in clase when object is instance of TemplateNamedView then get name from object
			if ($isName && $this->m_object instanceof TemplateNamedView)
			{
				$keys = array_keys ( $res ['value'] );
				$responce = $this->m_object->GetNameString ( $keys [0] );
				$this->PutToCurrentPlase ( $responce, $rmvCommens && !$isName);
			}
			else
			{
				$responce = $this->m_object->GetTemplateValue ( $res, $resp );
				$this->PutToCurrentPlase ( $responce, $rmvCommens && !$isName );
			}
		}
		
		return $this->m_template;
	}
}
?>