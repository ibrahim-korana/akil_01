#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_event.h"
#include "storage.h"
#include "../core.h"

#define LCD_PIXEL_CLOCK_HZ     (16 * 1000 * 1000)
#define PIN_NUM_BK_LIGHT       GPIO_NUM_0
#define PIN_NUM_HSYNC          21
#define PIN_NUM_VSYNC          47
#define PIN_NUM_DE             48
#define PIN_NUM_PCLK           45
#define PIN_NUM_DATA0          14 // B0
#define PIN_NUM_DATA1          13 // B1
#define PIN_NUM_DATA2          12 // B2
#define PIN_NUM_DATA3          11 // B3
#define PIN_NUM_DATA4          10 // B4
#define PIN_NUM_DATA5          9 // G0
#define PIN_NUM_DATA6          46 // G1
#define PIN_NUM_DATA7          3 // G2
#define PIN_NUM_DATA8          20 // G3
#define PIN_NUM_DATA9          19 // G4
#define PIN_NUM_DATA10         8 // G5
#define PIN_NUM_DATA11         18  // R0
#define PIN_NUM_DATA12         17  // R1
#define PIN_NUM_DATA13         16 // R2
#define PIN_NUM_DATA14         15 // R3
#define PIN_NUM_DATA15         7 // R4
#define PIN_NUM_DISP_EN        -1

#define LCD_H_RES              480
#define LCD_V_RES              480

#define LCD_MOSI        -1
#define LCD_MISO        40
#define LCD_SCLK        39
#define LCD_CS          38
#define LCD_HOST        SPI3_HOST
#define LCD_RESET       42 

#define VSYNC_BACK_PORCH    8     
#define VSYNC_FRONT_PORCH   4
#define VSYNC_PULSE_WITH    1
#define HSYNC_BACK_PORCH    40
#define HSYNC_FRONT_PORCH   20
#define HSYNC_PULSE_WITH    1
#define DE_IDLE_HIGH        0    
#define PCLK_IDLE_HIGH      0
#define HSYNC_POLARITY      1
#define VSYNC_POLARITY      1

#define I2C (i2c_port_t) 0
#define SDA      GPIO_NUM_5
#define SCL      GPIO_NUM_4
#define TP_INT   GPIO_NUM_6

#define ROLE1       0      
#define ROLE2       1
#define TERMOSTAT   4
#define BUZZER      5
//#define DISPLAY     6

#define ANAHTAR1    2
#define ANAHTAR2    3

#define RS485_RX    2 
#define RS485_TX    41
#define RS485_DIR   1

#define LVGL_TICK 2

#define GLOBAL_FILE "/config/global.bin"
#define STATUS_FILE "/config/status.bin"

void pre_config(Storage *dsk, status_t *gconf);
void global_default_config();


#endif