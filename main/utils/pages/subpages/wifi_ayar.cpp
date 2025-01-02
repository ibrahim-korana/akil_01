
#include "wifi_ayar.h"
#include "stdlib.h"
#include <stdio.h>


LV_IMG_DECLARE(save)


void Wifi_Ayarlari::Btn_Callback(void *arg, const void *inx)
{
    Wifi_Ayarlari *aa = (Wifi_Ayarlari *)arg;
    
    lv_msg_send(MSG_CLOSE_CLK,(void *)aa);
}

void Wifi_Ayarlari::btn_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    Wifi_Ayarlari *Self = (Wifi_Ayarlari *)lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI("WWWWW","Clicked: %s", lv_list_get_btn_text(Self->list, obj)); 
        strcpy(Self->tmp_ssid,lv_list_get_btn_text(Self->list, obj));
        

    }
}

#include "wifi_scan.cpp"

void Wifi_Ayarlari::screen_init(void)
{    
    tmp_ssid = (char *)calloc(64);
    tmp_pass = (char *)calloc(64);

    lv_coord_t w = lv_obj_get_width(_parent);
    uint16_t ww = (w/4)*3;
    _panel = lv_win_create(_parent,45);
    lv_obj_set_size(_panel,ww, 350);
    lv_obj_align(_panel, LV_ALIGN_CENTER,0,0);
    
    lv_win_add_title(_panel, Lng->get_text("wifiayar"));
    lv_obj_t *btn = lv_win_add_btn(_panel, LV_SYMBOL_CLOSE, 40);
    lv_obj_add_event_cb(btn, content_clk, LV_EVENT_CLICKED, (void *)this);

    lv_obj_t * con = lv_win_get_content(_panel);
    window_panel_style(con,4);

    
    Wifi_Sw =  new ISwitch(con,130,60,status->wifi_active,Lng->get_text("wifion"),Lng->get_text("wifioff"));
    lv_obj_align(Wifi_Sw->get(),LV_ALIGN_TOP_RIGHT,10,0);

    Time_Sw =  new ISwitch(con,130,60,status->time_int_sync,Lng->get_text("timsyncon"),Lng->get_text("timsyncoff"));
    lv_obj_align(Time_Sw->get(),LV_ALIGN_TOP_LEFT,10,0);
   
    list = lv_list_create(con);
    lv_obj_set_size(list, ww-40, 150);
    lv_obj_align(list,LV_ALIGN_TOP_MID,0,70);
    lv_obj_set_style_text_color(list,lv_color_hex(0xFFFFFF),LV_PART_MAIN);
    lv_obj_set_style_bg_color(list,lv_color_hex(0x000000),LV_PART_MAIN);

    esp_err_t kk=ESP_OK;
    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &scan_event_handler, (void *)this) );

    if (status->wifi_active==1)
    {
        kk = wifi_open_start();
    }
    else
    {
       // 
    }


    btn0 = new IButton(con,&save,80,80,255,0,(void *)this,Btn_Callback,"KAYIT",Lng->get_text("kayit"));
    lv_obj_align(btn0->get(),LV_ALIGN_BOTTOM_RIGHT,-10,-10);  
}