
#include "setup.h"

#include "subpages/backlite_ayar.h"
#include "subpages/screen_saver.h"
#include "subpages/wifi_ayar.h"
#include "subpages/admin_ayar.h"

LV_IMG_DECLARE(backlite);
LV_IMG_DECLARE(wifi);
LV_IMG_DECLARE(defa);
LV_IMG_DECLARE(admin0);
LV_IMG_DECLARE(screensaver_60)

CoreAyar *aktif_ekran = NULL;
void *subsribe=NULL;

void Setup_Page::add_btn(local_ibutton_t *btn)
{
   btn->next = (local_ibutton_t *)buttons;
   buttons = btn;
}

 void Setup_Page::pass_cb(void * s, lv_msg_t * m)
 {
    Setup_Page *ths = (Setup_Page *) lv_msg_get_user_data(m);
    printf("Admin Pass True\n");
    ths->admin = true;
   // aktif_ekran = new Admin_Ayar(lv_scr_act(),ths->status, ths->Lng);
 }

void Setup_Page::sub_button_cb(void * s, lv_msg_t * m)
{
     CoreAyar *aa = (CoreAyar *)lv_msg_get_payload(m);
     ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_PCHANGE_ON, NULL, 0, portMAX_DELAY));
     aa->unregister();
     lv_obj_del(aa->get());
     LV_UNUSED(aa);
     aktif_ekran=NULL;     
}

void Setup_Page::panel_cb(lv_event_t * e) {
    if(aktif_ekran!=NULL)
      {
        ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_PCHANGE_ON, NULL, 0, portMAX_DELAY));
        aktif_ekran->unregister();
        lv_obj_del(aktif_ekran->get());
        LV_UNUSED(aktif_ekran);
        aktif_ekran=NULL;        
      }
}

void Setup_Page::defevent_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    Setup_Page *aa = (Setup_Page *)lv_event_get_user_data(e);
    if (strcmp(lv_msgbox_get_active_btn_text(obj),"Evet")==0)
      {
         ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_CONFIG_SAVE_DEFULT, NULL, 0, portMAX_DELAY));  
      }
    lv_msgbox_close(aa->mbox1); 
}

void Setup_Page::Lcallback(void *arg, const void *inx)
{
    Setup_Page *my = (Setup_Page *)arg;
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_BUTTON_PRESSED, NULL, 0, portMAX_DELAY));
    if (aktif_ekran==NULL)
    {
        if (strcmp((char *)inx,"BCK")==0)
        {
            aktif_ekran = new Backlite_Ayar(lv_scr_act(),my->status->backlight, my->Lng);
        } 
        if (strcmp((char *)inx,"SSAV")==0)
        {
            aktif_ekran = new Screen_Saver(lv_scr_act(),my->status->screen_saver, my->Lng);
        } 
        if (strcmp((char *)inx,"WIFI")==0)
        {
            aktif_ekran = new Wifi_Ayarlari(lv_scr_act(), my->status, my->Lng);
        }   
        if (strcmp((char *)inx,"ADM")==0)
        {
            if (!my->admin)
            {
                NIPassword *ps1 = new NIPassword(my->status->security_adm_pass,"",4,false);                
                ps1->active();
            } else {
                aktif_ekran = new Admin_Ayar(lv_scr_act(),my->status, my->Lng);
            }         
        } 

        if (strcmp((char *)inx,"DEF")==0)
        {
            if (!my->admin)
            {
                NIPassword *ps1 = new NIPassword(my->status->security_adm_pass,"",4,false);                
                ps1->active();
            } else {
                static const char * btns[] = {"Evet", "Hayir", ""};
                my->mbox1 = lv_msgbox_create(NULL, "Dikkat", "Ayarlar, default degerlere alinarak cihaz resetlenecek. Eminmisiniz?", btns, true);
                lv_obj_add_event_cb(my->mbox1, defevent_cb, LV_EVENT_VALUE_CHANGED, (void *)my);
                lv_obj_center(my->mbox1);
            }                           
        }   
        if (aktif_ekran!=NULL) ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_PCHANGE_OFF, NULL, 0, portMAX_DELAY));  
    }

}

local_ibutton_t *Setup_Page::new_btn(void)
{
   return (local_ibutton_t *)calloc(1,sizeof(local_ibutton_t));
}

void Setup_Page::icerik(void) 
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
    lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_style_set_layout(&style, LV_LAYOUT_FLEX);
    lv_obj_add_style(mpanel, &style, 0);
    lv_obj_align(mpanel,LV_ALIGN_TOP_MID,0,0);
    lv_obj_set_style_pad_all(mpanel, 10, LV_STATE_DEFAULT); 
    lv_obj_add_event_cb(mpanel,panel_cb,LV_EVENT_CLICKED,NULL);

    uint8_t ww = 100, opa = 30, tck =1;

    local_ibutton_t *ii = new_btn();
    ii->btn = new IButton(mpanel,&backlite,ww,ww,opa,tck,(void *)this,Lcallback,"BCK",Lng->get_text("parlaklik"));
    add_btn(ii);

    ii=new_btn();
    ii->btn = new IButton(mpanel,&screensaver_60,ww,ww,opa,tck,(void *)this,Lcallback,"SSAV",Lng->get_text("ssaver"));
    add_btn(ii);

    ii=new_btn();
    ii->btn = new IButton(mpanel,&wifi,ww,ww,opa,tck,(void *)this,Lcallback,"WIFI",Lng->get_text("wifi"));
    add_btn(ii);  

    ii=new_btn();
    ii->btn = new IButton(mpanel,&admin0,ww,ww,opa,tck,(void *)this,Lcallback,"ADM",Lng->get_text("admin")); 
    add_btn(ii);

    ii=new_btn();
    ii->btn = new IButton(mpanel,&defa,ww,ww,opa,tck,(void *)this,Lcallback,"DEF",Lng->get_text("default"));  
    add_btn(ii);

    subs0 = lv_msg_subscribe(MSG_CLOSE_CLK, sub_button_cb, NULL);
    subs1 = lv_msg_subscribe(PASS_PASSWORD_OK, pass_cb, (void *)this);

}

void Setup_Page::unregister(void)
{
    local_ibutton_t *tt = (local_ibutton_t *)buttons;
    local_ibutton_t *temp_tt;
    while(tt) {
            delete tt->btn;
            tt->btn = NULL;
            temp_tt = tt;
            tt = (local_ibutton_t *)tt->next;
            delete temp_tt;
            temp_tt = NULL;
      };
      lv_msg_unsubscribe(subs0);
      lv_msg_unsubscribe(subs1);
}

