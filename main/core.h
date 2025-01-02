
#ifndef _CORE_H
#define _CORE_H

#include "stdint.h"
#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(AKIL_EVENTS);

enum {
    MSG_CLOSE_CLK,
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
    uint8_t current_set;
    float current_temp;

    uint8_t rs485_active;
    uint8_t wifi_active;
    uint8_t wifi_connected;
    char wifi_ssid[16];
    char wifi_pass[16];
    uint8_t time_int_sync;


    //-------------------
    uint8_t active_page;
    uint8_t max_page;

} status_t;

typedef struct {
    uint8_t status;
} dev_status_t;


typedef void (*exit_callback_t)(void *arg, uint8_t scr_num);

#endif