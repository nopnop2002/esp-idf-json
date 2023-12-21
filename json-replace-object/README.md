# JSON Example

## Serialize
```
I (349) JSON: Serialize.....
I (349) JSON: origial_json_string
{
        "version":      "v5.1.2-dirty",
        "cores":        2
}
I (359) JSON: new_json_string
{
        "version":      "abc",
        "cores":        1
}
I (369) JSON: new_json_string
{
        "cores":        1
}
```


## API
There are cJSON_ReplaceItemInObject() and cJSON_ReplaceItemInObjectCaseSensitive().
```
cJSON_ReplaceItemInObject(root, "cores", newitem);
cJSON_ReplaceItemInObject(root, "CORES", newitem);
cJSON_ReplaceItemInObjectCaseSensitive(root, "cores", newitem);
cJSON_ReplaceItemInObjectCaseSensitive(root, "CORES", newitem);
```

