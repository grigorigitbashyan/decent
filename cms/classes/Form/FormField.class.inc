<?php
/**
 * This class can by used to generate form field code.
 * Code parser use special script language.
 *
 */
class FormField
{
	/**
	 * Enter description here...
	 *
	 * @param string $inputName
	 * @param mixed $vlaue
	 * @param string $params myst be splitd :: (input, class, tagClass, tagName, splitter)
	 * @param array $listValues
	 * @return string
	 */
	static function GetFormFieldCode($inputName, $value = null, $params = null, $listValues = null)
	{
		// parse param to know input type and all perimeters (size, class, ...)
		$input = null;
		$class = null;
		$tagName = null;
		$tagClass = null;
		$splitter = null;
		$size = null;
		$cols = null;
		$rows = null;
		$multiple = null;
		$disabled = null;
		$readonly = null;
		
		if ($params)
		{
			$paramsSplited = explode ( '::', $params );
			
			foreach ( $paramsSplited as $currParam )
			{
				$paresdValue = explode ( '=', $currParam );
				
				switch ($paresdValue [0])
				{
					case 'input' :
					case 'tagName' :
					case 'splitter' :
						${$paresdValue [0]} = $paresdValue [1];
						break;
					case 'tagClass' :
						${$paresdValue [0]} = " class='{$paresdValue[1]}'";
						break;
					case 'size' :
					case 'cols' :
					case 'rows' :
					case 'class' :
					case 'multiple' :
					case 'disabled' :
					case 'readonly' :
						${$paresdValue [0]} = " {$paresdValue [0]}='{$paresdValue[1]}'";
						break;
				}
			}
		}
		
		// 
		$itemCount = ($listValues) ? count ( $listValues ) : 0;
		
		// create code
		$code = '';
		$index = 0;
		
		switch ($input)
		{
			case 'text' :
				$code = "<input type='text' ID='$inputName' name='$inputName' value='$value'{$class}{$size}{$disabled}{$readonly} />\n";
				break;
			case 'textarea' :
				$code = "<textarea ID='$inputName' name='$inputName'{$class}{$cols}{$rows}>$value</textarea>\n";
				break;
			case 'select' :
				if ($multiple)
				{
					$code = "<select ID='{$inputName}[]' name='{$inputName}[]'{$class}{$multiple}{$size} >\n";
				}
				else
				{
					$code = "<select ID='$inputName' name='$inputName'{$class}{$multiple}{$size} >\n";
				}
				
				if (! $multiple)
				{
					$code .= "<option value='0'>...</option>\n";
				}
				
				foreach ( $listValues as $key => $cValue )
				{
					
					if ($value == $key || (is_array ( $value ) && in_array ( $key, $value )))
					{
						$code .= "<option value='$key' selected='selected'>$cValue</option>\n";
					}
					else
					{
						$code .= "<option value='$key'>$cValue</option>\n";
					}
				}
				
				$code .= "</select>\n";
				break;
			case 'checkbox' :
				$inputName .= '[]';
				
				foreach ( $listValues as $key => $cValue )
				{
					// display tag
					if ($tagName)
					{
						$code .= "<{$tagName}{$tagClass}>\n";
					}
					
					// display value
					if ($value && ($value == $key || (is_array ( $value ) && in_array ( $key, $value ))))
					{
						$code .= "<input type='checkbox' ID='$inputName' name='$inputName'{$class} value='$key' checked='checked' />$cValue\n";
					}
					else
					{
						$code .= "<input type='checkbox' ID='$inputName' name='$inputName'{$class} value='$key' />$cValue\n";
					}
					
					// display tag end
					if ($tagName)
					{
						$code .= "</{$tagName}>\n";
					}
					
					// add spliter
					if ($splitter && $itemCount > ++ $index)
					{
						$code .= $splitter;
					}
				}
				break;
			case 'radio' :
				foreach ( $listValues as $key => $cValue )
				{
					// display tag
					if ($tagName)
					{
						$code .= "<{$tagName}{$tagClass}>\n";
					}
					
					// display value
					if ($value && ($value == $key || (is_array ( $value ) && in_array ( $key, $value ))))
					{
						$code .= "<input type='radio' ID='$inputName' name='$inputName'{$class} value='$key' checked='checked' />$cValue\n";
					}
					else
					{
						$code .= "<input type='radio' ID='$inputName' name='$inputName'{$class} value='$key' />$cValue\n";
					}
					
					// display tag end
					if ($tagName)
					{
						$code .= "</{$tagName}>\n";
					}
					
					// add spliter
					if ($splitter && $itemCount > ++ $index)
					{
						$code .= $splitter;
					}
				}
				break;
			default :
				if (is_array ( $value ))
				{
					// 
					$itemCount = count ( $value );
					
					foreach ( $value as $item )
					{
						// display tag
						if ($tagName)
						{
							$code .= "<{$tagName}{$tagClass}>\n";
						}
						
						$code .= $item;
						
						// display tag end
						if ($tagName)
						{
							$code .= "</{$tagName}>";
						}
						
						// add spliter
						if ($splitter && $itemCount > (++ $index))
						{
							$code .= $splitter;
						}
					}
				}
				else
				{
					// display tag
					if ($tagName)
					{
						$code .= "<{$tagName}{$tagClass}>\n";
					}
					
					$code .= $value;
					
					// display tag end
					if ($tagName)
					{
						$code .= "</{$tagName}>\n";
					}
				}
		}
		
		return $code;
	}
	
