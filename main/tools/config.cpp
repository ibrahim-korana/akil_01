
#include "config.h"

#include "nvs_flash.h"
#include "nvs.h"
#include "ArduinoJson.h"

Storage *GDisk;
status_t *GConfig; 


const char *CONFIGTAG = "CONFIG";

void global_default_config()
{   
    GConfig->home_default = 1; 
    GConfig->backlight = 128;
    GConfig->screen_saver = 15;
    strcpy((char*)GConfig->cihaz_adi, "Akil Anahtar (LV)");
    strcpy((char*)GConfig->current_ip, "");
    strcpy((char*)GConfig->current_gw, "");
    GConfig->max_temp = 31;
    GConfig->min_temp = 18;
    GConfig->isi_kalibrasyon = 100;
    GConfig->isi_okuma_suresi = 50;
    GConfig->current_temp = 0.0;
    GConfig->current_set = 18;
    GConfig->sensor_no = 1;
    GConfig->retry_count = 20;
    strcpy((char *)GConfig->current_server_ip, "0");
    GConfig->have_current_server_ip = false;
    GConfig->temp_stat = 0;
    GConfig->temp_current_stat=0;
    GConfig->termostat_local = 0;

    GConfig->rs485_active = 0;
    GConfig->udp_active = 1;
    GConfig->wifi_active = 1;
    GConfig->wifi_connected = 0;
    GConfig->time_int_sync = 0;

    GConfig->r1_stat = false;
    GConfig->r2_stat = false;

    GConfig->active_page = 1; //ilk sayfa
    GConfig->max_page = 4; //1-4 arası sayfa var
    strcpy((char*)GConfig->security_adm_pass,"1210");

    
   // strcpy((char*)GConfig->wifi_ssid,"ice");
   // strcpy((char*)GConfig->wifi_pass,"ice12345");
     strcpy((char*)GConfig->wifi_ssid,"ICE_Device");
     strcpy((char*)GConfig->wifi_pass,"ice12345");

    GDisk->file_control(GLOBAL_FILE);
    GDisk->write_file(GLOBAL_FILE, GConfig,sizeof(status_t),0);
    if (!GDisk->file_control(STATUS_FILE)) 
       {
          dev_status_t st = {};
          for (int i=0;i<255;i++)
            {
                GDisk->write_file(STATUS_FILE,&st,sizeof(st),i);
            }
       }
}


void pre_config(Storage *dsk, status_t *gconf)
{
    GConfig = gconf;
    GDisk = dsk;
    
    gpio_config_t intConfig = {};
    intConfig.pin_bit_mask = (1ULL<<LCD_RESET);
    intConfig.mode         = GPIO_MODE_OUTPUT;
    intConfig.pull_up_en   = GPIO_PULLUP_ENABLE;
    intConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    intConfig.intr_type    = GPIO_INTR_DISABLE;
    gpio_config(&intConfig);
    gpio_set_level((gpio_num_t)LCD_RESET,1);

    ESP_LOGI(CONFIGTAG,"NVS Flash Init");
    esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK( nvs_flash_erase() );
            ret = nvs_flash_init();
        }
     ESP_ERROR_CHECK( ret );
    
    ESP_LOGI(CONFIGTAG,"FFS Init");
    ret = GDisk->init();
    ESP_ERROR_CHECK (ret);

    

    GDisk->read_file(GLOBAL_FILE,GConfig,sizeof(status_t), 0);
    if (GConfig->home_default==0 ) {
        //Network ayarları diskte kayıtlı değil. Kaydet.
         global_default_config();
         GDisk->file_control(GLOBAL_FILE);
         GDisk->read_file(GLOBAL_FILE,GConfig,sizeof(status_t),0);
         if (GConfig->home_default==0 ) ESP_LOGW(CONFIGTAG, "Global Initilalize File ERROR !...");
    }
}

