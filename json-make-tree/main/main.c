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

	int tree_level = 5;
	cJSON *object[tree_level];
	for (int i=tree_level;i>0;i--) {
		object[i-1] = cJSON_CreateObject();
		cJSON_AddNumberToObject(object[i-1], "level", i);
		if (i != tree_level) {
			cJSON_AddItemToObject(object[i-1], "tree", object[i]);
		}
	}

	cJSON_AddItemToObject(root, "root", object[0]);
	//const char *my_json_string = cJSON_Print(root);
	char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);
}
