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
	int numbers[5] = {10, 20, 30, 40, 50};
	
	cJSON *root;
	root = cJSON_CreateIntArray(numbers, 5);

	char *my_json_string;
	my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_free(my_json_string);

	root = cJSON_CreateObject();
	cJSON *array = cJSON_CreateIntArray(numbers, 5);
	cJSON_AddItemToObject(root, "numbers", array);

	my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_free(my_json_string);
	cJSON_Delete(root);

}
