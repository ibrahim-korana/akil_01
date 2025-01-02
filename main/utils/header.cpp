

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