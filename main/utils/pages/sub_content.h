#ifndef _SUBCONTENT_H_
#define _SUBCONTENT_H_

     #include "../../core.h"
     #include "panel.h"
     #include "storage.h"

class SubContent {
    public:
        SubContent() {};
        void init(status_t *stat, lv_obj_t *par, Storage *dsk){
            parent = par;
            status = stat;
            disk = dsk;
            uint16_t w = 0, h = 0;
            w = lv_obj_get_width(parent);
            h = lv_obj_get_height(parent);   

            main_panel = new Panel(parent,w,h,0);
            mpanel = main_panel->get();            
        };
        virtual ~SubContent(){
            delete main_panel;
        };
        virtual void unregister(void) {};          
        lv_obj_t *get(void) {return mpanel;}
        virtual void icerik(void) {};
    private:     
        
    protected:  
        lv_obj_t *parent, *mpanel; 
        Panel *main_panel; 
        status_t *status;
        Storage *disk;
};


#endif