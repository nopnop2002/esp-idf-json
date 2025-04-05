# JSON Example

## Serialize
```
{
        "idf":  "v5.3.2-1-g9a6aba7401-dirty",
        "json": "1.7.18",
        "cores":        2,
        "flag_true":    true,
        "flag_false":   false}
```

## DeSerialize
```
I (344) JSON: idf=v5.3.2-1-g9a6aba7401-dirty
I (354) JSON: json=1.7.18
I (354) JSON: cores=2
I (364) JSON: flag_true=1
I (364) JSON: flag_false=0
```

## API
There are cJSON_GetObjectItem() and cJSON_GetObjectItemCaseSensitive().
```
cJSON_GetObjectItem(root2, "idf");
cJSON_GetObjectItem(root2, "IDF");
cJSON_GetObjectItemCaseSensitive(root2, "idf");
cJSON_GetObjectItemCaseSensitive(root2, "IDF");
```
