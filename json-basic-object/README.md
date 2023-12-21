# JSON Example

## Serialize
```
{
        "version":      "v4.1-dev-1572-g30372f5a4",
        "cores":        2,
        "flag_true":    true,
        "flag_false":   false
}
```

## DeSerialize
```
I (335) JSON: version=v4.1-dev-1572-g30372f5a4
I (345) JSON: cores=2
I (345) JSON: flag_true=1
I (345) JSON: flag_false=0
```

## API
There are cJSON_GetObjectItem() and cJSON_GetObjectItemCaseSensitive().
```
cJSON_GetObjectItem(root2, "version");
cJSON_GetObjectItem(root2, "VERSION");
cJSON_GetObjectItemCaseSensitive(root2, "VERSION");
```
