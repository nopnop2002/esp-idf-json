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

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static const char *TAG = "JSON";

static int s_retry_num = 0;

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "www.howsmyssl.com"
#define WEB_PORT "443"
#define WEB_URL "https://www.howsmyssl.com/a/check"

static const char HOWSMYSSL_REQUEST[] = "GET " WEB_URL " HTTP/1.1\r\n"
	"Host: "WEB_SERVER"\r\n"
	"User-Agent: esp-idf/1.0 esp32\r\n"
	"\r\n";

/*
	Root cert for metaweather.com, taken from metaweather_com_root_cert.pem

	The PEM file was extracted from the output of this command:
	openssl s_client -showcerts -connect jsonplaceholder.typicode.com.com:443 </dev/null

	The CA root cert is the last cert given in the chain of certs.

	To embed it in the app binary, the PEM file is named
	in the component.mk COMPONENT_EMBED_TXTFILES variable.
*/
extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");


esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
	static char *output_buffer;  // Buffer to store response of http request from event handler
	static int output_len;		 // Stores number of bytes read
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
			} else {
				if (output_buffer == NULL && esp_http_client_get_content_length(evt->client) > 0) {
					output_buffer = (char *) malloc(esp_http_client_get_content_length(evt->client));
					output_len = 0;
					if (output_buffer == NULL) {
						ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
						return ESP_FAIL;
					}
				}
				memcpy(output_buffer + output_len, evt->data, evt->data_len);
			}
			output_len += evt->data_len;
			break;
		case HTTP_EVENT_ON_FINISH:
			ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
			if (output_buffer != NULL) {
				// Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
				// ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
				free(output_buffer);
				output_buffer = NULL;
			}
			output_len = 0;
			break;
		case HTTP_EVENT_DISCONNECTED:
			ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
			int mbedtls_err = 0;
			esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
			if (err != 0) {
				if (output_buffer != NULL) {
					free(output_buffer);
					output_buffer = NULL;
				}
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

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (s_retry_num < CONFIG_ESP_MAXIMUM_RETRY) {
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
			.ssid = CONFIG_ESP_WIFI_SSID,
			.password = CONFIG_ESP_WIFI_PASSWORD,
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
		ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
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


void JSON_Analyze(const cJSON * const root) {
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
			JSON_Analyze(current_element);
		} else if (cJSON_IsObject(current_element)) {
			//ESP_LOGI(TAG, "Object");
			JSON_Analyze(current_element);
		} else if (cJSON_IsRaw(current_element)) {
			ESP_LOGI(TAG, "Raw(Not support)");
		}
	}
}

