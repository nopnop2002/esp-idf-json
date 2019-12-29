# JSON Example

DeSerialize JOSN message received by HTTP.   

## Install JSON (Fake REST) Server on your host
```
$ sudo apt-get install npm

$ sudo npm install -g json-server
```

## Create db.json
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

## Start JSON (Fake REST) Server
```
$ json-server --watch --host {My_IP_Address} db.json
```

![JSON_Server](https://user-images.githubusercontent.com/6020549/71557207-3320e700-2a86-11ea-9761-823007c4b370.jpg)


## Build ESP-IDF Project
```
$ make menuconfig
$ make flash monitor
```

![menuconfig-1](https://user-images.githubusercontent.com/6020549/71557211-3d42e580-2a86-11ea-804b-f25373ce589f.jpg)

![menuconfig-2](https://user-images.githubusercontent.com/6020549/71557214-416f0300-2a86-11ea-9574-051a7862d4e2.jpg)

## Console output
```
I (5253) JSON: buffer=
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
I (5343) JSON: HTTP_EVENT_DISCONNECTED
```
