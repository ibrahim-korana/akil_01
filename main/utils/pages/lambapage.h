
#ifndef _LAMBAPAGE0_H_
#define _LAMBAPAGE0_H_

     #include "../../core.h"
     #include "panel.h"
     #include "alarm.h"
     #include "ilabel.h"
     #include "ibutton.h"
     #include "sub_content.h"
     //#include "srbutton.h"

class Lamba_Page:public SubContent {
    public:
       Lamba_Page(status_t *stat, lv_obj_t *par, Storage *dsk) 
       {
            init(stat,par, dsk);
       }
       void set_lamps(lamps_t * lmb) 
       {
            lamp_list = lmb;
       }
       void icerik(void);
       void unregister(void) {
         
          //printf("Lambalar unregister\n");
       }
    private:
    protected:
       lamps_t *lamp_list;
      // local_srbutton_t *l_lamps=NULL;
      // void add_llamps(local_srbutton_t *lmp);
      // static void Lcallback(void *arg, const void *inx, void *btn);
      // static void Lcallback1(void *arg, const void *inx);
       //esp_event_handler_instance_t instance;
       //static void post_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);
};

#endif