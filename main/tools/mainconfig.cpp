
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_io_additions.h"
#include "esp_lcd_st7701.h"
#include "lvgl.h"
#include "esp_io_expander.h"
#include "ArduinoJson.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"


static const st7701_lcd_init_cmd_t lcd_init_cmds[] = {
    {0x01, (uint8_t []){0x00}, 0, 6},
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    {0xC0, (uint8_t []){0x3D, 0x00}, 2, 0},
    {0xC1, (uint8_t []){0x08, 0x04}, 2, 0},
    {0xC3, (uint8_t []){0x00, 0x10, 0x08}, 3, 0},
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x13}, 5, 0},
    {0xEF, (uint8_t []){0x08}, 1, 0}, 
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    {0xC2, (uint8_t []){0x07, 0x0A}, 2, 0},
    {0xCC, (uint8_t []){0x30}, 1, 0},
    {0xB0, (uint8_t []){0x40, 0x07, 0x53, 0x0E, 0x12, 0x07, 0x0A, 0x09, 0x09, 0x26, 0x05, 0x10, 0x0D, 0x6E, 0x3B, 0xD6}, 16, 0},
    {0xB1, (uint8_t []){0x40, 0x17, 0x5C, 0x0D, 0x11, 0x06, 0x08, 0x08, 0x08, 0x03, 0x12, 0x11, 0x65, 0x28, 0xE8}, 15, 0},
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x11}, 5, 0},
    {0xB0, (uint8_t []){0x4D}, 1, 0},
    {0xB1, (uint8_t []){0x4C}, 1, 0},
    {0xB2, (uint8_t []){0x81}, 1, 0},
    {0xB3, (uint8_t []){0x80}, 1, 0},
    {0xB5, (uint8_t []){0x4C}, 1, 0},
    {0xB7, (uint8_t []){0x85}, 1, 0},
    {0xB8, (uint8_t []){0x33}, 1, 0},
    {0xC1, (uint8_t []){0x78}, 1, 0},
    {0xC2, (uint8_t []){0x78}, 1, 0},
    {0xD0, (uint8_t []){0x88}, 1, 100},
    {0xE0, (uint8_t []){0x00, 0x00, 0x02}, 3, 0},
    {0xE1, (uint8_t []){0x05, 0x30, 0x00, 0x00, 0x06, 0x30, 0x00, 0x00, 0x0E, 0x30, 0x30}, 11, 0},
    {0xE2, (uint8_t []){0x10, 0x10, 0x30, 0x30, 0xF4, 0x00, 0x00, 0x00, 0xF4, 0x00, 0x00, 0x00}, 12, 0},
    {0xE3, (uint8_t []){0x00, 0x00, 0x11, 0x11}, 4, 0},
    {0xE4, (uint8_t []){0x44, 0x44}, 2, 0},
    {0xE5, (uint8_t []){0x0A, 0xF4, 0x30, 0xF0, 0x0C, 0xF6, 0x30, 0xF0, 0x06, 0xF0, 0x30, 0xF0, 0x08, 0xF2, 0x30, 0xF0}, 16, 0},
    {0xE6, (uint8_t []){0x00, 0x00, 0x11, 0x11}, 4, 0},
    {0xE7, (uint8_t []){0x44, 0x44}, 2, 0},
    {0xE8, (uint8_t []){0x0B, 0xF5, 0x30, 0xF0, 0x0D, 0xF7, 0x30, 0xF0, 0x07, 0xF1, 0x30, 0xF0, 0x09, 0xF3, 0x30, 0xF0}, 16, 0},
    {0xE9, (uint8_t []){0x36, 0x01}, 2, 0}, 
    {0xEB, (uint8_t []){0x00, 0x01, 0xE4, 0xE4, 0x44, 0x88, 0x33}, 7, 0},
    {0xED, (uint8_t []){0x20, 0xFA, 0xB7, 0x76, 0x65, 0x54, 0x4F, 0xFF, 0xFF, 0xF4, 0x45, 0x56, 0x67, 0x7B, 0xAF, 0x02}, 16, 0},
    {0xEF, (uint8_t []){0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F}, 6, 0},
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    {0x11, (uint8_t []){0x00}, 0, 120},
    {0x3A, (uint8_t []){0x55}, 1, 0},
    {0x29, (uint8_t []){0x00}, 0, 100},
};

