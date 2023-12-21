/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"
#include "cJSON.h"

static const char *TAG = "JSON";

typedef struct {
	int type;
	char name[32];
	int valueint;
	double valuedouble;
	char valuestring[32];
} JSON_t;

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


esp_err_t JSON_Analyze(const cJSON * const root, JSON_t **elements, size_t *elementsLength) {
	//ESP_LOGI(TAG, "root->type=%s", JSON_Types(root->type));
	cJSON *current_element = NULL;
	//ESP_LOGI(TAG, "roo->child=%p", root->child);
	//ESP_LOGI(TAG, "roo->next =%p", root->next);
	cJSON_ArrayForEach(current_element, root) {
		//ESP_LOGI(TAG, "type=%s", JSON_Types(current_element->type));
		//ESP_LOGI(TAG, "current_element->string=%p", current_element->string);

		if (*elements == NULL) {
			(*elementsLength)++;
			*elements = (JSON_t *)malloc(sizeof(JSON_t));
		} else {
			(*elementsLength)++;
			*elements = (JSON_t *)realloc(*elements, *elementsLength * sizeof(JSON_t));
		}
		if (*elements == NULL) {
			ESP_LOGE(TAG, "Error allocating memory for elements");
			return ESP_ERR_NO_MEM;
		}

		if (current_element->string) {
			const char* name = current_element->string;
			ESP_LOGI(TAG, "[%s]", name);
			strncpy((*elements+(*elementsLength)-1)->name, name, 31);
		} else {
			strcpy((*elements+(*elementsLength)-1)->name, "array item");
		}
		if (cJSON_IsInvalid(current_element)) {
			ESP_LOGI(TAG, "Invalid");
			(*elements+(*elementsLength)-1)->type = cJSON_Invalid;
		} else if (cJSON_IsFalse(current_element)) {
			ESP_LOGI(TAG, "False");
			(*elements+(*elementsLength)-1)->type = cJSON_False;
		} else if (cJSON_IsTrue(current_element)) {
			ESP_LOGI(TAG, "True");
			(*elements+(*elementsLength)-1)->type = cJSON_True;
		} else if (cJSON_IsNull(current_element)) {
			ESP_LOGI(TAG, "Null");
			(*elements+(*elementsLength)-1)->type = cJSON_NULL;
		} else if (cJSON_IsNumber(current_element)) {
			int valueint = current_element->valueint;
			double valuedouble = current_element->valuedouble;
			ESP_LOGI(TAG, "int=%d double=%f", valueint, valuedouble);
			(*elements+(*elementsLength)-1)->type = cJSON_Number;
			(*elements+(*elementsLength)-1)->valueint = valueint;
			(*elements+(*elementsLength)-1)->valuedouble = valuedouble;
		} else if (cJSON_IsString(current_element)) {
			const char* valuestring = current_element->valuestring;
			ESP_LOGI(TAG, "%s", valuestring);
			(*elements+(*elementsLength)-1)->type = cJSON_String;
			strncpy((*elements+(*elementsLength)-1)->valuestring, valuestring, 31);
		} else if (cJSON_IsArray(current_element)) {
			ESP_LOGD(TAG, "Array");
			(*elements+(*elementsLength)-1)->type = cJSON_Array;
			JSON_Analyze(current_element, elements, elementsLength);
		} else if (cJSON_IsObject(current_element)) {
			ESP_LOGD(TAG, "Object");
			(*elements+(*elementsLength)-1)->type = cJSON_Object;
			JSON_Analyze(current_element, elements, elementsLength);
		} else if (cJSON_IsRaw(current_element)) {
			ESP_LOGI(TAG, "Raw(Not support)");
		}
	}
	return ESP_OK;
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
	cJSON_AddTrueToObject(root, "wifi");
	cJSON_AddFalseToObject(root, "ethernet");
	cJSON_AddBoolToObject(root, "bool", 0);
	cJSON_AddNullToObject(root, "null");

	// Create object
	cJSON *object;
	cJSON_AddItemToObject(root, "screen", object=cJSON_CreateObject());
	cJSON_AddStringToObject(object,"type", "rect");
	cJSON_AddNumberToObject(object,"width", 1920);
	cJSON_AddNumberToObject(object,"height", 1080);
	cJSON_AddFalseToObject (object,"interlace");
	cJSON_AddTrueToObject(object,"luster");

	// Create array
	cJSON *array;
	array = cJSON_AddArrayToObject(root, "screen");
	cJSON *array_element;
	array_element = cJSON_CreateString("rect");
	cJSON_AddItemToArray(array, array_element);
	array_element = cJSON_CreateNumber(1920);
	cJSON_AddItemToArray(array, array_element);
	array_element = cJSON_CreateNumber(1080);
	cJSON_AddItemToArray(array, array_element);
	array_element = cJSON_CreateFalse();
	cJSON_AddItemToArray(array, array_element);
	array_element = cJSON_CreateTrue();
	cJSON_AddItemToArray(array, array_element);

	//const char *my_json_string = cJSON_Print(root);
	char *my_json_string = cJSON_Print(root);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(root);

	ESP_LOGI(TAG, "Deserialize.....");
	JSON_t *elements = NULL;
	size_t elementsLength = 0;
	cJSON *root2 = cJSON_Parse(my_json_string);
	JSON_Analyze(root2, &elements, &elementsLength);
	cJSON_Delete(root2);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);

	printf("elementsLength=%d\n", elementsLength);
	for (int i=0;i<elementsLength;i++) {
		printf("element[%d]", i);
		if ((elements+i)->type == cJSON_Invalid) {
			printf(" cJSON_Invalid");
		} else if ((elements+i)->type == cJSON_False) {
			printf(" cJSON_False");
			printf(" [%s]", (elements+i)->name);
		} else if ((elements+i)->type == cJSON_True) {
			printf(" cJSON_True");
			printf(" [%s]", (elements+i)->name);
		} else if ((elements+i)->type == cJSON_NULL) {
			printf(" cJSON_NULL");
			printf(" [%s]", (elements+i)->name);
		} else if ((elements+i)->type == cJSON_Number) {
			printf(" cJSON_Number");
			printf(" [%s]", (elements+i)->name);
			printf(" int=%d", (elements+i)->valueint);
			printf(" double=%lf", (elements+i)->valuedouble);
		} else if ((elements+i)->type == cJSON_String) {
			printf(" cJSON_String");
			printf(" [%s]", (elements+i)->name);
			printf(" [%s]", (elements+i)->valuestring);
		} else if ((elements+i)->type == cJSON_Array) {
			printf(" cJSON_Array");
			printf(" [%s]", (elements+i)->name);
		} else if ((elements+i)->type == cJSON_Object) {
			printf(" cJSON_Object");
			printf(" [%s]", (elements+i)->name);
		}
		printf("\n");
	}

	free(elements);
}
