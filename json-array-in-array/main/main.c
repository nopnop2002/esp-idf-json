/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"
#include "cJSON.h"

static const char *TAG = "JSON";

void JSON_Print(cJSON *element) {
	if (element->type == cJSON_Invalid) ESP_LOGI(TAG, "cJSON_Invalid");
	if (element->type == cJSON_False) ESP_LOGI(TAG, "cJSON_False");
	if (element->type == cJSON_True) ESP_LOGI(TAG, "cJSON_True");
	if (element->type == cJSON_NULL) ESP_LOGI(TAG, "cJSON_NULL");
	if (element->type == cJSON_Number) ESP_LOGI(TAG, "cJSON_Number int=%d double=%f", element->valueint, element->valuedouble);
	if (element->type == cJSON_String) ESP_LOGI(TAG, "cJSON_String string=%s", element->valuestring);
	if (element->type == cJSON_Array) {
		ESP_LOGI(TAG, "cJSON_Array");
		cJSON *child_element = NULL;
		cJSON_ArrayForEach(child_element, element) {
			JSON_Print(child_element);
		}
	}
	if (element->type == cJSON_Object) {
		ESP_LOGI(TAG, "cJSON_Object");
		cJSON *child_element = NULL;
		cJSON_ArrayForEach(child_element, element) {
			JSON_Print(child_element);
		}
	}
	if (element->type == cJSON_Raw) ESP_LOGI(TAG, "cJSON_Raw");
}


void app_main()
{
	ESP_LOGI(TAG, "Serialize.....");
	cJSON *root;
	root = cJSON_CreateArray();

	cJSON *array;
	cJSON *element;

	int array_num = 10;
	for (int i=0;i<array_num;i++) {
		array = cJSON_CreateArray();
		element = cJSON_CreateString("abc");
		cJSON_AddItemToArray(array, element);
		element = cJSON_CreateNumber(120+i);
		cJSON_AddItemToArray(array, element);
		cJSON_AddItemToArray(root, array);
	}
	char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);


	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root2 = cJSON_Parse(my_json_string);
	int root2_array_size = cJSON_GetArraySize(root2); 
	ESP_LOGI(TAG, "root2_array_size=%d", root2_array_size);
	for (int i=0;i<root2_array_size;i++) {
		cJSON *element2 = cJSON_GetArrayItem(root2, i);
		JSON_Print(element2);
	}
	cJSON_Delete(root2);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);
}
