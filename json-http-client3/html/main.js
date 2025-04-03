//document.getElementById("datetime").innerHTML = "WebSocket is not connected";

var websocket = new WebSocket('ws://'+location.hostname+'/');

// Initialize tree. Displays an empty tree.
var treeWrapper = document.getElementById("tree");
var tree = jsonTree.create({}, treeWrapper);

// Expands all tree nodes (objects or arrays) recursively
function expand_all() {
	tree.expand();
}

// Collapses all tree nodes (objects or arrays) recursively
function collapse_all() {
	tree.collapse();
}

function sendText(name) {
	console.log('sendText');
	var data = {};
	data["id"] = name;
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
}

websocket.onopen = function(evt) {
	console.log('WebSocket connection opened');
	var data = {};
	data["id"] = "init";
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
	//document.getElementById("datetime").innerHTML = "WebSocket is connected!";
}

websocket.onmessage = function(evt) {
	var msg = evt.data;
	console.log("msg=" + msg);
	var values = msg.split('\4'); // \4 is EOT
	//console.log("values=" + values);
	switch(values[0]) {
		case 'HEADER':
			/*
			console.log("HEAD values[1]=" + values[1]);
			var h1 = document.getElementById( 'header' );
			h1.textContent = values[1];
			*/
			break;

		case 'TREE':
			console.log("TREE values[1]=" + values[1]);
			// Convert string to JSON
			var data = JSON.parse(values[1]);
			// Update tree
			tree.loadData(data);
			break;

		case 'ELASPED':
			console.log("ELASPED values[1]=" + values[1]);

			// Get element object
			var elasped = document.getElementById("elasped");
			// Clear object
			elasped.textContent = "";
			// Convert string to JSON
			var data = JSON.parse(values[1]);
			// Create json-tree
			jsonTree.create(data, elasped);
			break;

		default:
			break;
	}
}

websocket.onclose = function(evt) {
	console.log('Websocket connection closed');
	//document.getElementById("datetime").innerHTML = "WebSocket closed";
}

websocket.onerror = function(evt) {
	console.log('Websocket error: ' + evt);
	//document.getElementById("datetime").innerHTML = "WebSocket error????!!!1!!";
}
