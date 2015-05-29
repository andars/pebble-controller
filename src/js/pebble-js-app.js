var ip = '192.168.1.0';

// Called when JS is ready
Pebble.addEventListener("ready",
    function(e) {
        console.log("yo yo");
    }

);

// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage",
    function(e) {
        if (e.payload['ip'] !== undefined) {
            var ip_arr = e.payload['ip'];
            ip = '' + ip_arr[0] + '.' + ip_arr[1] + '.' + ip_arr[2] + '.' + ip_arr[3];
        }
        console.log(ip);

        console.log(e.payload['message']);
        var req = new XMLHttpRequest();
        var params = 'button='+e.payload['message'];
        req.open('POST', 'http://' + ip + ':5000/data');

        req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        req.setRequestHeader("Content-length", params.length);
        req.setRequestHeader("Connection", "close");
        req.send(params);
    }
);
