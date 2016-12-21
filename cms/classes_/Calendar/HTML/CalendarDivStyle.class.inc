<?php
class CalendarDivStyle
{
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_titleClass;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_workdayClass;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_weekendClass;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_workdaySelectClass;
	/**
	 * Enter description here...
	 *
	 * @var string
	 */
	protected $m_weekendSelectClass;
	
	/**
	 * class for days that are out of selected mound
	 *
	 * @var string
	 */
	protected $m_outOfMonthClass;
	
	/**
	 * class for days that are out of selected mound
	 *
	 * @var string
	 */
	protected $m_outOfMonthWeekendClass;

	/**
	 * class for days that are out of selected mound
	 *
	 * @var int
	 */
	protected $m_weekendDays;
	
	/**
	 * today's style
	 *
	 * @var string
	 */
	protected $m_todayStyle;
	
	/*
	 * Contruct
	 */
	function __construct($weekend2Days = true)
	{
		$this->m_weekendDays = ($weekend2Days) ? 1 : 0;
	}
	
	/**
	 * @return string
	 */
	public function GetTitleClass()
	{
		return $this->m_titleClass;
	}
	
	/**
	 * @return string
	 */
	public function GetWeekendClass()
	{
		return $this->m_weekendClass;
	}
	
	/**
	 * @return string
	 */
	public function GetOutOfMonthWeekendClass()
	{
		return $this->m_outOfMonthWeekendClass;
	}
	
	/**
	 * @return string
	 */
	public function GetWeekendSelectClass()
	{
		return $this->m_weekendSelectClass;
	}
	
	/**
	 * @return string
	 */
	public function GetWorkdayClass()
	{
		return $this->m_workdayClass;
	}
	
	/**
	 * @return string
	 */
	public function GetWorkdaySelectClass()
	{
		return $this->m_workdaySelectClass;
	}
	
	/**
	 * @return string
	 */
	public function GetOutOfMonthClass()
	{
		return $this->m_outOfMonthClass;
	}
	
	/**
	 * Enter description here...
	 *
	 * @return string
	 */
	public function GetTodayClass()
	{
		return $this->m_todayStyle;
	}
	
	/**
	 * @param string $m_titleClass
	 */
	public function SetTitleClass($titleClass)
	{
		$this->m_titleClass = $titleClass;
	}
	
	/**
	 * @param string $m_weekendClass
	 */
	public function SetWeekendClass($weekendClass)
	{
		$this->m_weekendClass = $weekendClass;
	}
	
	/**
	 * @param string $outOfMonthWeekendClass
	 */
	public function SetOutOfMonthWeekendClass($outOfMonthWeekendClass)
	{
		$this->m_outOfMonthWeekendClass = $outOfMonthWeekendClass;
	}
	
	/**
	 * @param string $m_weekendSelectClass
	 */
	public function SetWeekendSelectClass($weekendSelectClass)
	{
		$this->m_weekendSelectClass = $weekendSelectClass;
	}
	
	/**
	 * @param string $m_workdayClass
	 */
	public function SetWorkdayClass($workdayClass)
	{
		$this->m_workdayClass = $workdayClass;
	}
	
	/**
	 * @param string $m_workdaySelectClass
	 */
	public function SetWorkdaySelectClass($workdaySelectClass)
	{
		$this->m_workdaySelectClass = $workdaySelectClass;
	}
	
	/**
	 * This function returns class for workday or weedend.
	 *
	 * @param int $dayIndex from 1 to 7 (monday, ...)
	 * @return string
	 */
	function GetDayClass($dayIndex)
	{
		// workday
		if($dayIndex < (7 - $this->m_weekendDays))
		{
			return $this->GetWorkdayClass();
		}
		
		// weekend 
		$weekend = $this->GetWeekendClass();

		return ($weekend) ? $weekend : $this->GetWorkdayClass();
	}
	
	/**
	 * This function returns class for selected workday or weedend.
	 *
	 * @param int $dayIndex
	 * @return string
	 */
	function GetSelectedDayClass($dayIndex)
	{
		// workday
		if($dayIndex < (7 - $this->m_weekendDays))
		{
			return $this->GetWorkdaySelectClass();
		}
		
		// weekend 
		$weekend = $this->GetWeekendSelectClass();

		return ($weekend) ? $weekend : $this->GetWorkdaySelectClass();
	}
	
	/**
	 * @param string $m_workdayClass
	 */
	public function SetOutOfMonthClass($outOfMonthClass)
	{
		$this->m_outOfMonthClass = $outOfMonthClass;
	}
	
	/**
	 * This function returns class for workday or weedend.
	 *
	 * @param int $dayIndex from 1 to 7 (monday, ...)
	 * @return string
	 */
	function GetOutDayClass($dayIndex)
	{
		// workday
		if($dayIndex < (7 - $this->m_weekendDays))
		{
			return $this->GetOutOfMonthClass();
		}
		
		// weekend 
		$weekend = $this->GetOutOfMonthWeekendClass();

		return ($weekend) ? $weekend : $this->GetOutOfMonthClass();
	}
	
	/**
	 * @param string $todayStyle
	 */
	public function SetTodayClass($todayStyle)
	{
		$this->m_todayStyle = $todayStyle;
	}
}
?>