/*
	Example using WEB Socket.
	This example code is in the Public Domain (or CC0 licensed, at your option.)
	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "web_client";

#include "websocket_server.h"

extern MessageBufferHandle_t xMessageBufferToClient;

// Timer callback
static void timer_cb(TimerHandle_t xTimer)
{
	TickType_t nowTick;
	nowTick = xTaskGetTickCount();
	ESP_LOGD(TAG, "timer is called, now=%"PRIu32, nowTick);

	cJSON *request;
	request = cJSON_CreateObject();
	cJSON_AddStringToObject(request, "id", "timer-request");
	//char *my_json_string = cJSON_Print(request);
	char *my_json_string = cJSON_PrintUnformatted(request);
	ESP_LOGD(TAG, "my_json_string\n%s",my_json_string);
	size_t xBytesSent = xMessageBufferSendFromISR(xMessageBufferToClient, my_json_string, strlen(my_json_string), NULL);
	if (xBytesSent != strlen(my_json_string)) {
		ESP_LOGE(TAG, "xMessageBufferSend fail");
	}
	cJSON_Delete(request);
	cJSON_free(my_json_string);
}

void client_task(void* pvParameters) {
	TaskHandle_t NotifyTaskHandle = (TaskHandle_t)pvParameters;
	ESP_LOGI(TAG, "Start NotifyTaskHandle=%"PRIx32, (uint32_t)NotifyTaskHandle);

	// Create Timer (Trigger a measurement every second)
	TimerHandle_t timerHandle = xTimerCreate("MY Trigger", 500, pdTRUE, NULL, timer_cb);
	if (timerHandle != NULL) {
		if (xTimerStart(timerHandle, 0) != pdPASS) {
			ESP_LOGE(TAG, "Unable to start Timer");
			vTaskDelete(NULL);
		} else {
			ESP_LOGI(TAG, "Success to start Timer");
		}
	} else {
		ESP_LOGE(TAG, "Unable to create Timer");
		vTaskDelete(NULL);
	}

	char inBuffer[1024];
	char DEL = 0x04;
	char outBuffer[1024];

	while (1) {
		size_t readBytes = xMessageBufferReceive(xMessageBufferToClient, inBuffer, sizeof(inBuffer), portMAX_DELAY );
		ESP_LOGI(TAG, "readBytes=%d", readBytes);
		ESP_LOGI(TAG, "inBuffer=[%.*s]", readBytes, inBuffer);
		cJSON *root = cJSON_Parse(inBuffer);
		if (cJSON_GetObjectItem(root, "id")) {
			char *id = cJSON_GetObjectItem(root,"id")->valuestring;
			ESP_LOGI(TAG, "id=[%s]",id);

			if ( strcmp (id, "init") == 0) {
				sprintf(outBuffer,"HEADER%cJSON Table View", DEL);
				ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
				ws_server_send_text_all(outBuffer,strlen(outBuffer));

				// Notify http client task
				// The http client task reads data from the server
				// The http client task return the response as a json-request
				xTaskNotifyGive( NotifyTaskHandle );
			} // end init

			if ( strcmp (id, "json-request") == 0) {
				char *json = cJSON_GetObjectItem(root,"json")->valuestring;
				ESP_LOGI(TAG, "json=[%s]",json);

				// Allocate memory for header
				cJSON *root2 = cJSON_Parse(json);
				int array_size = cJSON_GetArraySize(root2); 
				ESP_LOGI(TAG, "array_size=%d", array_size);
				cJSON *header = cJSON_GetArrayItem(root2, 0);
				int header_element_size = cJSON_GetArraySize(header); 
				ESP_LOGI(TAG, "header_element_size=%d", header_element_size);
				char** array;
				array = malloc(sizeof(char *) * header_element_size);
				if (array == NULL) {
					ESP_LOGE(TAG, "malloc fail");
					vTaskDelete(NULL);
				}
				for (int index = 0; index < header_element_size; index++) {
					array[index] = malloc(sizeof(char)* 32);
					if (array[index] == NULL) {
						ESP_LOGE(TAG, "malloc fail");
						vTaskDelete(NULL);
					}
				}

				// Collect headline information
				for (int i=0;i<header_element_size;i++) {
					cJSON *header_element = cJSON_GetArrayItem(header, i);
					ESP_LOGI(TAG, "header_element->valuestring=[%s]", header_element->valuestring);
					strcpy(array[i], header_element->valuestring);
				}
				for (int i=0;i<header_element_size;i++) {
					ESP_LOGI(TAG, "array[%d]=[%s]", i, array[i]);
				}


#if 0
From:
  [
	"id",
	"type",
	"name",
	"batter"
  ],
  [
	"0001",
	"donut",
	"Cake",
	"Regulard"
  ],
  [
    "0001",
    "donut",
    "Cake",
    "Chocolate"
  ],
  [
    "0001",
    "donut",
    "Cake",
    "Blueberry"
  ]

To:
[
	{
		"id":	"0001",
		"type": "donut",
		"name": "Cake",
		"batter":		"Regulard"
	}, {
		"id":	"0001",
		"type": "donut",
		"name": "Cake",
		"batter":		"Chocolate"
	}, {
		"id":	"0001",
		"type": "donut",
		"name": "Cake",
		"batter":		"Blueberry"
	}
]
#endif
				// Convert array to object
				cJSON *root_header = cJSON_CreateObject();
				for (int i=0;i<header_element_size;i++) {
					cJSON_AddStringToObject(root_header, array[i], array[i]);
				}
				//char *header_json_string = cJSON_Print(root_header);
				char *header_json_string = cJSON_PrintUnformatted(root_header);
				ESP_LOGI(TAG, "header_json_string\n%s",header_json_string);

				cJSON *root_table = cJSON_CreateArray();
				cJSON *object[array_size-1];
				for (int i=1;i<array_size;i++) {
					int object_index = i-1;
					int header_index = 0;
					object[object_index] = cJSON_CreateObject();
					cJSON *element = cJSON_GetArrayItem(root2, i);
					if (element->type == cJSON_Array) {
						ESP_LOGI(TAG, "cJSON_Array");
						cJSON *child_element = NULL;
						cJSON_ArrayForEach(child_element, element) {
							if (child_element->type == cJSON_String) {
								ESP_LOGI(TAG, "cJSON_String string=%s", child_element->valuestring);
								if (header_index == header_element_size) {
									ESP_LOGW(TAG, "Illegal element size");
								} else {
									cJSON_AddStringToObject(object[object_index], array[header_index], child_element->valuestring);
								}
							} else if (child_element->type == cJSON_Number) {
								ESP_LOGI(TAG, "cJSON_Number int=%d double=%f", child_element->valueint, child_element->valuedouble);
								if (header_index == header_element_size) {
									ESP_LOGW(TAG, "Illegal element size");
								} else {
									char wk[32];
									sprintf(wk, "%d", child_element->valueint);
									cJSON_AddStringToObject(object[object_index], array[header_index], wk);
								}
							} else {
								ESP_LOGE(TAG, "Illegal element type [%d]", child_element->type);
								vTaskDelete(NULL);	
							}
							header_index++;
						}
					} else {
						ESP_LOGE(TAG, "Illegal element type");
						vTaskDelete(NULL);	
					}
					cJSON_AddItemToArray(root_table, object[object_index]);
				}

				//char *table_json_string = cJSON_Print(root_table);
				char *table_json_string = cJSON_PrintUnformatted(root_table);
				ESP_LOGI(TAG, "table_json_string\n%s",table_json_string);
				sprintf(outBuffer,"TABLE1%c%s%c%s",DEL, header_json_string, DEL, table_json_string);
				ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
				ws_server_send_text_all(outBuffer,strlen(outBuffer));

				cJSON_free(header_json_string);
				cJSON_free(table_json_string);
				cJSON_Delete(root2);
				cJSON_Delete(root_header);
				cJSON_Delete(root_table);
				for (int index = 0; index < header_element_size; index++) {
					free(array[index]);
				}
				free(array);
			} // end json-request

			if ( strcmp (id, "timer-request") == 0) {
				cJSON *root_header = cJSON_CreateObject();
				cJSON_AddStringToObject(root_header, "elasped", "elasped");
				cJSON_AddStringToObject(root_header, "freeHeep", "freeHeep");
				//char *header_json_string = cJSON_Print(root_header);
				char *header_json_string = cJSON_PrintUnformatted(root_header);
				ESP_LOGI(TAG, "header_json_string\n%s",header_json_string);

				cJSON *root_table = cJSON_CreateArray();
				cJSON *object = cJSON_CreateObject();
				TickType_t nowTick = xTaskGetTickCount();
				char wk[32];
				sprintf(wk, "%"PRIi32, nowTick);
				cJSON_AddStringToObject(object, "elasped", wk);
				sprintf(wk, "%"PRIu32, esp_get_free_heap_size());
				cJSON_AddStringToObject(object, "freeHeep", wk);
				cJSON_AddItemToArray(root_table, object);
				//char *table_json_string = cJSON_Print(root_table);
				char *table_json_string = cJSON_PrintUnformatted(root_table);
				ESP_LOGI(TAG, "table_json_string\n%s",table_json_string);

				sprintf(outBuffer,"TABLE2%c%s%c%s",DEL, header_json_string, DEL, table_json_string);
				ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
				ws_server_send_text_all(outBuffer,strlen(outBuffer));

				cJSON_free(header_json_string);
				cJSON_free(table_json_string);
				cJSON_Delete(root_header);
				cJSON_Delete(root_table);
			} // end timer-request

		} // end if

		// Delete a cJSON structure
		cJSON_Delete(root);

	} // end while

	// Never reach here
	vTaskDelete(NULL);
}
