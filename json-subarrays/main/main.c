/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 *
 * I ported from here.
 * https://github.com/DaveGamble/cJSON/blob/master/tests/readme_examples.c
 *
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

void json_task(void *pvParameters)
{
	const unsigned int resolution_numbers[3][2] = {
		{1280, 720},
		{1920, 1080},
		{3840, 2160}
	};

	ESP_LOGI(TAG, "Serialize.....");
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "name", "Awesome 4K");
	cJSON *resolutions;
	resolutions = cJSON_CreateArray();
	cJSON_AddItemToObject(root, "resolutions", resolutions);

	int array_size = sizeof(resolution_numbers) / (2 * sizeof(int));
	ESP_LOGI(TAG, "array_size=%d", array_size);
	cJSON *object[array_size];
	cJSON *width[array_size];
	cJSON *height[array_size];
	for(int i=0;i<array_size;i++) {
		object[i] = cJSON_CreateObject();
		cJSON_AddItemToArray(resolutions, object[i]);
		width[i] = cJSON_CreateNumber(resolution_numbers[i][0]);
		cJSON_AddItemToObject(object[i], "width", width[i]);
		height[i] = cJSON_CreateNumber(resolution_numbers[i][1]);
		cJSON_AddItemToObject(object[i], "height", height[i]);
	}

	cJSON *resolutionsWidth;
	resolutionsWidth = cJSON_CreateArray();
	cJSON_AddItemToObject(root, "resolutionsWidth", resolutionsWidth);
	cJSON *width2[array_size];
	for(int i=0;i<array_size;i++) {
		width2[i] = cJSON_CreateNumber(resolution_numbers[i][0]);
		cJSON_AddItemToArray(resolutionsWidth, width2[i]);
	}

	cJSON *resolutionsHeight;
	resolutionsHeight = cJSON_CreateArray();
	cJSON_AddItemToObject(root, "resolutionsHeight", resolutionsHeight);
	cJSON *height2[array_size];
	for(int i=0;i<array_size;i++) {
		height2[i] = cJSON_CreateNumber(resolution_numbers[i][1]);
		cJSON_AddItemToArray(resolutionsHeight, height2[i]);
	}
	char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root2 = cJSON_Parse(my_json_string);
	char *name = cJSON_GetObjectItem(root2,"name")->valuestring;
	ESP_LOGI(TAG, "name=%s",name);

	cJSON *resolutions2 = cJSON_GetObjectItem(root2,"resolutions");
	ESP_LOGI(TAG, "resolutions2->type=%s", JSON_Types(resolutions2->type));
	int resolutions2_array_size = cJSON_GetArraySize(resolutions2);
	ESP_LOGI(TAG, "resolutions2_array_size=%d", resolutions2_array_size);
	for (int i=0;i<resolutions2_array_size;i++) {
		cJSON *array = cJSON_GetArrayItem(resolutions2,i);
		//ESP_LOGI(TAG, "array->type=%s", JSON_Types(array->type));
		int width = cJSON_GetObjectItem(array,"width")->valueint;
		int height = cJSON_GetObjectItem(array,"height")->valueint;
		ESP_LOGI(TAG, "width=%d",width);
		ESP_LOGI(TAG, "height=%d",height);
	}

	cJSON *resolutionsWidth2 = cJSON_GetObjectItem(root2,"resolutionsWidth");
	ESP_LOGI(TAG, "resolutionsWidth2->type=%s", JSON_Types(resolutionsWidth2->type));
	int resolutionsWidth2_array_size = cJSON_GetArraySize(resolutionsWidth2);
	ESP_LOGI(TAG, "resolutionsWidth2_array_size=%d", resolutionsWidth2_array_size);
	for (int i=0;i<resolutionsWidth2_array_size;i++) {
		cJSON *array = cJSON_GetArrayItem(resolutionsWidth2,i);
		//ESP_LOGI(TAG, "array->type=%s", JSON_Types(array->type));
		int resolutionsWidth = array->valueint;
		ESP_LOGI(TAG, "resolutionsWidth[%d]=%d",i, resolutionsWidth);
	}

	cJSON *resolutionsHeight2 = cJSON_GetObjectItem(root2,"resolutionsHeight");
	ESP_LOGI(TAG, "resolutionsHeight2->type=%s", JSON_Types(resolutionsHeight2->type));
	int resolutionsHeight2_array_size = cJSON_GetArraySize(resolutionsHeight2);
	ESP_LOGI(TAG, "resolutionsHeight2_array_size=%d", resolutionsHeight2_array_size);
	for (int i=0;i<resolutionsHeight2_array_size;i++) {
		cJSON *array = cJSON_GetArrayItem(resolutionsHeight2,i);
		//ESP_LOGI(TAG, "array->type=%s", JSON_Types(array->type));
		int resolutionsHeight = array->valueint;
		ESP_LOGI(TAG, "resolutionsHeight[%d]=%d",i, resolutionsHeight);
	}
	cJSON_Delete(root2);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);
	vTaskDelete(NULL);
}

void app_main()
{
	xTaskCreate(json_task, "JSON", 1024*2, NULL, 2, NULL);
}
