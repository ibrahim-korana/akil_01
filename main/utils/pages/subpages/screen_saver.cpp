#include "screen_saver.h"
#include "stdlib.h"
#include <stdio.h>


LV_IMG_DECLARE(save)

void Screen_Saver::r_create(lv_obj_t * parent, const char * txt, bool chk)
{
    lv_obj_t * obj = lv_checkbox_create(parent);
    lv_checkbox_set_text(obj, txt);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_style(obj, &style_rad, LV_PART_INDICATOR);
    lv_obj_add_style(obj, &style_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_coord_t box_size = 32;
    const lv_font_t * font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
    lv_coord_t font_h = lv_font_get_line_height(font);
    lv_coord_t pad = (box_size - font_h) / 2;
    lv_obj_set_style_pad_left(obj, pad, LV_PART_INDICATOR);
    lv_obj_set_style_pad_right(obj, pad, LV_PART_INDICATOR);
    lv_obj_set_style_pad_top(obj, pad, LV_PART_INDICATOR);
    lv_obj_set_style_pad_bottom(obj, pad, LV_PART_INDICATOR);

    if (chk)  lv_obj_add_state(obj, LV_STATE_CHECKED);
}


void Screen_Saver::check_handler(lv_event_t * e)
{
    uint32_t * active_id = (uint32_t *)lv_event_get_user_data(e);
    lv_obj_t * cont = lv_event_get_current_target(e);
    lv_obj_t * act_cb = lv_event_get_target(e);
    lv_obj_t * old_cb = lv_obj_get_child(cont, *active_id);
    if(act_cb == cont) return;
    lv_obj_clear_state(old_cb, LV_STATE_CHECKED); 
    lv_obj_add_state(act_cb, LV_STATE_CHECKED); 
    *active_id = lv_obj_get_index(act_cb);   
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_BUTTON_PRESSED, NULL, 0, portMAX_DELAY)); 
    
}

void Screen_Saver::Btn_Callback(void *arg, const void *inx)
{
    Screen_Saver *aa = (Screen_Saver *)arg;
    uint8_t val = 10;
    if(aa->inx==0) val=5;
    if(aa->inx==1) val=10;
    if(aa->inx==2) val=15;
    if(aa->inx==3) val=20;
    if(aa->inx==4) val=30;
    if(aa->inx==5) val=0;
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_BUTTON_PRESSED, NULL, 0, portMAX_DELAY)); 
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_SAVER_CHANGED, &val, sizeof(uint8_t), portMAX_DELAY));
    vTaskDelay(20/portTICK_PERIOD_MS);
    lv_msg_send(MSG_CLOSE_CLK,(void *)aa);
}

void Screen_Saver::screen_init(void)
{    
    lv_coord_t w = lv_obj_get_width(_parent);
    _panel = lv_win_create(_parent,45);
    lv_obj_set_size(_panel,(w/4)*2, 350);
    lv_obj_align(_panel, LV_ALIGN_CENTER,0,0);
    
    lv_win_add_title(_panel, Lng->get_text("ssaverayar"));
    lv_obj_t *btn = lv_win_add_btn(_panel, LV_SYMBOL_CLOSE, 40);
    lv_obj_add_event_cb(btn, content_clk, LV_EVENT_CLICKED, (void *)this);

    lv_obj_t * con = lv_win_get_content(_panel);
    window_panel_style(con,4);

    p0 = new Panel(con,(w/4)*1,200,0);
    lv_obj_t *pp0 = p0->get();
    lv_obj_align(pp0,LV_ALIGN_TOP_LEFT,15,10);
    lv_obj_set_style_text_color(pp0,lv_color_hex(0xFFFFFF),LV_PART_MAIN);

    lv_style_init(&style_rad);
    lv_style_set_radius(&style_rad, LV_RADIUS_CIRCLE);

    lv_style_init(&style_chk);
    lv_style_set_bg_img_src(&style_chk, NULL);

    lv_obj_set_flex_flow(pp0, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(pp0, lv_pct(40), lv_pct(115));
    lv_obj_add_event_cb(pp0, check_handler, LV_EVENT_CLICKED, &inx);
    
    if (level==5) {r_create(pp0,Lng->get_text("5dk"), true); inx=0;}else r_create(pp0,Lng->get_text("5dk"), false); 
    if (level==10)  {r_create(pp0,Lng->get_text("10dk"), true); inx=1;}else r_create(pp0,Lng->get_text("10dk"), false);
    if (level==15)  {r_create(pp0,Lng->get_text("15dk"), true); inx=2;}else r_create(pp0,Lng->get_text("15dk"), false);
    if (level==20)  {r_create(pp0,Lng->get_text("20dk"), true); inx=3;}else r_create(pp0,Lng->get_text("20dk"), false);
    if (level==30)  {r_create(pp0,Lng->get_text("30dk"), true); inx=4;}else r_create(pp0,Lng->get_text("30dk"), false);
    if (level==0)  {r_create(pp0,Lng->get_text("yok"), true); inx=5;}else r_create(pp0,Lng->get_text("yok"), false);

    btn0 = new IButton(con,&save,80,80,255,0,(void *)this,Btn_Callback,"KAYIT",Lng->get_text("kayit"));
    lv_obj_align(btn0->get(),LV_ALIGN_BOTTOM_RIGHT,-10,-10);  
}