esp_lcd_panel_handle_t lcd_hard_init(void)
{
    ESP_LOGI(MAIN_TAG, "Install 3-wire SPI panel IO");
    spi_line_config_t line_config = {
        .cs_io_type = IO_TYPE_GPIO,             // Set to `IO_TYPE_GPIO` if using GPIO, same to below
        .cs_gpio_num = LCD_CS,
        .scl_io_type = IO_TYPE_GPIO,
        .scl_gpio_num = LCD_SCLK,
        .sda_io_type = IO_TYPE_GPIO,
        .sda_gpio_num = LCD_MISO,
        .io_expander = NULL,                        // Set to NULL if not using IO expander
    };
    esp_lcd_panel_io_3wire_spi_config_t io_config = ST7701_PANEL_IO_3WIRE_SPI_CONFIG(line_config, 0);
    esp_lcd_panel_io_handle_t io_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_3wire_spi(&io_config, &io_handle));

    #define LCD_NUM_FB 2

    ESP_LOGI(MAIN_TAG, "Install ST7701S panel driver");
    
    esp_lcd_rgb_timing_t tim = {};
            tim.pclk_hz = LCD_PIXEL_CLOCK_HZ;
            tim.h_res = LCD_H_RES;
            tim.v_res = LCD_V_RES;
            tim.hsync_back_porch = HSYNC_BACK_PORCH;
            tim.hsync_front_porch = HSYNC_FRONT_PORCH;
            tim.hsync_pulse_width = HSYNC_PULSE_WITH;
            tim.vsync_back_porch = VSYNC_BACK_PORCH;
            tim.vsync_front_porch = VSYNC_FRONT_PORCH;
            tim.vsync_pulse_width = VSYNC_PULSE_WITH;
            tim.flags.pclk_active_neg = false;

    esp_lcd_rgb_panel_config_t panel_config = {};
        panel_config.data_width = 16; // RGB565 in parallel mode, thus 16bit in width
        panel_config.psram_trans_align = 64;
        panel_config.num_fbs = LCD_NUM_FB;
        panel_config.bounce_buffer_size_px = 10 * LCD_H_RES,
        panel_config.clk_src = LCD_CLK_SRC_DEFAULT;
        panel_config.disp_gpio_num = PIN_NUM_DISP_EN;
        panel_config.pclk_gpio_num = PIN_NUM_PCLK;
        panel_config.vsync_gpio_num = PIN_NUM_VSYNC;
        panel_config.hsync_gpio_num = PIN_NUM_HSYNC;
        panel_config.de_gpio_num = PIN_NUM_DE;
        panel_config.data_gpio_nums[0] = PIN_NUM_DATA0;
        panel_config.data_gpio_nums[1] = PIN_NUM_DATA1;
        panel_config.data_gpio_nums[2] = PIN_NUM_DATA2;
        panel_config.data_gpio_nums[3] = PIN_NUM_DATA3;
        panel_config.data_gpio_nums[4] = PIN_NUM_DATA4;
        panel_config.data_gpio_nums[5] = PIN_NUM_DATA5;
        panel_config.data_gpio_nums[6] = PIN_NUM_DATA6;
        panel_config.data_gpio_nums[7] = PIN_NUM_DATA7;
        panel_config.data_gpio_nums[8] = PIN_NUM_DATA8;
        panel_config.data_gpio_nums[9] = PIN_NUM_DATA9;
        panel_config.data_gpio_nums[10] = PIN_NUM_DATA10;
        panel_config.data_gpio_nums[11] = PIN_NUM_DATA11;
        panel_config.data_gpio_nums[12] = PIN_NUM_DATA12;
        panel_config.data_gpio_nums[13] = PIN_NUM_DATA13;
        panel_config.data_gpio_nums[14] = PIN_NUM_DATA14;
        panel_config.data_gpio_nums[15] = PIN_NUM_DATA15;
        panel_config.timings = tim;
        panel_config.flags.fb_in_psram = true;

    st7701_vendor_config_t vendor_config = {};
        vendor_config.rgb_config = &panel_config;
        vendor_config.init_cmds = lcd_init_cmds;      // Uncomment these line if use custom initialization commands
        vendor_config.init_cmds_size = sizeof(lcd_init_cmds) / sizeof(st7701_lcd_init_cmd_t),
        vendor_config.flags = {
            .use_mipi_interface = 0,
            .mirror_by_cmd = 1,
            .auto_del_panel_io = 0,    
        };
 
    #define RGB_BIT_PER_PIXEL      (16)

    esp_lcd_panel_dev_config_t dev_config = {};
        dev_config.reset_gpio_num = -1;           // Set to -1 if not use
        dev_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;     // Implemented by LCD command `36h`
        dev_config.bits_per_pixel = RGB_BIT_PER_PIXEL;    // Implemented by LCD command `3Ah` (16/18/24)
        dev_config.vendor_config = &vendor_config;

    esp_lcd_panel_handle_t panel_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7701(io_handle, &dev_config, &panel_handle)); 
    return panel_handle;
}

