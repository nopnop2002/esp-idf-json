# JSON Example

## Serialize
```
{
        "name": "Awesome 4K",
        "resolutions":  [{
                        "width":        1280,
                        "height":       720
                }, {
                        "width":        1920,
                        "height":       1080
                }, {
                        "width":        3840,
                        "height":       2160
                }],
        "resolutionsWidth":     [1280, 1920, 3840],
        "resolutionsHeight":    [720, 1080, 2160]
}
```

## DeSerialize
```
I (345) JSON: name=Awesome 4K
I (355) JSON: resolutions2->type=cJSON_Array
I (355) JSON: resolutions2_array_size=3
I (365) JSON: width=1280
I (365) JSON: height=720
I (365) JSON: width=1920
I (375) JSON: height=1080
I (375) JSON: width=3840
I (375) JSON: height=2160
I (385) JSON: resolutionsWidth2->type=cJSON_Array
I (385) JSON: resolutionsWidth2_array_size=3
I (395) JSON: resolutionsWidth[0]=1280
I (395) JSON: resolutionsWidth[1]=1920
I (405) JSON: resolutionsWidth[2]=3840
I (405) JSON: resolutionsHeight2->type=cJSON_Array
I (415) JSON: resolutionsHeight2_array_size=3
I (415) JSON: resolutionsHeight[0]=720
I (425) JSON: resolutionsHeight[1]=1080
I (425) JSON: resolutionsHeight[2]=2160
```
