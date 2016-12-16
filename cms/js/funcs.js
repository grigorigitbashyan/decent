// JavaScript Document
function ToggleArchaicTools(paremtid)
{
	var ArchaicTools = document.getElementById (paremtid);
	lastDirectory = paremtid;
	if (ArchaicTools) ArchaicTools.style.display = ArchaicTools.style.display == 'none' ? 'block' : 'none';
}

// java script for validatin
// 1. empoty
// 2. date
// 3. email

//////////////////////////////////////////////////////////////////////////////////
// trim function
function trim(var str)
{
  return str.replace(/^\s+|\s+$/, '');
} 

// date validation
function isDate(var str)
{
	if(/d{4}-d{2}-d{2}/.test(str))
		return true;
}


// return true if valu is empoty, ather ways return flase
function isEmpty(var str)
{
	s = trim(str);
	
	if(s == null || s.length == 0)
		return true;
		
	return !/\S/.test(s);
}

// e-mail validation

function isMail(var str) // fld is validation input fild
{
    var tfld = trim(str);                        // value of field with whitespace trimmed off
    var emailFilter = /^[^@]+@[^@.]+\.[^@]*\w\w$/ ;
    var illegalChars= /[\(\)\<\>\,\;\:\\\"\[\]]/ ;
   
    if (tfld == "") {
        alert("You didn't enter an email address.\n");
        return false;
    } else if (!emailFilter.test(tfld)) {              //test email for illegal characters
        alert("Please enter a valid email address.\n");
        return false;
    } else if (tfld.match(illegalChars)) {

        alert("The email address contains illegal characters.\n");
        return false;
    } else {
        return true;
    }
    
    return false;
}
//
