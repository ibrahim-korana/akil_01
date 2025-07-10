#ifndef ADMIN_AYAR_H_
#define ADMIN_AYAR_H_

#include "lvgl.h"
#include "../core_ayar.h"
#include "../../../core.h"
#include "../../../comp/language.h"

#include "stdio.h"

#include "panel.h"
#include "ibutton.h"
#include "niinput.h"
#include "iswitch.h"


class Admin_Ayar : public CoreAyar {
    public:
        Admin_Ayar(lv_obj_t *parent, status_t *stat, Language *lang) {
            _parent = parent;
            status = stat;
            Lng=lang;  

            subsribe0 = lv_msg_subscribe(NII_KLAVYE_OK, iinput_cb, (void *)this);
            subsribe1 = lv_msg_subscribe(ISW_CHANGED,swchg_callback,(void *)this);

            screen_init();
        };
        ~Admin_Ayar() {};
        void unregister(void) {
            lv_msg_unsubscribe(subsribe0);
            lv_msg_unsubscribe(subsribe1);
            delete ii;
            delete res;
            delete cid;
            delete maxt;
            delete mint;
            delete temp_cal;
            delete temp_read;
            delete udp_sw;
            delete rs_sw;
            delete lc_sw;
           // printf("unreg\n");
        };
                
    private:              
        void screen_init(void);
        lv_style_t style_rad;
        lv_style_t style_chk;
        uint32_t inx = 0;
        status_t *status;
        Language *Lng;
        Panel *pan, *pan1;
        IButton *ii;
        IButton *res;

        NIInput *cid;
        NIInput *maxt;
        NIInput *mint;
        NIInput *temp_cal;
        NIInput *temp_read;

        ISwitch *udp_sw;
        ISwitch *rs_sw;
        ISwitch *lc_sw;

        void *subsribe0, *subsribe1;

        static void Btn_Callback(void *arg, const void *inx);
        static void iinput_cb(void * s, lv_msg_t * m);
        static void swchg_callback(void * s, lv_msg_t * m);

    protected:
};

#endif
