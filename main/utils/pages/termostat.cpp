

#include "termostat.h"

LV_FONT_DECLARE(digi80)
LV_IMG_DECLARE(sag_buyuk)
LV_IMG_DECLARE(sol_buyuk)
LV_IMG_DECLARE(hmin_b)
LV_IMG_DECLARE(motor)

LV_IMG_DECLARE(night_60)
LV_IMG_DECLARE(manual_60)
LV_IMG_DECLARE(off_60)


void Termostat_Page::local_degerlendir(void)
{
    if (status->termostat_local==0) return;
    uint8_t temp = status->current_temp;
    air_status_t aa = {};
    
    if (temp >= status->current_set)
      {
         status->r1_stat = 1;          
      } else {
         status->r1_stat = 0;
      }

      aa.int_st = 0;
      aa.st.role_durumu = !status->r1_stat;
      aa.st.start_stop = 1;
    
    status->temp_current_stat = aa.int_st;
    ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_TERMOSTAT_CHANGE, NULL, 0, portMAX_DELAY));  

}

void Termostat_Page::callback(void *ths, const void *inx)
{
   Termostat_Page * my =(Termostat_Page *) ths;
   bool totalset = false;
    if (strcmp((char*)inx,"UPB")==0)
      {
            if (my->status->current_set<my->status->max_temp) my->status->current_set++; else my->status->current_set=my->status->max_temp;
             totalset = true; 
      }
    if (strcmp((char*)inx,"DOWNB")==0)
      {
         if (my->status->current_set>my->status->min_temp) my->status->current_set--; else my->status->current_set=my->status->min_temp;
         totalset = true;   
      } 
    if (totalset) 
      {
        my->local_degerlendir();
        my->Isaretler();
        ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_BUTTON_PRESSED, NULL, 0, portMAX_DELAY));  
        lv_meter_set_indicator_start_value(my->meter, my->set_indic, my->status->min_temp);
        lv_meter_set_indicator_end_value(my->meter, my->set_indic, my->status->current_set);
        lv_meter_set_indicator_value(my->meter, my->indic, my->status->current_temp);
        char *mm;
        asprintf(&mm,"%d",my->status->current_set);
        my->set_label->set_text(mm);  
        free(mm);
        ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_OUT_TEMPSET_CHANGED, NULL, 0, portMAX_DELAY));
        if (my->status->termostat_local==1) 
             ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_CONFIG_SAVE_NORESET, NULL, 0, portMAX_DELAY));
      
      }  
}

void Termostat_Page::tap_callback(lv_event_t * e)
{
   ESP_ERROR_CHECK(esp_event_post(AKIL_EVENTS, AEV_GET_STATUS_TERMOSTAT, NULL, 0, portMAX_DELAY));
}


void Termostat_Page::icerik(void) 
{
    meter = lv_meter_create(mpanel);
    lv_obj_align(meter,LV_ALIGN_TOP_MID,0,0);    
    lv_obj_set_size(meter, 250, 280);
    lv_obj_set_style_opa(meter,180,0);
    lv_obj_set_style_bg_color(meter,lv_color_hex(0x000000),0);
    lv_obj_set_style_border_width(meter,0,0);
    lv_obj_set_style_arc_width(meter,0,0);
    lv_obj_add_event_cb(meter, tap_callback, LV_EVENT_CLICKED, (void *)this);

    uint8_t major = (status->max_temp - status->min_temp)/5, minor = status->max_temp - status->min_temp;

    /*Add a scale first*/
    scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, minor, 2, 15, lv_palette_darken(LV_PALETTE_GREY,3));
    lv_meter_set_scale_major_ticks(meter, scale, major, 4, 20, lv_palette_darken(LV_PALETTE_GREY,2), 10);
    lv_meter_set_scale_range(meter, scale, status->min_temp, status->max_temp, 270, 135);
    
    //indic = lv_meter_add_needle_line(meter, scale, 3, lv_palette_main(LV_PALETTE_GREY), -30);
    indic = lv_meter_add_needle_img(meter, scale, &hmin_b, 5, 5);
    indic->opa = 50;
    
    set_indic = lv_meter_add_arc(meter, scale, 6, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(meter, set_indic, status->min_temp);
    lv_meter_set_indicator_end_value(meter, set_indic, status->current_set);
    lv_meter_set_indicator_value(meter, indic, status->current_temp);
    
    a0 = new ILabel(meter,250,30,250,0,LV_ALIGN_CENTER,&lv_font_montserrat_18,lv_color_hex(0xFFFFFF));
    lv_obj_align(a0->get(),LV_ALIGN_CENTER,0,-75);
    a0->set_text("SET");
       
    set_label = new ILabel(meter,250,30,0,0,LV_ALIGN_CENTER,&lv_font_montserrat_32,lv_color_hex(0xFFFFFF));
    lv_obj_align_to(set_label->get(),a0->get(),LV_ALIGN_OUT_BOTTOM_MID,0,0);
    char *mm;
    asprintf(&mm,"%d",status->current_set);
    set_label->set_text(mm);  
    free(mm);

    current_temp_label = new ILabel(meter,250,80,0,0,LV_ALIGN_CENTER,&digi80,lv_color_hex(0xFFFFFF));
    lv_obj_align(current_temp_label->get(),LV_ALIGN_BOTTOM_MID,0,20);
    char *mm0;
    asprintf(&mm0,"%.0f.0",status->current_temp);
    current_temp_label->set_text(mm0);    
    free(mm0);

    a2 = new IButton(mpanel,&sol_buyuk,80,200,40,0,(void *)this,callback,"DOWNB");
    lv_obj_align_to(a2->get(),meter,LV_ALIGN_OUT_LEFT_MID,20,-15); 

    a4 = new IButton(mpanel,&sag_buyuk,80,200,40,0,(void *)this,callback,"UPB"); 
    lv_obj_align_to(a4->get(),meter,LV_ALIGN_OUT_RIGHT_MID,-20,-15); 

    mot = lv_img_create(mpanel);
    lv_img_set_src(mot, &motor);
    set_img_sty(mot);
    lv_obj_align(mot,LV_ALIGN_BOTTOM_LEFT,120,0);

    au = lv_img_create(mpanel);
    lv_img_set_src(au, &manual_60);
    set_img_sty(au);
    lv_obj_align_to(au,mot,LV_ALIGN_OUT_RIGHT_MID,10,0);

    gece = lv_img_create(mpanel);
    lv_img_set_src(gece, &night_60);
    set_img_sty(gece);
    lv_obj_align_to(gece,au,LV_ALIGN_OUT_RIGHT_MID,10,0);

    stop = lv_img_create(mpanel);
    lv_img_set_src(stop, &off_60);
    set_img_sty(stop);
    lv_obj_align_to(stop,gece,LV_ALIGN_OUT_RIGHT_MID,10,0);

    local = lv_img_create(mpanel);
    lv_img_set_src(local, &manual_60);
    set_img_sty(local);
    lv_obj_align_to(local,stop,LV_ALIGN_OUT_RIGHT_MID,10,0);

    Isaretler();
}     

