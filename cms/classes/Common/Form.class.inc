<?php

/**
 * Enter description here...
 *
 * @author Vazgen
 * @package defaultPackage
 */
/*
 * There are the 3 classes for working with forms
 * 1) FormElement
 * 2) Form
 * 3) FormKit
 */
///////////////////////////////
/**
 * This class provide working with form elements witch is text, radio, checkbox, select, textarea , button , list and hidden
 * It has features for auto saving, loading element value and also clearing.
 * It has features for Drawing element with styles.
 */
class FormElement extends ComElement
{
	/**
	 * This property is for element type, it can be one of  following values 
	 * text (1), radio(2), checkbox(3), select(4), textarea(5) , button(6) , list(7) or hidden(8)
	 *
	 * @var int
	 */
	var $m_type;
	
	/**
	 * This is the value(s) of element 
	 * @var string
	 */
	var $m_value;
	
	/**
	 * If it is array, then it most content 'key' and 'value'
	 * $array['key1'] = "value1";
	 * $array['key2'] = "value2";
	 * ....
	 * $array['keyN'] = "valueN";
	 * 
	 * @var array
	 */
	var $m_valueList;
	
	/**
	 * Style for element, for example element length or colors
	 *
	 * @var string
	 */
	var $m_style;
	///////////////////////////////////////////////////////////////////////////////////
	
	function FormElement($form = null)
	{
		$this->m_type   = null;
		$this->m_title  = null;
		$this->m_ID	    = null;
		$this->m_value  = null;
		$this->m_status = null;
		$this->m_form   = &$form;
		$this->m_dynamic = false;
	}

