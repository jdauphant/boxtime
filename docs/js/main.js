var c = 0;
var nPage = $('div', $('#page-container')).length;

$('#prev, #next').click(function(e){
  e.preventDefault();
  var dir = this.id=='next' ? c++ : c-- ;
  c = c<0 ? nPage-1 : c%nPage;
  $('#page-container').stop().animate({ top: -c*$(window).height() },800);  
});

