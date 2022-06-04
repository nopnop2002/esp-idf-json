# esp-idf-json
Example of JSON Serialize and Deserialize with ESP-IDF.


ESP-IDF includes [this](https://github.com/DaveGamble/cJSON) cJSON library.   
You can use JSON components as standard.   
But the documentation is very poor.   
The official documentation for the JSON component is [here](https://github.com/espressif/esp-idf/tree/master/components/json/README).

[Here](https://github.com/DaveGamble/cJSON/blob/master/tests/readme_examples.c) is a example by Dave Gamble.

# How to use
```
git clone https://github.com/nopnop2002/esp-idf-json
cd esp-idf-json/json-basic-object
idf.py flash monitor
```

# About cJSON_Print
From print_value() function, the pointer returned is allocated by cJSON_strdup()  and it is returned to the caller.   
Buffers returned by cJSON_Print must be freed by the caller.   
Please use the proper API (cJSON_free) rather than directly calling stdlib free.   

```
char *my_json_string = cJSON_Print(root);
ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
cJSON_Delete(root);
cJSON_free(my_json_string);
```


# Basic Structure   
- Array   
Array, like this, have no name.    
Array starts with [ and ends with ].   
```
["abc", 123, true, false, null]
```

- Object   
Object, like this, have name.   
Object starts with { and ends with }.   

```
{
        "version":      "v5.0-dev-3202-ga2d5041492-dirty",
        "cores":        2,
}
```

# Complex structure   

- Object in Array   
```
["abc", 123, true, false, null, {
                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
                "cores":        2
        }]
```

```
[{
                "id":   0,
                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
                "cores":        2
        }, {
                "id":   1,
                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
                "cores":        2
        }]
```


- Array in Object   
```
{
        "version":      "v5.0-dev-3202-ga2d5041492-dirty",
        "cores":        2,
        "array":        ["abc", 123, true, false, null]
}
```

- Object in Array in Object   

```
{
        "version":      "v5.0-dev-3202-ga2d5041492-dirty",
        "cores":        2,
        "array":  [{
                        "width":        1280,
                        "height":       720
                }, {
                        "width":        1920,
                        "height":       1080
                }, {
                        "width":        3840,
                        "height":       2160
                }]
}
```


# API
API is [here](https://cjson.docsforge.com/master/api/)

