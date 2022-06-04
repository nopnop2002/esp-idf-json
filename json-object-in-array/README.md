# JSON Example

## Serialize
```
["abc", 123, true, false, null, {
                "version":      "v5.0-dev-3202-ga2d5041492-dirty",
                "cores":        2
        }]
```

## DeSerialize
```
I (355) JSON: cJSON_String string=abc
I (365) JSON: cJSON_Number int=123 double=123.000000
I (365) JSON: cJSON_True
I (375) JSON: cJSON_False
I (375) JSON: cJSON_NULL
I (375) JSON: cJSON_Object
I (385) JSON: cJSON_String string=v5.0-dev-3202-ga2d5041492-dirty
I (385) JSON: cJSON_Number int=2 double=2.000000
```
