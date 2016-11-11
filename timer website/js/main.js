var min = 0;
var sec = 30;

function getScore() {			
	$.get("http://192.168.43.167:8080/api/score", function(data) {
		$("div.scoreContainer div.score h1 span").text("Score: " + data);
	});
}

function sendTimeUpCommand() {
	$.get("http://192.168.43.167:8080/api/placecom/timeup");
	$.get("http://192.168.43.81:8080/api/placecom/timeup");
}

function updateTimer() {
	decrementTimer();
	var minString = min > 9 ? min : "0" + min;
	var secString = sec > 9 ? sec : "0" + sec;
	var timeString = minString + ":" + secString;
	$("div.timeContainer div.time h1 span").text(timeString);
}

function startClick() {
	setInterval(updateTimer, 1000);
	setInterval(getScore, 500);
	$.get("http://192.168.43.167:8080/api/placecom/init");
	$.get("http://192.168.43.81:8080/api/placecom/init");
}

function decrementTimer() {
	if (min === 0 && sec === 0) {
		sendTimeUpCommand();
		return;
	}
	
	if (sec === 0) {
		min--;
		sec = 59;
	}
	else {
		sec--;
	}
}