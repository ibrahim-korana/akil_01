
#include "lamp_button.h"

LV_IMG_DECLARE(lamba1_off_60)
LV_IMG_DECLARE(lamba1_on_60)
LV_IMG_DECLARE(lamba2_off_60)
LV_IMG_DECLARE(lamba2_on_60)


const void *Lamp_Button::choice_icon(uint8_t inx)
{
   switch (local)
   {
      case 0 : {
            if (inx==0) return &lamba1_off_60; else return &lamba1_on_60;
      } break;
      default :  {
            if (inx==0) return &lamba2_off_60; else return &lamba2_on_60;
      } break;
   }
   return NULL;
}
void Lamp_Button::local_callback(lv_event_t * e)
{
    Lamp_Button *arg = (Lamp_Button *) lv_event_get_user_data(e);
    lv_msg_send(MSG_LAMP_BUTTON_PRESSED, (void *)arg);
}

Lamp_Button::Lamp_Button(lv_obj_t* parent,int width, int height,const char *text, uint8_t loc)
{
    local = loc;
    panel = new Panel(parent,width,height,BTN_TICK,LINE_OPASITY);
    bck = panel->get();

    img = lv_img_create(bck);
    lv_img_set_src(img, choice_icon(0));
    lv_obj_set_width(img, LV_SIZE_CONTENT);
    lv_obj_center(img);
    lv_obj_set_style_opa(img, IMG_OFF_OPASITY, 0);  //Silikleştirir
    lv_obj_set_style_bg_opa(img, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(img, 0, LV_STATE_DEFAULT);
    lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE); 
    lv_obj_clear_flag(img, LV_OBJ_FLAG_SCROLLABLE);

    label = new ILabel(bck,width,15,255,0,LV_ALIGN_BOTTOM_MID,&lv_font_montserrat_14,lv_color_hex(0xFFFFFF));
    lv_obj_align(label->get(), LV_ALIGN_BOTTOM_MID, 0, 0);
    label->set_text(text);

    lv_obj_add_event_cb(bck, local_callback, LV_EVENT_CLICKED, (void *)this);
    lv_obj_add_event_cb(img, local_callback, LV_EVENT_CLICKED, (void *)this);
    lv_obj_add_event_cb(label->get(), local_callback, LV_EVENT_CLICKED, (void *)this);
}

void Lamp_Button::set_off(void)
{
    if (img!=NULL && bck!=NULL)
    {
        lv_img_set_src(img,choice_icon(0));
        lv_obj_set_style_opa(img, IMG_OFF_OPASITY, 0);
        lv_obj_set_style_bg_opa(bck, 0, 0);
        state = false;
    }
}

void Lamp_Button::set_on(void)
{
    if (img!=NULL && bck!=NULL)
    {
        lv_img_set_src(img, choice_icon(1));
        lv_obj_set_style_opa(img, IMG_ON_OPASITY, 0);
        lv_obj_set_style_bg_opa(bck, 15, 0);
        state = true;
    }
}

void Lamp_Button::set_state(bool dr)
{
    if (dr) set_on(); else set_off();
}
