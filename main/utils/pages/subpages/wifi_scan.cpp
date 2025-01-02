

#include "lvgl.h"

void Wifi_Ayarlari::scan_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    if (id == WIFI_EVENT_SCAN_DONE)
    {
        Wifi_Ayarlari *Self = (Wifi_Ayarlari *)handler_args;

        ESP_LOGI("WIFI_SCAN","Scan done");
        uint16_t ap_count = 0;
        esp_err_t err_code = esp_wifi_scan_get_ap_num(&ap_count);
        if (err_code != ESP_OK)
        {
            ESP_LOGE("WIFI_SCAN","esp_wifi_scan_get_ap_num() failed, error: %d", err_code);
            return;
        }
        ESP_LOGI("WIFI_SCAN","Number of access points found: %d", ap_count);
        wifi_ap_record_t *ap_list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * ap_count);
        if (!ap_list)
        {
            ESP_LOGE("WIFI_SCAN","Failed to allocate memory for access point list");
            return;
        }
        err_code = esp_wifi_scan_get_ap_records(&ap_count, ap_list);
        if (err_code != ESP_OK)
        {
            ESP_LOGE("WIFI_SCAN","esp_wifi_scan_get_ap_records() failed, error: %d", err_code);
            free(ap_list);
            return;
        }
        lv_obj_t *btn;
        for (int i = 0; i < ap_count; i++)
        {
            btn = lv_list_add_btn(Self->list, LV_SYMBOL_WIFI, (char*)ap_list[i].ssid);
            lv_obj_set_style_text_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
            lv_obj_set_style_bg_color(btn, lv_color_hex(0x000000), LV_PART_MAIN);       
            lv_obj_add_event_cb(btn, Self->btn_event_handler, LV_EVENT_CLICKED, Self);
            ESP_LOGI("WIFI_SCAN","SSID: %2d %3d %s", ap_list[i].primary,ap_list[i].rssi, ap_list[i].ssid);          
        }
        free(ap_list);
    }
}

esp_err_t Wifi_Ayarlari::wifi_open_start()
{
    
    ESP_LOGI("WIFI","Wifi Scan Start");
    esp_err_t err_code = esp_wifi_scan_start(NULL, false);
    if (err_code != ESP_OK)
    {
        ESP_LOGE("WIFI_SCAN","esp_wifi_scan_start() failed, error: %d", err_code);
        
    }
    return err_code;
}