<?php

interface TemplateView
{
	
	/**
	 * This functino will return string value of geven request
	 *
	 * @param array $req
	 */
	public function GetTemplateValue($req);
}

interface TemplateNamedView extends TemplateView
{
	/**
	 * This functino will return string value of geven request
	 * 
	 * @param string $req
	 */
	public function GetNameString($req);
}
?>