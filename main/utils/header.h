#ifndef _HEADER_H_
#define _HEADER_H_

     #include "../core.h"
     #include "panel.h"
     #include "ilabel.h"
     #include "ibutton.h"

LV_IMG_DECLARE(home_48)     
LV_IMG_DECLARE(Wifi_alarm_5_48) 
LV_IMG_DECLARE(Wifi_5_48) 
LV_IMG_DECLARE(Thermo2_48) 
LV_IMG_DECLARE(lamps_48) 
LV_IMG_DECLARE(settings_48) 



class Header {
    public:
        Header(status_t *stat, lv_obj_t *par, uint8_t yukseklik){
            parent = par;
            status = stat;

            uint16_t w = 0, h = yukseklik;
            w = lv_obj_get_width(parent)-10;
            uint8_t tck = 0; 
            

            main_panel = new Panel(parent,w,h,tck);
            mpanel = main_panel->get();  
                      
             
            i_page = new IButton(mpanel,&home_48,h,h,50,tck,NULL,NULL,"HOME"); 
            lv_obj_align(i_page->get(),LV_ALIGN_TOP_LEFT,0,0);
            
            i_wifi = new IButton(mpanel,&Wifi_alarm_5_48,h,h,50,tck,NULL,NULL,"WIFI"); 
            lv_obj_align_to(i_wifi->get(),i_page->get(),LV_ALIGN_OUT_RIGHT_MID,0,0);
            if (status->wifi_active==0) lv_obj_add_flag(i_wifi->get(), LV_OBJ_FLAG_HIDDEN); //eğer wifi kapalı ise ikonu gizle.
            lv_obj_add_event_cb(i_wifi->get(),wifi_callback,LV_EVENT_CLICKED,(void *)this);
           
            ESP_ERROR_CHECK(esp_event_handler_instance_register(AKIL_EVENTS, ESP_EVENT_ANY_ID, change_events, (void *)this, NULL));
        };
        ~Header(){
            free(i_wifi);
            free(i_page);
            free(main_panel);
        };
        void set_page(void) {
            if (status->active_page==1) i_page->set_sourge(&home_48);
            if (status->active_page==2) i_page->set_sourge(&Thermo2_48);
            if (status->active_page==3) i_page->set_sourge(&lamps_48);
            if (status->active_page==4) i_page->set_sourge(&settings_48);         
        };
        lv_obj_t *get(void) {return mpanel;}
    private:
        lv_obj_t *parent, *mpanel;
        Panel *main_panel;
        
        IButton *i_wifi, *i_page;
        status_t *status;

        static void change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);
        static void wifi_callback(lv_event_t * e);
    protected:   
};

#endif