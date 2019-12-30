# JSON Example

## Serialize
```
{
        "records":      [{
                        "id":   0,
                        "version":      "v4.0-dev-1186-gf16f4297c-dirty",
                        "cores":        2,
                        "flag": true
                }, {
                        "id":   1,
                        "version":      "v4.0-dev-1186-gf16f4297c-dirty",
                        "cores":        2,
                        "flag": true
                }, {
                        "id":   2,
                        "version":      "v4.0-dev-1186-gf16f4297c-dirty",
                        "cores":        2,
                        "flag": true
                }, {
                        "id":   3,
                        "version":      "v4.0-dev-1186-gf16f4297c-dirty",
                        "cores":        2,
                        "flag": true
                }, {
                        "id":   4,
                        "version":      "v4.0-dev-1186-gf16f4297c-dirty",
                        "cores":        2,
                        "flag": true
                }]
}
```

## DeSerialize
```
I (343) JSON: Deserialize.....
I (353) JSON: root2->type=cJSON_Object
I (353) JSON: records->type=cJSON_Array
I (363) JSON: records_array_size=5
I (363) JSON: id=0
I (363) JSON: version=v4.0-dev-1186-gf16f4297c-dirty
I (373) JSON: cores=2
I (373) JSON: flag=1
I (373) JSON: id=1
I (383) JSON: version=v4.0-dev-1186-gf16f4297c-dirty
I (383) JSON: cores=2
I (393) JSON: flag=1
I (393) JSON: id=2
I (393) JSON: version=v4.0-dev-1186-gf16f4297c-dirty
I (403) JSON: cores=2
I (403) JSON: flag=1
I (403) JSON: id=3
I (413) JSON: version=v4.0-dev-1186-gf16f4297c-dirty
I (413) JSON: cores=2
I (413) JSON: flag=1
I (423) JSON: id=4
I (423) JSON: version=v4.0-dev-1186-gf16f4297c-dirty
I (433) JSON: cores=2
I (433) JSON: flag=1
```
