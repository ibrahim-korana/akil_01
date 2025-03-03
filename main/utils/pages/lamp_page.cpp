

#include "lamp_page.h"

//#include "../comp/jsontool.h"


LV_IMG_DECLARE(lamba1_off_60)
LV_IMG_DECLARE(lamba1_on_60)
LV_IMG_DECLARE(lamba2_off_60)
LV_IMG_DECLARE(lamba2_on_60)

//LV_IMG_DECLARE(Down_60);
//LV_IMG_DECLARE(Up_60);

lamps_t *Lamba_Page::get_object(void *btn)
{
     lamps_t *temp = lamp_list;
     while(temp)
     {
          if (temp->button == btn)
          {
               return temp;
          }
          temp = (lamps_t *) temp->next;
     }
     return NULL;
}

void Lamba_Page::lamp_callback(void * s, lv_msg_t * msg)
{
     Lamba_Page *Self = (Lamba_Page *) lv_msg_get_user_data(msg);
     Lamp_Button *btn = (Lamp_Button *)lv_msg_get_payload(msg);
     lamps_t *temp = Self->get_object((void *)btn);     
     if (temp)
     {
          //Butona basıldı durumu Gondermek üzere akil event ile ana programa bildir
          ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_LAMP_PRESSED, (void *)temp, sizeof(lamps_t), 10/portTICK_PERIOD_MS));          
     }
}

void Lamba_Page::up_callback(void * s, lv_msg_t * msg)
{
     Lamba_Page *Self = (Lamba_Page *) lv_msg_get_user_data(msg);
     Cur_Button *btn = (Cur_Button *)lv_msg_get_payload(msg);
     lamps_t *temp = Self->get_object((void *)btn);     
     if (temp)
     {
          //CUR UP basıldı durumu Gondermek üzere akil event ile ana programa bildir
          ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_CUR_UP_PRESSED, (void *)temp, sizeof(lamps_t), 10/portTICK_PERIOD_MS));          
     }
}

void Lamba_Page::down_callback(void * s, lv_msg_t * msg)
{
     Lamba_Page *Self = (Lamba_Page *) lv_msg_get_user_data(msg);
     Cur_Button *btn = (Cur_Button *)lv_msg_get_payload(msg);
     lamps_t *temp = Self->get_object((void *)btn);     
     if (temp)
     {
          //CUR DOWN basıldı durumu Gondermek üzere akil event ile ana programa bildir
          ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_CUR_DOWN_PRESSED, (void *)temp, sizeof(lamps_t), 10/portTICK_PERIOD_MS));          
     }
}

void Lamba_Page::lamp_answer(void * s, lv_msg_t * msg)
{
     lamps_t *btn = (lamps_t *) lv_msg_get_payload(msg);
     if (btn)
     {
          Lamp_Button *temp = (Lamp_Button *)(btn->button); 
          temp->set_state(btn->state);
     }
}

void Lamba_Page::icerik(void) 
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
    lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_style_set_layout(&style, LV_LAYOUT_FLEX);
    lv_obj_add_style(mpanel, &style, 0);
    lv_obj_align(mpanel,LV_ALIGN_TOP_MID,0,0);
    lv_obj_set_style_pad_all(mpanel, 10, LV_STATE_DEFAULT); 

    //Lamba butonlarından gelen mesajları dinle 
    subs0 = lv_msg_subscribe(MSG_LAMP_BUTTON_PRESSED,lamp_callback,(void *)this);
    //Cur butonlarından gelen mesajları dinle 
    subs1 = lv_msg_subscribe(MSG_CUR_UP_BUTTON_PRESSED,up_callback,(void *)this);
    subs2 = lv_msg_subscribe(MSG_CUR_DOWN_BUTTON_PRESSED,down_callback,(void *)this);
    //Button geri dönüşlerini dinle
    subs3 = lv_msg_subscribe(MSG_LAMP_BUTTON_ANSWER,lamp_answer,(void *)this);

    lamps_t *temp = lamp_list;   
    while(temp)
     {         
      if (strcmp(temp->name,"lamp")==0)
      {   
          Lamp_Button *btn = new Lamp_Button(mpanel, temp->width, temp->height, temp->text, temp->local);
          if(temp->new_track==1) lv_obj_add_flag(btn->get(), LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
          temp->button = btn;
          btn->set_state(temp->state);
      }

      if (strcmp(temp->name,"cur")==0)
      { 
          Cur_Button *btn = new Cur_Button(mpanel, temp->width, temp->height, temp->text, temp->local);
          if(temp->new_track==1) lv_obj_add_flag(btn->get(), LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
          temp->button = btn;
          btn->set_state(temp->state); 
      }
      
      temp = (lamps_t *) temp->next;
     }
     
}


    