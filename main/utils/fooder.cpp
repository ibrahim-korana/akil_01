
#include "fooder.h"


void Fooder::callback(void *ths, const void *inx)
{
   Fooder * my =(Fooder *) ths;
   if (my->change_active==false) return;
   if (strcmp((char*)inx,"SAGB")==0)
      {
         if (my->page_index<my->max_page) my->page_index++; else my->page_index=1;
      }
   if (strcmp((char*)inx,"SOLB")==0)
      {
         if (my->page_index>1) my->page_index--; else my->page_index=my->max_page;
      } 
   my->status->active_page = my->page_index;
         
   ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_BUTTON_PRESSED, NULL, 0, portMAX_DELAY));  
   ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_PAGE_CHANGED, &my->page_index, sizeof(uint8_t), portMAX_DELAY));       
}

void Fooder::akil_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
   Fooder *my = (Fooder *) handler_args;
   if(id==AEV_GOT_IP)
   {
      my->ip->set_text(my->status->current_ip);
   }
   if (id==AEV_PCHANGE_OFF)
   {
      my->change_active = false;
   }
   if (id==AEV_PCHANGE_ON)
   {
      my->change_active = true;
   }
}