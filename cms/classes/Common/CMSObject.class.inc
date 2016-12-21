<?php
Site::IncludeFile ( 'classes/Common/View.class.inc' );

/**
 * Base class for all CMS objects
 *
 * @package Main
 */
abstract class CMSObject extends View
{
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected static $m_globalNameStrings;
	
	/**
	 * This variable contains message key, 
	 * which must be read from XML
	 *
	 * @var string
	 */
	protected $m_message;
	
	/**
	 * Enter description here...
	 *
	 * @var CMS
	 */
	protected static $CMS;
	
	/**
	 * Enter description here...
	 *
	 * @return CMSObject
	 */
	function __construct()
	{
		parent::__construct ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $name
	 * @param string $insertRight usually delte button
	 * @param bool $displayBR display <br /> int the end
	 */
	public function DisplayTitle($name, $insertRight = null, $displayBR = true)
	{
		print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
		          <tr>
		            <td>
		                <div class="titleColL"><h1>' . $name . '</h1></div>
		                <br class="clearFloat" />
		        	</td>' );
		if ($insertRight)
		{
			print ( '<td align="right">' );
			print ( $insertRight );
			print ( '</td>' );
		}
		print ( '</tr></table>' );
		
		// display new line
		if ($displayBR)
			print ( '<br />' );
	}
	
	
	/**
	 * Print form for CMSObject, and set action name as class name + 'Action'
	 * and set value equale $action
	 * 
	 * @param string $action
	 * @param string $formName
	 * @param string $metod
	 * @param string $formJS
	 * @param string $acation
	 */
	public function DisplayFormHeadr($action, $formName = '', $metod = 'post', $formJS = null, $acation = null)
	{
		
		$className = $this->GetFormActionName ();
		
		print ( "\n<form method='$metod' name='$formName' id='$formName' class='clearPD' action='$acation' $formJS>\n" );
		print ( "<input type='hidden' name='$className' value='$action'>\n" );
	}
	
	/**
	 * Return POST overloading name for each class
	 *
	 * @return string
	 */
	final protected function GetFormActionName()
	{
		$className = get_class ( $this );
		$className = strtolower ( $className ) . 'Action';
		
		return $className;
	}
	
	/**
	 * Print form footer with submit button.
	 * Set button name $buttonName
	 *
	 * @param string $buttonName
	 */
	public function DisplayFormFooter($buttonName = 'Done')
	{
		print ( "<input type='submit' value='$buttonName'>\n" );
		print ( '</form>' );
	}
	
	/**
	 * If there is a form submit with current class action, then return 
	 * current action, in other case return null
	 *
	 * @return string
	 */
	protected final function GetFormAction()
	{
		$className = $this->GetFormActionName ();
		
		// if is set action then return value, in other case return null
		if (($action = DB::POST ( $className )))
		{
			return $action;
		}
		
		return DB::GET ( $className );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $action
	 * @param string $label
	 * @param string $name
	 * @param string $value
	 * @param array $hiddens
	 * @param string $doneS
	 * @param string $formJS
	 */
	function DisplaySingleTextForm($action, $label = null, $name = 'name', $value = null, $hiddens = null, $doneS = 'Done', $formJS = null, $URL = null)
	{
		// display form header
		$this->DisplayFormHeadr ( $action, '', 'post', $formJS, $URL );
		
		// replase single '
		if ($value && is_string ( $value ))
		{
			$$value = str_replace ( "'", "&rsquo;", $value );
		}
		
		// if there are hidden elements then display them
		if ($hiddens)
		{
			foreach ( $hiddens as $hkey => $hvalue )
			{
				print ( "<input type='hidden' name='$hkey' value='$hvalue' />\n" );
			}
		}
		
		print ( '<table width="100%" border="0" cellspacing="0" cellpadding="0" class="titleBox">
			          <tr>
			            <td>
			                <div class="titleColL">' );
		// display label
		if ($label)
		{
			print ( "<label><strong>$label</strong></label>\n" );
		}
		
		print ( "<input type=\"text\" name=\"$name\" id=\"$name\" value=\"$value\" size=\"64\" maxlength=\"256\" style=\"font-size:14px\" />\n" );
		print ( '</div>
			                <div class="titleColR"><input name="" type="submit" value="' . $doneS . '" /></div>
			                <br class="clearFloat" />
			        	</td>
			          </tr>
			        </table>
			        </form>' );
	}
	
	/**
	 * display hidden element
	 *
	 * @param string $name
	 * @param string $value
	 */
	function DisplayHidden($name, $value)
	{
		print ( "<input type='hidden' name='$name' value='$value'/\n>" );
	}
	
	/**
	 * Create database for module
	 * @param $sql
	 * @return bool
	 */
	function CreateDB($sql = null)
	{
		if (! $sql)
		{
			$sql = $this->CreateDBSQL ();
		}
		// 
		if ($sql)
		{
			if(is_array($sql))
			{
				$res = true;
				foreach ($sql as $sqlC)
				{
					$res &= $this->CreateDB($sqlC);
				}
				
				return $res;
			}
			
			$res = DB::Query ( $sql );
			
			if (! $res)
			{
				$error = DB::GetError ();
				print ( "<p style='color: #F00'>$error</p>" );
			}
			
			return $res;
		}
		
		return true;
	}
	
	/**
	 * Return SQL statment to create table for current module.
	 *
	 * @return string
	 */
	function CreateDBSQL()
	{
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @return string
	 */
	function GetNameString($key)
	{
		$parName = parent::GetNameString ( $key );
		
		if ($parName)
		{
			return $parName;
		}
		
		return CMSObject::GetGlobalNameString ( $key );
	}
	
	/**
	 * Load global names by DOMDocument or simplexml
	 *
	 * @param $readByDOM
	 */
	static public function ReadGlobalNameStrings($readByDOM = true)
	{
		$fileName = 'globalnames.xml';
		
		// get CMS display language
		$currentLang = CMS::GetCMSLang ();
		
		if (! $currentLang)
		{
			$currentLang = 'en';
		}
		
		// get file full name
		$fullPath = 'classes/Common/langs/' . "$currentLang/" . $fileName;
		
		if (file_exists ( $fullPath ))
		{
			if ($readByDOM)
			{
				CMSObject::$m_globalNameStrings = View::ReadNamesDOM ( $fullPath );
			}
			else
			{
				CMSObject::$m_globalNameStrings = simplexml_load_file ( $fullPath );
			}
		}
		else
		{
			die ( "file $fullPath was not found!" );
		}
	}
	
	/**
	 * Read names from XML fiel for current module
	 *
	 * @param string $filePath
	 * @param string $fileName
	 */
	public function ReadNameStrings($filePath, $fileName, $currentLang = NULL, $readByDOM = true)
	{
		$currentLang = CMS::GetCMSLang ();
		
		if ($currentLang)
		{
			parent::ReadNameStrings ( $filePath, $fileName, $currentLang );
		}
	}
	/**
	 * Enter description here...
	 *
	 * @param string $key
	 * @return string
	 */
	static final public function GetGlobalNameString($key)
	{
		if (isset ( CMSObject::$m_globalNameStrings [$key] ))
		{
			return CMSObject::$m_globalNameStrings [$key];
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $lang
	 */
	static function SetCMSLang($lang)
	{
		CMSObject::$m_cmsLang = $lang;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param int $panelIndex
	 * @param string $rename
	 */
	public function DisplayTabHeader($panelIndex, $rename)
	{
		print ( "<div id='CollapsiblePanel$panelIndex' class='CollapsiblePanel'>
			                <div class='CollapsiblePanelTab' tabindex='0'>$rename</div>
			                <div class='CollapsiblePanelContent'>
			                    <div class='CollapsiblePanelContentPD'>" );
	}
	
	/**
	 * Enter description here...
	 *
	 */
	public function DisplayTabFooter()
	{
		print ( '</div></div></div>' );
	}
	
	/**
	 * Return test results
	 *
	 * @return bool
	 */
	public function SelfTest()
	{
		return $this->TestTable ();
	}
	
	/**
	 * This function display message if it has been set.
	 *
	 */
	function DisplayMessage()
	{
		if ($this->m_message)
		{
			// read message from XML
			$message = $this->GetNameString ( $this->m_message );
			
			if ($message)
			{
				print ( $message );
			}
			else
			{
				print ( "The message for '{$this->m_message}' key does not exist!" );
			}
		}
	}
	
	/**
	 * Display message if it has been set.
	 *
	 */
	function DisplayFeatures()
	{
		$this->DisplayMessage ();
	}
	
	/**
	 * Enter description here...
	 *
	 * @param mixed $arg
	 * @return string
	 */
	function GetAjax($arg=null)
	{
		return null;
	}
	
	/**
	 * This function retrive owerloaded perimeters from ajax '$_GET' perimeter.
	 *
	 * @param string $param
	 * @return array|null
	 */
	function RetriveAjaxParams($param)
	{
		$results = array ();
		$a = explode ( '&', $param );
		
		if (count ( $a ) > 0)
		{
			foreach ( $a as $item )
			{
				$b = explode ( '=', $item );
				$results ["$b[0]"] = $b [1];
			}
		}
		
		return $results;
	}
	
	/**
	 * This function return all table names, which has some relateion with current object
	 *
	 * @return array|null
	 */
	function GetObjectTables()
	{
		if(($sql = $this->CreateDBSQL()))
		{
			$res = array();
			
			// multiple table creates
			if(is_array($sql))
			{
				foreach ($sql as $sqlC)
				{
					$tableName = DBxp::GetTableNameFromCreateSQL($sqlC);
					array_push($res, $tableName);	
				}
			}
			else 
			{
				$tableName = DBxp::GetTableNameFromCreateSQL($sql);
				array_push($res, $tableName);	
			}
			
			return $res;
		}
		
		return null;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param CMS $CMS
	 */
	static function SetCMS($CMS)
	{
		CMSObject::$CMS = $CMS;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $table
	 * @param array $fields
	 */
	final function TestTable($table = null, $fields = null)
	{
		// create table sql statement
		$creteSQL = null;
		if (! ($table && $fields))
		{
			// do not have sql to create table
			if (! ($creteSQL = $this->CreateDBSQL ()))
			{
				return true;
			}
			
			// for multiple table craetes check for each
			if(!$table && is_array($creteSQL))
			{
				$res = true;
				foreach ($creteSQL as $creteSQLC)
				{
					$table = DBxp::GetTableNameFromCreateSQL ( $creteSQLC );
					$fields = DBxp::GetTableFieldsFromCreateSQL( $creteSQLC );
					$res &= $this->TestTable($table, array_keys($fields));
				}
				
				return $res;
			}
		}

		// if table name has not been set then //////////////////////// 
		//get filed from create table sql statement
		if (! $table)
		{
			$table = DBxp::GetTableNameFromCreateSQL ( $creteSQL );
		}
		///////////////////////////////////////////////////////////
		

		// if fileds has not been set then //////////////////////// 
		//get filed from create table sql statement
		if (! $fields)
		{
			$fieldsSQL = DBxp::GetTableFieldsFromCreateSQL ( $creteSQL );
			$fields = array_keys ( $fieldsSQL );
		}
		///////////////////////////////////////////////////////////
		
		// get request owerloaded value to corrent tables
		$currection = $this->CanBeCorrected();
		
		$langKeys = $this->m_site->GetLanguages ();
		$res = DBxp::TestTable ( $table, $fields, $langKeys, ! $currection );
		if (! $res && $currection)
		{
			// if table exist then correct only fileds
			if (in_array ( $table, DBxp::GetTables () ))
			{
				// correct fields
				if (($sql = $this->CreateDBSQL ()))
				{
					DBxp::AddMissingFieldsToTable ( $sql );
				}
			}
			else
			{
				$this->CreateDB ();
			}
			
			$res = DBxp::TestTable ( $table, $fields, $langKeys );
			
			if ($res)
			{
				print "The table <b>`$table`</b> has been successfully corrected!<br />";
			}
		}
		
		return $res;
	}
	
	/**
	 * Return bool value, can module be corrected?
	 *
	 * @return bool
	 */
	protected function CanBeCorrected()
	{
		return (DB::REQUEST ( 'correctCMS' ) != null);	
	}
}
?>