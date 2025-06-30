/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "esp_http_client.h" 
#include "esp_tls.h" 
#include "cJSON.h"

extern MessageBufferHandle_t xMessageBufferToClient;

static const char *TAG = "HTTP_GET";

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
	static int output_len; // Stores number of bytes read
	switch(evt->event_id) {
		case HTTP_EVENT_ERROR:
			ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
			break;
		case HTTP_EVENT_ON_CONNECTED:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
			break;
		case HTTP_EVENT_HEADER_SENT:
			ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
			break;
		case HTTP_EVENT_ON_HEADER:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
			break;
		case HTTP_EVENT_ON_DATA:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
			//ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, content_length=%d", esp_http_client_get_content_length(evt->client));
			ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, output_len=%d", output_len);
			// If user_data buffer is configured, copy the response into the buffer
			if (evt->user_data) {
				memcpy(evt->user_data + output_len, evt->data, evt->data_len);
			}
			output_len += evt->data_len;
			break;
		case HTTP_EVENT_ON_FINISH:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
			output_len = 0;
			break;
		case HTTP_EVENT_DISCONNECTED:
			ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
			int mbedtls_err = 0;
			esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
			if (err != 0) {
				output_len = 0;
				ESP_LOGE(TAG, "Last esp error code: 0x%x", err);
				ESP_LOGE(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
			}
			break;
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
		case HTTP_EVENT_REDIRECT:
			ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
			break;
#endif
	}
	return ESP_OK;
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

void JSON_Parse(const cJSON * const root) {
	//ESP_LOGI(TAG, "root->type=%s", JSON_Types(root->type));
	cJSON *current_element = NULL;
	//ESP_LOGI(TAG, "roo->child=%p", root->child);
	//ESP_LOGI(TAG, "roo->next =%p", root->next);
	cJSON_ArrayForEach(current_element, root) {
		//ESP_LOGI(TAG, "type=%s", JSON_Types(current_element->type));
		//ESP_LOGI(TAG, "current_element->string=%p", current_element->string);
		if (current_element->string) {
			const char* string = current_element->string;
			ESP_LOGI(TAG, "[%s]", string);
		}
		if (cJSON_IsInvalid(current_element)) {
			ESP_LOGI(TAG, "Invalid");
		} else if (cJSON_IsFalse(current_element)) {
			ESP_LOGI(TAG, "False");
		} else if (cJSON_IsTrue(current_element)) {
			ESP_LOGI(TAG, "True");
		} else if (cJSON_IsNull(current_element)) {
			ESP_LOGI(TAG, "Null");
		} else if (cJSON_IsNumber(current_element)) {
			int valueint = current_element->valueint;
			double valuedouble = current_element->valuedouble;
			ESP_LOGI(TAG, "int=%d double=%f", valueint, valuedouble);
		} else if (cJSON_IsString(current_element)) {
			const char* valuestring = current_element->valuestring;
			ESP_LOGI(TAG, "%s", valuestring);
		} else if (cJSON_IsArray(current_element)) {
			//ESP_LOGI(TAG, "Array");
			JSON_Parse(current_element);
		} else if (cJSON_IsObject(current_element)) {
			//ESP_LOGI(TAG, "Object");
			JSON_Parse(current_element);
		} else if (cJSON_IsRaw(current_element)) {
			ESP_LOGI(TAG, "Raw(Not support)");
		}
	}
}

int http_client_content_length(char * url, char * cert_pem)
{
	ESP_LOGI(TAG, "http_client_content_length url=%s",url);
	int content_length;
	
	esp_http_client_config_t config = {
		.url = url,
		.event_handler = _http_event_handler,
		.user_data = NULL,
		//.user_data = local_response_buffer, // Pass address of local buffer to get response
		.cert_pem = cert_pem,
	};
	esp_http_client_handle_t client = esp_http_client_init(&config);

	// GET
	esp_err_t err = esp_http_client_perform(client);
	if (err == ESP_OK) {
		ESP_LOGD(TAG, "HTTP GET Status = %d, content_length = %"PRId64,
			esp_http_client_get_status_code(client),
			esp_http_client_get_content_length(client));
		content_length = esp_http_client_get_content_length(client);

	} else {
		ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
		content_length = 0;
	}
	esp_http_client_cleanup(client);
	return content_length;
}

esp_err_t http_client_content_get(char * url, char * cert_pem, char * response_buffer)
{
	ESP_LOGI(TAG, "http_client_content_get url=%s",url);

	esp_http_client_config_t config = {
		.url = url,
		.event_handler = _http_event_handler,
		.user_data = response_buffer, // Pass address of local buffer to get response
		.cert_pem = cert_pem,
	};
	esp_http_client_handle_t client = esp_http_client_init(&config);

	// GET
	esp_err_t err = esp_http_client_perform(client);
	if (err == ESP_OK) {
		ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRId64,
			esp_http_client_get_status_code(client),
			esp_http_client_get_content_length(client));
		ESP_LOGD(TAG, "\n%s", response_buffer);
	} else {
		ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
	}
	esp_http_client_cleanup(client);
	return err;
}

void http_get_task(void* pvParameters)
{
	TaskHandle_t MyTaskHandle = xTaskGetCurrentTaskHandle();
	ESP_LOGI(TAG, "START MyTaskHandle=%"PRIx32, (uint32_t)MyTaskHandle);
	while(1) {
		uint32_t value = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
		ESP_LOGI(TAG, "ulTaskNotifyTake value=0x%"PRIx32, value);

		// Get content length
		int content_length;
		for (int retry=0;retry<10;retry++) {
			content_length = http_client_content_length(CONFIG_ESP_REST_URL, NULL);
			ESP_LOGI(TAG, "content_length=%d", content_length);
			if (content_length > 0) break;
			vTaskDelay(100);
		}

		if (content_length <= 0) {
			ESP_LOGE(TAG, "[%s] server does not respond", CONFIG_ESP_REST_URL);
			while(1) {
				vTaskDelay(100);
			}
		}

		char *response_buffer; // Buffer to store response of http request from event handler
		response_buffer = (char *) malloc(content_length+1);
		if (response_buffer == NULL) {
			ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
			while(1) {
				vTaskDelay(1);
			}
		}
		bzero(response_buffer, content_length+1);

		// Get content
		while(1) {
			esp_err_t err = http_client_content_get(CONFIG_ESP_REST_URL, NULL, response_buffer);
			if (err == ESP_OK) break;
			vTaskDelay(100);
		}
		ESP_LOGI(TAG, "content_length=%d", content_length);
		ESP_LOGI(TAG, "\n[%s]", response_buffer);

#if 0
		ESP_LOGI(TAG, "Deserialize.....");
		cJSON *root = cJSON_Parse(response_buffer);
		JSON_Parse(root);
		cJSON_Delete(root);
#endif

		cJSON *root = cJSON_CreateObject();
		cJSON_AddStringToObject(root, "id", "json-request");
		cJSON_AddStringToObject(root, "json", response_buffer);
		char *my_json_string = cJSON_Print(root);
		ESP_LOGI(TAG, "my_json_string=%d bytes", strlen(my_json_string));
		ESP_LOGD(TAG, "\n[%s]",my_json_string);
		size_t xBytesSent = xMessageBufferSend(xMessageBufferToClient, my_json_string, strlen(my_json_string), portMAX_DELAY);
		if (xBytesSent != strlen(my_json_string)) {
			ESP_LOGE(TAG, "xMessageBufferSend fail");
		}
		cJSON_free(my_json_string);
		free(response_buffer);
	}

	vTaskDelete(NULL);
}
