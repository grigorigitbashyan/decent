// trim function
function trim(str) {
	return str.replace(/^\s+|\s+$/, '');
}

// date validation
function isDate(str) {
	if (/d{4}-d{2}-d{2}/.test(str))
		return true;
}

// return true if valu is empoty, ather ways return flase
function isEmpty(str) {
	s = trim(str);

	if (s == null || s.length == 0)
		return true;

	return !/\S/.test(s);
}
function isMail(str) // fld is validation input fild
{
	var tfld = trim(str);
	var emailFilter = /^[^@]+@[^@.]+\.[^@]*\w\w$/;
	var illegalChars = /[\(\)\<\>\,\;\:\\\"\[\]]/;

	if (tfld == "") {
		return false;
	} else if (!emailFilter.test(tfld)) {
		return false;
	} else if (tfld.match(illegalChars)) {
		return false;
	} else {
		return true;
	}

	return false;
}

function isRadioeChecked(formName, elementName) {
	formEl = document[formName];
	langht = formEl[elementName].length;

	checked = false;
	if (langht > 0) {
		for ( var i = 0; i < langht; i++) {
			if (formEl[elementName][i].checked == true) {
				checked = true;
				break;
			}
		}
	} else if (formEl[elementName]) {
		checked = formEl[elementName].checked;
	}

	return checked;
}
// this function return selected IDs of multiple select
// if join has been set then join array
function GetMultiSelectIDs(menu) {
	var i, IDs = [];
	for (i = 0; i < menu.options.length; i++) {
		if (menu.options[i].selected) {
			IDs.push(menu.options[i].value);
		}
	}

	return IDs;
}

function isZip(zipV) {
	var reg = new RegExp("^\\d{5}$");

	if (reg.test(zipV)) {
		return true;
	}

	return false;
}
function isPhone(phoneV) {
	reg = new RegExp("^\\d{3}\-\\d{3}\-\\d{4}$");
	if (reg.test(phoneV))
		return true;

	return false;
}
// //
// / this function validat email, number, zip code, phone and not empty values
function TextValid(value, type) {
	switch (type) {
	case 'email':
		return isMail(value);
		break;
	case 'number':
		reg = new RegExp("^\\d{1,7}$");
		return reg.test(value);
		break;
	case 'zip':
		return isZip(value);
		break;
	case 'phone':
		return isPhone(value);
		break;
	default:
		return !isEmpty(value);
	}
}
// /
function TextFildValid(fild, type, tag) {
	var testFild = document.getElementById(fild);
	var tagHTML = (tag) ? document.getElementById(tag) : null;

	if (TextValid(testFild.value, type)) {
		if (tagHTML)
			tagHTML.className = "wsuAlertDeactivated";
		return true;
	} else {
		if (tagHTML)
			tagHTML.className = "wsuAlertActive";
		return false;
	}
}
function RadioFildValid(fild, formName, tag) {
	var tagHTML = document.getElementById(tag);

	res = isRadioeChecked(formName, fild);
	if (res) {
		if (tag)
			tagHTML.className = "wsuAlertDeactivated";
		return res;
	} else {
		if (tag)
			tagHTML.className = "wsuAlertActive";
		return false;
	}
}
function SelectFildValid(fild, tag) {
	var selectFild = document.getElementById(fild);

	index = selectFild.selectedIndex;
	value = selectFild.options[index].value;
	if (!value) {
		if (tag) {
			SetTagClass(tag, "wsuAlertActive");
		}
		return false;
	} else {
		if (tag) {
			SetTagClass(tag, "wsuAlertDeactivated");
		}
		return value;
	}
}
function SerErrorText(res, message, errorDiv) {
	// get error tag
	errorDiv = typeof (errorDiv) != 'undefined' ? errorDiv : 'errorDiv';

	var tag = document.getElementById(errorDiv);
	if (tag) {
		if (res)
			tag.innerHTML = "&nbsp;";
		else
			tag.innerHTML = message;
	}
}
function SetTagClass(tag, className) {
	var tagHTML = document.getElementById(tag);

	if (tagHTML)
		tagHTML.className = className;
}

// chake all chakebixes or unchake

function chekeBoxes(formName, elementName, isChake) {
	formEl = document[formName];
	langht = formEl[elementName].length;

	if (langht > 0) {
		for ( var i = 0; i < langht; i++) {
			var e = formEl[elementName][i];
			if (!e.disabled) {
				e.checked = isChake;
			}
		}
	} else if (formEl[elementName]) {
		var e = formEl[elementName];
		if (!e.disabled) {
			e.checked = isChake;
		}
	}

}
// single field form validation with alert

function SingleForm(filedName, message) {
	res = TextFildValid(filedName);

	if (res == false) {
		alert(message);
	}

	return res;
}

// this function checks are the check boxes has been checked
// and display error message to check
function ChecksForm(formName, radioName, message) {
	if (!isRadioeChecked(formName, radioName)) {
		alert(message);
		return false;
	}

	return true;
}

// this function checks are the check boxes has been checked
// and display error message to check
function ChecksFormDelete(formName, radioName, checkMsg, deleteMsg) {
	if (!isRadioeChecked(formName, radioName)) {
		alert(checkMsg);
		return false;
	}

	return confirm(deleteMsg);
}
function isEqual(field1, field2, field1HTML, field2HTML) {
	var field1Value = document.getElementById(field1);
	var field2Value = document.getElementById(field2);

	var res = (field1Value.value == field2Value.value);

	if (!res) {
		var field1HTML = document.getElementById(field1HTML);
		if (field1HTML)
			field1HTML.className = "wsuAlertActive";

		var field2HTML = document.getElementById(field2HTML);
		if (field2HTML)
			field2HTML.className = "wsuAlertActive";
	}

	return res;
}
function IsChecked(fild, form, tag) {
	var testFild = document.forms[form].agg;
	var tagHTML = document.getElementById(tag);

	if (testFild.checked) {
		if (tagHTML)
			tagHTML.className = "wsuAlertDeactivated";
		return true;
	} else {
		if (tagHTML)
			tagHTML.className = "wsuAlertActive";
		return false;
	}
}
function GetFieldValueLenght(field) {
	var field = document.getElementById(field);

	if (field && field.value.length) {
		return field.value.length;
	}

	return 0;
}