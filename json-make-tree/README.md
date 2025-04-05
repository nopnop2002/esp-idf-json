# JSON Example
Converts a node table into a nested JSON string.   
This is a complex structure of objects and arrays.   
You need to serialize from the lowest level up.   

# Node table
```
I (333) JSON: node[0] self=[0] parent=[]
I (333) JSON: node[1] self=[101] parent=[0]
I (333) JSON: node[2] self=[102] parent=[0]
I (343) JSON: node[3] self=[103] parent=[0]
I (343) JSON: node[4] self=[201] parent=[101]
I (353) JSON: node[5] self=[202] parent=[102]
I (353) JSON: node[6] self=[203] parent=[102]
I (363) JSON: node[7] self=[301] parent=[201]
I (363) JSON: node[8] self=[302] parent=[202]
I (373) JSON: node[9] self=[401] parent=[301]
```

## Serialize
```
{
        "level":        0,
        "self": "0",
        "child":        [{
                        "level":        1,
                        "self": "101",
                        "child":        [{
                                        "level":        2,
                                        "self": "201",
                                        "child":        [{
                                                        "level":        3,
                                                        "self": "301",
                                                        "child":        [{
                                                                        "level":        4,
                                                                        "self": "401"
                                                                }]
                                                }]
                                }]
                }, {
                        "level":        1,
                        "self": "102",
                        "child":        [{
                                        "level":        2,
                                        "self": "202",
                                        "child":        [{
                                                        "level":        3,
                                                        "self": "302"
                                                }]
                                }, {
                                        "level":        2,
                                        "self": "203"
                                }]
                }, {
                        "level":        1,
                        "self": "103"
                }]
}
```
