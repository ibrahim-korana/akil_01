
#include "cur_button.h"

LV_IMG_DECLARE(Down_60);
LV_IMG_DECLARE(Up_60);

void Cur_Button::up_callback(lv_event_t * e)
{
    Cur_Button *arg = (Cur_Button *) lv_event_get_user_data(e);
    lv_msg_send(MSG_CUR_UP_BUTTON_PRESSED, (void *)arg);
}

void Cur_Button::down_callback(lv_event_t * e)
{
    Cur_Button *arg = (Cur_Button *) lv_event_get_user_data(e);
    lv_msg_send(MSG_CUR_DOWN_BUTTON_PRESSED, (void *)arg);
}

Cur_Button::Cur_Button(lv_obj_t* parent,int width, int height,const char *text, uint8_t loc)
{
    local = loc;
    panel = new Panel(parent,width,height,BTN_TICK,LINE_OPASITY);
    bck = panel->get();

    panel1 = new Panel(bck,width,(height/2)-8,0,LINE_OPASITY);
    lv_obj_align(panel1->get(), LV_ALIGN_TOP_MID, 0, 0);
    panel2 = new Panel(bck,width,(height/2)-8,0,LINE_OPASITY);
    lv_obj_align_to(panel2->get(), panel1->get(),LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);


    img1 = lv_img_create(panel1->get());
    lv_img_set_src(img1, &Up_60);
    lv_obj_set_width(img1, LV_SIZE_CONTENT);
    lv_obj_center(img1);
    lv_obj_set_style_opa(img1, IMG_OFF_OPASITY, 0);  //Silikleştirir
    lv_obj_set_style_bg_opa(img1, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(img1, 0, LV_STATE_DEFAULT);
    lv_obj_add_flag(img1, LV_OBJ_FLAG_CLICKABLE); 
    lv_obj_clear_flag(img1, LV_OBJ_FLAG_SCROLLABLE);

    img2 = lv_img_create(panel2->get());
    lv_img_set_src(img2, &Down_60);
    lv_obj_set_width(img2, LV_SIZE_CONTENT);
    lv_obj_center(img2);
    lv_obj_set_style_opa(img2, IMG_OFF_OPASITY, 0);  //Silikleştirir
    lv_obj_set_style_bg_opa(img2, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(img2, 0, LV_STATE_DEFAULT);
    lv_obj_add_flag(img2, LV_OBJ_FLAG_CLICKABLE); 
    lv_obj_clear_flag(img2, LV_OBJ_FLAG_SCROLLABLE);

    label = new ILabel(bck,width,15,255,0,LV_ALIGN_BOTTOM_MID,&lv_font_montserrat_14,lv_color_hex(0xFFFFFF));
    lv_obj_align(label->get(), LV_ALIGN_BOTTOM_MID, 0, 0);
    label->set_text(text);

 //   lv_obj_add_event_cb(bck, local_callback, LV_EVENT_CLICKED, (void *)this);
      lv_obj_add_event_cb(img1, up_callback, LV_EVENT_CLICKED, (void *)this);
      lv_obj_add_event_cb(img2, down_callback, LV_EVENT_CLICKED, (void *)this);
      
 //   lv_obj_add_event_cb(label->get(), local_callback, LV_EVENT_CLICKED, (void *)this);
}

void Cur_Button::set_off(void)
{
    if (img1!=NULL && img2!=NULL && bck!=NULL)
    {
        lv_obj_set_style_opa(img1, IMG_OFF_OPASITY, 0);
        lv_obj_set_style_opa(img2, IMG_OFF_OPASITY, 0);
        lv_obj_set_style_bg_opa(bck, 0, 0);
        state = false;
    }
}

void Cur_Button::set_on(void)
{
    if (img1!=NULL && img2!=NULL && bck!=NULL)
    {
        lv_obj_set_style_opa(img1, IMG_ON_OPASITY, 0);
        lv_obj_set_style_opa(img2, IMG_ON_OPASITY, 0);
        lv_obj_set_style_bg_opa(bck, 15, 0);
        state = true;
    }
}

void Cur_Button::set_state(bool dr)
{
    if (dr) set_on(); else set_off();
}
