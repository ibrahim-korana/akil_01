

#include "mainpage.h"

const char *MAIN_TAG0 = "MAIN_PAGE";

void Main_Page::unregister(void) 
{   
    lv_obj_clean(img1);
    ESP_LOGI(MAIN_TAG0,"Ana ekran unregister");
}
void Main_Page::icerik(void) 
{
    ESP_LOGI(MAIN_TAG0,"Ana ekran Show");
    img1 = lv_img_create(mpanel);
    lv_img_set_src(img1,"A:/firma.bin");
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(img1, 300, 200);    
           
}     

   