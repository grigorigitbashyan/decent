<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
	<title>DG Crop</title>	
	<link rel="stylesheet" href="css/dg-crop.css" type="text/css"></link>
	<script type="text/javascript" src="js/external/mootools.js"></script>
	<script type="text/javascript" src="js/effects/effect-element.js"></script>
	<script type="text/javascript" src="js/effects/movable.js"></script>
	<script type="text/javascript" src="js/effects/resizable.js"></script>
	<script type="text/javascript" src="js/dg-crop.js"></script>
	<script type="text/javascript">

	var formElements = null;
	function setElements() {
		if(!formElements) {
			formElements = {
				left : $('left'),
				top : $('top'),
				width : $('width'),
				height : $('height')	
			}
		};	
	}
	function updateForm(coordinates) {
		this.setElements();
		formElements.width.value = coordinates.width;			
		formElements.height.value = coordinates.height;			
		formElements.left.value = coordinates.left;			
		formElements.top.value = coordinates.top;		
	}
	function clearForm() {
		this.setElements();
		formElements.width.value = '...';
		formElements.height.value = '...';			
		formElements.left.value = '...';			
		formElements.top.value = '...';			
	}
	
	var cropObject;
	function initCrop() {
		cropObject = new DG.ImageCrop('cropImage', {	
			lazyShadows : true,		
			resizeConfig: {
				preserveAspectRatio : false,
				minWidth : 40,
				minHeight: 40
			},
			moveConfig : {
				keyNavEnabled : true	
			},
			initialCoordinates : {
				left : 0,
				top : 0,
				width: 400,
				height:300
			},			
			originalCoordinates : {
				width: 	667,
				height : 1000				
			},
			previewCoordinates : {
				width: 	667,
				height : 1000
			},
			listeners : {
				render : function() {
					updateForm(this.getCoordinates());	
				},
				crop : function() {
					updateForm(this.getCoordinates());	
				}									
			}			
		});	
	
	}
	
	</script>
	<style type="text/css">
	.borderDiv{
		border-top:1px solid #555;
		border-left:1px solid #555;
		border-right:1px solid #CCC;
		border-bottom: 1px solid #CCC;
		width:667px;
		float:left;
	}
	p{
		margin:0px;
	}
	</style>
</head>
<body>
<div style="width:1000px">
	<div style="float:left;width:150px">
		<table>
		<form action="image.php" method="POST">
			<tr><td>Left:</td><td><input id="left" name="left" size="4"></td></tr>
			<tr><td>Top:</td><td><input id="top" name="top" size="4"></td></tr>
			<tr><td>Width:</td><td><input id="width" name="width" size="4"></td></tr>
			<tr><td>Height:</td><td><input id="height" name="height" size="4"></td></tr>
			<tr><td colspan="2"><input type="submit" value="Crop"></td></tr>
		</form>
		</table>
		<p><b>Example of commands</b></p>
		<p><a href="#" onclick="cropObject.maximize();return false">Maximize</a></p>	
		<p><a href="#" onclick="cropObject.minimize();return false">Minimize</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('nw');return false">alignTo('nw')</a></p>
		<p><a href="#" onclick="cropObject.alignTo('n');return false">alignTo('n')</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('ne');return false">alignTo('ne')</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('w');return false">alignTo('w')</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('e');return false">alignTo('e')</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('sw');return false">alignTo('sw')</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('s');return false">alignTo('s')</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('se');return false">alignTo('se')</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('vcenter');return false">alignTo('vcenter')</a></p>	
		<p><a href="#" onclick="cropObject.alignTo('hcenter');return false">alignTo('hcenter')</a></p>	
	</div>
	<div class="borderDiv">
		<div style="position:relative;width:667px;height:1000px" id="cropImage"><img src="sex.jpg"></div>
	</div>
</div>
<div id="debug"></div>
<script type="text/javascript">
initCrop();
</script>

</body>