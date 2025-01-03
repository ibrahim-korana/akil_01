
#include "config.h"

#include "nvs_flash.h"
#include "nvs.h"
#include "ArduinoJson.h"

Storage *GDisk;
status_t *GConfig; 
lamps_t *lamps = NULL;

const char *CONFIGTAG = "CONFIG";

void global_default_config()
{   
    GConfig->home_default = 1; 
    GConfig->backlight = 128;
    GConfig->screen_saver = 15;
    strcpy((char*)GConfig->cihaz_adi, "Akil Anahtar");
    strcpy((char*)GConfig->current_ip, "");
    strcpy((char*)GConfig->current_gw, "");
    GConfig->max_temp = 31;
    GConfig->min_temp = 18;
    GConfig->isi_kalibrasyon = 100;
    GConfig->isi_okuma_suresi = 50;
    GConfig->current_temp = 0.0;
    GConfig->current_set = 18;

    GConfig->rs485_active = 0;
    GConfig->udp_active = 0;
    GConfig->wifi_active = 0;
    GConfig->wifi_connected = 0;
    GConfig->time_int_sync = 0;

    GConfig->active_page = 1; //ilk sayfa
    GConfig->max_page = 4; //1-4 arası sayfa var
    strcpy((char*)GConfig->security_adm_pass,"1210");

    
   // strcpy((char*)GConfig->wifi_ssid,"ice");
   // strcpy((char*)GConfig->wifi_pass,"ice12345");
     strcpy((char*)GConfig->wifi_ssid,"IMS_YAZILIM");
     strcpy((char*)GConfig->wifi_pass,"mer6514a4c");

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


void pre_config(Storage *dsk, status_t *gconf, lamps_t *lamp_list)
{
    GConfig = gconf;
    GDisk = dsk;
    lamps = lamp_list;
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

void list_lamp(void)
{
    lamps_t *target = lamps;
    ESP_LOGI(CONFIGTAG,"     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    ESP_LOGI(CONFIGTAG,"     LAMPS");
    ESP_LOGI(CONFIGTAG,"     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 
    while(target)
      {
        ESP_LOGI(CONFIGTAG,"     %3d %-20s %d %-20s %d %d %s", 
              target->id,
              target->name,
              target->local,
              target->text,
              target->width,
              target->height,
              target->iname
              );
          target=(lamps_t *)target->next;
      }
    ESP_LOGI(CONFIGTAG,"     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");  
}


void add_lamps(lamps_t *lc )
{
  lc->next = (lamps_t *)lamps;
  lamps = lc;
}

void read_lamps(void)
{
    const char *name1="/config/lamp.json";
    if (GDisk->file_search(name1))
      {
        int fsize = GDisk->file_size(name1); 
        char *buf = (char *) malloc(fsize+5);
        if (buf==NULL) {ESP_LOGE(CONFIGTAG, "memory not allogate"); return;}
        FILE *fd = fopen(name1, "r");
        if (fd == NULL) {ESP_LOGE(CONFIGTAG, "%s not open",name1); return;}
        fread(buf, fsize, 1, fd);
        fclose(fd);
        DynamicJsonDocument doc(3000);
        DeserializationError error = deserializeJson(doc, buf);

        if (error) {
          ESP_LOGE(CONFIGTAG,"deserializeJson() failed: %s",error.c_str());
          return;
        }
        for (JsonObject function : doc["lamps"].as<JsonArray>()) {
          const char* a_name = function["name"];
          const char* i_name = function["rname"];
          const char* a_text = function["text"];
          int a_id = function["id"];
          int a_local = function["local"];           
          int a_w = function["width"]; 
          int a_h = function["height"]; 
          int a_nt = function["new_track"];          
          if (a_w==0) a_w=90;
          if (a_h==0) a_h=90;
         
          lamps_t *bb0 = (lamps_t *)calloc(1, sizeof(lamps_t)); 
          strcpy(bb0->name,a_name);
          strcpy(bb0->text,a_text);
          if (i_name!=NULL) {
              strcpy(bb0->iname,i_name);
          } else strcpy(bb0->iname,"");
          bb0->id = a_id;
          bb0->local = a_local;
          bb0->width = a_w;
          bb0->height = a_h;
          bb0->new_track = a_nt;
          add_lamps(bb0);
        }      
        doc.clear();                       
        free(buf);
      } else ESP_LOGW(CONFIGTAG, "Config File BULUNAMADI !...");
   return;   
}