	/**
	 * Print Form validation javascript code
	 *
	 */
	static function GetFormValidJSCode($elements, $formName, $funcName)
	{
		
		// javascript validation
		$output = "<script type='text/javascript'>\n";
		$output .= "<!--\n";
		$output .= ("function {$funcName}()\n{\n\t");
		
		$res = "res = (true ";
		
		// steping over elements
		foreach ( $elements as $key => $value )
		{
			$jsCode = self::GetElValidJSCode ( $key, $value, $formName );
			
			// no dynamics
			if ($jsCode)
			{
				$keyS = (is_int(intval($key))) ? 'field'.$key : $key; 
				$output .= "$keyS = $jsCode\n\t";
				$res .= "&& $keyS ";
			}
		}
		
		$res .= ");";
		$output .= "$res\n\t";
		
		$output .= "SerErrorText(res);\n\t";
		$output .= "return res;\n";
		$output .= "}\n-->\n</script>\n";
		
		// return code
		return $output;
	}
	
	/**
	 * Return element validation JavaScript code
	 *
	 * @return string|null
	 */
	static protected function GetElValidJSCode($fieldName, $fieldType, $formName = '', $validType = 1)
	{
		//		// if validation is string, then return string (alterantive validation)
		//		if (is_string ( $this->m_validation ))
		//			return $this->m_validation;
		
		// if validation is number
		switch ($validType)
		{
			case 1 : // not empoty for text and text aray, selected for select , list and radiogroups
				switch ($fieldType)
				{
					case 'text' : // text
						return "TextFildValid('$fieldName', null, '{$fieldName}HTML');";
					case 'select' : // select
						return "SelectFildValid('$fieldName', '{$fieldName}HTML');";
					case 'radio' : // radio
					case 'checkbox' : // radio
						return "RadioFildValid('$fieldName', '{$formName}', '{$fieldName}HTML');";
				
				}
			default :
				return null;
		}
	}
	
	static function GetRequierdFields($pattern)
	{
		$regPatt = '/@{2}([^:.]*):{1}[^@]*input=([^:{2}.]*)[^@]*required[^@]*@{2}/';
		$results1 = array ();
		preg_match_all ( $regPatt, $pattern, $results1 );
		
		$regPatt = '/@{2}([^:.]*):{1}[^@]*required[^@]*input=([^@^:{2}.]*)[^@]*@{2}/';
		$results2 = array ();
		preg_match_all ( $regPatt, $pattern, $results2 );
		
		$res = array();
		$res['field'] = array_merge_recursive($results1[1], $results2[1]);
		$res['type'] = array_merge_recursive($results1[2], $results2[2]);

		return $res;
	}
}
?>