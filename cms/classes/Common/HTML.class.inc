<?php
class HTML
{
	/**
	 * Enter description here...
	 *
	 * @param mixed $value
	 * @return mixed
	 */
	static function RemoveXSS($value)
	{
		// the folowing must be removed
		// < > ' " \ 
//		$tags = array('<', '>', '\'', '\\', '"', '\/', "\n", "\r");
		$tags = array('<', '>', '\\', '"', '\/', "\n", "\r");
		return str_replace($tags, '', $value);
	}
}
?>