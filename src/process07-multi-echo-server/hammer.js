var net = require('net');

var PHRASE = "hello world!";
var write = function(socket) {
    socket.write(PHRASE, 'utf8');
}

for (var i = 0; i < 8; i++) {
(function(i) {
    var socket = net.connect(7000, 'localhost', function() {
        socket.on('data', function(reply) {
            console.log(i, reply.toString());
            if (reply.toString().indexOf(PHRASE) != 0)
                console.error("Problem! '" + reply + "'" + "  '" + PHRASE + "'");
            else
                write(socket);
        });
        write(socket);
    });
})(i);
}
