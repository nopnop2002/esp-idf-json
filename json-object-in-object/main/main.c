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
	cJSON *root, *format;
	root = cJSON_CreateObject();
	format = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "name", "Jack (\"Bee\") Nimble");
	cJSON_AddItemToObject(root, "format", format);
	cJSON_AddStringToObject(format,"type","rect");
	cJSON_AddNumberToObject(format,"width",1920);
	cJSON_AddNumberToObject(format,"height",1080);
	cJSON_AddFalseToObject (format,"interlace");
	cJSON_AddNumberToObject(format,"frame rate",24);
	//const char *my_json_string = cJSON_Print(root);
	char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root2 = cJSON_Parse(my_json_string);
	char *name = cJSON_GetObjectItem(root2,"name")->valuestring;
	cJSON *format2 = cJSON_GetObjectItem(root2,"format");
	char *type = cJSON_GetObjectItem(format2,"type")->valuestring;
	int width = cJSON_GetObjectItem(format2,"width")->valueint;
	int height = cJSON_GetObjectItem(format2,"height")->valueint;
	bool interlace = cJSON_GetObjectItem(format2,"interlace")->valueint;
	int frame_ratet = cJSON_GetObjectItem(format2,"frame rate")->valueint;
	ESP_LOGI(TAG, "name=%s",name);
	ESP_LOGI(TAG, "type=%s",type);
	ESP_LOGI(TAG, "width=%d",width);
	ESP_LOGI(TAG, "height=%d",height);
	ESP_LOGI(TAG, "interlace=%d",interlace);
	ESP_LOGI(TAG, "frame_ratet=%d",frame_ratet);
	//cJSON_Delete(format2);
	cJSON_Delete(root2);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);
}
