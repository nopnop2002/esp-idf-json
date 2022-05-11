# JSON Example

DeSerialize JOSN message received by HTTP.   
This project gets JSON data from https://www.metaweather.com/.   

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
![config-app-http-client2](https://user-images.githubusercontent.com/6020549/167752802-98adb244-0c82-40cb-a1e9-d4b0e3333ad5.jpg)

WOEID(Where On Earth IDentifire) can be changed according to the area where you live.   

# How to get WOEID
How to use location search:   
https://www.metaweather.com/api/#locationsearch.   

Cities containing 'san':   
https://www.metaweather.com/api/location/search/?query=san

```
[
    {
        "woeid": 2487956,
        "latt_long": "37.777119, -122.41964",
        "location_type": "City",
        "title": "San Francisco"
    },
    {
        "woeid": 2487889,
        "latt_long": "32.715691,-117.161720",
        "location_type": "City",
        "title": "San Diego"
    },
    {
        "woeid": 2488042,
        "latt_long": "37.338581,-121.885567",
        "location_type": "City",
        "title": "San Jose"
    },
    {
        "woeid": 2487796,
        "latt_long": "29.424580,-98.494614",
        "location_type": "City",
        "title": "San Antonio"
    },
    {
        "woeid": 2488853,
        "latt_long": "36.974018,-122.030952",
        "location_type": "City",
        "title": "Santa Cruz"
    },
    {
        "woeid": 349859,
        "latt_long": "-33.463039,-70.647942",
        "location_type": "City",
        "title": "Santiago"
    },
    {
        "woeid": 56558361,
        "latt_long": "36.406651,25.456530",
        "location_type": "City",
        "title": "Santorini"
    },
    {
        "woeid": 773964,
        "latt_long": "43.461498,-3.810010",
        "location_type": "City",
        "title": "Santander"
    },
    {
        "woeid": 1132447,
        "latt_long": "35.170429,128.999481",
        "location_type": "City",
        "title": "Busan"
    },
    {
        "woeid": 773692,
        "latt_long": "28.46163,-16.267059",
        "location_type": "City",
        "title": "Santa Cruz de Tenerife"
    },
    {
        "woeid": 2488867,
        "latt_long": "35.666431,-105.972572",
        "location_type": "City",
        "title": "Santa Fe"
    }
]
```

Cities containing 'kyo':  
https://www.metaweather.com/api/location/search/?query=kyo

```
[
    {
        "woeid": 1118370,
        "latt_long": "35.670479,139.740921",
        "location_type": "City",
        "title": "Tokyo"
    },
    {
        "woeid": 15015372,
        "latt_long": "35.098129,135.718933",
        "location_type": "City",
        "title": "Kyoto"
    }
]
```

WOEID of TOKYO is 1118370.   
You can know the weather forecast of TOKYO.   
https://www.metaweather.com/api/location/1118370/   


# Console output   
This is the weather forecast for Tokyo.
```
I (11104) JSON: Deserialize.....
I (11114) JSON: [consolidated_weather]
I (11114) JSON: [id]
I (11114) JSON: int=2147483647 double=4572727370317824.000000
I (11114) JSON: [weather_state_name]
I (11114) JSON: Showers
I (11124) JSON: [weather_state_abbr]
I (11124) JSON: s
I (11124) JSON: [wind_direction_compass]
I (11134) JSON: ESE
I (11134) JSON: [created]
I (11144) JSON: 2022-05-11T01:03:46.644846Z
I (11144) JSON: [applicable_date]
I (11144) JSON: 2022-05-11
I (11154) JSON: [min_temp]
I (11154) JSON: int=14 double=14.885000
I (11164) JSON: [max_temp]
I (11164) JSON: int=22 double=22.690000
I (11164) JSON: [the_temp]
I (11174) JSON: int=21 double=21.500000
I (11174) JSON: [wind_speed]
I (11184) JSON: int=5 double=5.979915
I (11184) JSON: [wind_direction]
I (11184) JSON: int=101 double=101.442735
I (11194) JSON: [air_pressure]
I (11194) JSON: int=1018 double=1018.500000
I (11204) JSON: [humidity]
I (11204) JSON: int=56 double=56.000000
I (11204) JSON: [visibility]
I (11214) JSON: int=13 double=13.413540
I (11214) JSON: [predictability]
I (11224) JSON: int=73 double=73.000000
I (11224) JSON: [id]
I (11224) JSON: int=2147483647 double=4506117662048256.000000
I (11234) JSON: [weather_state_name]
I (11234) JSON: Light Rain
I (11244) JSON: [weather_state_abbr]
I (11244) JSON: lr
I (11244) JSON: [wind_direction_compass]
I (11254) JSON: SSE
I (11254) JSON: [created]
I (11264) JSON: 2022-05-11T01:03:49.641782Z
I (11264) JSON: [applicable_date]
I (11264) JSON: 2022-05-12
I (11274) JSON: [min_temp]
I (11274) JSON: int=15 double=15.900000
I (11284) JSON: [max_temp]
I (11284) JSON: int=21 double=21.315000
I (11284) JSON: [the_temp]
I (11294) JSON: int=20 double=20.420000
I (11294) JSON: [wind_speed]
I (11304) JSON: int=4 double=4.689001
I (11304) JSON: [wind_direction]
I (11304) JSON: int=150 double=150.281423
I (11314) JSON: [air_pressure]
I (11314) JSON: int=1014 double=1014.500000
I (11324) JSON: [humidity]
I (11324) JSON: int=76 double=76.000000
I (11324) JSON: [visibility]
I (11334) JSON: int=12 double=12.451036
I (11334) JSON: [predictability]
I (11344) JSON: int=75 double=75.000000
I (11344) JSON: [id]
I (11344) JSON: int=2147483647 double=6458825952460800.000000
I (11354) JSON: [weather_state_name]
I (11354) JSON: Heavy Rain
I (11364) JSON: [weather_state_abbr]
I (11364) JSON: hr
I (11364) JSON: [wind_direction_compass]
I (11374) JSON: S
I (11374) JSON: [created]
I (11384) JSON: 2022-05-11T01:03:52.852033Z
I (11384) JSON: [applicable_date]
I (11384) JSON: 2022-05-13
I (11394) JSON: [min_temp]
I (11394) JSON: int=18 double=18.120000
I (11404) JSON: [max_temp]
I (11404) JSON: int=23 double=23.625000
I (11404) JSON: [the_temp]
I (11414) JSON: int=21 double=21.950000
I (11414) JSON: [wind_speed]
I (11414) JSON: int=8 double=8.405169
I (11424) JSON: [wind_direction]
I (11424) JSON: int=177 double=177.215548
I (11434) JSON: [air_pressure]
I (11434) JSON: int=1010 double=1010.000000
I (11444) JSON: [humidity]
I (11444) JSON: int=82 double=82.000000
I (11444) JSON: [visibility]
I (11454) JSON: int=8 double=8.622768
I (11454) JSON: [predictability]
I (11464) JSON: int=77 double=77.000000
I (11464) JSON: [id]
I (11464) JSON: int=2147483647 double=4506850490843136.000000
I (11474) JSON: [weather_state_name]
I (11474) JSON: Heavy Rain
I (11484) JSON: [weather_state_abbr]
I (11484) JSON: hr
I (11484) JSON: [wind_direction_compass]
I (11494) JSON: SSW
I (11494) JSON: [created]
I (11504) JSON: 2022-05-11T01:03:55.666809Z
I (11504) JSON: [applicable_date]
I (11504) JSON: 2022-05-14
I (11514) JSON: [min_temp]
I (11514) JSON: int=16 double=16.350000
I (11524) JSON: [max_temp]
I (11524) JSON: int=21 double=21.165000
I (11524) JSON: [the_temp]
I (11534) JSON: int=19 double=19.145000
I (11534) JSON: [wind_speed]
I (11534) JSON: int=10 double=10.716647
I (11544) JSON: [wind_direction]
I (11544) JSON: int=193 double=193.594973
I (11554) JSON: [air_pressure]
I (11554) JSON: int=1010 double=1010.000000
I (11564) JSON: [humidity]
I (11564) JSON: int=81 double=81.000000
I (11564) JSON: [visibility]
I (11574) JSON: int=8 double=8.163575
I (11574) JSON: [predictability]
I (11584) JSON: int=77 double=77.000000
I (11584) JSON: [id]
I (11584) JSON: int=2147483647 double=6591077055201280.000000
I (11594) JSON: [weather_state_name]
I (11594) JSON: Light Rain
I (11604) JSON: [weather_state_abbr]
I (11604) JSON: lr
I (11604) JSON: [wind_direction_compass]
I (11614) JSON: N
I (11614) JSON: [created]
I (11614) JSON: 2022-05-11T01:03:58.945362Z
I (11624) JSON: [applicable_date]
I (11624) JSON: 2022-05-15
I (11634) JSON: [min_temp]
I (11634) JSON: int=15 double=15.310000
I (11644) JSON: [max_temp]
I (11644) JSON: int=18 double=18.300000
I (11644) JSON: [the_temp]
I (11654) JSON: int=21 double=21.320000
I (11654) JSON: [wind_speed]
I (11654) JSON: int=5 double=5.120477
I (11664) JSON: [wind_direction]
I (11664) JSON: int=2 double=2.559210
I (11674) JSON: [air_pressure]
I (11674) JSON: int=1007 double=1007.500000
I (11684) JSON: [humidity]
I (11684) JSON: int=75 double=75.000000
I (11684) JSON: [visibility]
I (11694) JSON: int=10 double=10.345830
I (11694) JSON: [predictability]
I (11704) JSON: int=75 double=75.000000
I (11704) JSON: [id]
I (11704) JSON: int=2147483647 double=4769677390643200.000000
I (11714) JSON: [weather_state_name]
I (11714) JSON: Heavy Rain
I (11724) JSON: [weather_state_abbr]
I (11724) JSON: hr
I (11724) JSON: [wind_direction_compass]
I (11734) JSON: NNE
I (11734) JSON: [created]
I (11734) JSON: 2022-05-11T01:04:01.852335Z
I (11744) JSON: [applicable_date]
I (11744) JSON: 2022-05-16
I (11754) JSON: [min_temp]
I (11754) JSON: int=13 double=13.980000
I (11754) JSON: [max_temp]
I (11764) JSON: int=16 double=16.370000
I (11764) JSON: [the_temp]
I (11774) JSON: int=15 double=15.570000
I (11774) JSON: [wind_speed]
I (11774) JSON: int=5 double=5.011007
I (11784) JSON: [wind_direction]
I (11784) JSON: int=24 double=24.500000
I (11794) JSON: [air_pressure]
I (11794) JSON: int=1013 double=1013.000000
I (11804) JSON: [humidity]
I (11804) JSON: int=88 double=88.000000
I (11804) JSON: [visibility]
I (11814) JSON: int=8 double=8.966386
I (11814) JSON: [predictability]
I (11824) JSON: int=77 double=77.000000
I (11824) JSON: [time]
I (11824) JSON: 2022-05-11T10:26:22.156114+09:00
I (11834) JSON: [sun_rise]
I (11834) JSON: 2022-05-11T04:38:54.957769+09:00
I (11844) JSON: [sun_set]
I (11844) JSON: 2022-05-11T18:35:33.428463+09:00
I (11854) JSON: [timezone_name]
I (11854) JSON: JST
I (11854) JSON: [parent]
I (11864) JSON: [title]
I (11864) JSON: Japan
I (11864) JSON: [location_type]
I (11864) JSON: Country
I (11874) JSON: [woeid]
I (11874) JSON: int=23424856 double=23424856.000000
I (11884) JSON: [latt_long]
I (11884) JSON: 37.487598,139.838287
I (11884) JSON: [sources]
I (11894) JSON: [title]
I (11894) JSON: BBC
I (11894) JSON: [slug]
I (11904) JSON: bbc
I (11904) JSON: [url]
I (11904) JSON: http://www.bbc.co.uk/weather/
I (11914) JSON: [crawl_rate]
I (11914) JSON: int=360 double=360.000000
I (11924) JSON: [title]
I (11924) JSON: Forecast.io
I (11924) JSON: [slug]
I (11934) JSON: forecast-io
I (11934) JSON: [url]
I (11934) JSON: http://forecast.io/
I (11944) JSON: [crawl_rate]
I (11944) JSON: int=480 double=480.000000
I (11944) JSON: [title]
I (11954) JSON: HAMweather
I (11954) JSON: [slug]
I (11954) JSON: hamweather
I (11964) JSON: [url]
I (11964) JSON: http://www.hamweather.com/
I (11964) JSON: [crawl_rate]
I (11974) JSON: int=360 double=360.000000
I (11974) JSON: [title]
I (11984) JSON: Met Office
I (11984) JSON: [slug]
I (11984) JSON: met-office
I (11994) JSON: [url]
I (11994) JSON: http://www.metoffice.gov.uk/
I (11994) JSON: [crawl_rate]
I (12004) JSON: int=180 double=180.000000
I (12004) JSON: [title]
I (12014) JSON: OpenWeatherMap
I (12014) JSON: [slug]
I (12014) JSON: openweathermap
I (12024) JSON: [url]
I (12024) JSON: http://openweathermap.org/
I (12024) JSON: [crawl_rate]
I (12034) JSON: int=360 double=360.000000
I (12034) JSON: [title]
I (12034) JSON: Weather Underground
I (12044) JSON: [slug]
I (12044) JSON: wunderground
I (12054) JSON: [url]
I (12054) JSON: https://www.wunderground.com/?apiref=fc30dc3cd224e19b
I (12064) JSON: [crawl_rate]
I (12064) JSON: int=720 double=720.000000
I (12064) JSON: [title]
I (12074) JSON: World Weather Online
I (12074) JSON: [slug]
I (12074) JSON: world-weather-online
I (12084) JSON: [url]
I (12084) JSON: http://www.worldweatheronline.com/
I (12094) JSON: [crawl_rate]
I (12094) JSON: int=360 double=360.000000
I (12104) JSON: [title]
I (12104) JSON: Tokyo
I (12104) JSON: [location_type]
I (12114) JSON: City
I (12114) JSON: [woeid]
I (12114) JSON: int=1118370 double=1118370.000000
I (12124) JSON: [latt_long]
I (12124) JSON: 35.670479,139.740921
I (12124) JSON: [timezone]
I (12134) JSON: Asia/Tokyo
```
