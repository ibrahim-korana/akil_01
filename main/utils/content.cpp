

#include "content.h"
#include "stdio.h"

#include "pages/mainpage.h"
#include "pages/setup.h"

void Content::page_create()
{
    bool ok = false;
    if (Active_Page!=NULL) {
        Active_Page->unregister();
        delete Active_Page;
        Active_Page = NULL;
        }

    if (status->active_page==1)
    {       
        Active_Page = (SubContent *) new Main_Page(status,mpanel,disk, Lng);
        ok=true;
    }
    if (status->active_page==4)
    {       
        Active_Page = (SubContent *) new Setup_Page(status,mpanel,disk, Lng);
        ok=true;
    }
   
   if (ok) Active_Page->icerik();
}
     
void Content::change_events(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{  
   if (id==AEV_PAGE_CHANGED) {
     Content * my = (Content *)handler_args;
     my->page_create(); 
   }
}