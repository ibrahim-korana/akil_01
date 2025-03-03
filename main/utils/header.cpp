

#include "header.h"

void Header::change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{  
   Header * ths = (Header *)handler_args;

   if (id==AEV_GOT_IP ) 
   {        
        ths->i_wifi->set_sourge(&Wifi_5_48);
   }
   if (id==AEV_DISCONNECTED) 
   {
        ths->i_wifi->set_sourge(&Wifi_alarm_5_48);
   }
   
   if (id==AEV_PAGE_CHANGED) {
     ths->set_page();     
   }
}

void Header::wifi_callback(lv_event_t * e)
{
     Header *Self = (Header *)lv_event_get_user_data(e);
     //lv_obj_t obj = lv_event_get_target(e);
     if (Self->status->wifi_connected==0) ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_WIFI_CONNECT, NULL, 0, portMAX_DELAY)); 

}