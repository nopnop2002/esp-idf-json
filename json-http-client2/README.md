# JSON Example

DeSerialize JOSN message received by HTTP.   
This project gets JSON data from https://weatherdbi.herokuapp.com/   

# Installation

```
git clone https://github.com/nopnop2002/esp-idf-json
cd esp-idf-json/json-http-client2
chmod 777 getpem.sh
./getpem.sh
idf.py menuconfig
idf.py flash monitor
```

# Configuration
![config-top-http-client2](https://user-images.githubusercontent.com/6020549/167752797-15918c8b-996b-4fa8-84e3-8f3fcff098f8.jpg)
![config-app-http-client2](https://user-images.githubusercontent.com/6020549/171975938-864cc437-046d-4b8b-950f-6746f07db8c6.jpg)


# Console output   
This is the weather forecast for Tokyo.
```
I (12392) JSON: Deserialize.....
I (12392) JSON: [region]
I (12392) JSON: Tokyo, Japan
I (12392) JSON: [currentConditions]
I (12392) JSON: [dayhour]
I (12392) JSON: Saturday 11:00 AM
I (12402) JSON: [temp]
I (12402) JSON: [c]
I (12402) JSON: int=25 double=25.000000
I (12412) JSON: [f]
I (12412) JSON: int=77 double=77.000000
I (12422) JSON: [precip]
I (12422) JSON: 10%
I (12422) JSON: [humidity]
I (12432) JSON: 39%
I (12432) JSON: [wind]
I (12432) JSON: [km]
I (12432) JSON: int=3 double=3.000000
I (12442) JSON: [mile]
I (12442) JSON: int=2 double=2.000000
I (12452) JSON: [iconURL]
I (12452) JSON: https://ssl.gstatic.com/onebox/weather/64/sunny.png
I (12462) JSON: [comment]
I (12462) JSON: Clear
I (12462) JSON: [next_days]
I (12472) JSON: [day]
I (12472) JSON: Saturday
I (12472) JSON: [comment]
I (12482) JSON: Clear with periodic clouds
I (12482) JSON: [max_temp]
I (12482) JSON: [c]
I (12492) JSON: int=27 double=27.000000
I (12492) JSON: [f]
I (12492) JSON: int=81 double=81.000000
I (12502) JSON: [min_temp]
I (12502) JSON: [c]
I (12502) JSON: int=18 double=18.000000
I (12512) JSON: [f]
I (12512) JSON: int=64 double=64.000000
I (12522) JSON: [iconURL]
I (12522) JSON: https://ssl.gstatic.com/onebox/weather/48/sunny_s_cloudy.png
I (12532) JSON: [day]
I (12532) JSON: Sunday
I (12532) JSON: [comment]
I (12542) JSON: Cloudy with periodic rain
I (12542) JSON: [max_temp]
I (12542) JSON: [c]
I (12552) JSON: int=26 double=26.000000
I (12552) JSON: [f]
I (12552) JSON: int=79 double=79.000000
I (12562) JSON: [min_temp]
I (12562) JSON: [c]
I (12572) JSON: int=18 double=18.000000
I (12572) JSON: [f]
I (12572) JSON: int=64 double=64.000000
I (12582) JSON: [iconURL]
I (12582) JSON: https://ssl.gstatic.com/onebox/weather/48/cloudy_s_rain.png
I (12592) JSON: [day]
I (12592) JSON: Monday
I (12592) JSON: [comment]
I (12602) JSON: Rain
I (12602) JSON: [max_temp]
I (12602) JSON: [c]
I (12612) JSON: int=20 double=20.000000
I (12612) JSON: [f]
I (12612) JSON: int=68 double=68.000000
I (12622) JSON: [min_temp]
I (12622) JSON: [c]
I (12622) JSON: int=16 double=16.000000
I (12632) JSON: [f]
I (12632) JSON: int=61 double=61.000000
I (12642) JSON: [iconURL]
I (12642) JSON: https://ssl.gstatic.com/onebox/weather/48/rain.png
I (12652) JSON: [day]
I (12652) JSON: Tuesday
I (12652) JSON: [comment]
I (12662) JSON: Cloudy with brief rain
I (12662) JSON: [max_temp]
I (12662) JSON: [c]
I (12672) JSON: int=20 double=20.000000
I (12672) JSON: [f]
I (12672) JSON: int=68 double=68.000000
I (12682) JSON: [min_temp]
I (12682) JSON: [c]
I (12682) JSON: int=14 double=14.000000
I (12692) JSON: [f]
I (12692) JSON: int=57 double=57.000000
I (12702) JSON: [iconURL]
I (12702) JSON: https://ssl.gstatic.com/onebox/weather/48/cloudy_s_rain.png
I (12712) JSON: [day]
I (12712) JSON: Wednesday
I (12712) JSON: [comment]
I (12722) JSON: Cloudy
I (12722) JSON: [max_temp]
I (12722) JSON: [c]
I (12732) JSON: int=20 double=20.000000
I (12732) JSON: [f]
I (12732) JSON: int=68 double=68.000000
I (12742) JSON: [min_temp]
I (12742) JSON: [c]
I (12742) JSON: int=14 double=14.000000
I (12752) JSON: [f]
I (12752) JSON: int=57 double=57.000000
I (12762) JSON: [iconURL]
I (12762) JSON: https://ssl.gstatic.com/onebox/weather/48/cloudy.png
I (12772) JSON: [day]
I (12772) JSON: Thursday
I (12772) JSON: [comment]
I (12782) JSON: Cloudy
I (12782) JSON: [max_temp]
I (12782) JSON: [c]
I (12782) JSON: int=21 double=21.000000
I (12792) JSON: [f]
I (12792) JSON: int=70 double=70.000000
I (12802) JSON: [min_temp]
I (12802) JSON: [c]
I (12802) JSON: int=14 double=14.000000
I (12812) JSON: [f]
I (12812) JSON: int=57 double=57.000000
I (12812) JSON: [iconURL]
I (12822) JSON: https://ssl.gstatic.com/onebox/weather/48/cloudy.png
I (12822) JSON: [day]
I (12832) JSON: Friday
I (12832) JSON: [comment]
I (12832) JSON: Cloudy
I (12842) JSON: [max_temp]
I (12842) JSON: [c]
I (12842) JSON: int=24 double=24.000000
I (12852) JSON: [f]
I (12852) JSON: int=75 double=75.000000
I (12852) JSON: [min_temp]
I (12862) JSON: [c]
I (12862) JSON: int=15 double=15.000000
I (12872) JSON: [f]
I (12872) JSON: int=59 double=59.000000
I (12872) JSON: [iconURL]
I (12882) JSON: https://ssl.gstatic.com/onebox/weather/48/cloudy.png
I (12882) JSON: [day]
I (12892) JSON: Saturday
I (12892) JSON: [comment]
I (12892) JSON: Cloudy
I (12902) JSON: [max_temp]
I (12902) JSON: [c]
I (12902) JSON: int=23 double=23.000000
I (12912) JSON: [f]
I (12912) JSON: int=73 double=73.000000
I (12912) JSON: [min_temp]
I (12922) JSON: [c]
I (12922) JSON: int=16 double=16.000000
I (12922) JSON: [f]
I (12932) JSON: int=61 double=61.000000
I (12932) JSON: [iconURL]
I (12932) JSON: https://ssl.gstatic.com/onebox/weather/48/cloudy.png
I (12942) JSON: [contact_author]
I (12952) JSON: [email]
I (12952) JSON: communication.with.users@gmail.com
I (12952) JSON: [auth_note]
I (12962) JSON: Mail me for feature requests, improvement, bug, help, ect... Please tell me if you want me to provide any other free easy-to-use API services
I (12972) JSON: [data_source]
I (12982) JSON: https://www.google.com/search?lr=lang_en&q=weather+in+tokyo
```