static bool on_vsync_event(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *event_data, void *user_data)
{
    BaseType_t high_task_awoken = pdFALSE;
    #if TEAR_EFFECT
        if (xSemaphoreTakeFromISR(sem_gui_ready, &high_task_awoken) == pdTRUE) {
            xSemaphoreGiveFromISR(sem_vsync_end, &high_task_awoken);
        }
    #endif
    return high_task_awoken == pdTRUE;
}

esp_lcd_panel_handle_t Panel_Install(void)
{
    esp_lcd_panel_handle_t panel_handle = NULL;
    return panel_handle;
}


lv_disp_drv_t Panel_Register_Event(esp_lcd_panel_handle_t hand, esp_lcd_rgb_panel_vsync_cb_t proc)
{
    ESP_LOGI("PANEL_EVENT_REGISTER", "Register event callbacks");
    lv_disp_drv_t disp_drv;
    esp_lcd_rgb_panel_event_callbacks_t cbs = {};
        cbs.on_vsync = proc;
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(hand, &cbs, &disp_drv));
    return disp_drv;
}

static void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    //lvglin oluşturdugu ekran bufferına aktarır
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    #if TEAR_EFFECT
        xSemaphoreGive(sem_gui_ready);
        xSemaphoreTake(sem_vsync_end, portMAX_DELAY);
    #endif
    // pass the draw buffer to the driver
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
    lv_disp_flush_ready(drv);
}

void Read_Temperature(void)
{
    float utemp = 0.0;
    hlm75.read_temperature(&utemp);
    if (utemp>0) GlobalConfig.current_temp = utemp;
    if (GlobalConfig.current_temp>0) 
    {
        float katsayi = 1.0;
        if (GlobalConfig.isi_kalibrasyon>0) katsayi=(GlobalConfig.isi_kalibrasyon/100.0);
        GlobalConfig.current_temp = GlobalConfig.current_temp * katsayi;
        //printf("%.2f\n",GlobalConfig.current_temp);
        //ESP_ERROR_CHECK(esp_event_post(ALARM_EVENTS, EVENT_TEMPARATURE, &GlobalConfig.current_temp, sizeof(float), portMAX_DELAY));
        lv_msg_send(MSG_TEMPERATURE,NULL);
    }
}

static void lvgl_touch_cb(lv_indev_drv_t * drv, lv_indev_data_t * data)
{
    TPoint xy0;
    TEvent ev;
    lv_point_t xy; 
    static uint16_t count1=0;
       
    if (tp.get_event(&xy0,&ev)) {
        xy.x = (lv_coord_t)xy0.y;
        xy.y = (lv_coord_t)xy0.x;
        data->point = xy;
        data->state = LV_INDEV_STATE_PRESSED;
       // printf("Dokundun %d %d %d\n",xy.x,xy.y,(int)ev);
        if (GlobalConfig.screen_saver>0) {
            backlight.ss_restart();
            if (backlight.ss_is_active()) backlight.On();
        }

    } else {
        xy.x = (lv_coord_t)xy0.y;
        xy.y = (lv_coord_t)xy0.x;
        data->state = LV_INDEV_STATE_RELEASED;
        data->point = xy;
    } 

    //Isı okuma süresi varsa Isı okuma yapar
    if (GlobalConfig.isi_okuma_suresi>0)
    {
        if (count1++>(GlobalConfig.isi_okuma_suresi*3))
        {
            count1=0;
            Read_Temperature();
        }
    }

    if (ROLE1_CHANGE)
    {
        h8574.pin_write(ROLE1,GlobalConfig.r1_stat);
        ROLE1_CHANGE=false;
    }
    if (ROLE2_CHANGE)
    {
        h8574.pin_write(ROLE2,GlobalConfig.r2_stat);
        ROLE2_CHANGE=false;
    }



    
}

static void increase_tick(void *arg)
{
    //lvgl e süreyi söyler
    lv_tick_inc(LVGL_TICK);
}

