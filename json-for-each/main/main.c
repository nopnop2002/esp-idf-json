/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
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

void app_main()
{
	ESP_LOGI(TAG, "Serialize.....");
	cJSON *root;
	root = cJSON_CreateObject();
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	cJSON_AddStringToObject(root, "version", IDF_VER);
	cJSON_AddNumberToObject(root, "cores", chip_info.cores);
	cJSON_AddTrueToObject(root, "true");
	cJSON_AddFalseToObject(root, "false");
	cJSON_AddBoolToObject(root, "bool", 0);
	cJSON_AddNullToObject(root, "null");
	const char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root2 = cJSON_Parse(my_json_string);
	//ESP_LOGI(TAG, "root2->type=%s", JSON_Types(root2->type));
	cJSON *current_element = NULL;
	cJSON_ArrayForEach(current_element, root2) {
		ESP_LOGI(TAG, "type=%s", JSON_Types(current_element->type));
		if (cJSON_IsInvalid(current_element)) {
			ESP_LOGI(TAG, "Invalid");
		} else if (cJSON_IsFalse(current_element)) {
			ESP_LOGI(TAG, "False");
		} else if (cJSON_IsTrue(current_element)) {
			ESP_LOGI(TAG, "True");
		} else if (cJSON_IsNull(current_element)) {
			ESP_LOGI(TAG, "Null");
		} else if (cJSON_IsNumber(current_element)) {
			int valueint = current_element->valueint;
			double valuedouble = current_element->valuedouble;
			ESP_LOGI(TAG, "valueint=%d valuedouble=%f", valueint, valuedouble);
		} else if (cJSON_IsString(current_element)) {
			const char* string = current_element->valuestring;
			ESP_LOGI(TAG, "string=%s", string);
		} else if (cJSON_IsArray(current_element)) {
			ESP_LOGI(TAG, "Array");
		} else if (cJSON_IsObject(current_element)) {
			ESP_LOGI(TAG, "Object");
		} else if (cJSON_IsRaw(current_element)) {
			ESP_LOGI(TAG, "Raw");
		}
	}
	cJSON_Delete(root2);
}
