#ifndef _CONTENT_H_
#define _CONTENT_H_

     #include "../core.h"
     #include "panel.h"    
     #include "storage.h"
     #include "../comp/language.h"
     #include "pages/sub_content.h"


class Content {
    public:
        Content(status_t *stat, lv_obj_t *par, uint16_t yukseklik,Storage *dsk, Language *lang ){
            parent = par;
            status = stat;
            disk = dsk;
            Lng = lang;
            uint16_t w = 0, h = yukseklik;
            w = lv_obj_get_width(parent);
            
            main_panel = new Panel(parent,w,h,0);
            mpanel = main_panel->get();            
                       
            ESP_ERROR_CHECK(esp_event_handler_instance_register(AKIL_EVENTS, ESP_EVENT_ANY_ID, change_events, (void *)this, NULL));
        };
        ~Content(){
            free(main_panel);
        };
       
        lv_obj_t *get(void) {return mpanel;}
    private:
        lv_obj_t *parent, *mpanel;
        Panel *main_panel;
        
        status_t *status;
        uint8_t page_index;
        Storage *disk;
        Language *Lng;

        SubContent *Active_Page=NULL; 
        void page_create(void);
       
        static void change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);
        
    protected:   
};

#endif