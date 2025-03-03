
#ifndef _CORE_H
#define _CORE_H

#include "stdint.h"
#include "esp_event.h"


ESP_EVENT_DECLARE_BASE(AKIL_EVENTS);

enum {
    MSG_CLOSE_CLK,
    MSG_IN_TEMPSET_CHANGED,
    MSG_TEMPERATURE,
};

enum {
    AEV_BUTTON_PRESSED,
    AEV_PAGE_CHANGED,
    AEV_GOT_IP,
    AEV_DISCONNECTED,
    AEV_PCHANGE_OFF,
    AEV_PCHANGE_ON,
    AEV_BACKLITE_CHANGED,
    AEV_BACKLITE_CHANGING,
    AEV_SAVER_CHANGED,
    AEV_CONFIG_SAVE,
    AEV_CONFIG_SAVE_NORESET,
    AEV_CONFIG_SAVE_DEFULT,
    AEV_OUT_TEMPSET_CHANGED,
    AEV_LAMP_PRESSED, 
    AEV_CUR_UP_PRESSED,
    AEV_CUR_DOWN_PRESSED,  
    AEV_GET_STATUS,
    AEV_WIFI_CONNECT,
    AEV_GET_STATUS_TERMOSTAT,
    AEV_TERMOSTAT_CHANGE,
};

enum {
    MSG_LAMP_BUTTON_PRESSED=0x1000,
    MSG_LAMP_BUTTON_ANSWER,
    MSG_CUR_UP_BUTTON_PRESSED,
    MSG_CUR_DOWN_BUTTON_PRESSED,
    MSG_CUR_BUTTON_ANSWER,
};

typedef struct {
    uint8_t home_default;
    uint8_t backlight;
    uint8_t screen_saver;
    char cihaz_adi[45];
    char current_ip[24];
    char current_gw[24];

    uint8_t max_temp;
    uint8_t min_temp;
    uint8_t isi_kalibrasyon;
    uint8_t isi_okuma_suresi;
    uint8_t sensor_no;
    uint8_t current_set;
    float current_temp;
    uint8_t temp_stat;
    uint8_t temp_current_stat;
    uint8_t termostat_local;

    uint8_t rs485_active; //Transmisyon olarak kullan
    uint8_t udp_active;   //Transmisyon olarak kullan
    uint8_t wifi_active;
    uint8_t wifi_connected;
    char wifi_ssid[16];
    char wifi_pass[16];
    uint8_t time_int_sync;
    uint8_t retry_count;
    char current_server_ip[24];
    bool have_current_server_ip;

    //-------------------
    uint8_t active_page;
    uint8_t max_page;
    char security_adm_pass[6];
    bool r1_stat;
    bool r2_stat;

} status_t;

typedef struct {
    uint8_t status;
} dev_status_t;

typedef struct lamps {
    char name[16];
    char text[20];
    char iname[16];
    uint8_t id;  
    uint8_t local;    
    uint8_t width;
    uint8_t height;
    uint8_t new_track; 
    //-------------------
    void *button;
    bool state;   
    uint8_t dev_id;
    void *next; 
} lamps_t;

typedef void (*exit_callback_t)(void *arg, uint8_t scr_num);

#define BTN_TICK 1
#define LINE_OPASITY 50
#define IMG_OFF_OPASITY 60
#define IMG_ON_OPASITY 200

typedef struct {
  union {
    struct {
      uint8_t isitma_sogutma:1; //0 isitma 1 sogutma
      uint8_t auto_manuel:1; //0 auto 1 manuel
      uint8_t motor_aktif_pasif:1; //0 pasif 1 aktif; manulede roleyi aç kapat 
      uint8_t start_stop:1; //1 start 0 stop;
      uint8_t role_durumu:1; //1 start 0 stop rölenin aktifteki durumu
      uint8_t gece_gunduz:1; //1 gece 0 gündüz
      uint8_t bos:2;
          } st;
	    uint8_t int_st;
  };
} air_status_t;

typedef struct {
   air_status_t stat;
   float temp;
   float set_temp;     
} air_t;

#endif