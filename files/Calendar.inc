<?php
Site::IncludeFile ( 'classes/Calendar/HTML/CalendarHTML.class.inc' );
Site::IncludeFile ( 'classes/Calendar/CalendarDataList.class.inc' );

// global names
global $params, $lang;

//	get events path
$eventsS = new Category(146);
$eventsPath = $eventsS->GetPath();

$prTypeID = 9;
$query = "
	select 
		PM.`date`, PC1.`value` as `datefrom`, PC2.`value` as `dateto` 
	from 
		`pr_main` PM 
	join 
		`pr_char` PC1 on (PM.`ID` = PC1.`productID` and PC1.`fieldID` = 42) 
	join 
		`pr_char` PC2 on (PM.`ID` = PC2.`productID` and PC2.`fieldID` = 43) 
	where 
		PM.`typeID` = ".$prTypeID." and 
		PM.`ID` IN 
  		(
  			SELECT 
  				`productID` 
  			FROM 
  				`pr_options` 
			WHERE 
				`pr_options`.`relatedID` = 146 AND 
				`pr_options`.`fieldID` IN 
										(
											SELECT 
												`ID` 
											FROM 
												`pr_field` 
											WHERE 
												`pr_field`.`type` = 2 || `pr_field`.`type` = 8
										)
		)
";
$retArr = DB::QueryToArray($query);

$datesArr = array();
for ( $i=0; $i<count($retArr); $i++ )
{
	if ( $retArr[$i]["datefrom"]!="" && $retArr[$i]["dateto"]!="")
	{
		$dateFrom = strtotime($retArr[$i]["datefrom"]);
		$dateTo = strtotime($retArr[$i]["dateto"]);
		
		while ( $dateFrom<=$dateTo )
		{
			if ( !in_array(date("Y-m-d", $dateFrom), $datesArr) )
			{
				$datesArr[] = date("Y-m-d", $dateFrom);
			}
			
			$dateFrom = $dateFrom + 86400;
		}
	}
	elseif ( $retArr[$i]["datefrom"]!="" )
	{
		if ( !in_array($retArr[$i]["datefrom"], $datesArr) )
		{
			$datesArr[] = $retArr[$i]["datefrom"];
		}
	}
	elseif ( $retArr[$i]["dateto"]!="" )
	{
		if ( !in_array($retArr[$i]["dateto"], $datesArr) )
		{
			$datesArr[] = $retArr[$i]["dateto"];
		}
	}
}

///////////////////////////////////////
$mainLink = "$lang/calendar/";

$currDate = date ( 'Y-m-d' );
foreach ( $params as $param )
{
	if (preg_match('/^[0-9]{4}-[0-9]{2}-[0-9]{2}$/', $param))
	{
		$currDate = $param;
	}
}

$Year = substr ( $currDate, 0, 4 );
$Month = substr ( $currDate, 5, 2 );

// retrive days
$cdl = new CalendarDataList ( 1 );
$days = $cdl->GetMonthData ( $Year, $Month );

if ($days)
{
	$keys = array_keys ( $days );
	
	foreach ( $keys as $key )
	{
		$keyS = ($key < 10) ? "0".$key : $key;
		$days [$key] = array ('link' => "{$mainLink}{$Year}-{$Month}-{$keyS}/" );
	}
}

/////////////////////////////////////////////////
$cal = new CalendarHTML ( $Year, $Month, $days );

// create style
$divClasses = new CalendarDivStyle ( );

$divClasses->SetOutOfMonthClass ( 'workdayH' );
$divClasses->SetOutOfMonthWeekendClass ( 'weekendH' );

$divClasses->SetWorkdayClass ( 'workday' );
$divClasses->SetWorkdaySelectClass ( 'workdayS' );

$divClasses->SetWeekendClass ( 'weekend' );
$divClasses->SetWeekendSelectClass ( 'weekendS' );

$divClasses->SetTodayClass ( 'today' );

//////////// display calendar /////////////////////////////////////////
?>
<!-- Calendar -->
<script src="tools/calendar/src/js/jscal2.js" type="text/javascript"></script>
<script src="tools/calendar/src/js/lang/ru.js" type="text/javascript"></script>

<link rel="stylesheet" type="text/css" href="tools/calendar/src/css/jscal2.css" />
<link rel="stylesheet" type="text/css" href="tools/calendar/src/css/border-radius.css" />
<link rel="stylesheet" type="text/css" href="tools/calendar/src/css/steel/steel.css" />
<!-- end Calindar -->

<div class="someWBox">
	<div id="cont"></div>
	<script type="text/javascript">
	//<![CDATA[
	function updateFields(cal) {
		var date = cal.selection.get();
		date = Calendar.intToDate(date);
		dateS = Calendar.printDate(date, "%Y-%m-%d");
		
		window.location.href = "<?php echo $lang."/".$eventsPath;?>page1/?date=" + dateS ;
	};
	var dateArr = new Array();
	<?php
	for ( $d=0; $d<count($datesArr); $d++ )
	{
		?>
		dateArr[<?php echo $d;?>] = "<?php echo $datesArr[$d];?>";
		<?php
	}
	?>
	Calendar.setup({
		cont         : "cont",
		onSelect     : updateFields,
		onTimeChange : updateFields,
		date		 : "<?php echo $currDate;?>",
		disabled: function(date) {
			var MyDateString;
			
			MyDateString = date.getFullYear() + '-' + ('0' + (date.getMonth()+1)).slice(-2) + '-' + ('0' + date.getDate()).slice(-2);
			
			if ( jQuery.inArray(MyDateString, dateArr)>=0 )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	});
    //]]>
    </script>
</div>
<?php
/*print ( '<div class="calendar">' );

// display header
$pattern = '<div class="calendarHeaer">
    	<div class="back"><a href="' . $lang . '/calendar/@@prevMonth@@/"><img src="tools/calendar/back.png" alt="" /></a></div>
    	<div class="month">@@month@@</div>
        <div class="year">@@year@@</div>
        <div class="next"><a href="' . $lang . '/calendar/@@nextMonth@@/"><img src="tools/calendar/next.png" alt="" /></a></div>
        <br clear="all" />
    </div>';
echo $cal->PutPattern ( $pattern );
// display body ///////////////////////////////////////////////////////
print ( '<div class="calendarBody">' );

// display week days
$pattern = '<div class="workdayTitle">@@name:day1@@</div>
	       <div class="workdayTitle">@@name:day2@@</div>
	       <div class="workdayTitle">@@name:day3@@</div>
	       <div class="workdayTitle">@@name:day4@@</div>
	       <div class="workdayTitle">@@name:day5@@</div>
	       <div class="weekendTitle">@@name:day6@@</div>
	       <div class="weekendTitle">@@name:day7@@</div>
	       <br clear="all" />';

echo $cal->PutPattern ( $pattern );
// display days

$cal->DisplayInTag ( $divClasses );
print ( '</div>' );
$pattern = '<div class="calendarFooter">@@name:today@@: <a href="' . $lang . '/calendar/@@today@@/">@@d@@ @@realMonthName@@ @@Y@@</a></div>';
echo $cal->PutPattern ( $pattern );
print ( '</div>' );*/
?>