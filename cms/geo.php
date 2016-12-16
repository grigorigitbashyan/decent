<?php
set_include_path('classes/');
error_reporting(0);
include_once ('Common/ContentCaching.class.inc');
include_once ('Folder/FolderXP.class.inc');

include_once ('Common/DB.class.inc');
include_once ('files/db.inc');

include_once ('Common/Site.class.inc');
include_once ('Common/HTML.class.inc');

Site::LoadStatics ();

include_once ('Common/View.class.inc');

include_once ('Common/NamedRecord.class.inc');
include_once ('Content/Content.class.inc');

include_once ('Category/CategoryView.class.inc');
include_once ('Menu/MenuViewSite.class.inc');  

include_once ('Common/functions.inc');

include_once ('Product/ProductViewSite.class.inc');
include_once ('Product/ProductAll.class.inc');
include_once ('Product/pr_main/Pr_main.class.inc');

include_once ('Product/ProductSearch.class.inc');

$productID = intval($_GET["id"]);
$group = intval($_GET["group"]);
$tPr = new Pr_main($productID);
$prLat = $tPr->GetFieldValue(55, "ru", false, null, $group);
$prLng = $tPr->GetFieldValue(56, "ru", false, null, $group);
$address = trim($tPr->GetFieldValue(28, "ru", false, null, $group));
$zip = trim($tPr->GetFieldValue(29, "ru", false, null, $group));
$city = trim($tPr->GetFieldValue(18, 'ru', true, "key", $group));
if ( $city)
{
	$city = str_replace(" ", "+", $city);
	
	if ( $address )
	{
		$address = str_replace(" ", "+", $address);
	}
	
	$ch = curl_init();
	$timeout = 5;
	$userAgent = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.1.4322)';
	
	if ( $address )
	{
		if ( $zip )
		{
			$url = 'http://maps.googleapis.com/maps/api/geocode/json?address='.$address.',+'.$zip.',+'.$city.',+Switzerland&sensor=false';
		}
		else 
		{
			$url = 'http://maps.googleapis.com/maps/api/geocode/json?address='.$address.',+'.$city.',+Switzerland&sensor=false';
		}
	}
	else 
	{
		$url = 'http://maps.googleapis.com/maps/api/geocode/json?address='.$city.',+Switzerland&sensor=false';
	}
	
	curl_setopt($ch,CURLOPT_URL,$url);
	curl_setopt($ch,CURLOPT_USERAGENT, $userAgent);
	curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
	curl_setopt($ch,CURLOPT_CONNECTTIMEOUT,$timeout);
	curl_setopt($ch,CURLOPT_FAILONERROR, true);
	curl_setopt($ch,CURLOPT_FOLLOWLOCATION, true);
	curl_setopt($ch,CURLOPT_AUTOREFERER, true);
	curl_setopt($ch,CURLOPT_TIMEOUT, 10);
	$data = curl_exec($ch);
	curl_close($ch);
	
	$data = json_decode($data);
	if ( $data->status == "OK" )
	{
		$lat = $data->results[0]->geometry->location->lat;
		$lng = $data->results[0]->geometry->location->lng;
		
		if ( $prLat=="" || $prLat==0 )
		{
			$prLat = $lat;
			$prLng = $lng;
		}
		?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=UTF-8"/>
<title>Find latitude and longitude with Google Maps</title>
<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=AIzaSyDLnmrTWD_FqweogMXrZ6g9zoh7u9cB3KA" type="text/javascript"></script>
<script type="text/javascript">
function load(lat, lng) {
	if (GBrowserIsCompatible()) {
		var map = new GMap2(document.getElementById("map"));
		map.addControl(new GSmallMapControl());
		map.addControl(new GMapTypeControl());
		var center = new GLatLng(lat, lng);
		map.setCenter(center, 15);
		geocoder = new GClientGeocoder();
		var marker = new GMarker(center, {draggable: true});  
		map.addOverlay(marker);
		document.getElementById("lat").value = center.lat().toFixed(5);
		document.getElementById("lng").value = center.lng().toFixed(5);
		
		GEvent.addListener(marker, "dragend", function() {
			var point = marker.getPoint();
			map.panTo(point);
			document.getElementById("lat").value = point.lat().toFixed(5);
			document.getElementById("lng").value = point.lng().toFixed(5);
		});
		
		GEvent.addListener(map, "moveend", function() {
			map.clearOverlays();
			var center = map.getCenter();
			var marker = new GMarker(center, {draggable: true});
			map.addOverlay(marker);
			document.getElementById("lat").value = center.lat().toFixed(5);
			document.getElementById("lng").value = center.lng().toFixed(5);
			
			GEvent.addListener(marker, "dragend", function() {
				var point = marker.getPoint();
				map.panTo(point);
				document.getElementById("lat").value = point.lat().toFixed(5);
				document.getElementById("lng").value = point.lng().toFixed(5);
			});
		});
	}
}

function Confirm()
{
	<?php
	if ( isset($_GET["group"]) )
	{
		?>
		window.parent.document.getElementById("productfield55_<?php echo $group;?>").value = document.getElementById("lat").value;
		window.parent.document.getElementById("productfield56_<?php echo $group;?>").value = document.getElementById("lng").value;
		<?php
	}
	else 
	{
		?>
		window.parent.document.getElementById("productfield55").value = document.getElementById("lat").value;
		window.parent.document.getElementById("productfield56").value = document.getElementById("lng").value;
		<?php
	}
	?>
}
</script>
<script type="text/javascript">
//<![CDATA[
var gs_d=new Date,DoW=gs_d.getDay();gs_d.setDate(gs_d.getDate()-(DoW+6)%7+3);
var ms=gs_d.valueOf();gs_d.setMonth(0);gs_d.setDate(4);
var gs_r=(Math.round((ms-gs_d.valueOf())/6048E5)+1)*gs_d.getFullYear();
var gs_p = (("https:" == document.location.protocol) ? "https://" : "http://");
document.write(unescape("%3Cscript src='" + gs_p + "s.gstat.orange.fr/lib/gs.js?"+gs_r+"' type='text/javascript'%3E%3C/script%3E"));
//]]>
</script>
</head>

<body onload="load(<?php echo $prLat;?>, <?php echo $prLng;?>)" onunload="GUnload()" >
<input type="hidden" name="lat" id="lat" value="<?php echo $lat;?>">
<input type="hidden" name="lng" id="lng" value="<?php echo $lng;?>">
<p>
<input type="button" value="Auto" onclick="load(<?php echo $lat;?>, <?php echo $lng;?>);">
<input type="button" value="Set" onclick="Confirm();">
</p>
<p>
<div align="center" id="map" style="width: 385px; height: 265px"><br/></div>
</p>
</div>
<script type="text/javascript">
//<![CDATA[
if (typeof _gstat != "undefined") _gstat.audience('','pagesperso-orange.fr');
//]]>
</script>
</body>

</html>
		<?php
	}
}
?>