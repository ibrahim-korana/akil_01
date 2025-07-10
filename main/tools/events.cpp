
#include "../comp/jsontool.h"


void send_transmisyon_task(void *arg)
{    
    char *tmp = (char *)arg;
    if (tmp!=NULL)
    {
        char *data;
        asprintf(&data,"%s",tmp);
        
            if (GlobalConfig.wifi_connected==1 && GlobalConfig.udp_active==1) 
            {
                
                //UDP üzerinden gönder
                if (!GlobalConfig.have_current_server_ip)
                 {
                    udp_server.send_broadcast((uint8_t *)data,strlen(data));
                    ESP_LOGI("EVENT","Broadcast UDP >> %s",data);
                 } else {
                    remote_t *rem = udp_server.create_remote((char*)GlobalConfig.current_server_ip);
                    udp_server.send_unicast(rem,(uint8_t *)data,strlen(data));
                    ESP_LOGI("EVENT","Unicast UDP >> %s",data);
                 }
                
            }
            if (GlobalConfig.rs485_active==1)
            {
                //RS485 üzerinden gönder
                while(rs485.is_busy()) vTaskDelay(1/portTICK_PERIOD_MS);
                rs485.Sender(data,254); 
                ESP_LOGI("EVENT","Send RS485 >> %s",data);           
            }
        
        free(data);
    }
    vTaskDelete(NULL);
}

