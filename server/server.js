var express = require('express');
var app = express();
var bp = require('body-parser');
var exec = require('child_process').exec;

app.use(bp.urlencoded({extended: true}));

function exec_error(error, stdout, stderr) {
    if (error !== null) console.error("error: " + error);
}

app.post('/data', function(req,res){
    res.status = 200;
    res.writeHead(res.status);
    res.end();

    if (req.body.button == 0) {
        exec("handlers/back.sh", exec_error);
    } else if (req.body.button == 1) {
        exec("handlers/up.sh", exec_error);
    } else if (req.body.button == 2) {
        exec("handlers/select.sh", exec_error);
    } else if (req.body.button == 3) {
        exec("handlers/down.sh", exec_error);
    }
});

var server = app.listen(5000, function() {
    console.log('Listening on port %d', server.address().port);
});

