# JSON Example

## Serialize
```
{
        "version":      "v4.1-dev-1572-g30372f5a4",
        "cores":        2,
        "true": true,
        "false":        false,
        "bool": false,
        "null": null
}
```

## DeSerialize
```
I (345) JSON: type=cJSON_String
I (345) JSON: string=v4.1-dev-1572-g30372f5a4
I (355) JSON: type=cJSON_Number
I (355) JSON: valueint=2 valuedouble=2.000000
I (355) JSON: type=cJSON_True
I (365) JSON: True
I (365) JSON: type=cJSON_False
I (365) JSON: False
I (375) JSON: type=cJSON_False
I (375) JSON: False
I (375) JSON: type=cJSON_NULL
I (385) JSON: Null
```
