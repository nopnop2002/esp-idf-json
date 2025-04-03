# JSON Example
Demonstration of displaying a JSON message in table format.   
An array JSON message is well suited for table representation.   
This project gets JSON data from Fake REST Server.   
![Image](https://github.com/user-attachments/assets/7d1a49f3-8643-4420-85a6-1fcbf39119db)

# Install json-server (Fake REST Server) stable version on your host
```
sudo apt install npm
sudo npm install -g json-server@0.17.4

```

# Start json-server (Fake REST Server)
If the host parameter is not specified, it can only be accessed from local host.   
When accessing from ESP32, host parameter is required.   
```
cd esp-idf-json/json-http-client4
json-server --watch --host {My_IP_Address} ./db.json
```
![Image](https://github.com/user-attachments/assets/583585a4-d810-4a51-9602-f08af65d9399)


# Installation
```
git clone https://github.com/nopnop2002/esp-idf-json
cd esp-idf-json/json-http-client4
idf.py menuconfig
idf.py flash monitor
```

# Configuration
![Image](https://github.com/user-attachments/assets/d2c27b89-e715-4750-913c-2161ae0a4531)

The IP address in the URL of REST Server is the IP address of the host where you installed json-server.   
The IP address of the REST server needs to be changed according to your environment.   
![Image](https://github.com/user-attachments/assets/5490ff79-4517-491f-8ffe-513c9017b703)

# How to use
Open a browser and enter the IP address of the esp32 in the browser's address bar.   
The ESP32 retrieves data from the Fake REST Server and displays it in tree format.   
![Image](https://github.com/user-attachments/assets/7d1a49f3-8643-4420-85a6-1fcbf39119db)

Instead of IP addresses, you can use mDNS hostnames.   
![Image](https://github.com/user-attachments/assets/c6033956-9cd8-4054-83ce-861aa3542779)

# Modify JSON data
Modify the db.json file.   
The changes will be reflected immediately.   
There is no need to restart json-server (Fake REST Server).   
```
$ cat db.json
{
        "array": [
                ["id","type","name","batter"],
                ["0001","donut","Cake","Regulard"],
                ["0001","donut","Cake","Chocolate"],
                ["0001","donut","Cake","Blueberry"],
                ["0001","donut","Cake","Devil's Food"],
                ["0002","donut","Raised","Regular"],
                ["0003","donut","Old Fashioned","Regular"],
                ["0003","donut","Old Fashioned","Chocolate"],
                ["0004","bar","Bar","Regular"],
                ["0005","twist","Twist","Regular"],
                ["0006","filled","Filled","Regular"]
        ]

}
```
