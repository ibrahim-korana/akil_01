#ifndef SCREEN_SAVER_H_
#define SCREEN_SAVER_H_

#include "lvgl.h"
#include "../core_ayar.h"
#include "../../../core.h"
#include "../../../comp/language.h"
#include "stdio.h"
#include "panel.h"
#include "ibutton.h"

class Screen_Saver : public CoreAyar {
    public:
        Screen_Saver(lv_obj_t *parent, uint8_t blevel, Language *lang) {
            _parent = parent;
            level = blevel;
            Lng = lang;
            screen_init();
        };
        ~Screen_Saver() {}
        void unregister(void) {
            delete p0;
            delete btn0;
            //printf("unreg\n");
        };
        
        
    private:
        void screen_init(void);
        lv_style_t style_rad;
        lv_style_t style_chk;
        uint32_t inx = 0;
        static void check_handler(lv_event_t * e);
        void r_create(lv_obj_t * parent, const char * txt, bool chk);
        static void Btn_Callback(void *arg, const void *inx);
        IButton *btn0;
        Panel *p0;
        uint8_t level;
        Language *Lng; 
    protected:
};

#endif
