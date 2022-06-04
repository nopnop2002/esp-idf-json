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

void JSON_Print(const cJSON * const array) {
	int id = cJSON_GetObjectItem(array,"id")->valueint;
	char *version = cJSON_GetObjectItem(array,"version")->valuestring;
	int cores = cJSON_GetObjectItem(array,"cores")->valueint;
	bool flag = cJSON_GetObjectItem(array,"flag")->valueint;
	ESP_LOGI(TAG, "id=%d",id);
	ESP_LOGI(TAG, "version=%s",version);
	ESP_LOGI(TAG, "cores=%d",cores);
	ESP_LOGI(TAG, "flag=%d",flag);
}


void app_main()
{
	ESP_LOGI(TAG, "Serialize.....");
	cJSON *root;
	root = cJSON_CreateArray();

	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	cJSON *object;
	int array_num = 5;
	for (int i=0;i<array_num;i++) {
		object = cJSON_CreateObject();
		cJSON_AddNumberToObject(object, "id", i);
		cJSON_AddStringToObject(object, "version", IDF_VER);
		cJSON_AddNumberToObject(object, "cores", chip_info.cores);
		cJSON_AddTrueToObject(object, "flag");
		cJSON_AddItemToArray(root, object);
	}

	//const char *my_json_string = cJSON_Print(root);
	char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root2 = cJSON_Parse(my_json_string);
	//ESP_LOGI(TAG, "root2->type=%s", JSON_Types(root2->type));
	int root2_array_size = cJSON_GetArraySize(root2); 
	//ESP_LOGI(TAG, "root2_array_size=%d", root2_array_size);
	for (int i=0;i<root2_array_size;i++) {
		cJSON *array = cJSON_GetArrayItem(root2,i);
		JSON_Print(array);
	}
	cJSON_Delete(root2);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);
}
