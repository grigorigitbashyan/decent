<?php
Site::IncludeFile ( 'classes/Calendar/HTML/CalendarDivStyle.class.inc' );
Site::IncludeFile ( 'classes/Common/TemplateView.interface.inc' );
Site::IncludeFile ( 'classes/Common/MyScript.class.inc' );

class CalendarHTML extends View implements TemplateNamedView
{
	protected $m_year;
	protected $m_month;
	protected $m_days;
	
	function __construct($year, $month, $days = null)
	{
		parent::__construct ();
		
		$this->m_year = $year;
		$this->m_month = $month;
		$this->m_days = $days;
		
		$this->ReadNameStrings ( 'langs/', 'calendar.xml', $this->m_displayLang );
	}
	
	function DisplayInTag(CalendarDivStyle $divStyle, $showOutDays = true, $tag = 'div')
	{
		// calculate days in given month
		$daysInMonth = date ( 't', mktime ( 0, 0, 0, $this->m_month, 1, $this->m_year ) );
		$startWeedDayIndex = date ( 'N', mktime ( 0, 0, 0, $this->m_month, 1, $this->m_year ) );

		// get today
		$todayDay = ($this->m_year == date('Y') && $this->m_month == date('m')) ? date('j') : null;
		$daysInPrevMonth = date ( 't', mktime ( 0, 0, 0, $this->m_month - 1, 1, $this->m_year ) );
		
		// function display out of dayes befor month days
		for($index = 1; $index < $startWeedDayIndex; $index ++)
		{
			// class for out
			$outClass = $divStyle->GetOutDayClass ( $index + 1 );
			$sClass = ($outClass) ? " class=\"$outClass\"" : '';
			
			$value = '&nbsp;';
			
			// calculate last month day
			if ($showOutDays)
			{
				$value = $daysInPrevMonth - $startWeedDayIndex + $index + 1;
			}
			
			print ( "\n\t<{$tag}{$sClass}>$value</$tag>" );
		}
		
		// display month dayes
		$positionIndex = --$startWeedDayIndex;
		for($index = 1; $index <= $daysInMonth; $index ++)
		{
			$tagClass = null;
			$link = null;
			$title = null;
			
			if ($this->m_days && array_key_exists ( $index, $this->m_days ))
			{
				$dayIndex = ($startWeedDayIndex + $index - 1) % 7 + 1;
				
				if($todayDay == $index)
				{
					$tagClass = $divStyle->GetTodayClass();
				}
				else
				{
					$tagClass = $divStyle->GetSelectedDayClass ( $dayIndex );
				}
				
				// link
				if (isset ( $this->m_days [$index] ['link'] ))
				{
					$link = $this->m_days [$index] ['link'];
				}
				
				// title
				if (isset ( $this->m_days [$index] ['title'] ))
				{
					$title = $this->m_days [$index] ['title'];
					$title = " title=\"$title\"";
				}
			}
			else
			{
				$dayIndex = ($startWeedDayIndex + $index - 1) % 7 + 1;
				if($todayDay == $index)
				{
					$tagClass = $divStyle->GetTodayClass();
				}
				else 
				{
					$tagClass = $divStyle->GetDayClass ( $dayIndex );
				}
			}
			
			$tClass = ($tagClass) ? " class=\"$tagClass\"" : '';
			
			if ($link)
			{
				print ( "\n\t<{$tag}{$tClass}{$title}><a href=\"$link\">$index</a></$tag>" );
			}
			else
			{
				print ( "\n\t<{$tag}{$tClass}{$title}>$index</$tag>" );
			}
			
			if ((++ $positionIndex) % 7 == 0)
			{
				print ( "\n\t<br clear=\"all\" />" );
			}
		}
		
		// function display out of dayes after month days
		$nextMonthDay = 1;
		while ( ($positionIndex ++) % 7 != 0 )
		{
			$outClass = $divStyle->GetOutDayClass ( ($positionIndex - 1) % 7 + 1 );
			$sClass = ($outClass) ? " class=\"$outClass\"" : '';
			
			$value = '&nbsp;';
			
			// calculate last month day
			if ($showOutDays)
			{
				$value = $nextMonthDay ++;
			}
			
			print ( "\n\t<{$tag}{$sClass}>$value</$tag>" );
		}
		
		if($nextMonthDay > 1)
		{
			print('<br clear="all" />');
		}
	}
	
	/**
	 * @see TemplateView::GetTemplateValue()
	 *
	 * @param array $req if single char then get valu from date function in other clas 
	 * 					 it can be on of the following values:
	 * 					nextYear - next year of selected year (not current)
	 * 					nextMonth -  next month 01 day of selected month
	 * 					today - like date('Y-m-d');
	 * 					prevMonth - preview month of selected month
	 * 					prevYear - preview year of selected year
	 * 					year - selected year
	 * 					month - name of selected month
	 * 					realMonthName - current month name
	 */
	public function GetTemplateValue($req)
	{
		$res = '';
//		$properties = (isset($req ['value'])) ? $req ['value'] : '';
		
		if (strlen ( $req ['name'] ) == 1)
		{
			$res = date ( $req ['name'] );
		}
		else
		{
			switch ($req ['name'])
			{
				case 'nextYear' :
					$res = $this->m_year + 1;
					break;
				case 'nextMonth' :
					if ($this->m_month < 12)
					{
						$nextMonth = $this->m_month + 1;
						if ($nextMonth < 10)
						{
							$nextMonth = "0" . intval ( $nextMonth );
						}
						
						$res = "{$this->m_year}-{$nextMonth}-01";
					}
					else
					{
						$nextYear = $this->m_year + 1;
						$res = "{$nextYear}-01-01";
					}
					break;
				case 'today' :
					// display today
					$res = date ( 'Y-m-d' );
					break;
				case 'prevMonth' :
					if ($this->m_month > 1)
					{
						$nextMonth = $this->m_month - 1;
						if ($nextMonth < 10)
						{
							$nextMonth = "0" . intval ( $nextMonth );
						}
						
						$res = "{$this->m_year}-{$nextMonth}-01";
					}
					else
					{
						$nextYear = $this->m_year - 1;
						$res = "{$nextYear}-12-01";
					}
					break;
				case 'prevYear' :
					$res = $this->m_year - 1;
					break;
				case 'year' :
					$res = $this->m_year;
					break;
				case 'month' :
					$res = $this->GetNameString ( 'month' . intval ( $this->m_month ) );
					break;
				case 'realMonthName' :
					$month = date ( 'n' );
					$res = $this->GetNameString ( 'month' . $month );
					break;
			}
		}
		
		return $res;
	}
	
	function PutPattern($pattern)
	{
		$ms = new MyScript ( $pattern, $this );
		return $ms->GetParsedTemplate ();
	}
}
?> 