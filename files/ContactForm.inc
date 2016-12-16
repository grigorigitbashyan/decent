<!--from validation-->
  <link rel="stylesheet" type="text/css" href="tools/validation/assets/styles.css" />
    <script type="text/javascript" src="tools/validation/jquery.ufvalidator-1.0.4.js"></script>
	<script type="text/javascript">
		$(function () {
			if($('#contactFormBtn'))
			{
				$('#contactFormBtn').formValidator({
					scope		: '#contactForm',
					errorDiv	: '#contactFormErrorDiv'
				});
			}
		});
	</script>
<!--//from validation-->
<?php 
// 
global $lang;
global $catID;
//

global $_POST;
if (isset($_POST["act"]) && ($_POST["act"] == "send"))
{
	print "<p>Your message has been successfully sent.</p>";
	print "<p>Sincerely,</p>";
	print "<p>Harvest Song Ventures</p>";
	
	print "<meta http-equiv=\"refresh\" content=\"3;url=/" . $lang . "/\" />";
	$hdr  = "From:" . "Website" . " <siteius@siteius.com>\n";
	$hdr .= "Content-Type: text/plain; charset=utf-8";
	
	//$emailTo = $_POST["grantee"];
	$emailTo = "arman@arattauna.com";
	$subj = "Harvest Song Ventures Website\n";
	
	$msg =  "\n";
	$msg .= "Contact Person: " . $_POST["name"] . "\n";
	$msg .= "Email Address: " . $_POST["email"] . "\n";
	$msg .= "Subject: " . $_POST["subject"] . "\n";
	$msg .= "-------------------------------------------\n";
	$msg .= "Comments:\n" . $_POST["comments"] . "\n";
	
	@mail($emailTo, $subj, $msg, $hdr);
}
else
{
?>
<h1>Contacts</h1>
<p>Have a question? Contact us direct, or use this form to contact us.</p>
<p>Harvest Song Ventures<br />6 Yennicock Avenue<br />Port Washington, New York 11050<br />Phone: 516-773-7356 Main | Secondary: 516-773-7346<br />Fax: 516-773-7726</p>
<p>E-mail: <a href="mailto:sales@harvestsongventures.com">sales@harvestsongventures.com</a></p>
<p>
<form name="contactForm" id="contactForm" action="<?php $lang . '/' . $catID ?>" method="post" class="form">
<input type="hidden" name="act" value="send">
    <fieldset class="contactForm">
        <legend align="right">&nbsp;</legend>
        <dl>
            <dt><label for="name">Full Name:</label></dt>
            <dd><input type="text" name="name" id="name" class="input req-min" minlength="1" maxlength="128" /></dd>
        </dl>
        <dl>
            <dt><label for="email">E-Mail:</label></dt>
            <dd><input type="text" name="email" id="email" class="input req-email req-same" maxlength="128" /></dd>
        </dl>
        <dl>
            <dt><label for="subject">Subject:</label></dt>
            <dd><input type="text" name="subject" id="subject" class="input req-min" minlength="1" maxlength="128" /></dd>
        </dl>
        <dl>
            <dt><label for="comments">Your Questions:</label></dt>
            <dd><textarea name="comments" id="comments" class="textarea req-min" minlength="1"></textarea></dd>
        </dl>
    </fieldset>
    <fieldset class="contactForm act">
		<dl>
			<dt><label>&nbsp;</label></dt>
			<dd><input type="submit" name="contactFormBtn" id="contactFormBtn" value="Send" /></dd>
			<dd><input type="reset" name="reset" id="reset" value="Reset" /></dd>
		</dl>
    </fieldset>
</form>
</p>
<?php 
}
?>