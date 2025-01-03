
#include "wifi_ayar.h"
#include "stdlib.h"
#include <stdio.h>


LV_IMG_DECLARE(save)

void Wifi_Ayarlari::swchg_callback(void * s, lv_msg_t * m)
{
    Wifi_Ayarlari *ths = (Wifi_Ayarlari *) lv_msg_get_user_data(m);
    ISwitch *sw = (ISwitch *)lv_msg_get_payload(m);
    lv_obj_clear_flag(ths->btn0->get(), LV_OBJ_FLAG_HIDDEN);
    bool chk = sw->get_state();
    if (chk)
    {
        if (strcmp(sw->get_inx(),"wifi")==0 && ths->status->wifi_active==0)
        {
            ths->wifi_close_start();
            lv_obj_clear_flag(ths->list, LV_OBJ_FLAG_HIDDEN);
        }
    }  
    ESP_LOGI("WIFI","swg: %d %s", chk, sw->get_inx());    
}

void Wifi_Ayarlari::keyb_callback(void * s, lv_msg_t * m)
{
    Wifi_Ayarlari *ths = (Wifi_Ayarlari *) lv_msg_get_user_data(m);
    strcpy(ths->tmp_pass,ths->keyb->get_value());
    ESP_LOGI("WIFI","Pass: %s", ths->tmp_pass); 
    ths->pass_ok = true;
    lv_obj_clear_flag(ths->btn0->get(), LV_OBJ_FLAG_HIDDEN); 
}

void Wifi_Ayarlari::box_event_cb(lv_event_t * e)
{
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_CONFIG_SAVE, NULL, 0, portMAX_DELAY));
}

void Wifi_Ayarlari::Btn_Callback(void *arg, const void *inx)
{
    Wifi_Ayarlari *aa = (Wifi_Ayarlari *)arg;

        if (aa->ssid_ok && aa->pass_ok && aa->Wifi_Sw->get_state()==1)
        {
            strcpy(aa->status->wifi_ssid,aa->tmp_ssid);
            strcpy(aa->status->wifi_pass,aa->tmp_pass);
        }
        aa->status->wifi_active = aa->Wifi_Sw->get_state();
        aa->status->time_int_sync = aa->Time_Sw->get_state();
        ESP_LOGI("WIFI","SSID: %s Pass: %s Wifi:%d Sync:%d", aa->status->wifi_ssid, aa->status->wifi_pass, aa->status->wifi_active, aa->status->time_int_sync); 
        static const char * btns[] = {"Tamam", ""};
        aa->mbox1 = lv_msgbox_create(NULL, aa->Lng->get_text("dikkat"), aa->Lng->get_text("mes1"), btns, true);
        lv_obj_center(aa->mbox1);
        lv_obj_add_event_cb(aa->mbox1, box_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        
       // lv_msg_send(MSG_CLOSE_CLK,(void *)aa);
 
}

void Wifi_Ayarlari::btn_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    Wifi_Ayarlari *Self = (Wifi_Ayarlari *)lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI("WIFI","Clicked: %s", lv_list_get_btn_text(Self->list, obj)); 
        strcpy(Self->tmp_ssid,lv_list_get_btn_text(Self->list, obj));
        Self->keyb = new IKeyboard("Password",(char *)Self->status->wifi_pass,1);
        Self->ssid_ok = true;
    }
}

#include "wifi_scan.cpp"

void Wifi_Ayarlari::screen_init(void)
{    
    tmp_ssid = (char *)calloc(1,64);
    tmp_pass = (char *)calloc(1,64);

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
    Wifi_Sw->set_inx("wifi");

    Time_Sw =  new ISwitch(con,130,60,status->time_int_sync,Lng->get_text("timsyncon"),Lng->get_text("timsyncoff"));
    lv_obj_align(Time_Sw->get(),LV_ALIGN_TOP_LEFT,10,0);
    Time_Sw->set_inx("time");
   
    list = lv_list_create(con);
    lv_obj_set_size(list, ww-40, 150);
    lv_obj_align(list,LV_ALIGN_TOP_MID,0,70);
    lv_obj_set_style_text_color(list,lv_color_hex(0xFFFFFF),LV_PART_MAIN);
    lv_obj_set_style_bg_color(list,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_add_flag(list, LV_OBJ_FLAG_HIDDEN);

    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &scan_event_handler, (void *)this) );
    subs0 = lv_msg_subscribe(IKB_OK,keyb_callback,(void *)this); 
    subs1 = lv_msg_subscribe(ISW_CHANGED,swchg_callback,(void *)this); 

    if (status->wifi_active==1)
    {
        wifi_open_start();
        lv_obj_clear_flag(list, LV_OBJ_FLAG_HIDDEN);
    }
    
    btn0 = new IButton(con,&save,80,80,255,0,(void *)this,Btn_Callback,"KAYIT",Lng->get_text("kayit"));
    lv_obj_align(btn0->get(),LV_ALIGN_BOTTOM_RIGHT,-10,-10);  
    lv_obj_add_flag(btn0->get(), LV_OBJ_FLAG_HIDDEN); 
    
}