	/**
	 * Enter description here...
	 *
	 * @param int $type
	 * @param string $ID
	 * @param string $title
	 * @param string|array $value
	 * @param bool $status
	 * @param string $style
	 */
	function Create($type, $ID, $title, $value=null, $validation=0, $name=null, $status = true, $style = null)
	{
		$this->m_type   = $type;
		$this->m_title  = $title;
		$this->m_ID	    = $ID;
		$this->m_value  = $value;
		$this->m_status = $status;
		$this->m_style  =  $style;
		$this->m_validation = $validation;
		
		$this->m_name = ($name) ? $name : $this->m_ID;
		
		// update value
		$this->Update();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 */
	function CreateText($ID, $title, $value=null, $validation=null,  $status = true, $style = null)
	{
		// 1
		$this->Create(1, $ID, $title, $value, $validation, $ID,  $status, $style);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 */
	function CreateTextarea($ID, $title, $value=null,$validation=null, $status = true, $style = null)
	{
		// 5
		$this->Create(5, $ID, $title, $value, $validation, $ID, $status, $style);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 */
	function CreateButton($ID, $title, $value=null, $status = true, $style = null)
	{
		// 6
		$this->Create(6, $ID, $title, $value,  null, $ID, $status, $style);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $value
	 */
	function CreateHidden($ID, $value=null)
	{
		// 8
		$this->Create(8, $ID, null, $value);
	}
	
	function CreateRadio($ID, $name, $title, $valueList, $value=null, $validation=null, $status = true, $style = null)
	{
		// 2
		$this->Create(2, $ID, $title, $value, $validation, $name, $status, $style);
		$this->m_valueList = $valueList;
	}
	function CreateCheckBox($ID,  $title, $valueList, $value=null,$validation=null, $status = true, $style = null)
	{
		// 3
		$this->Create(3, $ID, $title, $value, $validation, $ID, $status, $style);
		$this->m_valueList = $valueList;
	}
	function CreateSelect($ID, $title, $valueList, $value=null, $validation=null, $status = true, $style = null)
	{
		// 4
		$this->Create(4, $ID, $title, $value, $validation, $ID, $status, $style);
		$this->m_valueList = $valueList;
	}
	
	function CreateList($ID, $title, $valueList, $value=null,$validation=null, $status = true, $style = null)
	{
		// 7
		$this->Create(7, $ID, $title, $value, $validation, $ID, $status, $style);
		$this->m_valueList = $valueList;
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DrawTitle()
	{
		$class = null;
		
		$firstTime = $this->m_form->m_firstTime;
		// dynamic
		if($this->m_dynamic)
		{
			foreach ($this->m_form->m_dynamics as $dynamic)
			{
				list($contEl, $contElVal, $childEl, $childElStatus) = $dynamic; 
				
				if($childEl == $this->m_name)
				{
					if($this->m_form->m_elements[$contEl]->m_value == $contElVal)
						$class = "fBold";
					else
						$class = "fGrey";
					break;
				}
			}
		}
		else 
		{
			if($this->m_status)
			{
				if($this->m_validation)
				{
					if ($this->IsValid() || $firstTime)
						$class = "fBold";
					else
						$class = "fRed";
				}
			}
			else 
				$class = "fGrey";
		}
		
		print("<div id='{$this->m_ID}HTML' nmae='{$this->m_ID}HTML' class='$class'>{$this->m_title}</div>");
	}
	
	
	/**
	 * $rID is second ID for radio button and emp value control for Select or List
	 * $size is size control for all elements, for text array it most be array 1:2 and for other case int
	 * $selEmVal is text for select emp value
	 * 
	 * @param int $rID
	 * @param int|array $size
	 * @param string $selEmVal
	 */
	function DrawElement($rID=null, $size=null, $selEmVal = null)
	{
		// status
		$status = "";
		if($this->m_status == false)
			$status = " disabled";
		
		// value
		$value = "";
		if($this->m_value)
			$value = $this->m_value;
			
		//size
		$sizeT = "";
		if($size)
		{
			if(is_int($size))
				$sizeT = " size='$size'";
			elseif (is_array($size))
				$sizeT = " cols='{$size[0]}' rows='{$size[1]}'";
		}
		/////////////////////////////////////////
		switch ($this->m_type)
		{
			case 1: // text
				print("<input type='text' name='$this->m_name' id='$this->m_ID' value='$value' $status $sizeT>\n");
				break;
			case 5: // textaray
				print("<textarea name='$this->m_name' id='$this->m_ID' $status  $sizeT>$value</textarea>\n");
				break;
			case 6: // button
				print("<input type='button' name='$this->m_name' id='$this->m_ID' value='$value' $status $sizeT>\n");
				break;
			case 8: // hidden
				print("<input type='hidden' name='$this->m_name' id='$this->m_ID' value='$value' $sizeT>\n");
				break;
			case 4: // select
			case 7: // list
				// search is selec is dynamic
				$dyScript = "";
				
				foreach ($this->m_form->m_dynamics as $dynamic)
				{
					$count = count($dynamic);
					if($count == 4)
					{
						list($contEl, $contElVal, $childEl, $childElStatus) = $dynamic; 
						
						$childElStatus = ($childElStatus) ? 1:0;
						if($contEl == $this->m_name)
						{			
							$dyScript = "onChange=\"DyEnSelect('$childEl', $childElStatus, '$contEl', '$contElVal');\"";
							break;
						}
					}
					
					// if it is string , then writhe this string as script
					if($count == 2)
					{
						list($contEl, $script) = $dynamic; 
						
						if($contEl == $this->m_name)
						{
							$dyScript = $script;
							break;
						}
					}
					
					
				}
				
				/////////////////////////////////////////////////////
				if($this->m_type == 7)
					print("<select name='$this->m_name' id='$this->m_ID' $dyScript $status $sizeT multiple>\n");
				else 
					print("<select name='$this->m_name' id='$this->m_ID' $dyScript $status $sizeT>\n");
					
				if($rID == null)
				{
					if($selEmVal)
					{
						print("<option value=''>$selEmVal</option>\n");
					}
					else 
					{
						print("<option value=''>--Select--</option>\n");
					}
				}
					
				foreach ($this->m_valueList as $key => $svalue)
				{
					if($value == $key)
						print("<option value='$key' selected>$svalue</option>\n");
					else
						print("<option value='$key'>$svalue</option>\n");
				}
				print("</select>\n");
				break;
			case 2: //radio
			
				// search is selec is dynamic

				$dyScriptY = "";
				$dyScriptN = "";
				$pValue = false;
				
				foreach ($this->m_form->m_dynamics as $dynamic)
				{
					list($contEl, $contElVal, $childEl, $childElStatus) = $dynamic; 
					
					if($contEl == $this->m_name)
					{
						$y = ($childElStatus) ? 1 : 0;
						$n = ($childElStatus) ? 0 : 1;
						
						$pValue = $contElVal;
						//if elem is array
						// if child element is FormElement
						$contElC=null;
						
						if(is_a($this->m_form->m_elements[$childEl] , "FormElement"))
						{
							$contElC="'$childEl'";
						}
						else 
						{
							$contElC = $this->m_form->m_elements[$childEl]->GetAllElNames();
							$contElC = join("', '", $contElC);
							$contElC = "['$contElC']";
						}
						
						$dyScriptY = "onclick=\"ChangeElStatus($contElC, $y);\"";
						$dyScriptN = "onclick=\"ChangeElStatus($contElC, $n);\"";
						
						break;
					}
				}

				if(isset($this->m_valueList[$rID]))
				{
					$v = $this->m_valueList[$rID];
					
					if($rID == $this->m_value)
					{
						
						if($pValue)
						{
							if($rID == $pValue)
								print("<input type='radio'  name='$this->m_name' id='$rID' value='$rID' $status $dyScriptY checked>$v\n");
							else
								print("<input type='radio'  name='$this->m_name' id='$rID' value='$rID' $status $dyScriptN checked>$v\n");
						}
						else
							print("<input type='radio'  name='$this->m_name' id='$rID' value='$rID' $status checked>$v\n");
					}
					else
					{ 
						if($pValue)
						{
							if($rID == $pValue)
								print("<input type='radio'  name='$this->m_name' id='$rID' value='$rID' $status $dyScriptY>$v\n");
							else
								print("<input type='radio'  name='$this->m_name' id='$rID' value='$rID' $status $dyScriptN>$v\n");
						}
						else
							print("<input type='radio'  name='$this->m_name' id='$rID' value='$rID' $status>$v\n");
					}
				}

				break;	
			case 3: //chekbox
				list($k, $v) = each($this->m_valueList);
				
				if($k == $this->m_value)
					print("<input type='checkbox' name='$this->m_name' id='$this->m_ID' value='$k' checked>$v\n");
				else
					print("<input type='checkbox' name='$this->m_name' id='$this->m_ID' value='$k'>$v\n");
				break;	
		}
	}

	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function Save()
	{
		if($this->m_type == 6)
			return true;
		
		// get value
		$value = null;
		if($this->m_form->m_method) // for post
		{
			if(DB::POST($this->m_name))
				$value = DB::POST($this->m_name);
		}
		else 						// for get
		{
			if(DB::GET($this->m_name))
				$value = DB::GET($this->m_name);
		}

		$value = HTML::RemoveXSS($value);
		
		// save to session
		$_SESSION[$this->m_form->m_ID][$this->m_name] = $value;
		// save to value
		$this->m_value = $value;

		return true;
	}
	
	function SetStatus($status)
	{
		$this->m_status = $status;
	}
	
	/**
	 * Clear value from session and set value property null
	 *
	 */
	function Clear()
	{
		if(isset($_SESSION[$this->m_form->m_ID][$this->m_ID]))
			unset($_SESSION[$this->m_form->m_ID][$this->m_ID]);
			
		$this->m_value = null;
	}
		
	/**
	 * Update value from session
	 *
	 */
	function Update()
	{
		if(isset($_SESSION[$this->m_form->m_ID][$this->m_ID]))
			$this->m_value = $_SESSION[$this->m_form->m_ID][$this->m_ID];
	} 
	
	/**
	 * Return element validation JavaScript code
	 *
	 * @return string|null
	 */
	function GetElValidJSCode()
	{
		// if validation is string, then return string (alterantive validation)
		if(is_string($this->m_validation))
			return $this->m_validation;
			
		// if validation is number
		switch($this->m_validation)
		{
			case 1:	// not empoty for text and text aray, selected for select , list and radiogroups
				switch ($this->m_type)
				{
					case 1:	// text
					case 5: // textarray
						return "TextFildValid('{$this->m_name}', null, '{$this->m_name}HTML');";
					case 4: // select
					case 7: // list
						return "SelectFildValid('{$this->m_name}', '{$this->m_name}HTML');";
					case 2: // radio
						return "RadioFildValid('{$this->m_name}', '{$this->m_form->m_ID}', '{$this->m_name}HTML');";
						
				}
			case 2:	// zip code validation for
				switch ($this->m_type)
				{
					case 1:	// text
					case 5: // textarray
						return "TextFildValid('{$this->m_name}', 'zip', '{$this->m_name}HTML');";
				}
			case 3:	// email number validation for
				switch ($this->m_type)
				{
					case 1:	// text
					case 5: // textarray
						return "TextFildValid('{$this->m_name}', 'email', '{$this->m_name}HTML');";
				}
			case 4:	// phone number validation for
				switch ($this->m_type)
				{
					case 1:	// text
					case 5: // textarray
						return "TextFildValid('{$this->m_name}', 'phone', '{$this->m_name}HTML');";
				}
			default:
				return null;
		}
	}
	
	/**
	 * Return elementt validation
	 *
	 * @return bool
	 */
	function IsValid()
	{
		if(!$this->m_validation)
			return true;

		$vv = new ValueValidation();
		
		$result = false;
		// if validation is number
		switch($this->m_validation)
		{
			case 1:	// not empoty for text and text aray, selected for select , list and radiogroups
					$result = !($vv->isEmpty($this->m_value));
					break;
			case 2:	// zip code validation for
					$result = $vv->isZip($this->m_value);
					break;
			case 3:	// email number validation for
					$result = $vv->isEmail($this->m_value);
					break;
			case 4:	// phone number validation for
					$result = $vv->isPhone($this->m_value);
					break;
			default:
				return true;
		}
		return $result;
	}
	/**
	 * Return element value text
	 * if element is select, list, radio ro chackbox, return text value
	 *
	 * @return string
	 */
	function GetValueText()
	{
		switch ($this->m_type)
		{
			case 1: // text	
			case 5:	// textarray
			case 2: // radeo
			case 3: // chekbox
				return $this->m_value;
				break;
			case 4: // select
			case 7: // list
				return $this->m_valueList[$this->m_value];
				break;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
class ComElement
{
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	var $m_name;
	/**
	 * Enter description here...
	 *
	 * @var Form
	 */
	var $m_form;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	var $m_ID;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	var $m_title;
	/**
	 * Enter description here...
	 *
	 * @var int
	 */
	var $m_validation;
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	var $m_status;
	/**
	 * Enter description here...
	 *
	 * @var bool
	 */
	var $m_dynamic;

	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	function GetTitle()
	{
		return $this->m_title;
	}
//	function Save(){}
//	function Clear(){}
//	function DrawTitle(){}
//	function DrawElement(){} 
//	function IsValid(){}
//	function Update(){}
}
class DateSelects extends ComElement 
{
	/**
	 * Enter description here...
	 *
	 * @var FormElement
	 */
	var $m_year;
	/**
	 * Enter description here...
	 *
	 * @var FormElement
	 */
	var $m_month;
	/**
	 * Enter description here...
	 *
	 * @var FormElement
	 */
	var $m_day;

	/**
	 * Enter description here...
	 *
	 * @param Form $form
	 * @return DateSelects
	 */
	function DateSelects($form = null)
	{
		$this->m_form   = &$form;
		$this->m_dynamic = false;	
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param int $yearStart
	 * @param int $yearEnd
	 * @param int $validation
	 * @param bool $status
	 * @param string $style
	 * @return bool
	 */
	function Create($ID, $title, $yearStart, $yearEnd, $validation=null, $status = true, $style = null)
	{
		///////////////////
		$this->m_title = $title;
		$this->m_ID = $ID;
		$this->m_name = $ID;
		$this->m_validation = $validation;
		$this->m_status = $status;
		///////////////////
		
		// month
		if(isset($this->m_form->m_elements[$ID."m"]) || isset($this->m_form->m_elements[$ID."d"]) || isset($this->m_form->m_elements[$ID."y"]))
			return false;
			
		// create text element
		// month
//		$months["month"] = "Month";
		$months["1"] = "Jan";
		$months["2"] = "Feb";
		$months["3"] = "Mar";
		$months["4"] = "Apr";
		$months["5"] = "May";
		$months["6"] = "Jun";
		$months["7"] = "Jul";
		$months["8"] = "Aug";
		$months["9"] = "Sep";
		$months["10"] = "Oct";
		$months["11"] = "Nov";
		$months["12"] = "Dec";
		
		$this->m_month = new FormElement($this->m_form);
		$this->m_month->CreateSelect($ID."m", null, $months, null, null, $status, $style);
		// day
		
//		$days["day"] = "Day";
		for($index = 1; $index < 32; $index++)
			$days[$index] = $index;
			
		$this->m_day = new FormElement($this->m_form);
		$this->m_day->CreateSelect($ID."d", null, $days, null, null, $status, $style);
		// year
		
//		$years["year"] = "Year";
		for($index = $yearEnd; $index >= $yearStart; $index--)
			$years[$index] = $index;
		
		$this->m_year = new FormElement($this->m_form);
		$this->m_year->CreateSelect($ID."y", null, $years, null, null, $status, $style);
		
		return true;
	} 

	/**
	 * Enter description here...
	 *
	 */
	function DrawTitle()
	{
		$class = null;
		if($this->m_status)
		{
			if($this->m_validation)
			{
				if ($this->IsValid())
					$class = "fBold";
				else
					$class = "fRed";
			}
		}
		else 
			$class = "fGrey";
		
		
		print("<div id='{$this->m_ID}HTML' nmae='{$this->m_ID}HTML' class='$class'>{$this->m_title}</div>");
	}
	
	function DrawElement()
	{
		global $lang;
		
		$this->m_day->DrawElement(null, null, "Day");
		$this->m_month->DrawElement(null, null, "Month");
		$this->m_year->DrawElement(null, null, "Year");
	}
	
	function Save()
	{
		$this->m_month->Save();
		$this->m_day->Save();
		$this->m_year->Save();
	}
	
	function Update()
	{
		$this->m_month->Update();
		$this->m_day->Update();
		$this->m_year->Update();
	}
	
	function Clear()
	{
		$this->m_month->Clear();
		$this->m_day->Clear();
		$this->m_year->Clear();
	}
	/**
	 * Enter description here...
	 *
	 */
	function GetElValidJSCode()
	{
		if($this->m_validation)
			return "isDateSelects('{$this->m_name}y', '{$this->m_name}m','{$this->m_name}d', '{$this->m_name}HTML');";
			
		return null;
	}
	
	function GetValueText()
	{
		$year = $this->m_year->GetValueText();
		$month = $this->m_month->GetValueText();
		$day = $this->m_day->GetValueText();
		
		return "$day:$month:$year";
	}
	/**
	 * Enter description here...
	 *
	 * @return true
	 */
	function IsValid()
	{
		return true;
	}
	
	
	function SetStatus($status)
	{
		$this->m_status = $status;
		
		$this->m_month->SetStatus($status);
		$this->m_day->SetStatus($status);
		$this->m_year->SetStatus($status);
	}
	
    /**
     * Enter description here...
     *
     * @return array
     */
    function GetAllElNames()
    {
    	return array($this->m_name, $this->m_year->m_name, $this->m_day->m_name, $this->m_month->m_name);
    }
}
/**
 * Enter description here...
 *
 */
class Form
{
	/**
	 * This is the ID of form
	 *
	 * @var string
	 */
	var $m_ID;
	
	/**
	 * Form method, it can be post or get
	 *
	 * @var bool
	 */
	var $m_method;
	
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	var $m_action;
	
	/**
	 * Form elements
	 *
	 * @var array
	 */
	var $m_elements;
	
	/**
	 * Dynamic efects array
	 *
	 * @var array
	 */
	var $m_dynamics;
	
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	var $m_groupValidation;
	
	/**
	 * If for uset fiirst time
	 *
	 * @var bool
	 */
	var $m_firstTime;
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $action
	 * @param bool $method
	 * @return Form
	 */
	function Form($ID, $action=null, $method = true)
	{
		$this->m_ID = $ID;
		$this->m_firstTime = true;
		
		if($action)
		{
			$this->m_action = $action;
		}
		else 
		{
			$this->m_action = "?formAction=save";
		}
		$this->m_method = $method;
		
		$this->m_elements = array();
		$this->m_dynamics = array();
		$this->m_groupValidation = array();
	}
	
	/**
	 * Enter description here...
	 * @param bool $validation
	 */
	function DrawBegin($validation = true, $errorDiv = true)
	{
		$this->RunDyEnableds();
		
		if($validation)
			$this->SetFrmValidJSCode();
		/////////////////////////////
		if($errorDiv)
			print('<div id="errorDiv" class="fRed">&nbsp;</div>');
		
		if($this->m_method)
			print("<form id='$this->m_ID' action='$this->m_action' method='post' onsubmit=\"return Val_{$this->m_ID}();\">\n");
		else 
			print("<form id='$this->m_ID' action='$this->m_action' method='get' onsubmit=\"return Val_{$this->m_ID}();\">\n");
		
		// print save hiddent element
		print("<input type='hidden' id='save{$this->m_ID}' name='save{$this->m_ID}' value='save'>\n");
	}
	
	/**
	 * Enter description here...
	 *
	 */
	function DrawEnd()
	{
		print("</form>\n");
	}
	/**
	 * Draw submit button
	 *
	 */
	function DrawSubmit($title = "Submit")
	{
		print("<input type='submit' value='$title'>\n");
	}
	/**
	 * Draw Reset button
	 *
	 */
	function DrawReset($title = "Reset")
	{
		print("<input type='reset' value='$title'>\n");
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 * @return bool
	 */
	function AddText($ID, $title, $value=null,$validation=null, $status = true, $style = null)
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		// create text element
		$element = new FormElement($this);
		$element->CreateText($ID, $title, $value, $validation, $status, $style);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 * @return bool
	 */
	function AddTextarea($ID, $title, $value=null, $validation=null,$status = true, $style = null)
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		// create text element
		$element = new FormElement($this);
		$element->CreateTextarea($ID, $title, $value, $validation, $status, $style);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 * @return bool
	 */
	function AddButton($ID, $title, $value=null, $validation=null, $status = true, $style = null)
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		// create text element
		$element = new FormElement($this);
		$element->CreateButton($ID, $title, $value, $validation, $status, $style);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $value
	 * @return bool
	 */
	function AddHidden($ID, $value)
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		// create text element
		$element = new FormElement($this);
		$element->CreateHidden($ID, $value);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 * @return bool
	 */
	function AddSelect($ID, $title, $valueList, $value=null, $validation=null, $status = true, $style = null)
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		// create text element
		$element = new FormElement($this);
		$element->CreateSelect($ID, $title, $valueList, $value, $validation, $status, $style);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Draw 3 selects for date` month, day and year
	 *
	 * @param string $ID
	 * @param string $title
	 * @param int $validation
	 * @return bool
	 */
	function AddSelectDate($ID, $title, $validation, $yearStart, $yearEnd)  
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		// if $yearEnd doesnt set then set corrent year
		if(!$yearEnd)
		{
			$yearEnd = date('Y');
		}
		// create text element
		$element = new DateSelects($this);
		$element->Create($ID, $title, $yearStart, $yearEnd, $validation);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 * @return bool
	 */
	function AddList($ID, $title, $valueList, $value=null, $validation=null, $status = true, $style = null)
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		// create text element
		$element = new FormElement($this);
		$element->CreateList($ID, $title, $valueList, $value,$validation, $status, $style);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 * @return bool
	 */
	function AddRadioGroup($ID, $name, $title, $valueList=null, $value=null, $validation=null, $status = true, $style = null)
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		if(!$valueList)
		{
			$valueList["yes"] = "Yes";
			$valueList["no"] = "No";
		}
		// create text element
		$element = new FormElement($this);
		$element->CreateRadio($ID, $name, $title, $valueList, $value, $validation, $status, $style);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $title
	 * @param string $value
	 * @param bool $status
	 * @param string $style
	 * @return bool
	 */
	function AddCheckBox($ID, $title, $valueList, $value=null, $status = true, $style = null)
	{
		if(isset($this->m_elements[$ID]))
			return false;
			
		// create text element
		$element = new FormElement($this);
		$element->CreateCheckBox($ID, $title, $valueList, $value, $status, $style);
		
		// add text element to elements array
		$this->m_elements[$ID] = $element;
		
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function Save()
	{
		if(DB::POST("save{$this->m_ID}") && DB::POST("save{$this->m_ID}") == "save")
		{
			$keys = array_keys($this->m_elements);
			foreach ($keys as $key)
			{
				if(isset($this->m_elements[$key]))
					$this->m_elements[$key]->Save();
			}
			
			$this->RunDyEnableds();
			$this->m_firstTime = false;
			return true;
		}
		
		return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $el
	 * @return bool
	 */
	function DrawElTitle($el)
	{
		if(isset($this->m_elements[$el]))
		{
			$this->m_elements[$el]->DrawTitle();
			return true;
		}
		
		return false;
	}
	
	/**
	 * $el is element ID
	 * $rID is second ID for radio button and emp value control for Select or List
	 * $size is size control for all elements, for text array it most be array 1:2 and for other case int
	 * $selEmVal is text for select emp value
	 * 
	 * @param $el string
	 * @param int $rID
	 * @param int|array $size
	 * @param string $selEmVal
	 */
	function DrawEl($el, $rID=null, $size=null, $selEmVal = null)
	{
		if(isset($this->m_elements[$el]))
		{
			$this->m_elements[$el]->DrawElement($rID, $size, $selEmVal);
			return true;
		}
		
		return false;
	}
	
	/**
	 * Print Form validation javascript code
	 *
	 */
	function SetFrmValidJSCode()
	{
		// javascript validation
		print("<script type='text/javascript'>\n");
		print("<!--\n");
		print("function Val_{$this->m_ID}()\n{\n\t");

		$res = "res = (true ";
		
		// get all elemets
		$keys = array_keys($this->m_elements);
		
		// steping over elements
		foreach ($keys as $key)
		{
			$jsCode = $this->m_elements[$key]->GetElValidJSCode();
			
			// no dynamics
			if(!($this->m_elements[$key]->m_dynamic))
			{
				if($jsCode)
				{
					print("$key = $jsCode\n\t");
					$res .= "&& $key "; 
				}
			}
			else  // dynamics
			{
				// for parent
				foreach ($this->m_dynamics as $dinamic)
				{
					list($contEl, $contElVal, $childEl, $childElStatus) = $dinamic;
					
					if($childEl == $key)
					{
						print("// dynamic $childEl\n\t");
						// paret //most
						if(is_a($this->m_elements[$contEl], "FormElement"))
							switch ($this->m_elements[$contEl]->m_type)
							{
								case 4: // select
								case 7: // list
									print("$contEl = SelectFildValid('$contEl');\n\t");
									break;
								case 2: // radio
									print("$contEl = RadioFildValid('$contEl', '{$this->m_ID}');\n\t");
									break;
							}
		
						// child
						print("$childEl = true;\n\t");
						
						if($childElStatus)
							print("if($contEl == '$contElVal')\n\t");
						else
							print("if($contEl != '$contElVal')\n\t");
							
						print("{\n\t\t");
							print("$childEl = $jsCode\n\t");
							$res .= "&& $childEl ";
						print("}\n\t");
						// child
						break;
					}
				}
			}
		}
		
		$res .= ");";
		print("$res\n\t");
		
		//// group validations
		print("// group validation\n\t");
		if(count($this->m_groupValidation) > 0)
		{
			foreach ($this->m_groupValidation as $groupVal)
			{
				$elements = $groupVal[0];
				$tag = $groupVal[1];
				
				print("resGV = (true");
				foreach ($elements as $element)
				{	
					print(" && $element");
				}
				print(");\n\t");
				
				print("if(resGV)\n\t\t");
					print("SetTagClass('$tag', 'fBold');\n\t");
				print("else\n\t\t");
					print("SetTagClass('$tag', 'fRed');\n\t");
					
				print("res = (res && resGV);\n\t");
			}
		}
		
		print("SerErrorText(res);\n\t");
		print("return res;\n");
		print("}\n-->\n</script>\n");
	}
	
	/**
	 * Chake Form Validation
	 *
	 * @return bool
	 */
	function IsValid()
	{
		// get all elemets
		$keys = array_keys($this->m_elements);
		// steping over elements
		foreach ($keys as $key)
		{
			
			if($this->m_elements[$key]->m_validation)
			{
				
				// no dynamics
				if($this->m_elements[$key]->m_dynamic == 0)
				{
					if(!$this->m_elements[$key]->IsValid())
						return false;
				}
				else
				{
					// for parent
					foreach ($this->m_dynamics as $dinamic)
					{
						$contEl 	= $dinamic[0];
						$contElVal  = $dinamic[1];
						$childEl    = $dinamic[2];
						$childElStatus = $dinamic[3];
						
						if($childEl == $key)
						{
							// paret //most
							$contValue = $this->m_elements[$contEl]->GetElValue();
			
							// child
							if($childElStatus)
								$chake = ($contValue == $contElVal);
							else
								$chake = ($contValue !== $contElVal);
								
							if($chake)
							{
								if(!$this->m_elements[$key]->IsValid());
									return false;
							}
							
							break;
						}
					}
				}
			}
		}

		return true;
	}
	
	/**
	 * add dynamic enabled features, when $contEl has $contElVal value
	 * then $childEl give status $childElStatus
	 *
	 * @param string $contEl
	 * @param string $contElVal
	 * @param string $childEl
	 * @param bool $childElStatus
	 * @return bool
	 */
	function AddDyEnabled($contEl, $contElVal, $childEl, $childElStatus = true)
	{
		// if control emenent exist
		if(isset($this->m_elements[$contEl]))
		{
			// add new dynamic efect to the dynamic array
			array_push($this->m_dynamics, array($contEl, $contElVal, $childEl, $childElStatus));
			$this->m_elements[$childEl]->m_dynamic = true;
			
			return true;
		}
		
		return false;
	}
	/**
	 * Enter description here...
	 *
	 * @param string $contEl
	 * @param string $script
	 * @return bool
	 */
	function AddDyScript($contEl, $script)
	{
		// if control emenent exist
		if(isset($this->m_elements[$contEl]))
		{
			// add new dynamic efect to the dynamic array
			array_push($this->m_dynamics, array($contEl, $script));
			
			return true;
		}
		
		return false;
	}
	
	/**
	 * Run all dynamic enableds
	 *
	 */
	function RunDyEnableds()
	{
		foreach ($this->m_dynamics as $dynamic)
		{
			list($contEl, $contElVal, $childEl, $childElStatus) = $dynamic;

			$childElStatus = ($childElStatus) ? true :false;
			
			// if control and child elements exist
			if(isset($this->m_elements[$contEl]) && isset($this->m_elements[$childEl]))
			{
				if($this->m_elements[$contEl]->m_value == $contElVal)
				{
					$this->m_elements[$childEl]->SetStatus($childElStatus);
				}
				else
				{
					$this->m_elements[$childEl]->SetStatus(!$childElStatus);
				}
			}
		}
	}
	
	/**
	 * return element value (from session)
	 *
	 * @param string $el
	 * @return string
	 */
	function GetElValue($el)
	{
		if(isset($_SESSION[$this->m_ID][$el]))
			return $_SESSION[$this->m_ID][$el];
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $el
	 * @return string
	 */
	function GetElValueText($el)
	{
		// if element exist
		if(isset($this->m_elements[$el]))	
		{
			// if element is FormElement
			if(is_a($this->m_elements[$el], 'FormElement'))
			{ 
				return $this->m_elements[$el]->GetValueText();
			}
			
			// data select
			if(is_a($this->m_elements[$el], 'DateSelects'))
			{
				return $this->m_elements[$el]->GetValueText();
			}
		}
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $el
	 * @return string
	 */
	function GetElTitle($el)
	{
		if(isset($this->m_elements[$el]))
		{
			return $this->m_elements[$el]->GetTitle();
		}
		
		return null;
	}
	
	/**
	 * Clear all form elements values from session
	 *
	 */
	function ClearSession()
	{
		if(isset($_SESSION[$this->m_ID]))
			unset($_SESSION[$this->m_ID]);
	}
	
	
	/**
	 * Add group elements, thant most validate tugether
	 *
	 * @param array $elements
	 * @param string $tag
	 * 
	 * @return bool
	 */
	function AddGroupValidation($elements, $tag = null)
	{
		$this->m_groupValidation;

		// chake that all eelements is exist
		$hasElements = true;
		foreach ($elements as $element)
		{
			if(!isset($this->m_elements[$element]))
			{
				$hasElements = false;
				break;
			}
		}

		// add group validation
		if($hasElements)
		{
			array_push($this->m_groupValidation, array($elements, $tag));
			return true;
		}
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
/**
 * This class has more features for working with multiple form. 
 * In case  form must be used  for many different  data add some form 
 * and use second name or the class will add it automaticity.
 *
 */
class FormMultiple
{
	var $m_ID;
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	var $m_secondNames;
	
	/**
	 * Enter description here...
	 *
	 * @var Form
	 */
	var $m_form;
	
	
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	var $m_sID;
	
	var $m_multipl;
	/**
	 * Enter description here...
	 *
	 * @param string $ID
	 * @param string $sID
	 * @param string $action
	 * @param bool $method
	 * @return FormMultiple
	 */
	function FormMultiple($ID, $sID = null, $action=null, $multipl="MULTIPLEIDS", $method = true)
	{
		$this->m_ID = $ID;
		$this->m_multipl = $multipl;
		/// only first time
		if(!isset($_SESSION[$ID][$this->m_multipl]))
		{
			$_SESSION[$ID][$this->m_multipl] = array();
		}
		// get second names
		// for thes case from SESSION, it can be reded from database
		$this->m_secondNames = $_SESSION[$ID][$this->m_multipl];
		
		// if $sID is null then add aoutomaticly get new id
		if(!$sID)
		{
			$this->m_sID = $this->GetNExtID();
		}
		else 
		{
			$this->m_sID = $sID;
		}
		
		$this->m_form = new Form("{$ID}_{$this->m_sID}", $action, $method);
		$this->m_form->AddHidden("secondname", $this->m_sID);
	}
	
	/**
	 * Get filds for all data
	 *
	 * @param array $filds
	 * @return array
	 */
	function GetDatas($filds)
	{
		$results = array();
		
		$values = array_values($this->m_secondNames);
		foreach ($values as $currentVal)
		{
			$current = array();
			$form = new Form("{$this->m_ID}_{$currentVal}");
			foreach ($filds as $fild)
			{
				$val = $form->GetElValue($fild);
				$current = array_merge($current, array($fild => "$val")); 
			}
			
			// add curent to results
			$results[$currentVal] = $current;
		}
		
		return $results;
	}
	
	/**
	 * Get count of records
	 * @return int
	 */
	function GetDataCount()
	{
		return count($this->m_secondNames);
	}
	/**
	 * Save form values and add second name to session, ths function us POST method
	 * for geting secong name (ID)
	 *
	 * @return bool
	 */
	function Save()
	{
		$res = $this->m_form->Save();
		
		if($res)
		{
			// get second name
			$sID = DB::POST('secondname');
			
			// if second name isn`t in second names then add it
			// if $sID is new then add it to $m_secondNames and to the SESSION (in SESSION case)
			
			if($sID && !in_array($sID, $this->m_secondNames))
			{
				array_push($this->m_secondNames, $sID);
				array_push($_SESSION[$this->m_ID][$this->m_multipl], $sID);
			}
		}
		
		return $res;
	}
	
	/**
	 * Draw form begin and seet hidden element for second name 
	 *
	 */
	function DrawBegin($validation = true)
	{
		$this->m_form->DrawBegin($validation);
		$this->m_form->DrawEl("secondname");
	}
	
	/**
	 * return next ID for second name //it is private function
	 *
	 * @return int
	 */
	function GetNExtID()
	{
		$sID = 0;
		
		if(count($this->m_secondNames) > 0)
		{
			$max = max($this->m_secondNames);
			$sID = $max + 1;
		}
		else 
		{
			$sID = 1;
		}
		return $sID;
	}
	
	/**
	 * This function do all actions for FormMultiple,
	 *  for example save, delete
	 *
	 */
	function DoAction()
	{
		$this->Save();
		
		if(DB::GET('formmAction'))
		{
			switch (DB::GET('formmAction'))
			{
				case "delete":
					$key = array_search($this->m_sID, $this->m_secondNames);
					
					$temp = $this->m_secondNames;
					$this->m_secondNames = array();
					
					// delete from second names array
					foreach ($temp as $skey => $val)
					{
						if($key != $skey)
							$this->m_secondNames[$skey] = $val;
					}
					// set to session
					$_SESSION[$this->m_ID][$this->m_multipl] = $this->m_secondNames;
					
					$this->m_form->ClearSession();
					break;
			}
		}
	}
}

/**
 * Value Validation
 *
 */
class ValueValidation
{
	/**
	 * trim text and chage empty
	 *
	 * @param string $value
	 * @return bool
	 */
	function isEmpty($value)
	{
		return (strlen(trim($value)) == 0);
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $value
	 * @return bool
	 */
	function isEmail($value)
	{
//		return (eregi("^([a-zA-Z0-9_-])+@[a-zA-Z0-9]+\.[a-zA-Z0-9_-]+$", $value)) ;
		return true;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $value
	 * @return bool
	 */
	function isZip($value)
	{
		if(strlen(trim($value)) == 5)
			return is_int($value);
		else
			return false;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $value
	 * @return bool
	 */
	function isPhone($value)
	{
		return (strlen(trim($value)) == 0);
	}
}
?>