
#ifndef _CORE_H
#define _CORE_H

#include "stdint.h"

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

} status_t;

typedef struct {
    uint8_t status;
} dev_status_t;


typedef void (*exit_callback_t)(void *arg, uint8_t scr_num);

#endif