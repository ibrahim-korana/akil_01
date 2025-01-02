#ifndef _SETUP_H_
#define _SETUP_H_

     #include "../../core.h"
     #include "panel.h"
     #include "ilabel.h"
     #include "ibutton.h"
     #include "sub_content.h"
     #include "core_ayar.h"
     #include "esp_timer.h"
     #include "../../comp/language.h"

class Setup_Page:public SubContent {
    public:
       Setup_Page(status_t *stat, lv_obj_t *par, Storage *dsk, Language *lang) 
       {
            Lng = lang;
            init(stat,par, dsk);
       }
       void icerik(void);
       void unregister(void);
    private:
    protected:
       Language *Lng;
       
       static void Lcallback(void *arg, const void *inx);
       static void panel_cb(lv_event_t * e);
       static void sub_button_cb(void * s, lv_msg_t * m);

       local_ibutton_t *buttons = NULL;
       void add_btn(local_ibutton_t *btn);
       local_ibutton_t *new_btn(void);               
};


#endif