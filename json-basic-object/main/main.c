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

void app_main()
{
	ESP_LOGI(TAG, "Serialize.....");
	cJSON *root;
	root = cJSON_CreateObject();
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	cJSON_AddStringToObject(root, "idf", IDF_VER);
	cJSON_AddStringToObject(root, "json", cJSON_Version());
	cJSON_AddNumberToObject(root, "cores", chip_info.cores);
	cJSON_AddTrueToObject(root, "flag_true");
	cJSON_AddFalseToObject(root, "flag_false");
	//const char *my_json_string = cJSON_Print(root);
	char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root2 = cJSON_Parse(my_json_string);
	if (cJSON_GetObjectItem(root2, "idf")) {
		char *idf = cJSON_GetObjectItem(root2,"idf")->valuestring;
		ESP_LOGI(TAG, "idf=%s",idf);
	}
	if (cJSON_GetObjectItem(root2, "json")) {
		char *json = cJSON_GetObjectItem(root2,"json")->valuestring;
		ESP_LOGI(TAG, "json=%s",json);
	}
	if (cJSON_GetObjectItem(root2, "cores")) {
		int cores = cJSON_GetObjectItem(root2,"cores")->valueint;
		ESP_LOGI(TAG, "cores=%d",cores);
	}
	if (cJSON_GetObjectItem(root2, "flag_true")) {
		bool flag_true = cJSON_GetObjectItem(root2,"flag_true")->valueint;
		ESP_LOGI(TAG, "flag_true=%d",flag_true);
	}
	if (cJSON_GetObjectItem(root2, "flag_false")) {
		bool flag_false = cJSON_GetObjectItem(root2,"flag_false")->valueint;
		ESP_LOGI(TAG, "flag_false=%d",flag_false);
	}
	cJSON_Delete(root2);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);
}
