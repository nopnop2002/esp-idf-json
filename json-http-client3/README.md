# JSON Example
Demonstration of displaying a JSON message in tree format.   
Nested JSON messages lend themselves to a Tree representation.   
I used [this](https://github.com/summerstyle/jsonTreeViewer) library.   
This project gets JSON data from Fake REST Server.   
![Image](https://github.com/user-attachments/assets/a23bdbe5-1e35-4706-892a-4bb9c2e8df5d)

# Install json-server (Fake REST Server) stable version on your host
```
sudo apt install npm
sudo npm install -g json-server@0.17.4

```

# Start json-server (Fake REST Server)
If the host parameter is not specified, it can only be accessed from local host.   
When accessing from ESP32, host parameter is required.   
```
cd esp-idf-json/json-http-client3
json-server --watch --host {My_IP_Address} ./db.json
```
![Image](https://github.com/user-attachments/assets/fc181d30-2b98-4199-8983-4359bf14239e)


# Installation
```
git clone https://github.com/nopnop2002/esp-idf-json
cd esp-idf-json/json-http-client3
idf.py menuconfig
idf.py flash monitor
```

# Configuration
![Image](https://github.com/user-attachments/assets/069b30d7-a27c-4a22-a2c6-29d6d37ff1d3)

The IP address in the URL of REST Server is the IP address of the host where you installed json-server.   
The IP address of the REST server needs to be changed according to your environment.   
![Image](https://github.com/user-attachments/assets/743c0249-a3d2-4fb5-84a1-6506db84c0af)


# How to use
Open a browser and enter the IP address of the esp32 in the browser's address bar.   
The ESP32 retrieves data from the Fake REST Server and displays it in tree format.   
![Image](https://github.com/user-attachments/assets/562a3c75-8297-413b-8b07-58749c950af2)

Instead of IP addresses, you can use mDNS hostnames.
![Image](https://github.com/user-attachments/assets/22396443-a204-4733-a979-ae154623f7da)

# Modify JSON data
Edit db.json file.
```
$ cat db.json
{
        "tree": {
                "firstName": "John",
                "lastName": "Smith",
                "isAlive": true,
                "age": 25,
                "company": null,
                "height_cm": 167.64,
                "address": {
                        "streetAddress": "21 2nd Street",
                        "city": "New York",
                        "state": "NY",
                        "postalCode": "10021-3100"
                },
                "phoneNumbers": [
                        {
                                "type": "home",
                                "number": "212 555-1234"
                        },
                        {
                                "type": "fax",
                                "number": "646 555-4567"
                        }
                ]
        }

}
```
