# JSON Example

## Serialize
```
{
        "name": "CreateArray",
        "intArray":     [1, 11, 111],
        "floatArray":   [1.1000000238418579, 22.200000762939453, 333.29998779296875, 4444.39990234375],
        "doubleArray":  [1.2, 12.3, 123.4, 1234.5, 12345.6],
        "stringArray":  ["Rena", "Yuki", "Mimi"]
}
```

## DeSerialize
```
I (343) JSON: name=CreateArray
I (353) JSON: valueint[0]=1 valuedouble[0]=1.000000
I (353) JSON: valueint[1]=11 valuedouble[1]=11.000000
I (363) JSON: valueint[2]=111 valuedouble[2]=111.000000
I (363) JSON: valueint[0]=1 valuedouble[0]=1.100000
I (373) JSON: valueint[1]=22 valuedouble[1]=22.200001
I (373) JSON: valueint[2]=333 valuedouble[2]=333.299988
I (383) JSON: valueint[3]=4444 valuedouble[3]=4444.399902
I (393) JSON: valueint[0]=1 valuedouble[0]=1.200000
I (393) JSON: valueint[1]=12 valuedouble[1]=12.300000
I (403) JSON: valueint[2]=123 valuedouble[2]=123.400000
I (403) JSON: valueint[3]=1234 valuedouble[3]=1234.500000
I (413) JSON: valueint[4]=12345 valuedouble[4]=12345.600000
I (423) JSON: string[0]=Rena
I (423) JSON: string[1]=Yuki
I (423) JSON: string[2]=Mimi
```

__JSON library does not distinguish Interger, Float, Double.__   
__These are all parsed as Numbers and stored in Interger and Double value.__

