# json-generator
json-generator component is published [here](https://components.espressif.com/components/espressif/json_generator).   
We can generate JSON messages using this component.   

## Serialize
```
flush_str buf=[{"first_bool":true,]
flush_str buf=["first_int":30,"flo]
flush_str buf=[at_val":54.16430,"m]
flush_str buf=[y_str":"new_name","]
flush_str buf=[null_obj":null,"arr]
flush_str buf=[":[["arr_string",fa]
flush_str buf=[lse,45.12000,null,2]
flush_str buf=[5,{"arr_obj_str":"s]
flush_str buf=[ample"}]],"my_obj":]
flush_str buf=[{"only_val":5}}]
Generated: {"first_bool":true,"first_int":30,"float_val":54.16430,"my_str":"new_name","null_obj":null,"arr":[["arr_string",false,45.12000,null,25,{"arr_obj_str":"sample"}]],"my_obj":{"only_val":5}}
```