static size_t https_get_request(int request, esp_tls_cfg_t cfg, const char *WEB_SERVER_URL, const char *REQUEST, char *content)
{
	char buf[512];
	int ret, len;
	size_t content_length = 0;
	int index = 0;
	int header_flag = 0;

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
	esp_tls_t *tls = esp_tls_init();
	if (!tls) {
		ESP_LOGE(TAG, "Failed to allocate esp_tls handle!");
		return 0;
	}

	if (esp_tls_conn_http_new_sync(WEB_SERVER_URL, &cfg, tls) == 1) {
		ESP_LOGI(TAG, "Connection established...");
	} else {
		ESP_LOGE(TAG, "Connection failed...");
		goto exit;
	}
#else
	struct esp_tls *tls = esp_tls_conn_http_new(WEB_SERVER_URL, &cfg);

	if (tls != NULL) {
		ESP_LOGI(TAG, "Connection established...");
	} else {
		ESP_LOGE(TAG, "Connection failed...");
		goto exit;
	}
#endif

	size_t written_bytes = 0;
	do {
		ret = esp_tls_conn_write(tls,
								 REQUEST + written_bytes,
								 strlen(REQUEST) - written_bytes);
		if (ret >= 0) {
			ESP_LOGI(TAG, "%d bytes written", ret);
			written_bytes += ret;
		} else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
			ESP_LOGE(TAG, "esp_tls_conn_write  returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
			goto exit;
		}
	} while (written_bytes < strlen(REQUEST));

	ESP_LOGI(TAG, "Reading HTTP response...");

	do {
		len = sizeof(buf) - 1;
		memset(buf, 0x00, sizeof(buf));
		ret = esp_tls_conn_read(tls, buf, len);

		if (ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ) {
			continue;
		}

		if (ret < 0) {
			ESP_LOGE(TAG, "esp_tls_conn_read  returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
			break;
		}

		if (ret == 0) {
			ESP_LOGI(TAG, "connection closed");
			break;
		}

		ESP_LOGI(TAG, "%d bytes read", ret);

		char *addr;
		if (request == 0) {
			addr = strstr(buf, "Content-Length:");
			ESP_LOGI(TAG, "addr=%p", addr);
			if (addr != NULL) {
				char wk[32];
				memset(wk, 0x00, sizeof(wk));
				for (int i=0;i<32;i++) {
					ESP_LOGI(TAG, "addr[i+16]=0x%x", addr[i+16]);
					wk[i] = addr[i+16];
					if (addr[i+17] == 0x0d) break;
				}
				content_length = atoi(wk);
				ESP_LOGI(TAG, "wk=[%s] content_length=%d", wk, content_length);
				break;
			} 
		} else {
			int start = 0;
			if (header_flag == 0) {
				int marker = 0;
				for (int i = 0; i < ret; i++) {
					ESP_LOGD(TAG, "buf[%d]=0x%x", i, buf[i]);
					if (buf[i] == 0x0d) {
						marker++;
					} else if (buf[i] == 0x0a) {
						marker++;
						ESP_LOGD(TAG, "marker=%d", marker);
						if (marker == 4) {
							start = i+1;
							header_flag = 1;
							break;
						}
					} else {
						marker = 0;
					}
				}
			}
			ESP_LOGI(TAG, "start=%d", start);
			for (int i = start; i < ret; i++) {
				content[index++] = buf[i];
				content[index] = 0x00;
			}
			ESP_LOGI(TAG, "index=%d", index);
		}

		/* Print response directly to stdout as it is read */
		for (int i = 0; i < ret; i++) {
			putchar(buf[i]);
		}
		putchar('\n'); // JSON output doesn't have a newline at end
	} while (1);

exit:
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
	esp_tls_conn_destroy(tls);
#else
	esp_tls_conn_delete(tls);
#endif
	return content_length;
}


size_t http_client_content_length()
{
	esp_tls_cfg_t cfg = {
		.cacert_buf = (const unsigned char *) server_root_cert_pem_start,
		.cacert_bytes = server_root_cert_pem_end - server_root_cert_pem_start,
	};
	size_t content_length = https_get_request(0, cfg, WEB_URL, HOWSMYSSL_REQUEST, NULL);
	return content_length;
}

size_t http_client_content_body(char * buf)
{
	esp_tls_cfg_t cfg = {
		.cacert_buf = (const unsigned char *) server_root_cert_pem_start,
		.cacert_bytes = server_root_cert_pem_end - server_root_cert_pem_start,
	};
	size_t content_length = https_get_request(1, cfg, WEB_URL, HOWSMYSSL_REQUEST, buf);
	return content_length;
}

void http_client()
{
	// Get content length
	size_t content_length;
	content_length = http_client_content_length();
	ESP_LOGI(TAG, "content_length=%d", content_length);

	// Allocate buffer to store response of http request from event handler
	char *buffer;
	buffer = (char *) malloc(content_length+1);
	if (buffer == NULL) {
		ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
		while(1) {
			vTaskDelay(1);
		}
	}

	// Get content body
	http_client_content_body(buffer);
	ESP_LOGI(TAG, "\n[%s]", buffer);

	// Deserialize JSON
	ESP_LOGI(TAG, "Deserialize.....");
	cJSON *root = cJSON_Parse(buffer);
	JSON_Analyze(root);
	cJSON_Delete(root);
	free(buffer);
}


void app_main()
{
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Initialize WiFi
	wifi_init_sta();

	// http client request
	http_client(); 
}
