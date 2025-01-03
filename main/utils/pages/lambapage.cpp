

#include "lambapage.h"
//#include "../comp/jsontool.h"


LV_IMG_DECLARE(lamba1_off_60)
LV_IMG_DECLARE(lamba1_on_60)
LV_IMG_DECLARE(lamba2_off_60)
LV_IMG_DECLARE(lamba2_on_60)

//LV_IMG_DECLARE(Down_60);
//LV_IMG_DECLARE(Up_60);


void Lamba_Page::icerik(void) 
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
    lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_style_set_layout(&style, LV_LAYOUT_FLEX);
    lv_obj_add_style(mpanel, &style, 0);
    lv_obj_align(mpanel,LV_ALIGN_TOP_MID,0,0);
    lv_obj_set_style_pad_all(mpanel, 10, LV_STATE_DEFAULT); 

    lamps_t *temp = lamp_list;
   
    while(temp)
     { 
      if (strcmp(temp->name,"lamp")==0)
      {   
           
      }

      if (strcmp(temp->name,"cur")==0)
      { 
            
      }

      temp = (lamps_t *) temp->next;
     }
     
}


    