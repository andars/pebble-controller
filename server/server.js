var express = require('express');
var app = express();
var bp = require('body-parser');

var socket;

app.use(bp.urlencoded());

app.post('/data', function(req,res){
	res.status = 200;
	res.writeHead(res.status);
	res.end();
	if (typeof socket !== 'undefined') {
		socket.send(req.param('button'));
	}
	console.error('handler');
	console.log(req.param('button'));
});
var server = app.listen(5000, function() {
    console.log('Listening on port %d', server.address().port);
});

WebSocketServer = require('ws').Server
, wss = new WebSocketServer({server:server});

wss.on('connection', function(ws) {
    socket = ws;
    ws.on('message', function(message) {
        console.log('received: %s', message);
    });
    ws.send('something');
});
