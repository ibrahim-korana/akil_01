#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_timer.h"
#include "storage.h"

const char* MAIN_TAG = "MAIN";

#include "core.h"
#include "tools/config.h"
#include "home_i2c.h"
#include "home_8574.h"
#include "home_8563.h"
#include "home_lm75.h"
#include "home_ft6x36.h"
#include "backlight.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "lvgl.h"
#include "esp_io_expander.h"
#include "iptool.h"
#include "buzzer.h"
#include "cJSON.h"
#include "comp/rs485.h"
#include "lvgl.h"

#include "comp/language.h"
#include "comp/wifi.h"
#include "comp/udp_server.h"
#include "tftp_ota_server.h"
#include "tftp_disk_server.h"


ESP_EVENT_DEFINE_BASE(LWIFI_EVENTS);
ESP_EVENT_DEFINE_BASE(UDP_EVENT);
ESP_EVENT_DEFINE_BASE(AKIL_EVENTS);

status_t GlobalConfig = {};
Storage disk = Storage();
Home_i2c bus = Home_i2c();
Home_8574 h8574 = Home_8574(); 
Home_8563 h8563 = Home_8563();
Home_LM75 hlm75 = Home_LM75();
Home_FT6X36 tp = Home_FT6X36(GPIO_NUM_NC);
Backlight backlight = Backlight(PIN_NUM_BK_LIGHT);
RS485_config_t rs485_cfg={};
RS485 rs485 = RS485();
esp_lcd_panel_handle_t panel_handle = NULL;
lv_disp_t *display = NULL;
static lv_disp_drv_t disp_drv; 
static lv_disp_draw_buf_t disp_buf;
Language Lang = Language();
Udp_Server udp_server = Udp_Server(); 
lamps_t *lamp_list = NULL;
TftpOtaServer ota_server(79);
TftpDiskServer disk_server(69);


bool ROLE1_CHANGE = false;
bool ROLE2_CHANGE = false;

void rs485_callback(char *data, uint8_t sender, transmisyon_t transmisyon);

#include "tools/events.cpp"
#include "tools/mainconfig.cpp"
#include "tools/udp_server_callback.cpp"
#include "screen/startup.h"
#include "screen/ana_ekran.h"

#include "tools/tools.cpp"

static void status_task(void *arg)
{
    if (GlobalConfig.wifi_active==1 || GlobalConfig.rs485_active==1)
    {
        if (GlobalConfig.wifi_active==1) 
          while(GlobalConfig.wifi_connected==0) vTaskDelay(1000/portTICK_PERIOD_MS);
        vTaskDelay(5000/portTICK_PERIOD_MS);  
        ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_GET_STATUS, NULL, 0, 10/portTICK_PERIOD_MS));                              
    }
    ESP_LOGI("MAIN","Status Isteniyor");
    vTaskDelete(NULL);
}

void screen_exit(void *arg, uint8_t Scr_num)
{
    if (Scr_num==1) { 
        //Startup Sayfasından cıktı
        startup_destroy(); 
        Read_Temperature();
        lv_msg_subscribe(MSG_TEMPERATURE, temparature_callback, NULL);
        anaekran_init(&GlobalConfig, screen_exit, &disk, &Lang, lamp_list);
        anaekran_load(); 
        if(GlobalConfig.wifi_active==1) 
        {
            esp_wifi_set_ps(WIFI_PS_NONE); 
            ESP_LOGI(MAIN_TAG, "WIFI starting");
            initialise_wifi(&GlobalConfig);
            wifi_sta();
            esp_wifi_set_ps(WIFI_PS_NONE); 
            udp_server.start(0xD002);
            ESP_ERROR_CHECK(esp_event_handler_register(UDP_EVENT, UDP_EVENT_RECV, on_udp_recv, NULL));
            ota_server.ota_start();
            disk_server.disk_start();
            xTaskCreate(status_task,"status_task",2048,NULL,5,NULL);
        }             
    }
}

extern "C" void app_main()
{
    esp_log_level_set("phy_init", ESP_LOG_NONE); 
    esp_log_level_set("wifi", ESP_LOG_NONE); 
    esp_log_level_set("wifi_init", ESP_LOG_NONE);
    

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_LOGI(MAIN_TAG,"START");

    pre_config(&disk, &GlobalConfig);
    mainconfig();
    strcpy((char *)GlobalConfig.current_server_ip, "0");
    GlobalConfig.have_current_server_ip = false;

    read_lamps();
   // list_lamp();

    Lang.init("TR-tr",&disk);

    ESP_ERROR_CHECK(esp_event_handler_instance_register(LWIFI_EVENTS, ESP_EVENT_ANY_ID, wifi_change_events, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(AKIL_EVENTS, ESP_EVENT_ANY_ID, akil_change_events, NULL, NULL));

    disk.list("/config","*.*");

    ESP_LOGI(MAIN_TAG, "Startrup Loading");
    startup_init(&GlobalConfig, screen_exit,&Lang); 
    startup_load();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_timer_handler();       
    }

}