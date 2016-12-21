<?php
/**
 * @author Vazgen
 *
 */
class ErrorHandler {
	/**
	 * @var array
	 */
	protected $m_messages;
	
	/**
	 * @var string
	 */
	protected $m_email;
	/**
	 * @var array
	 */
	protected $m_message;
	/**
	 * @return ErrorHandler
	 */
	function __construct() {
		$this->m_messages = array ();
	}
	
	/**
	 * @param $errno
	 * @param $errstr
	 * @param $errfile
	 * @param $errline
	 * @return null
	 */
	function Add($errno, $errstr, $errfile, $errline) {
		$this->m_messages [] = array (date ( 'c' ), $errno, $errstr, $errfile, $errline );
	}
	
	/**
	 * @return null
	 */
	function __destruct() {
		if (count ( $this->m_messages ) > 0) {
			
			$total = '';
			// write all messages
			foreach ( $this->m_messages as $messsage ) {
				$res = implode ( ' :: ', $messsage );
				$total .= "$res \n";
			}

			// write to file
			$file = fopen ( 'Log.txt', 'a' );
			if ($file) {
				// open file
				fwrite ( $file, $total );
			}
			
			// send email ////////////////////////
			// open error config file
			if(file_exists('config/error.xml'))
			{
				$xml = simplexml_load_file('config/error.xml');
			}
		}
	}
	
	function DisplayErrorMessage($lang)
	{
		
	}
}
?>