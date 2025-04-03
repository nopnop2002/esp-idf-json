//document.getElementById("datetime").innerHTML = "WebSocket is not connected";

var websocket = new WebSocket('ws://'+location.hostname+'/');

// https://gist.github.com/pistatium/5963229
var tableBuilder = function(header, data) {
	var html = '<table class="table"><thead><tr>';
	//console.log('header=', header);
	for (var key in header) {
		//console.log('key=', key);
		html += '<th id="label_' + key + '">' + header[key] + '</th>';
	}
	html += '</tr></thead><tbody>';
	for (var i in data) {
		//console.log('i=', i);
		var d = data[i];
		html += '<tr>';
		for (var key in header) {
			//console.log('key=', key);
			//console.log('d[key]=', d[key]);
			if(d[key]) {
				html += '<td>' + d[key] + '</td>';
			} else {
				// 対応するキーがない場合
				html += '<td>--</td>';
			}
		}
		html += '</tr></tbody>';
	}
	html += '</table>';
	return html;
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
			console.log("HEAD values[1]=" + values[1]);
			var h1 = document.getElementById( 'header' );
			h1.textContent = values[1];
			break;

		case 'TABLE1':
			console.log("TABLE1 values[1]=" + values[1]);
			console.log("TABLE1 values[2]=" + values[2]);
			// Convert string to JSON
			var table_header = JSON.parse(values[1]);
			var table_data = JSON.parse(values[2]);

			// Test data
			// Define table header
			var table_header_test = {
				'id'		 : '#',
				'title'		 : 'タイトル',
				'updated_at' : '更新日',
			};

			// Data to display in table
			var table_data_test = [
				{'id':1, 'created_at':'2013/05/01', 'updated_at':'2013/07/01','title':'記事1'},
				{'id':2, 'created_at':'2013/06/01', 'updated_at':'2013/07/02','title':'記事2'},
				{'id':3, 'created_at':'2013/07/01','title':'記事3'},
			];

			//var html = tableBuilder(table_header_test, table_data_test);
			var html = tableBuilder(table_header, table_data);
			console.log("TABLE1 html=" + html);
			var table1 = document.getElementById("table1");
			table1.innerHTML = html;
			break;

		case 'TABLE2':
			console.log("TABLE2 values[1]=" + values[1]);
			console.log("TABLE2 values[2]=" + values[2]);
			// Convert string to JSON
			var table_header = JSON.parse(values[1]);
			var table_data = JSON.parse(values[2]);

			var html = tableBuilder(table_header, table_data);
			console.log("TABLE2 html=" + html);
			var table2 = document.getElementById("table2");
			table2.innerHTML = html;
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
