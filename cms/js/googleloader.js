  google.load("language", "1");

    var useEdit = null;
    google.setOnLoadCallback(init);

    
    function init() {
      var src = document.getElementById('src');
      var dst = document.getElementById('dst');
      var i=0;
      for (l in google.language.Languages) {
        var lng = l.toLowerCase();
        var lngCode = google.language.Languages[l];
        if (google.language.isTranslatable(lngCode)) {
          src.options.add(new Option(lng, lngCode));
          dst.options.add(new Option(lng, lngCode));
        }
      }

      google.language.getBranding('branding', { type : 'vertical' });

      submitChange(null);
    }

    function submitChange(useEditValue) {
      useEdit = useEditValue;
      var value = 'to';
      
      if(useEdit != null)
      {
    	  value = tinyMCE.get(useEdit).getContent();;
    	  
      }

      var src = document.getElementById('src').value;
      var dest = document.getElementById('dst').value;
      google.language.translate(value, src, dest, translateResult);
      return false;
    }

    function translateResult(result) {
        if(useEdit != null)
        {
	      if (result.translation) {
	        var str = result.translation;
	        tinyMCE.get(useEdit).setContent(str);
	      } else {
	        alert('Error Translating');
	      }
        }
    }