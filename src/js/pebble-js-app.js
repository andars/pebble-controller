
// Called when JS is ready
Pebble.addEventListener("ready",
	function(e) {
        console.log("yo yo");
	}

);
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage",
	function(e) {
		console.log(e.payload['message']);
		var req = new XMLHttpRequest();
		var params = 'button='+e.payload['message'];
		req.open('POST', 'http://169.254.254.192:5000/data');
		
		req.onreadystatechange = function (e) {
			console.log(req.readyState);
			console.log(req.status);
			console.log(req.responseText);
		};
		req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		req.setRequestHeader("Content-length", params.length);
		req.setRequestHeader("Connection", "close");
		req.send(params);
	}
);
