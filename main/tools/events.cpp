


uint8_t Global_send_type = 0;
enum {
    SEND_TEMPERATURE,
    SEND_TEMP_SET,
};

void send_transmisyon_task(void *arg)
{
    if (Global_send_type==SEND_TEMPERATURE)
    {
        //Temparature gönder
        uint8_t cur_temp = (uint8_t)GlobalConfig.current_temp;
        printf("Send Temparature %d\n",cur_temp);
        if (GlobalConfig.wifi_connected==1 && GlobalConfig.udp_active==1) 
        {
            //UDP üzerinden gönder
        }
        if (GlobalConfig.rs485_active==1)
        {
            //RS485 üzerinden gönder
        }
    }
    if (Global_send_type==SEND_TEMP_SET)
    {
        //Temparature set gönder
        printf("Send Temp SET \n");
    }

    vTaskDelete(NULL);
}

void temparature_callback(void * s, lv_msg_t * m)
{
    //MSG_TEMPERATURE geldiğinde değişiklik varsa gerekli arayuzlere gönderir
    static uint8_t tmp_temparature = 0;
    uint8_t cur_temp = (uint8_t)GlobalConfig.current_temp;
    if (cur_temp!=tmp_temparature)
    {
        tmp_temparature = cur_temp;
        //Arayuzlere gönder
        Global_send_type = SEND_TEMPERATURE;
        xTaskCreate(send_transmisyon_task,"Send_Temp",2048,NULL,5,NULL);        
    }  
}

void wifi_change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
   // printf("wifi %ld\n",id);

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
        ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_GOT_IP, NULL, 0, portMAX_DELAY));
    }
    if (id==EVENT_LWIFI_TIME_SYNC)
    {
        ESP_LOGI("EVENT","Time SYNC");
       // struct tm timeinfo;
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

void akil_change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    if (id==AEV_OUT_TEMPSET_CHANGED)
    {
        //Termostat Set degişti gönder
        Global_send_type = SEND_TEMP_SET;
        xTaskCreate(send_transmisyon_task,"Send_Temp",2048,NULL,5,NULL);
    }

    if (id==AEV_BUTTON_PRESSED)
    {
       // ESP_LOGI("EVENT","Button Pressed");
       backlight.ss_restart();
    }
    if (id==AEV_PAGE_CHANGED)
    {
        uint8_t *event = (uint8_t*) event_data;
        disk.write_file(GLOBAL_FILE,&GlobalConfig,sizeof(status_t), 0);
        ESP_LOGI("EVENT","Page Changed %d",*event);
    }
    if (id==AEV_GOT_IP)
    {
        ESP_LOGI("EVENT","Got IP");
    }
    if (id==AEV_BACKLITE_CHANGED) 
    {
        uint8_t *data = (uint8_t*) event_data;
        GlobalConfig.backlight = *data;
        backlight.Set(GlobalConfig.backlight);
        disk.write_file(GLOBAL_FILE,&GlobalConfig,sizeof(status_t), 0);
        ESP_LOGI("EVENT","Backlite Changed %d",*data);
    }
    if (id==AEV_BACKLITE_CHANGING) 
    {
        uint8_t *data = (uint8_t*) event_data;
        backlight.Set(*data);
        ESP_LOGI("EVENT","Backlite Changing %d",*data);
    }
    if (id==AEV_SAVER_CHANGED) 
    {
        uint8_t *data = (uint8_t*) event_data;
        GlobalConfig.screen_saver = *data;
        disk.write_file(GLOBAL_FILE,&GlobalConfig,sizeof(status_t), 0);
        backlight.ss_set_duration(GlobalConfig.screen_saver);
        ESP_LOGI("EVENT","Screen Saver Changed %d",*data);
    }
    if(id==AEV_CONFIG_SAVE)
    {
        disk.write_file(GLOBAL_FILE,&GlobalConfig,sizeof(status_t), 0);
        ESP_LOGI("EVENT","Config Save  and RESET");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        esp_restart();
    }
    if(id==AEV_CONFIG_SAVE_NORESET)
    {
        disk.write_file(GLOBAL_FILE,&GlobalConfig,sizeof(status_t), 0);
        ESP_LOGI("EVENT","Config Save");
    }
    if(id==AEV_CONFIG_SAVE_DEFULT)
    {
        global_default_config();
        disk.write_file(GLOBAL_FILE,&GlobalConfig,sizeof(status_t), 0);
        ESP_LOGI("EVENT","Config Save and RESET");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        esp_restart();
    }
}