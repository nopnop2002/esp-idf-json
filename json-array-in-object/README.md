# JSON Example

## Serialize
```
{
        "version":      "v5.0-dev-3202-ga2d5041492-dirty",
        "cores":        2,
        "array":        ["abc", 123, true, false, null]
}
```

## DeSerialize
```
I (355) JSON: version=v5.0-dev-3202-ga2d5041492-dirty
I (365) JSON: cores=2
I (365) JSON: cJSON_String string=abc
I (375) JSON: cJSON_Number int=123 double=123.000000
I (375) JSON: cJSON_True
I (385) JSON: cJSON_False
I (385) JSON: cJSON_NULL
```
