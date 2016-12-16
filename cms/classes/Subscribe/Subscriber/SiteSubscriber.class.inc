<?php
Site::IncludeFile ( 'classes/Subscribe/Subscriber/Subscriber.class.inc' );

class SiteSubscriber extends View
{
	/**
	 * Enter description here...
	 *
	 * @var array
	 */
	protected $m_params;
	
	function __construct()
	{
		parent::__construct ();
		$this->ReadNameStrings ( 'langs/', 'subscribe.xml' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @return bool
	 */
	function DoAction()
	{
		$res = false;
		if (DB::POST ( 'newSubscriber' ))
		{
			$properties = array ();
			$adds = array ();
			
			$properties ['name'] = '';
			$properties ['email'] = '';
			$properties ['statistic'] = '';
			
			foreach ( $this->m_params as $param )
			{
				// if value has been set
				$value = DB::POST ( $param );
				if ($value)
				{
					switch ($param)
					{
						case 'name' :
						case 'email' :
						case 'statistic' :
							$properties [$param] = $value;
							break;
						default :
							$adds [$param] = $value;
							break;
					}
				}
			}
			
			$properties ['additional'] = (count ( $adds )) ? serialize ( $adds ) : "";
			$properties ['code'] = GetRandCode ( 10 );
			
			// insert subscriber
			$subscriber = new Subscriber ( );
			$message = '';
			
			// chech email
			if (! $subscriber->CheckEmail ( $properties ['email'] ))
			{
				// insert
				$res = $subscriber->Insert ( $properties );
				$messageN = ($res) ? 'subSucc' : 'subUnSucc';
				$message = $this->GetNameString ( $messageN );
			}
			else
			{
				$message = $this->GetNameString ( 'emailExist' );
			}
		
			print($message);
		}
		
		return $res;
	}
	
	/**
	 * Enter description here...
	 *
	 * @param array $params
	 */
	function SetParams($params)
	{
		$this->m_params = $params;
	}
	
	/**
	 * This function must be called in subscriber form
	 *
	 */
	function DisplaySubscriberFormHeader()
	{
		print ( '<input type="hidden" name="newSubscriber" value="new" />' );
	}
	
	/**
	 * Enter description here...
	 *
	 * @param string $unsubCode
	 * @return bool
	 */
	function Unsubscribe($unsubCode)
	{
		if(strlen($unsubCode) > 0)
		{
			$sub = new Subscriber ( $unsubCode, 'code' );
			
			$res = $sub->Delete ();
			
			$messageN = ($res) ? 'unSubSucc' : 'unSubUnSucc';
			$message = $this->GetNameString ( $messageN );
			
			print ( $message );
			
			return $res;
		}
		
		return false;
	}
}
?>