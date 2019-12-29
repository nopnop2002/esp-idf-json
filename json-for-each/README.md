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
I (314) JSON: Deserialize.....
I (314) JSON: type=cJSON_String
I (324) JSON: [version]=v4.0-dev-1186-gf16f4297c-dirty
I (324) JSON: type=cJSON_Number
I (334) JSON: [cores]=2 2.000000
I (334) JSON: type=cJSON_True
I (334) JSON: [true] is True
I (344) JSON: type=cJSON_False
I (344) JSON: [false] is False
I (344) JSON: type=cJSON_False
I (354) JSON: [bool] is False
I (354) JSON: type=cJSON_NULL
I (364) JSON: [null] is Null
```