void mainconfig(void)
{
    //I2C master bus oluşturuluyor
    ESP_LOGI(MAIN_TAG,"I2C bus oluşturuluyor");
    ESP_ERROR_CHECK(bus.init_bus(SDA,SCL,I2C));
    bus.device_list();

    //bus içine 8574 ilave ediliyor
    ESP_ERROR_CHECK_WITHOUT_ABORT(h8574.init_device(&bus,0x20,0x00));
    ESP_LOGI(MAIN_TAG,"IO Entegresi bulundu");
    h8574.pin_write(6,0);
    ESP_ERROR_CHECK_WITHOUT_ABORT(h8563.init_device(&bus,0x00,0x00));
    ESP_LOGI(MAIN_TAG,"Saat Entegresi bulundu");
    struct tm rtcinfo;
    bool Valid=false;
    if (h8563.get_time(&rtcinfo, &Valid)==ESP_OK) {
        ESP_LOGI(MAIN_TAG,"Saat okundu. Degerler Stabil %s",(!Valid)?"Degil":"");
        if (!Valid)
        {
            ESP_LOGW(MAIN_TAG, "Saat default degere ayarlaniyor");
            struct tm tt0 = {};
            tt0.tm_sec = 0;
            tt0.tm_min = 59;
            tt0.tm_hour = 13;
            tt0.tm_mday = 1;
            tt0.tm_mon = 3;
            tt0.tm_year = 2024;
            h8563.set_time(&tt0,true);
            h8563.get_time(&rtcinfo, &Valid); 
            //pcf8563_set_sync(true);
            if (Valid) ESP_LOGE(MAIN_TAG,"Saat AYARLANAMADI");
        } else {
            //rtc degerlerini espye yazıyor
            h8563.device_to_esp();
            char *rr = (char *)malloc(50);
            struct tm tt0 = {};
            h8563.Get_current_date_time(rr,&tt0);
            ESP_LOGW(MAIN_TAG, "Tarih/Saat %s", rr);
            free(rr);
        }
    }

    ESP_ERROR_CHECK_WITHOUT_ABORT(hlm75.init_device(&bus,0x4F,0x00));
    ESP_LOGI(MAIN_TAG,"Sıcaklık sensörü bulundu");
    lm75_config_t config = {};
        config.mode           = LM75_MODE_SHUTDOWN;
        config.os_pol         = LM75_OSP_HIGH;
        config.os_mode        = LM75_OS_MODE_COMP;
        config.os_fault_queue = LM75_FAULT_QUEUE4;
  
    ESP_ERROR_CHECK(hlm75.init(config));
    ESP_ERROR_CHECK(hlm75.wakeup());

    ESP_LOGI(MAIN_TAG,"RS485 Init");
    rs485_cfg.uart_num = 1;
    rs485_cfg.dev_num  = 2;
    rs485_cfg.rx_pin   = RS485_RX;
    rs485_cfg.tx_pin   = RS485_TX;
    rs485_cfg.oe_pin   = RS485_DIR;
    rs485_cfg.baud     = 57600;

    rs485.initialize(&rs485_cfg, &rs485_callback, &rs485_callback, (gpio_num_t)-1);

    ESP_ERROR_CHECK_WITHOUT_ABORT(tp.init_device(&bus,0x38,0x00));
    ESP_LOGI(MAIN_TAG,"Dokunmatik Panel bulundu");
    tp.begin(22,LCD_V_RES,LCD_H_RES);
    tp.setRotation(3);
    //Backlight kapatıldı
    backlight.Off();

    panel_handle = lcd_hard_init();
    #if TEAR_EFFECT
        ESP_LOGI(MAIN_TAG, "Create vsync semaphores");
        sem_vsync_end = xSemaphoreCreateBinary();
        assert(sem_vsync_end);
        sem_gui_ready = xSemaphoreCreateBinary();
        assert(sem_gui_ready);
    #endif
    
    ESP_LOGI(MAIN_TAG, "Initialize RGB LCD panel");

    disp_drv = Panel_Register_Event(panel_handle, &on_vsync_event);   
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_LOGI(MAIN_TAG, "Turn on LCD backlight");
    backlight.On();

    ESP_LOGI(MAIN_TAG, "Initialize LVGL library");
    lv_init();
    void *buf1 = NULL;
    void *buf2 = NULL;   
    ESP_LOGI(MAIN_TAG, "Allocate separate LVGL draw buffers from PSRAM");
    buf1 = heap_caps_malloc(LCD_H_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf1);
    buf2 = heap_caps_malloc(LCD_H_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf2);
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LCD_H_RES * 100);

    ESP_LOGI(MAIN_TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_H_RES;
    disp_drv.ver_res = LCD_V_RES;
    disp_drv.flush_cb = lvgl_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;   
    disp_drv.full_refresh = true;
    display = lv_disp_drv_register(&disp_drv);

    ESP_LOGI(MAIN_TAG, "Register touch driver to LVGL");
    static lv_indev_drv_t indev_drv;    
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.disp = display;
    indev_drv.read_cb = lvgl_touch_cb;
    //indev_drv.user_data = tp;
    lv_indev_drv_register(&indev_drv);

    ESP_LOGI(MAIN_TAG, "Install LVGL tick timer");
    esp_timer_create_args_t tim_arg = {};
    tim_arg.callback = &increase_tick;
    tim_arg.name = "lvgl_tick";

    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&tim_arg, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK * 1000));

    backlight.Set(GlobalConfig.backlight);
    lv_png_init(); 
    backlight.ss_init();
    backlight.ss_set_duration(GlobalConfig.screen_saver);

}