void temparature_callback(void * s, lv_msg_t * m)
{
    //MSG_TEMPERATURE geldiğinde değişiklik varsa gerekli arayuzlere gönderir
    if (GlobalConfig.termostat_local==0) return;
    static uint8_t tmp_temparature = 0;
    uint8_t cur_temp = (uint8_t)GlobalConfig.current_temp;
    if (cur_temp!=tmp_temparature)
    {
        tmp_temparature = cur_temp;
        cJSON *root1 = cJSON_CreateObject();
        cJSON_AddStringToObject(root1, "com", "E_REQ");
        cJSON_AddNumberToObject(root1, "id", 0);
        cJSON *drm = cJSON_CreateObject();
        uint8_t tm = GlobalConfig.current_temp;
        cJSON_AddItemToObject(root1,"durum",drm);
        cJSON_AddNumberToObject(drm, "temp", tm);
        cJSON_AddNumberToObject(drm, "stemp", GlobalConfig.current_set);
        char *nm;
        asprintf(&nm,"TEMP%02d",GlobalConfig.sensor_no);
        cJSON_AddStringToObject(drm, "irval", nm);
        char *dat1 = cJSON_PrintUnformatted(root1); 
        xTaskCreate(send_transmisyon_task,"Send_Temp",4096,(char*)dat1,5,NULL);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        cJSON_free(dat1);
        cJSON_Delete(root1); 
        free(nm);        
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

static void get_status_task(void *arg)
{
    lamps_t *temp = lamp_list;
    while(temp)
    {
        if (strcmp((char *)temp->name,"lamp")==0)
        {
            cJSON *root1 = cJSON_CreateObject();
            cJSON_AddStringToObject(root1, "com", "V_STAT");
            cJSON_AddNumberToObject(root1, "id", 0);
            cJSON_AddStringToObject(root1, "name", (char *)temp->iname);
            char *dat1 = cJSON_PrintUnformatted(root1);
            xTaskCreate(send_transmisyon_task,"Send_Temp",4096,(char*)dat1,5,NULL);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            cJSON_Delete(root1); 
        } 
        temp = (lamps_t *) temp->next;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    cJSON *root1 = cJSON_CreateObject();
    cJSON_AddStringToObject(root1, "com", "V_STAT");
    cJSON_AddNumberToObject(root1, "id", 0);
    char *nm;
    asprintf(&nm,"TEMP%02d",GlobalConfig.sensor_no);
    cJSON_AddStringToObject(root1, "name", nm);
    char *dat1 = cJSON_PrintUnformatted(root1);
    xTaskCreate(send_transmisyon_task,"Send_Temp",4096,(char*)dat1,5,NULL);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    free(nm);
    cJSON_Delete(root1);

    vTaskDelete(NULL);
}

static void get_status_termostat_task(void *arg)
{
    cJSON *root1 = cJSON_CreateObject();
    cJSON_AddStringToObject(root1, "com", "V_STAT");
    cJSON_AddNumberToObject(root1, "id", 0);
    char *nm;
    asprintf(&nm,"TEMP%02d",GlobalConfig.sensor_no);
    cJSON_AddStringToObject(root1, "name", nm);
    char *dat1 = cJSON_PrintUnformatted(root1);
    xTaskCreate(send_transmisyon_task,"Send_Temp",4096,(char*)dat1,5,NULL);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    free(nm);
    cJSON_Delete(root1);

    vTaskDelete(NULL);
}

void akil_change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    if (id==AEV_TERMOSTAT_CHANGE) {
        ROLE1_CHANGE = true;
    }
    if(id==AEV_GET_STATUS_TERMOSTAT)
    {
        xTaskCreate(get_status_termostat_task,"Get_Status_term_task",2048,NULL,5,NULL);
    }

    if (id==AEV_WIFI_CONNECT) {
        esp_wifi_disconnect();
        esp_wifi_connect();
    }
    if(id==AEV_GET_STATUS)
    {
        xTaskCreate(get_status_task,"Get_Status_task",2048,NULL,5,NULL);
    }

    if (id==AEV_CUR_UP_PRESSED) {
        lamps_t *temp = (lamps_t *) event_data;
        cJSON *root1 = cJSON_CreateObject();
        cJSON_AddStringToObject(root1, "com", "E_REQ");
        cJSON_AddNumberToObject(root1, "id", 0);
        cJSON *drm = cJSON_CreateObject();
        cJSON_AddItemToObject(root1,"durum",drm);
        cJSON_AddStringToObject(drm, "irval", temp->iname);
        cJSON_AddNumberToObject(drm, "status", 2);
        char *dat1 = cJSON_PrintUnformatted(root1); 
        xTaskCreate(send_transmisyon_task,"Send_Temp",4096,(void *)dat1,5,NULL);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        cJSON_free(dat1);
        cJSON_Delete(root1); 

        ESP_LOGI("EVENT","Cur UP Pressed %s %s",temp->name,temp->iname);
        //Json hazırlayıp send_transmisyon_task ile gonder
    }
    if (id==AEV_CUR_DOWN_PRESSED) {
        lamps_t *temp = (lamps_t *) event_data;
        cJSON *root1 = cJSON_CreateObject();
        cJSON_AddStringToObject(root1, "com", "E_REQ");
        cJSON_AddNumberToObject(root1, "id", 0);
        cJSON *drm = cJSON_CreateObject();
        cJSON_AddItemToObject(root1,"durum",drm);
        cJSON_AddStringToObject(drm, "irval", temp->iname);
        cJSON_AddNumberToObject(drm, "status", 0);
        char *dat1 = cJSON_PrintUnformatted(root1); 
        xTaskCreate(send_transmisyon_task,"Send_Temp",4096,(void *)dat1,5,NULL);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        cJSON_free(dat1);
        cJSON_Delete(root1); 
        ESP_LOGI("EVENT","Cur DOWN Pressed %s %s",temp->name,temp->iname);
        //Json hazırlayıp send_transmisyon_task ile gonder
    }
    if (id==AEV_LAMP_PRESSED) {
        lamps_t *temp = (lamps_t *) event_data;
        if (temp->local==0)
        {
        cJSON *root1 = cJSON_CreateObject();
        cJSON_AddStringToObject(root1, "com", "E_REQ");
        cJSON_AddNumberToObject(root1, "id", 0);
        cJSON *drm = cJSON_CreateObject();
        cJSON_AddItemToObject(root1,"durum",drm);
        cJSON_AddBoolToObject(drm,"stat",(!temp->state==1)?true:false);
        cJSON_AddStringToObject(drm, "irval", temp->iname);
        char *dat1 = cJSON_PrintUnformatted(root1); 
        xTaskCreate(send_transmisyon_task,"Send_Temp",4096,(void *)dat1,5,NULL);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        cJSON_free(dat1);
        cJSON_Delete(root1); 
        //Json hazırlayıp send_transmisyon_task ile gonder
        } else {
            if (temp->local==1) {
                GlobalConfig.r1_stat = !GlobalConfig.r1_stat;
                temp->state = GlobalConfig.r1_stat;
                temp->dev_id = 0;
                ROLE1_CHANGE = true;
                lv_msg_send(MSG_LAMP_BUTTON_ANSWER, (void *)temp);
            }
            if (temp->local==2) {
                GlobalConfig.r2_stat = !GlobalConfig.r2_stat;
                temp->state = GlobalConfig.r2_stat;
                temp->dev_id = 0;
                ROLE2_CHANGE = true;
                lv_msg_send(MSG_LAMP_BUTTON_ANSWER, (void *)temp);
            }
        }
    }
    if (id==AEV_OUT_TEMPSET_CHANGED)
    {
        //Termostat Set degişti gönder
        if (GlobalConfig.termostat_local==0)
        {
            cJSON *root1 = cJSON_CreateObject();
            cJSON_AddStringToObject(root1, "com", "E_REQ");
            cJSON_AddNumberToObject(root1, "id", 0);
            cJSON *drm = cJSON_CreateObject();
            uint8_t tm = GlobalConfig.current_temp;
            cJSON_AddItemToObject(root1,"durum",drm);
            cJSON_AddNumberToObject(drm, "temp", tm);
            cJSON_AddNumberToObject(drm, "stemp", GlobalConfig.current_set);
            char *nm;
            asprintf(&nm,"TEMP%02d",GlobalConfig.sensor_no);
            cJSON_AddStringToObject(drm, "irval", nm);
            char *dat1 = cJSON_PrintUnformatted(root1); 
            xTaskCreate(send_transmisyon_task,"Send_Temp",4096,(char*)dat1,5,NULL);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            cJSON_free(dat1);
            cJSON_Delete(root1); 
            free(nm);
        }
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


void rs485_callback(char *data, uint8_t sender, transmisyon_t transmisyon)
{
   ESP_LOGI(MAIN_TAG,"RS485 GELEN << %s",data);

   cJSON *rcv = cJSON_Parse(data);
   if (rcv==NULL) return; 
   char *command = (char *)calloc(1,20); 
   JSON_getstring(rcv,"com", command,19);
   if (strcmp(command,"stat")==0)
    {
        char *nm = (char *)calloc(1,20); 
        uint8_t st=0,stmp = 0, iid = 0, sta=0, col = 0;
        uint16_t mm = 0;
        JSON_getstring(rcv,"name", nm,19); 
        JSON_getint(rcv,"stat",&st);
        JSON_getint(rcv,"stemp",&stmp);
        JSON_getint(rcv,"dev_id",&iid);
        JSON_getint(rcv,"status",&sta);
        JSON_getint(rcv,"coun",&col);
        JSON_getint(rcv,"color",&mm);


        if (strncmp(nm,"AN",2)==0)
        { 
            lamps_t *temp = lamp_list;
            while(temp)
            {
                if (strcmp(temp->iname,nm)==0)
                {
                    temp->state = st;
                    temp->dev_id = iid;
                    lv_msg_send(MSG_LAMP_BUTTON_ANSWER, (void *)temp);
                    break;
                }
                temp = (lamps_t *) temp->next;
            }
                    
        }
        if (strncmp(nm,"TE",2)==0)
        {   
             char *pp;
            asprintf(&pp,"TEMP%02d",GlobalConfig.sensor_no);

            if (strcmp(nm,pp)==0)
            {
            GlobalConfig.current_set = stmp;
            GlobalConfig.temp_current_stat = col;
            //GlobalConfig.temp_stat = sta;
            if (mm>0) {
                GlobalConfig.max_temp = (mm >> 8);
                GlobalConfig.min_temp = mm & 0x00FF;
            }
            
            lv_msg_send(MSG_IN_TEMPSET_CHANGED, NULL);
            ESP_LOGI("EVENT","Termostat Set %d Status %d Min %d Max %d",GlobalConfig.current_set,GlobalConfig.temp_current_stat,GlobalConfig.min_temp,GlobalConfig.max_temp );

        }
        free(pp);
           // ESP_ERROR_CHECK(esp_event_post(ALARM_EVENTS, EVENT_POST_SET, &GlobalConfig.current_set, sizeof(uint8_t), portMAX_DELAY));
           // ESP_ERROR_CHECK(esp_event_post(ALARM_EVENTS, EVENT_CONFIG_SAVE, NULL, 0, portMAX_DELAY)); 
                    
        }
    }

   
   
    
}