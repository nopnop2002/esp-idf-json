/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */
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
	cJSON_AddStringToObject(root, "version", IDF_VER);
	cJSON_AddNumberToObject(root, "cores", 2);
	char *origial_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "origial_json_string\n%s",origial_json_string);

	cJSON *newitem;
	newitem = cJSON_CreateString("abc");
	cJSON_ReplaceItemInObject(root, "version", newitem);
	newitem = cJSON_CreateNumber(1);
	cJSON_ReplaceItemInObject(root, "cores", newitem);
	char *new_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "new_json_string\n%s",new_json_string);

	cJSON_Delete(root);
}
