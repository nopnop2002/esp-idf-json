# JSON Example

DeSerialize JOSN message received by HTTP.   
This project gets JSON data from Fake REST Server.   


# Install json-server (Fake REST Server) stable version on your host
```
sudo apt install npm
sudo npm install -g json-server@0.17.4

```

# Create db.json on your host
```
{
  "todos": [
    {
      "id": 1,
      "title": "Hello World!",
      "time":1351824120,
      "data":[1.00, 2.00, 3.00]
    },
    {
      "id": 2,
      "title": "Hello Japan!",
      "time":1351824240,
      "data":[1.11, 2.22]
    }
  ]
}
```

# Start JSON (Fake REST) Server
If the host parameter is not specified, it can only be accessed from local host.   
When accessing from ESP32, host parameter is required.   
```
json-server --watch --host {My_IP_Address} db.json
```

![JSON_Server](https://user-images.githubusercontent.com/6020549/71557207-3320e700-2a86-11ea-9761-823007c4b370.jpg)


# Installation
```
git clone https://github.com/nopnop2002/esp-idf-json
cd esp-idf-json/json-http-client1
idf.py menuconfig
idf.py flash monitor
```

# Configuration
![config-top-http-client1](https://user-images.githubusercontent.com/6020549/167752001-36383f6a-17cd-44e1-9550-49883fc1d408.jpg)
![config-app-http-client1](https://user-images.githubusercontent.com/6020549/167751997-071eec15-36c6-4159-bcd1-850a605277fb.jpg)

The IP address of the REST server needs to be changed according to your environment.   

# Console output
```
[
  {
    "id": 1,
    "title": "Hello World!",
    "time": 1351824120,
    "data": [
      1,
      2,
      3
    ]
  },
  {
    "id": 2,
    "title": "Hello Japan!",
    "time": 1351824240,
    "data": [
      1.11,
      2.22
    ]
  }
]
I (5263) JSON: Deserialize.....
I (5273) JSON: [id]
I (5273) JSON: int=1 double=1.000000
I (5283) JSON: [title]
I (5283) JSON: Hello World!
I (5283) JSON: [time]
I (5283) JSON: int=1351824120 double=1351824120.000000
I (5293) JSON: [data]
I (5293) JSON: int=1 double=1.000000
I (5303) JSON: int=2 double=2.000000
I (5303) JSON: int=3 double=3.000000
I (5313) JSON: [id]
I (5313) JSON: int=2 double=2.000000
I (5313) JSON: [title]
I (5323) JSON: Hello Japan!
I (5323) JSON: [time]
I (5323) JSON: int=1351824240 double=1351824240.000000
I (5333) JSON: [data]
I (5333) JSON: int=1 double=1.110000
I (5343) JSON: int=2 double=2.220000



{
  "id": 2,
  "title": "Hello Japan!",
  "time": 1351824240,
  "data": [
    1.11,
    2.22
  ]
}
I (6487) JSON: Deserialize.....
I (6497) JSON: [id]
I (6497) JSON: int=2 double=2.000000
I (6497) JSON: [title]
I (6497) JSON: Hello Japan!
I (6507) JSON: [time]
I (6507) JSON: int=1351824240 double=1351824240.000000
I (6517) JSON: [data]
I (6517) JSON: int=1 double=1.110000
I (6517) JSON: int=2 double=2.220000

```
