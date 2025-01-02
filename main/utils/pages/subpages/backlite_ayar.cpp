
#include "backlite_ayar.h"
#include "stdlib.h"
#include <stdio.h>
#include "lvgl.h"


void Backlite_Ayar::value_change(lv_event_t * e) {
    lv_obj_t *slider = lv_event_get_target(e); 
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_BACKLITE_CHANGING, &val, sizeof(uint8_t), portMAX_DELAY));
    char *mm = (char *)malloc(5);
    sprintf(mm,"%d",val);
    lv_msg_send(1001,mm);
    free(mm);  
}

void Backlite_Ayar::backlite_change(lv_event_t * e) {
    lv_obj_t *slider = lv_event_get_target(e); 
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_BACKLITE_CHANGED, &val, sizeof(uint8_t), portMAX_DELAY));
    char *mm = (char *)malloc(5);
    sprintf(mm,"%d",val);
    lv_msg_send(1001,mm);
    free(mm);  
}

void Backlite_Ayar::label_cb(lv_event_t * e)
{
    lv_obj_t * label = lv_event_get_target(e);
    lv_msg_t * m = lv_event_get_msg(e);
    lv_label_set_text(label, (char*)lv_msg_get_payload(m));
}

void Backlite_Ayar::screen_init(void)
{
    uint8_t backlight_swp_value = level;

    lv_coord_t w = lv_obj_get_width(_parent);
    _panel = lv_win_create(_parent,45);
    lv_obj_set_size(_panel,(w/4)*3, 150);
    lv_obj_align(_panel, LV_ALIGN_CENTER,0,0);
    
    lv_win_add_title(_panel, Lng->get_text("backlite"));
    lv_obj_t *btn = lv_win_add_btn(_panel, LV_SYMBOL_CLOSE, 40);
    lv_obj_add_event_cb(btn, content_clk, LV_EVENT_CLICKED, (void *)this);

    lv_obj_t * con = lv_win_get_content(_panel);
    window_panel_style(con,4);

    static lv_style_t style_main;
    lv_style_init(&style_main);
    lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_main, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
    lv_style_set_pad_ver(&style_main, -5);

    static lv_style_t style_knob;
    lv_style_init(&style_knob);
    lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
    lv_style_set_bg_color(&style_knob, lv_palette_main(LV_PALETTE_TEAL));
    lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_TEAL, 1));
    lv_style_set_border_width(&style_knob, 2);
    lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&style_knob, 10); 

    static lv_style_t style_indicator;  
    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&style_indicator, LV_RADIUS_CIRCLE);

    lv_obj_t *backlite_swp = lv_slider_create(con);
    lv_obj_add_event_cb(backlite_swp, backlite_change, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(backlite_swp, value_change, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align(backlite_swp,LV_ALIGN_CENTER, 20, 0);
    lv_slider_set_range(backlite_swp, 5 , 255);
    lv_slider_set_value(backlite_swp,backlight_swp_value, LV_ANIM_ON);
    lv_obj_add_style(backlite_swp, &style_knob, LV_PART_KNOB);
    lv_obj_add_style(backlite_swp, &style_main, LV_PART_MAIN);
    lv_obj_add_style(backlite_swp, &style_indicator, LV_PART_INDICATOR);

    lv_obj_t *backlite_swp_lab = lv_label_create(con);
    lv_obj_set_style_text_font(backlite_swp_lab, &lv_font_montserrat_18, LV_STATE_DEFAULT);
    lv_obj_align_to(backlite_swp_lab, backlite_swp,LV_ALIGN_OUT_LEFT_MID, -5, 0);
    
    lv_obj_add_event_cb(backlite_swp_lab, label_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(1001, backlite_swp_lab, NULL);

    char *mm = (char *)malloc(5);
    sprintf(mm,"%d",backlight_swp_value);
    lv_msg_send(1001,mm);
    free(mm);
   
}