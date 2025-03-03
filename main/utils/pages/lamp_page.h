
#ifndef _LAMBAPAGE0_H_
#define _LAMBAPAGE0_H_

     #include "../../core.h"
     #include "panel.h"
     #include "ilabel.h"
     #include "ibutton.h"
     #include "sub_content.h"
     #include "../../comp/lamp_button.h"
     #include "../../comp/cur_button.h"

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
          lv_msg_unsubscribe(subs0);
          lv_msg_unsubscribe(subs1);
          lv_msg_unsubscribe(subs2);
          lv_msg_unsubscribe(subs3);
          lamps_t *temp = lamp_list;   
          while(temp)
               { 
                    if (strcmp(temp->name,"lamp")==0)
                    {   
                         delete (Lamp_Button *)temp->button;
                    }
                    if (strcmp(temp->name,"cur")==0)
                    {   
                         delete (Cur_Button *)temp->button;
                    }
                    temp = (lamps_t *) temp->next;
               }
         // printf("Lambalar unregister\n");
       }
    private:
    protected:
       lamps_t *lamp_list;
       void *subs0, *subs1, *subs2, *subs3;
       static void lamp_callback(void * s, lv_msg_t * msg);
       static void lamp_answer(void * s, lv_msg_t * msg);
       static void up_callback(void * s, lv_msg_t * msg);
       static void down_callback(void * s, lv_msg_t * msg);
       lamps_t *get_object(void *btn);
      // local_srbutton_t *l_lamps=NULL;
      // void add_llamps(local_srbutton_t *lmp);
      // static void Lcallback(void *arg, const void *inx, void *btn);
      // static void Lcallback1(void *arg, const void *inx);
       //esp_event_handler_instance_t instance;
       //static void post_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);
};

#endif