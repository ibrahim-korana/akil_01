#ifndef _COREAYAR_H_
#define _COREAYAR_H_

     #include "../../core.h"
     #include "lvgl.h"

class CoreAyar {
    public:
        CoreAyar() {};
        virtual ~CoreAyar(){};
        virtual void unregister(void) {};  
        lv_obj_t *get(void) {return _panel;} 
        void window_panel_style(lv_obj_t *pan, uint8_t bck)
        {
            lv_obj_set_style_border_width(pan, 0, 0);
        // lv_obj_set_style_bg_opa(pan, LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_color(pan,lv_palette_darken(LV_PALETTE_GREY, bck),0);
            lv_obj_set_scrollbar_mode(pan, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_style_pad_all(pan, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(pan,lv_color_hex(0xFFFFFF),LV_PART_MAIN);
            lv_obj_set_style_text_font(pan,&lv_font_montserrat_14,  LV_PART_MAIN);
        };
        static void content_clk(lv_event_t * e)
        {
            CoreAyar *aa = (CoreAyar *)lv_event_get_user_data(e);
            lv_msg_send(MSG_CLOSE_CLK,(void *)aa);
        }
       
    private:     
    protected:  
        lv_obj_t *_parent, *_panel;
        
};


#endif