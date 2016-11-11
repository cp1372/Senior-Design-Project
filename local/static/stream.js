var refreshRate = 400;
var algCheckRate = 2000;
var currentAlgorithm = "";


function updateFrames() {
        if (currentAlgorithm == "remote"){
                document.getElementById('stream1').src = '/api/frame/1?rand=' + Math.random();   
        } else if(currentAlgorithm == "colorball") {
                document.getElementById('stream1').src = '/api/frame/1?rand=' + Math.random();
                document.getElementById('stream2').src = '/api/frame/2?rand=' + Math.random();
        }
}

function checkAlg() {
        var algjson;
        var httpAlg = new XMLHttpRequest();
        httpAlg.open('get', '/api/algType');
        httpAlg.onreadystatechange = function() {
                if ( httpAlg.readyState != 4) return;
                if ( httpAlg.status == 200 || xmlHttp.status == 400) {
                        algjson = JSON.parse(httpAlg.response);
                        setStream(algjson);
                        setControls(algjson);
                }
        }
        httpAlg.send(null);
}


function setStream (algjson) {
        if(algjson.algorithm != currentAlgorithm){
                if (algjson.algorithm == "remote"){;
                        var rc = document.getElementById("stream2");
                        rc.style.display = "none";
                } else if (algjson.algorithm == "colorball"){
                        var hsv = document.getElementById("stream2");
                        hsv.style.display = "inline";
                }
        }
}

function setControls (algjson) {
        if(algjson.algorithm != currentAlgorithm){
                if (algjson.algorithm == "remote" || algjson.algorithm == "barcode"){
                        currentAlgorithm = algjson.algorithm;
                        var rc = document.getElementById("rc");
                        rc.style.display = "block";
                        var hsv = document.getElementById("hsv");
                        hsv.style.display = "none"; 
                } else if(algjson.algorithm == "colorball"){
                        currentAlgorithm = algjson.algorithm;
                        var rc = document.getElementById("rc");
                        rc.style.display = "none";
                        var hsv = document.getElementById("hsv");
                        hsv.style.display = "block";                    
                }

        }

}

function sendCommand(com){
        var xhr = new XMLHttpRequest();
        xhr.open('get', '/api/placecom/' + com);
        xhr.setRequestHeader('Content-Type','application/json');
        var request = '{}';
        xhr.send(request);
};

function sendColorCommand(value, type){
        var xhr = new XMLHttpRequest();
        xhr.open('get', '/api/color/' + value + "/" + type);
        xhr.setRequestHeader('Content-Type','application/json');
        var request = '{}';
        xhr.send(request);
};

function getScore() {
	var response = $.get("http://192.168.43.167:8080/api/score");
	return response.responseText;
}


function startTimer() {
        setInterval(updateFrames, refreshRate);
        setInterval(checkAlg, algCheckRate);
}
