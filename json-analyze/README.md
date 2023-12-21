# JSON Example

## Serialize
```
{
        "version":      "v5.1.2-dirty",
        "cores":        2,
        "wifi": true,
        "ethernet":     false,
        "bool": false,
        "null": null,
        "screen":       {
                "type": "rect",
                "width":        1920,
                "height":       1080,
                "interlace":    false,
                "luster":       true
        },
        "screen":       ["rect", 1920, 1080, false, true]
}
```

## DeSerialize
```
I (381) JSON: Deserialize.....
I (381) JSON: [version]
I (391) JSON: v5.1.2-dirty
I (391) JSON: [cores]
I (391) JSON: int=2 double=2.000000
I (401) JSON: [wifi]
I (401) JSON: True
I (401) JSON: [ethernet]
I (411) JSON: False
I (411) JSON: [bool]
I (411) JSON: False
I (411) JSON: [null]
I (421) JSON: Null
I (421) JSON: [screen]
I (421) JSON: [type]
I (431) JSON: rect
I (431) JSON: [width]
I (431) JSON: int=1920 double=1920.000000
I (441) JSON: [height]
I (441) JSON: int=1080 double=1080.000000
I (441) JSON: [interlace]
I (451) JSON: False
I (451) JSON: [luster]
I (451) JSON: True
I (461) JSON: [screen]
I (461) JSON: rect
I (461) JSON: int=1920 double=1920.000000
I (471) JSON: int=1080 double=1080.000000
I (471) JSON: False
I (471) JSON: True
```

## Analysis
```
elementsLength=18
element[0] cJSON_String [version] [v5.1.2-dirty]
element[1] cJSON_Number [cores] int=2 double=2.000000
element[2] cJSON_True [wifi]
element[3] cJSON_False [ethernet]
element[4] cJSON_False [bool]
element[5] cJSON_NULL [null]
element[6] cJSON_Object [screen]
element[7] cJSON_String [type] [rect]
element[8] cJSON_Number [width] int=1920 double=1920.000000
element[9] cJSON_Number [height] int=1080 double=1080.000000
element[10] cJSON_False [interlace]
element[11] cJSON_True [luster]
element[12] cJSON_Array [screen]
element[13] cJSON_String [array item] [rect]
element[14] cJSON_Number [array item] int=1920 double=1920.000000
element[15] cJSON_Number [array item] int=1080 double=1080.000000
element[16] cJSON_False [array item]
element[17] cJSON_True [array item]
```
