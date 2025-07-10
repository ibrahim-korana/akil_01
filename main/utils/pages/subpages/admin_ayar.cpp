#include "admin_ayar.h"
#include "stdlib.h"
#include <stdio.h>




LV_IMG_DECLARE(save);
LV_IMG_DECLARE(defa);


void Admin_Ayar::swchg_callback(void * s, lv_msg_t * m)
{
    Admin_Ayar *Self = (Admin_Ayar *) lv_msg_get_user_data(m);
    ISwitch *sw = (ISwitch *)lv_msg_get_payload(m);
    lv_obj_clear_flag(Self->ii->get(),LV_OBJ_FLAG_HIDDEN);
    bool chk = sw->get_state();
    if (chk)
    {
        if (strcmp(sw->get_inx(),"usb")==0)
        {
            
        }
    }  
    ESP_LOGI("WIFI","swg: %d %s", chk, sw->get_inx());    
}


void Admin_Ayar::iinput_cb(void * s, lv_msg_t * m)
{
    Admin_Ayar *Self = (Admin_Ayar *)lv_msg_get_user_data(m);
 //   NIInput *aa = (NIInput *)lv_msg_get_payload(m);
   // printf("Adm IInput %d %d\n",aa->get_index(),aa->get_value());
    lv_obj_clear_flag(Self->ii->get(),LV_OBJ_FLAG_HIDDEN);
}

void Admin_Ayar::Btn_Callback(void *arg, const void *inx)
{
    Admin_Ayar *aa = (Admin_Ayar *)arg;
    
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_BUTTON_PRESSED, NULL, 0, 10/portTICK_PERIOD_MS)); 
    vTaskDelay(20/portTICK_PERIOD_MS);
    if (strcmp((char*)inx,"KAYIT")==0) { 

        aa->status->sensor_no = aa->cid->get_value();
        aa->status->max_temp = aa->maxt->get_value();
        aa->status->min_temp = aa->mint->get_value();
        aa->status->isi_kalibrasyon = aa->temp_cal->get_value();
        aa->status->isi_okuma_suresi = aa->temp_read->get_value();
        aa->status->udp_active = aa->udp_sw->get_state();
        aa->status->rs485_active = aa->rs_sw->get_state();
        aa->status->termostat_local = aa->lc_sw->get_state();
        
        ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_CONFIG_SAVE_NORESET, NULL, 0, portMAX_DELAY));        
        lv_msg_send(MSG_CLOSE_CLK,(void *)aa);
    }
    if (strcmp((char*)inx,"RESET")==0) { 
        esp_restart();
    }

}

void Admin_Ayar::screen_init(void)
{    
    lv_coord_t w = lv_obj_get_width(_parent);
    _panel = lv_win_create(_parent,45);
    lv_obj_set_size(_panel,(w/16)*15, 400);
    lv_obj_align(_panel, LV_ALIGN_CENTER,0,0);
    
    lv_win_add_title(_panel, Lng->get_text("yonetimayar"));
    lv_obj_t *btn = lv_win_add_btn(_panel, LV_SYMBOL_CLOSE, 40);
    lv_obj_add_event_cb(btn, content_clk, LV_EVENT_CLICKED, (void*)this);

    lv_obj_t *con = lv_win_get_content(_panel);
    window_panel_style(con,4);
   
    cid = new NIInput(con,130,50,Lng->get_text("cihazid"),status->sensor_no,110);
    lv_obj_align(cid->get(),LV_ALIGN_TOP_LEFT,20,15);

    maxt = new NIInput(con,130,50,Lng->get_text("maxtemp"),status->max_temp,101);
    lv_obj_align_to(maxt->get(),cid->get(),LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
       
    mint = new NIInput(con,130,50,Lng->get_text("mintemp"),status->min_temp,102);
    lv_obj_align_to(mint->get(),maxt->get(),LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
    
    temp_cal = new NIInput(con,130,50,Lng->get_text("tempcal"),status->isi_kalibrasyon,103);
    lv_obj_align_to(temp_cal->get(),mint->get(),LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
    
    temp_read = new NIInput(con,130,50,Lng->get_text("tempread"),status->isi_okuma_suresi,104);
    lv_obj_align_to(temp_read->get(),temp_cal->get(),LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
       
    ii = new IButton(con,&save,80,80,255,0,(void *)this,Btn_Callback,"KAYIT",Lng->get_text("kayit"));
    lv_obj_align(ii->get(),LV_ALIGN_BOTTOM_RIGHT,-10,-10);
    lv_obj_add_flag(ii->get(),LV_OBJ_FLAG_HIDDEN);

    res = new IButton(con,&defa,80,80,255,0,(void *)this,Btn_Callback,"RESET",Lng->get_text("reset"));
    lv_obj_align(res->get(),LV_ALIGN_BOTTOM_LEFT,10,-10);

    udp_sw =  new ISwitch(con,130,60,status->udp_active,Lng->get_text("udpon"),Lng->get_text("udpoff"));
    lv_obj_align(udp_sw->get(),LV_ALIGN_TOP_RIGHT,-30,15);
    udp_sw->set_inx("udp");

    rs_sw =  new ISwitch(con,130,60,status->rs485_active,Lng->get_text("rson"),Lng->get_text("rsoff"));
    lv_obj_align_to(rs_sw->get(),udp_sw->get(),LV_ALIGN_OUT_BOTTOM_LEFT,0,20);
    rs_sw->set_inx("rs");

    lc_sw =  new ISwitch(con,130,60,status->termostat_local,Lng->get_text("teron"),Lng->get_text("teroff"));
    lv_obj_align_to(lc_sw->get(),rs_sw->get(),LV_ALIGN_OUT_BOTTOM_LEFT,0,20);
    lc_sw->set_inx("ter");

}