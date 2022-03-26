/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/ringbuf.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_http_client.h" 
#include "esp_tls.h" 
#include "cJSON.h"


/* The examples use WiFi configuration that you can set via project configuration menu

	 If you'd rather not, just change the below entries to strings with
	 the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID		CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS		CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY	CONFIG_ESP_MAXIMUM_RETRY
#define EXAMPLE_ESP_REST_URL		CONFIG_ESP_REST_URL

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT			 BIT1

static const char *TAG = "JSON";

static int s_retry_num = 0;

RingbufHandle_t xRingbuffer;


esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
	switch(evt->event_id) {
		case HTTP_EVENT_ERROR:
			ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
			break;
		case HTTP_EVENT_ON_CONNECTED:
			ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
			break;
		case HTTP_EVENT_HEADER_SENT:
			ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
			break;
		case HTTP_EVENT_ON_HEADER:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
			break;
		case HTTP_EVENT_ON_DATA:
			ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
			if (!esp_http_client_is_chunked_response(evt->client)) {
				//char buffer[512];
				char *buffer = malloc(evt->data_len + 1);
				esp_http_client_read(evt->client, buffer, evt->data_len);
				buffer[evt->data_len] = 0;
				//ESP_LOGI(TAG, "buffer=%s", buffer);
				//UBaseType_t res = xRingbufferSend(xRingbuffer, buffer, evt->data_len, pdMS_TO_TICKS(1000));
				UBaseType_t res = xRingbufferSendFromISR(xRingbuffer, buffer, evt->data_len, NULL);
				if (res != pdTRUE) {
					ESP_LOGE(TAG, "Failed to xRingbufferSend");
				}
				free(buffer);
			}
			break;
		case HTTP_EVENT_ON_FINISH:
			ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
			break;
		case HTTP_EVENT_DISCONNECTED:
			ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
			break;
	}
	return ESP_OK;
}


static void event_handler(void* arg, esp_event_base_t event_base,
																int32_t event_id, void* event_data)
{
		if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
				esp_wifi_connect();
		} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
				if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
						esp_wifi_connect();
						s_retry_num++;
						ESP_LOGI(TAG, "retry to connect to the AP");
				} else {
						xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
				}
				ESP_LOGI(TAG,"connect to the AP fail");
		} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
				ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
				ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
				s_retry_num = 0;
				xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
		}
}

void wifi_init_sta(void)
{
		s_wifi_event_group = xEventGroupCreate();

		ESP_ERROR_CHECK(esp_netif_init());

		ESP_ERROR_CHECK(esp_event_loop_create_default());
		esp_netif_create_default_wifi_sta();

		wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
		ESP_ERROR_CHECK(esp_wifi_init(&cfg));

		esp_event_handler_instance_t instance_any_id;
		esp_event_handler_instance_t instance_got_ip;
		ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
																												ESP_EVENT_ANY_ID,
																												&event_handler,
																												NULL,
																												&instance_any_id));
		ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
																												IP_EVENT_STA_GOT_IP,
																												&event_handler,
																												NULL,
																												&instance_got_ip));

		wifi_config_t wifi_config = {
				.sta = {
						.ssid = EXAMPLE_ESP_WIFI_SSID,
						.password = EXAMPLE_ESP_WIFI_PASS,
						/* Setting a password implies station will connect to all security modes including WEP/WPA.
						 * However these modes are deprecated and not advisable to be used. Incase your Access point
						 * doesn't support WPA2, these mode can be enabled by commenting below line */
			 .threshold.authmode = WIFI_AUTH_WPA2_PSK,

						.pmf_cfg = {
								.capable = true,
								.required = false
						},
				},
		};
		ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
		ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
		ESP_ERROR_CHECK(esp_wifi_start() );

		ESP_LOGI(TAG, "wifi_init_sta finished.");

		/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
		 * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
		EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
						WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
						pdFALSE,
						pdFALSE,
						portMAX_DELAY);

		/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
		 * happened. */
		if (bits & WIFI_CONNECTED_BIT) {
				ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
								 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
		} else if (bits & WIFI_FAIL_BIT) {
				ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
								 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
		} else {
				ESP_LOGE(TAG, "UNEXPECTED EVENT");
		}

		/* The event will not be processed after unregister */
		ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
		ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
		vEventGroupDelete(s_wifi_event_group);
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

void http_client(char * url)
{
	
	esp_http_client_config_t config = {
		//.url = "http://192.168.10.43:3000/todos",
		.url = url,
		.event_handler = _http_event_handler,
	};
	esp_http_client_handle_t client = esp_http_client_init(&config);

	// GET
	esp_err_t err = esp_http_client_perform(client);
	if (err == ESP_OK) {
		ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
				esp_http_client_get_status_code(client),
				esp_http_client_get_content_length(client));
		//Receive an item from no-split ring buffer
		int bufferSize = esp_http_client_get_content_length(client);
		char *buffer = malloc(bufferSize + 1); 
		size_t item_size;
		int	index = 0;
		while (1) {
			char *item = (char *)xRingbufferReceive(xRingbuffer, &item_size, pdMS_TO_TICKS(1000));
			if (item != NULL) {
				for (int i = 0; i < item_size; i++) {
					//printf("%c", item[i]);
					buffer[index] = item[i];
					index++;
					buffer[index] = 0;
				}
				//printf("\n");
				//Return Item
				vRingbufferReturnItem(xRingbuffer, (void *)item);
			} else {
				//Failed to receive item
				ESP_LOGI(TAG, "End of receive item");
				break;
			}
		}
		ESP_LOGI(TAG, "buffer=\n%s", buffer);

		ESP_LOGI(TAG, "Deserialize.....");
		cJSON *root = cJSON_Parse(buffer);
		JSON_Parse(root);
		cJSON_Delete(root);
		free(buffer);

	} else {
		ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
	}
	esp_http_client_cleanup(client);
}


void app_main()
{
	//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
	wifi_init_sta();

	//Create Ring Buffer 
	//No Split
	xRingbuffer = xRingbufferCreate(1024, RINGBUF_TYPE_NOSPLIT);
	//Allow_Split
	//xRingbuffer = xRingbufferCreate(1024, RINGBUF_TYPE_ALLOWSPLIT);

	//Check everything was created
	configASSERT( xRingbuffer );

	//Array
	char url[64];
	sprintf(url, "%s", EXAMPLE_ESP_REST_URL);
	ESP_LOGI(TAG, "url=%s",url);
	//http_client("http://192.168.10.43:3000/todos"); 
	http_client(url); 

	//Object
	sprintf(url, "%s/2", EXAMPLE_ESP_REST_URL);
	ESP_LOGI(TAG, "url=%s",url);
	http_client(url); 

}
