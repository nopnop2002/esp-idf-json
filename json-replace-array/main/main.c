/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "JSON";

void JSON_Print(cJSON *element) {
	if (element->type == cJSON_Invalid) ESP_LOGI(TAG, "cJSON_Invalid");
	if (element->type == cJSON_False) ESP_LOGI(TAG, "cJSON_False");
	if (element->type == cJSON_True) ESP_LOGI(TAG, "cJSON_True");
	if (element->type == cJSON_NULL) ESP_LOGI(TAG, "cJSON_NULL");
	if (element->type == cJSON_Number) ESP_LOGI(TAG, "cJSON_Number int=%d double=%f", element->valueint, element->valuedouble);
	if (element->type == cJSON_String) ESP_LOGI(TAG, "cJSON_String string=%s", element->valuestring);
	if (element->type == cJSON_Array) ESP_LOGI(TAG, "cJSON_Array");
	if (element->type == cJSON_Object) ESP_LOGI(TAG, "cJSON_Object");
	if (element->type == cJSON_Raw) ESP_LOGI(TAG, "cJSON_Raw");
}


void app_main()
{
	ESP_LOGI(TAG, "Serialize.....");
	cJSON *root;
	root = cJSON_CreateArray();
	cJSON *element;
	element = cJSON_CreateString("abc");
	cJSON_AddItemToArray(root, element);
	element = cJSON_CreateNumber(123);
	cJSON_AddItemToArray(root, element);
	char *origial_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "origial_json_string\n%s",origial_json_string);

	cJSON *newitem;
	newitem = cJSON_CreateString("ABC");
	cJSON_ReplaceItemInArray(root, 0, newitem);
	newitem = cJSON_CreateNumber(456);
	cJSON_ReplaceItemInArray(root, 1, newitem);
    char *new_json_string = cJSON_Print(root);
    ESP_LOGI(TAG, "new_json_string\n%s",new_json_string);

	cJSON_Delete(root);
}
