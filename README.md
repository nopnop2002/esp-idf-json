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
root is defined by cJSON_CreateArray.   
element is added by cJSON_AddItemToArray.   
```
["abc", 123, true, false, null]
```

- Object   
Object, like this, have name.   
Object starts with { and ends with }.   
root is defined by cJSON_CreateObject.   
element is added by cJSON_AddItemToObject.   
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

- Array in Array   
```
[["abc", 120], ["abc", 121], ["abc", 122], ["abc", 123], ["abc", 124], ["abc", 125], ["abc", 126], ["abc", 127], ["abc", 128], ["abc", 129]]
```



- Object in Object   
```
{
        "name": "Jack (\"Bee\") Nimble",
        "format":       {
                "type": "rect",
                "width":        1920,
                "height":       1080,
                "interlace":    false,
                "frame rate":   24
        }
}
```

- Array in Object   
```
{
        "version":      "v5.0-dev-3202-ga2d5041492-dirty",
        "cores":        2,
        "array":        ["abc", 123, true, false, null]
}
```

# More Complex structure   

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

- Array in Object in Array   
```
[{
                "id":   0,
                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
                "cores":        2,
                "array":        ["abc", 123, true, false, null]
        }, {
                "id":   1,
                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
                "cores":        2,
                "array":        ["abc", 123, true, false, null]
        }]
```

# How to Serialize
Serialize from the bottom layer.

- Target structure
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


	- Create 3 object   
		This is simple object.
		```
		{
		                        "width":        1280,
		                        "height":       720
		                }
		```
		```
		{
		                        "width":        1920,
		                        "height":       1080
		                }
		```
		```
		{
		                        "width":        3840,
		                        "height":       2160
		                }

		```

	- Create array   
		This is object in array.
		```
		[{
		                        "width":        1280,
		                        "height":       720
		                }, {
		                        "width":        1920,
		                        "height":       1080
		                }, {
		                        "width":        3840,
		                        "height":       2160
		                }]
		```

	- Create object as root
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


- Target structure
	```
	[{
	                "id":   0,
	                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
	                "cores":        2,
	                "array":        ["abc", 123, true, false, null]
	        }, {
	                "id":   1,
	                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
	                "cores":        2,
	                "array":        ["abc", 123, true, false, null]
	        }]
	```


	- Create 2 array   
		This is simple array.
		```
		["abc", 123, true, false, null]
		["abc", 123, true, false, null]
		```

	- Create 1st object   
		This is array in object.
		```
		{
		                "id":   1,
		                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
		                "cores":        2,
		                "array":        ["abc", 123, true, false, null]
		        }
		```

	- Create 2nd object   
		This is array in object.
		```
		{
		                "id":   0,
		                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
		                "cores":        2,
		                "array":        ["abc", 123, true, false, null]
		        }
		```

	- Create array as root
		```
		[{
		                "id":   0,
		                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
		                "cores":        2,
		                "array":        ["abc", 123, true, false, null]
		        }, {
		                "id":   1,
		                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
		                "cores":        2,
		                "array":        ["abc", 123, true, false, null]
		        }]
		```



# JSON Data Set Sample
This is a sample JSON data set.   
https://opensource.adobe.com/Spry/samples/data_region/JSONDataSetSample.html
