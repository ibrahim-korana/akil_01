
void wifi_change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    if (id==EVENT_LWIFI_GOTIP)
    {
        ip_t* event = (ip_t*) event_data;
        char *mm = (char *)calloc(1,20);
        IPAddr aa = IPAddr();
        strcpy(mm,aa.to_string(event->ip));
        ESP_LOGI("EVENT", "Alınan ip: %s",mm );
        strcpy(GlobalConfig.current_ip,mm);
        strcpy(mm,aa.to_string(event->gw));
        strcpy(GlobalConfig.current_gw,mm);
        ESP_LOGI("EVENT", "Gateway : %s",mm ); 
        free(mm);
       // ESP_ERROR_CHECK(esp_event_post(ALARM_EVENTS, EVENT_GOT_IP, NULL, 0, portMAX_DELAY));
    }
    if (id==EVENT_LWIFI_TIME_SYNC)
    {
        ESP_LOGI("EVENT","Time SYNC");
        struct tm timeinfo;
        //useralarm.get_time(&timeinfo);
      //  ESP_ERROR_CHECK(esp_event_post(ALARM_EVENTS, EVENT_DAY_CHANGE, &timeinfo, sizeof(timeinfo), portMAX_DELAY));
      //  ESP_ERROR_CHECK(esp_event_post(ALARM_EVENTS, EVENT_MIN_CHANGE, &timeinfo, sizeof(timeinfo), portMAX_DELAY));
    }
    if (id==EVENT_LWIFI_CONNECTED)
    {
        GlobalConfig.wifi_connected = 1;
       // ESP_ERROR_CHECK(esp_event_post(ALARM_EVENTS, EVENT_WIFI_CONNECTED, NULL, 0, portMAX_DELAY));
    }
    if (id==EVENT_LWIFI_DISCONNECTED)
    {
        GlobalConfig.wifi_connected = 0;
      //  ESP_ERROR_CHECK(esp_event_post(ALARM_EVENTS, EVENT_WIFI_DISCONNECTED, NULL, 0, portMAX_DELAY));
    }
}