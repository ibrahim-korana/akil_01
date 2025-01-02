#ifndef BACKLITE_AYAR_H_
#define BACKLITE_AYAR_H_

#include "lvgl.h"
#include "../core_ayar.h"
#include "stdio.h"
#include "../../../core.h"
#include "../../../comp/language.h"

class Backlite_Ayar : public CoreAyar {
    public:
        Backlite_Ayar(lv_obj_t *parent, uint8_t blevel, Language *lang) {
            _parent = parent;
            level = blevel;
            Lng = lang;
            screen_init();
        };
        ~Backlite_Ayar() {}
        void unregister(void) {};  
        
        
    private:       
       void screen_init(void);
       static void backlite_change(lv_event_t * e);
       static void value_change(lv_event_t * e);
       static void label_cb(lv_event_t * e);
       uint8_t level; 
       Language *Lng;
    protected:
};

#endif
