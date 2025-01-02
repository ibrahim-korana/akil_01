#ifndef _FOODER_H_
#define _FOODER_H_

     #include "../core.h"
     #include "panel.h"
     #include "ibutton.h"

LV_IMG_DECLARE(sag_60)
LV_IMG_DECLARE(sol_60)     


class Fooder {
    public:
        Fooder(status_t *stat, lv_obj_t *par, uint8_t yukseklik)
        {
            parent = par;
            status = stat;
            uint16_t w = 0, h = yukseklik;
            w = lv_obj_get_width(parent);
            uint8_t tar_w = yukseklik-5, tck = 0; 
                       
            page_index = status->active_page;
            max_page = status->max_page;
            if (page_index<1 || page_index>max_page) page_index = 1;

            ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_PAGE_CHANGED, &page_index, sizeof(uint8_t), portMAX_DELAY)); 

            main_panel = new Panel(parent,w,h,tck);
            mpanel = main_panel->get();
            
            solb = new IButton(mpanel,&sol_60,tar_w,tar_w,30,tck,(void *)this,callback,"SOLB");
            lv_obj_align(solb->get(), LV_ALIGN_BOTTOM_LEFT, 0, 0);

            sagb = new IButton(mpanel,&sag_60,tar_w,tar_w,30,tck,(void *)this,callback,"SAGB");
            lv_obj_align(sagb->get(), LV_ALIGN_BOTTOM_RIGHT, 0, 0);

            ip = new ILabel(mpanel,150,30,0,0,LV_ALIGN_CENTER,&lv_font_montserrat_14,lv_color_hex(0xFFFFFF));
            ip->set_text(status->current_ip);
            lv_obj_align(ip->get(),LV_ALIGN_CENTER,0,0); 

            ESP_ERROR_CHECK(esp_event_handler_instance_register(AKIL_EVENTS, ESP_EVENT_ANY_ID, akil_events, (void *) this, NULL));
                                         
        };
        ~Fooder(){
            free(solb);
            free(sagb);
            free(ip);
            free(main_panel);
        };
        void set_page(uint8_t inx, const void *src) {
            page_index = inx;
            //i_page->set_sourge(src);
        };
        lv_obj_t *get(void) {return mpanel;}

    private:
        lv_obj_t *parent, *mpanel;
        Panel *main_panel;
        IButton *solb, *sagb;
        ILabel *ip;
        status_t *status;
        uint8_t page_index;
        uint8_t max_page;
        bool change_active = true;
        
        
        static void callback(void *ths, const void *inx);
        static void akil_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);
    protected:   
};

#endif