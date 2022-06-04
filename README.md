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

# API
API is [here](https://cjson.docsforge.com/master/api/)

