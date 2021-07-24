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
__These are all parsed as numbers and stored in both Interger and Double.__

## Problems with the cJSON library version 1.7.14   
cJSON_CreateIntArray(i_numbers, 0) causes a panic.   
I can't think of a workaround at this time.   
```
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "JSON";

void app_main()
{
        ESP_LOGI(TAG, "Serialize.....");
        cJSON *root;
        root = cJSON_CreateObject();

        int i_numbers[3];
        i_numbers[0] = 1;
        i_numbers[1] = 11;
        i_numbers[2] = 111;
        cJSON *intArray;
        intArray = cJSON_CreateIntArray(i_numbers, 3);
        cJSON_AddItemToObject(root, "intArray", intArray);

        char *my_json_string = cJSON_Print(root);
        ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
        cJSON_Delete(root);

        // Buffers returned by cJSON_Print must be freed by the caller.
        // Please use the proper API (cJSON_free) rather than directly calling stdlib free.
        cJSON_free(my_json_string);

        cJSON *root2;
        root2 = cJSON_CreateObject();
        intArray = cJSON_CreateIntArray(i_numbers, 0);
        cJSON_AddItemToObject(root2, "intArray", intArray);

        char *my_json_string2 = cJSON_Print(root2);
        ESP_LOGI(TAG, "my_json_string\n%s",my_json_string2);
        cJSON_Delete(root2);

        // Buffers returned by cJSON_Print must be freed by the caller.
        // Please use the proper API (cJSON_free) rather than directly calling stdlib free.
        cJSON_free(my_json_string2);
}
```