void Termostat_Page::set_img_sty(lv_obj_t *obj)
{
    lv_obj_set_width(obj, LV_SIZE_CONTENT);
    lv_obj_set_style_opa(obj, IMG_OFF_OPASITY, 0);  //Silikleştirir
    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE); 
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
}

void Termostat_Page::Isaretler(void)
{
    air_t gelen = {};
    gelen.stat.int_st = status->temp_current_stat;
    gelen.set_temp = status->current_set;
    if (gelen.stat.st.role_durumu==1) 
      {
        lv_obj_clear_flag(mot, LV_OBJ_FLAG_HIDDEN);
      } else {
        lv_obj_add_flag(mot, LV_OBJ_FLAG_HIDDEN);
      }
    if (gelen.stat.st.auto_manuel==1) 
      {
        lv_obj_clear_flag(au, LV_OBJ_FLAG_HIDDEN);
      } else {
        lv_obj_add_flag(au, LV_OBJ_FLAG_HIDDEN);
      }  
    if (gelen.stat.st.gece_gunduz==1) 
      {
        lv_obj_clear_flag(gece, LV_OBJ_FLAG_HIDDEN);
      } else {
        lv_obj_add_flag(gece, LV_OBJ_FLAG_HIDDEN);
      }   
    if (gelen.stat.st.start_stop==0) 
      {
        lv_obj_clear_flag(stop, LV_OBJ_FLAG_HIDDEN);
      } else {
        lv_obj_add_flag(stop, LV_OBJ_FLAG_HIDDEN);
      }    
    if (status->termostat_local==1) 
      {
        lv_obj_clear_flag(local, LV_OBJ_FLAG_HIDDEN);
      } else {
        lv_obj_add_flag(local, LV_OBJ_FLAG_HIDDEN);
      }     
   
  //  printf("Auto %d Gece %d Start %d\n",gelen.stat.st.auto_manuel, gelen.stat.st.gece_gunduz,gelen.stat.st.start_stop);
    //1 motor aktif 0 pasif
    //0 Auto 1 Manuel
    //1 Gunduz 0 Gece
    //Air 1 Start 0 stop 
    //Bunlara göre ikon konulacak
}

void Termostat_Page::tmp_callback(void * s, lv_msg_t * m)
{
    //MSG_TEMPERATURE geldiğinde ekranı degiştirir
    Termostat_Page *Self = (Termostat_Page *)lv_msg_get_user_data(m);
    uint32_t cnv = Self->status->current_temp;           
    lv_meter_set_indicator_value(Self->meter, Self->indic, cnv);
    char *mm;
    asprintf(&mm,"%.0f.0",Self->status->current_temp);
    Self->current_temp_label->set_text(mm);
    free(mm); 
    Self->local_degerlendir();
    Self->Isaretler();
}

void Termostat_Page::set_change_callback(void * s, lv_msg_t * m)
{
    //MSG_IN_TEMPSET_CHANGED geldiğinde ekranı degiştirir
    Termostat_Page *Self = (Termostat_Page *)lv_msg_get_user_data(m);

    Self->local_degerlendir();

    Self->Isaretler();

    lv_meter_set_indicator_start_value(Self->meter, Self->set_indic, Self->status->min_temp);
    lv_meter_set_indicator_end_value(Self->meter, Self->set_indic, Self->status->current_set);
    lv_meter_set_indicator_value(Self->meter, Self->indic, Self->status->current_temp);
    char *mm;
    asprintf(&mm,"%d",Self->status->current_set);
    Self->set_label->set_text(mm);  
    free(mm);
}   

