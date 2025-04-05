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

#define STR_SIZE 32
typedef struct {
	uint8_t level;
	char self[STR_SIZE+1];
	char parent[STR_SIZE+1];
	cJSON *json;
} NODE_t;

#define NODE_SIZE 10

void app_main()
{
	// Initialize node table
	NODE_t nodes[NODE_SIZE];

	// top level
	nodes[0].level = 0;
	strcpy(nodes[0].self, "0");
	strcpy(nodes[0].parent, "");

	// 2'nd level
	nodes[1].level = 1;
	strcpy(nodes[1].self, "101");
	strcpy(nodes[1].parent, "0");
	nodes[2].level = 1;
	strcpy(nodes[2].self, "102");
	strcpy(nodes[2].parent, "0");
	nodes[3].level = 1;
	strcpy(nodes[3].self, "103");
	strcpy(nodes[3].parent, "0");

	// 3'rd level
	nodes[4].level = 2;
	strcpy(nodes[4].self, "201");
	strcpy(nodes[4].parent, "101");
	nodes[5].level = 2;
	strcpy(nodes[5].self, "202");
	strcpy(nodes[5].parent, "102");
	nodes[6].level = 2;
	strcpy(nodes[6].self, "203");
	strcpy(nodes[6].parent, "102");

	// 4'th level
	nodes[7].level = 3;
	strcpy(nodes[7].self, "301");
	strcpy(nodes[7].parent, "201");
	nodes[8].level = 3;
	strcpy(nodes[8].self, "302");
	strcpy(nodes[8].parent, "202");

	// 5'th level
	nodes[9].level = 4;
	strcpy(nodes[9].self, "401");
	strcpy(nodes[9].parent, "301");

	// View node table
	for (int i=0;i<NODE_SIZE;i++) {
		ESP_LOGI(TAG, "node[%d] self=[%s] parent=[%s]", i, nodes[i].self, nodes[i].parent);
	}

	// Build tree format json
	ESP_LOGI(TAG, "Serialize.....");
	for (int target_level=10;target_level>=0;target_level--) {
		for (int i=0;i<NODE_SIZE;i++) {
			if (nodes[i].level != target_level) continue;
			nodes[i].json = cJSON_CreateObject();
			ESP_LOGD(TAG, "target_level=%d nodes[%d] self=[%s] parent=[%s]",
				target_level, i, nodes[i].self, nodes[i].parent);
			cJSON_AddNumberToObject(nodes[i].json, "level", nodes[i].level);
			cJSON_AddStringToObject(nodes[i].json, "self", nodes[i].self);

			for (int j=0;j<NODE_SIZE;j++) {
				if (strcmp(nodes[i].self, nodes[j].parent) != 0) continue;
				ESP_LOGD(TAG, "nodes[%d] has child node. self=[%s]", j, nodes[j].self);
				cJSON *array;
				array = cJSON_CreateArray();
				for (int k=0;k<NODE_SIZE;k++) {
					if (strcmp(nodes[i].self, nodes[k].parent) != 0) continue;
					cJSON_AddItemToArray(array, nodes[k].json);
				}
				cJSON_AddItemToObject(nodes[i].json, "child", array);
				break;
			}
		}
	}

	// print json string
	char *my_json_string = cJSON_Print(nodes[0].json);
	ESP_LOGI(TAG, "my_json_string\n%s",my_json_string);
	cJSON_Delete(nodes[0].json);

	// Buffers returned by cJSON_Print must be freed by the caller.
	// Please use the proper API (cJSON_free) rather than directly calling stdlib free.
	cJSON_free(my_json_string);
}
