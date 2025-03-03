
#ifndef _TERMOSTAT_H_
#define _TERMOSTAT_H_

     #include "../../core.h"
     #include "panel.h"
     #include "ilabel.h"
     #include "ibutton.h"
     #include "sub_content.h"

class Termostat_Page:public SubContent {
    public:
       Termostat_Page(status_t *stat, lv_obj_t *par, Storage *dsk) 
       {
            init(stat,par, dsk);
            point=1;
            //ESP_ERROR_CHECK(esp_event_handler_instance_register(ALARM_EVENTS, EVENT_TEMPARATURE, change_events, (void *)this, &instance));
            //ESP_ERROR_CHECK(esp_event_handler_instance_register(ALARM_EVENTS, EVENT_POST_SET, change_set_events, (void *)this, &instance1));
            subs0 = lv_msg_subscribe(MSG_TEMPERATURE, tmp_callback, (void*)this);
            subs1 = lv_msg_subscribe(MSG_IN_TEMPSET_CHANGED, set_change_callback, (void*)this);
             
       }
       
       void icerik(void);
       void unregister(void) { 
         lv_msg_unsubscribe(subs0);  
         lv_msg_unsubscribe(subs1); 
         //ESP_ERROR_CHECK(esp_event_handler_instance_unregister(ALARM_EVENTS, EVENT_TEMPARATURE, instance ));   
         //ESP_ERROR_CHECK(esp_event_handler_instance_unregister(ALARM_EVENTS, EVENT_POST_SET, instance1 ));     
         delete a0;
         delete set_label;
         delete current_temp_label;
         delete a2;
         delete a4;
         lv_obj_clean(meter);
         lv_obj_clean(mot);
         lv_obj_clean(au);
         lv_obj_clean(gece);
         lv_obj_clean(stop);
         lv_obj_clean(local);
         point=0;                   
         printf("Termostat unregister\n");
       };
       void Isaretler(void);
    private:
          
    protected:
          void local_degerlendir(void);
          //static void change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);
          //static void change_set_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);
          static void callback(void *ths, const void *inx);
          lv_obj_t *meter, *mot, *au, *gece, *stop, *local;
          lv_meter_indicator_t *indic;
          lv_meter_indicator_t *set_indic;
          ILabel *set_label, *current_temp_label;
          ILabel *a0;
          uint8_t point;
         // esp_event_loop_handle_t loop_handle;
          //esp_event_handler_instance_t instance, instance1;
          IButton *a2, *a4;
          lv_meter_scale_t * scale;
          void *subs0, *subs1;
          static void tmp_callback(void * s, lv_msg_t * m);
          static void set_change_callback(void * s, lv_msg_t * m);
          static void tap_callback(lv_event_t * e);

          void set_img_sty(lv_obj_t *obj);

};

#endif