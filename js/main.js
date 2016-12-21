$(document).ready(function (e) {
    var scrollTopPosition = $(window).scrollTop();
    var $scrollDown = $('.scroll-down');
    var $navbarBrand = $('.navbar-brand');
    var $fixedMenu = $('.fixed-menu');
    $scrollDown.on('click', function () {
        $('html,body').animate({
            scrollTop: $("#video").offset().top - 50
        }, 'slow');
    });
    if (scrollTopPosition > $(window).height() - 80) {
        $fixedMenu.css({'background': '#fff'});
    }

    if (scrollTopPosition > $(window).height() / 2) {
        $navbarBrand.find('img').css('height', '40px');
        $fixedMenu.css({'padding': '15px 0'})
    } else {
        $navbarBrand.find('img').css('height', '70px');
        $fixedMenu.css({'padding': '40px 0'})
    }


    var lastScrollTop = 0, acting = true;
    $(window).scroll(function (e) {
        var scrollTopPosition = $(this).scrollTop(), targetOpacity;
        var $fixedMenu = $('.fixed-menu');
        var $navbarBrand = $('.navbar-brand');

        if (scrollTopPosition > lastScrollTop) {
            if ( acting && scrollTopPosition < ($(window).height() - 80) ) {
                $('html, body').animate({
                    scrollTop: $("#video").offset().top
                }, "slow", function(){
                    acting = true;
                });
                // player.playVideo();
                acting = false;
            }
        }

        lastScrollTop = scrollTopPosition;

        if ($(window).width() >= 768) {
            $('header').css('top', -scrollTopPosition / 2);
            // $('.title').css('top', '' + 50 - scrollTopPosition / 40 + '%');
        }
        if (scrollTopPosition < $(window).height()) {
            targetOpacity = scrollTopPosition * 100 / $(window).height() / 80;
        } else {
            targetOpacity = 1;
        }
        $fixedMenu.css({
            'background-color': 'rgba(255, 255, 255, ' + targetOpacity + ')'
        });
        if (scrollTopPosition > $(window).height() / 2) {
            $navbarBrand.find('img').css('height', '40px');
            $fixedMenu.css({'padding': '15px 0'})
        } else {
            $navbarBrand.find('img').css('height', '70px');
            $fixedMenu.css({'padding': '40px 0'})
        }
    });
});

$(window).resize(function () {

    if ($(window).width() > 1200) {

    }

    if ($(window).width() > 992) {

    }

    if ($(window).width() > 768) {

    }

    if ($(window).width() > 500) {

    } else {

    }

});

/// Youtube API ///

var tag = document.createElement('script');
tag.src = "https://www.youtube.com/iframe_api";
var firstScriptTag = document.getElementsByTagName('script')[0];
firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);

var player;

function onYouTubeIframeAPIReady() {
    player = new YT.Player('video', {
        videoId: 'omAAUtjuJrw',
        height: 300,
        width: 400,
        playerVars: {
            autoplay: 0,
            controls: 0,
            showinfo: 0,
            rel: 0,
            fs: 1
        }
    });
}

// var p = document.getElementById("player");
// $(p).hide();
//
// var t = document.getElementById("thumbnail");
// t.src = "http://img.youtube.com/vi/AkyQgpqRyBY/0.jpg";
// onPlayerStateChange = function (event) {
//     if (event.data == YT.PlayerState.ENDED) {
//         $('.start-video').fadeIn('normal');
//     }
// };
//
// $(document).on('click', '.start-video', function () {
//     $(this).hide();
//     $("#player").show();
//     $("#thumbnail_container").hide();
//     player.playVideo();
// });

/// END Youtube API ///



