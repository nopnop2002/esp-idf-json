/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 *
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "JSON";

char *JSON_Types(int type) {
	if (type == cJSON_Invalid) return ("cJSON_Invalid");
	if (type == cJSON_False) return ("cJSON_False");
	if (type == cJSON_True) return ("cJSON_True");
	if (type == cJSON_NULL) return ("cJSON_NULL");
	if (type == cJSON_Number) return ("cJSON_Number");
	if (type == cJSON_String) return ("cJSON_String");
	if (type == cJSON_Array) return ("cJSON_Array");
	if (type == cJSON_Object) return ("cJSON_Object");
	if (type == cJSON_Raw) return ("cJSON_Raw");
	return NULL;
}

int JSON_Array(const cJSON * const item) {
	if (cJSON_IsArray(item)) {
		cJSON *current_element = NULL;
		int itemNumber = 0;
		cJSON_ArrayForEach(current_element, item) {
			//ESP_LOGI(TAG, "current_element->type=%s", JSON_Types(current_element->type));
			if (cJSON_IsNumber(current_element)) {
				int valueint = current_element->valueint;
				double valuedouble = current_element->valuedouble;
				ESP_LOGI(TAG, "valueint[%d]=%d valuedouble[%d]=%f",itemNumber, valueint, itemNumber, valuedouble);
			}
			if (cJSON_IsString(current_element)) {
				const char* string = current_element->valuestring;
				ESP_LOGI(TAG, "string[%d]=%s",itemNumber, string);
			}
			itemNumber++;
		}
		return cJSON_GetArraySize(item);
	} else {
		return 0;
	}
}

void app_main()
{
	ESP_LOGI(TAG, "Serialize.....");
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "name", "CreateArray");

	int i_numbers[3];
	i_numbers[0] = 1;
	i_numbers[1] = 11;
	i_numbers[2] = 111;
	cJSON *intArray;
	intArray = cJSON_CreateIntArray(i_numbers, 3);
	cJSON_AddItemToObject(root, "intArray", intArray);

	float f_numbers[4];
	f_numbers[0] = 1.1;
	f_numbers[1] = 22.2;
	f_numbers[2] = 333.3;
	f_numbers[3] = 4444.4;
	cJSON *floatArray;
	floatArray = cJSON_CreateFloatArray(f_numbers, 4);
	cJSON_AddItemToObject(root, "floatArray", floatArray);

	double d_numbers[5];
	d_numbers[0] = 1.2;
	d_numbers[1] = 12.3;
	d_numbers[2] = 123.4;
	d_numbers[3] = 1234.5;
	d_numbers[4] = 12345.6;
	cJSON *doubleArray;
	doubleArray = cJSON_CreateDoubleArray(d_numbers, 5);
	cJSON_AddItemToObject(root, "doubleArray", doubleArray);

	char str[3][5] = { "Rena" , "Yuki" , "Mimi" };
	const char *ptr[3];
	ptr[0] = str[0];
	ptr[1] = str[1];
	ptr[2] = str[2];
	cJSON *stringArray;
	stringArray = cJSON_CreateStringArray(ptr, 3);
	cJSON_AddItemToObject(root, "stringArray", stringArray);

	//const char *my_json_string = cJSON_Print(root);
	char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root2 = cJSON_Parse(my_json_string);
	char *name = cJSON_GetObjectItem(root2,"name")->valuestring;
	ESP_LOGI(TAG, "name=%s",name);

	cJSON *intArray2 = cJSON_GetObjectItem(root2,"intArray");
	//ESP_LOGI(TAG, "intArray2->type=%s", JSON_Types(intArray2->type));
	JSON_Array(intArray2);

	cJSON *floatArray2 = cJSON_GetObjectItem(root2,"floatArray");
	//ESP_LOGI(TAG, "floatArray2->type=%s", JSON_Types(floatArray2->type));
	JSON_Array(floatArray2);

	cJSON *doubleArray2 = cJSON_GetObjectItem(root2,"doubleArray");
	//ESP_LOGI(TAG, "doubleArray2->type=%s", JSON_Types(doubleArray2->type));
	JSON_Array(doubleArray2);

	cJSON *stringArray2 = cJSON_GetObjectItem(root2,"stringArray");
	JSON_Array(stringArray2);

	cJSON_Delete(root2);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);
}
