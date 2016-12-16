// this function is used to owerloading URL by array keys 
function collAjaxAdd(owerloadURL, menu, targetDiv, target, nullTag)
{
	IDs = GetMultiSelectIDs(menu);
	IDsS = IDs.join(',');
	xajax_callAjax(owerloadURL+IDsS, targetDiv, target, nullTag);
}