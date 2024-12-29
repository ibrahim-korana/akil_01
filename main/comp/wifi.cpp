

#include "wifi.h"

#include "esp_wifi.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "lvgl.h"
#include "esp_sntp.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"


#define MAXIMUM_RETRY 10

#define AP_WIFI_SSID "iceAP"
#define AP_WIFI_PASSWORD "ice12345"
#define AP_WIFI_CHANNEL 1


static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

static const char *TAG_WIFI = "wifi_station";

static int s_retry_num = 0;
status_t *config1 = NULL;

void time_sync_notification_cb(struct timeval *tv)
{
	//ESP_LOGI(TAG_WIFI, "Notification of a time synchronization event");
    settimeofday(tv, NULL);   
    ESP_ERROR_CHECK(esp_event_post(LWIFI_EVENTS, EVENT_LWIFI_TIME_SYNC, NULL, 0, portMAX_DELAY)); 
}

void obtain_time(void *arg)
{
    int retry = 0;
	const int retry_count = 10;
    time_t now = 0;
    struct tm timeinfo = {};
	while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
		//ESP_LOGI(TAG_WIFI, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
    if (retry < retry_count) {
       time(&now);
       localtime_r(&now, &timeinfo); 
       //lv_msg_send(MSG_TIME_SYNC, NULL); 
    }
    vTaskDelete(NULL);
}

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_ERROR_CHECK(esp_event_post(LWIFI_EVENTS, EVENT_LWIFI_DISCONNECTED, NULL, 0, portMAX_DELAY)); 
        if (s_retry_num < MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG_WIFI, "%d retry to connect to the AP",MAXIMUM_RETRY-s_retry_num);
        } else wifi_ap();
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ESP_LOGI(TAG_WIFI, "Ip Alındı");
		xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ip_t kk = {};
        kk.ip = event->ip_info.ip.addr;
        kk.gw = event->ip_info.gw.addr;
        kk.netmask = event->ip_info.netmask.addr;
        s_retry_num = 0;
        ESP_ERROR_CHECK(esp_event_post(LWIFI_EVENTS, EVENT_LWIFI_GOTIP, &kk, sizeof(ip_t), portMAX_DELAY)); 
        ESP_ERROR_CHECK(esp_event_post(LWIFI_EVENTS, EVENT_LWIFI_CONNECTED, NULL, 0, portMAX_DELAY)); 
        if (config1->time_int_sync==1)
        {
            ESP_LOGI(TAG_WIFI, "Initializing SNTP");
            esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
            esp_sntp_setservername(0, "pool.ntp.org");
            esp_sntp_set_time_sync_notification_cb(time_sync_notification_cb);
            esp_sntp_init();
            xTaskCreate(obtain_time,"obtain",4096,NULL,2,NULL);  
        } 
	}

}

void initialise_wifi(status_t *cnf)
{
	static bool initialized = false;
	if (initialized) {
		return;
	}
    config1 = cnf;
	ESP_ERROR_CHECK(esp_netif_init());
	wifi_event_group = xEventGroupCreate();
	//ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
	assert(ap_netif);
	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
	assert(sta_netif);
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &event_handler, NULL) );
	ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );

	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );
	ESP_ERROR_CHECK( esp_wifi_start() );
	initialized = true;
}

bool wifi_ap(void)
{
	wifi_config_t wifi_config = {};
	strcpy((char *)wifi_config.ap.ssid,AP_WIFI_SSID);
	strcpy((char *)wifi_config.ap.password, AP_WIFI_PASSWORD);
	wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
	wifi_config.ap.ssid_len = strlen(AP_WIFI_SSID);
	wifi_config.ap.max_connection = 5;
	wifi_config.ap.channel = AP_WIFI_CHANNEL;

	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
	ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &wifi_config) );
	ESP_ERROR_CHECK( esp_wifi_start() );
	ESP_LOGI(TAG_WIFI, "WIFI_MODE_AP started. SSID:%s password:%s channel:%d",
			 AP_WIFI_SSID, AP_WIFI_PASSWORD, AP_WIFI_CHANNEL);
	return ESP_OK;
}

bool wifi_sta()
{
	wifi_config_t wifi_config = {};
	strcpy((char *)wifi_config.sta.ssid, (char*)config1->wifi_ssid);
	strcpy((char *)wifi_config.sta.password, (char*)config1->wifi_pass);

	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
	ESP_ERROR_CHECK( esp_wifi_connect() );

	int bits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
								   pdFALSE, pdTRUE, 100 / portTICK_PERIOD_MS);
	return (bits & CONNECTED_BIT) != 0;
}

bool wifi_apsta()
{
	wifi_config_t ap_config = {};
	strcpy((char *)ap_config.ap.ssid,AP_WIFI_SSID);
	strcpy((char *)ap_config.ap.password, AP_WIFI_PASSWORD);
	ap_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
	ap_config.ap.ssid_len = strlen(AP_WIFI_SSID);
	ap_config.ap.max_connection = 5;
	ap_config.ap.channel = AP_WIFI_CHANNEL;

	wifi_config_t sta_config = {};
	strcpy((char *)sta_config.sta.ssid, (char*)config1->wifi_ssid);
	strcpy((char *)sta_config.sta.password, (char*)config1->wifi_pass);

	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_APSTA) );
	ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &ap_config) );
	ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
	ESP_ERROR_CHECK( esp_wifi_start() );
	ESP_LOGI(TAG_WIFI, "WIFI_MODE_AP started. SSID:%s password:%s channel:%d",
			 AP_WIFI_SSID, AP_WIFI_PASSWORD, AP_WIFI_CHANNEL);

	ESP_ERROR_CHECK( esp_wifi_connect() );
	int bits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
								   pdFALSE, pdTRUE, portMAX_DELAY);
	return (bits & CONNECTED_BIT) != 0;
}

