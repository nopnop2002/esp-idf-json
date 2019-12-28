/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "JSON";


// Create array
cJSON *Create_array_of_anything(cJSON **objects,int array_num)
{
	cJSON *prev = 0;
	cJSON *root;
	root = cJSON_CreateArray();
	for (int i=0;i<array_num;i++) {
		if (!i)	{
			root->child=objects[i];
		} else {
			prev->next=objects[i];
			objects[i]->prev=prev;
		}
		prev=objects[i];
	}
	return root;
}

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
	int array_num = 5;
	cJSON *objects[5];
	for(int i=0;i<array_num;i++) {
		objects[i] = cJSON_CreateObject();
	}
	cJSON *root;
	root = Create_array_of_anything(objects, array_num);
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);

	for (int i=0;i<array_num;i++) {
		cJSON_AddNumberToObject(objects[i], "id", i);
		cJSON_AddStringToObject(objects[i], "version", IDF_VER);
		cJSON_AddNumberToObject(objects[i], "cores", chip_info.cores);
		cJSON_AddTrueToObject(objects[i], "flag");
	}
	const char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root2 = cJSON_Parse(my_json_string);
	ESP_LOGI(TAG, "root2->type=%s", JSON_Types(root2->type));
	int root2_array_size = cJSON_GetArraySize(root2); 
	ESP_LOGI(TAG, "root2_array_size=%d", root2_array_size);
	for (int i=0;i<root2_array_size;i++) {
		cJSON *array = cJSON_GetArrayItem(root2,i);
		//ESP_LOGI(TAG, "array->type=%s", JSON_Types(array->type));
		int id = cJSON_GetObjectItem(array,"id")->valueint;
		char *version = cJSON_GetObjectItem(array,"version")->valuestring;
		int cores = cJSON_GetObjectItem(array,"cores")->valueint;
		bool flag = cJSON_GetObjectItem(array,"flag")->valueint;
		ESP_LOGI(TAG, "id=%d",id);
		ESP_LOGI(TAG, "version=%s",version);
		ESP_LOGI(TAG, "cores=%d",cores);
		ESP_LOGI(TAG, "flag=%d",flag);
	}
	cJSON_Delete(root2);
}
