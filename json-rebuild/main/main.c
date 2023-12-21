/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "JSON";

void app_main()
{
	ESP_LOGI(TAG, "Initialize.....");
	char *my_json_string = "{\"roll\": 57, \"pitch\": 26, \"yaw\":114}";
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON *root = cJSON_Parse(my_json_string);

	ESP_LOGI(TAG, "Rebuilding.....");
	cJSON_AddStringToObject(root, "version", IDF_VER);
	char *my_json_string2 = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string2\n%s",my_json_string2);
	cJSON_Delete(root);
	cJSON_free(my_json_string2);
}
