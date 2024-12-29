
#include "lvgl.h"
#include "stdio.h"
#include "esp_https_ota.h"
#include "esp_timer.h"
#include "startup.h"
#include "ilabel.h"
#include "../comp/language.h"

static lv_coord_t screen_h;
static lv_coord_t screen_w;
status_t *st_config = NULL;
exit_callback_t callback = NULL;
Language *Lng0;


static lv_obj_t *screen_container;
//Styles
static lv_style_t style_bg;

LV_IMG_DECLARE(logo);

void create_styles(void)
{
    screen_h = lv_obj_get_height(lv_scr_act());
    screen_w = lv_obj_get_width(lv_scr_act());

    //Background styles
    lv_style_init(&style_bg);
    //lv_style_set_bg_opa(&style_bg, LV_OPA_90);
    lv_style_set_radius(&style_bg, 0);
    lv_style_set_bg_color(&style_bg, lv_color_hex(0x000000));
    lv_style_set_text_color(&style_bg, lv_color_hex(0xFFFFFF));
    lv_style_set_text_opa(&style_bg,LV_OPA_50);
    lv_disp_set_bg_opa(NULL, LV_OPA_TRANSP);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_TRANSP, LV_STATE_DEFAULT);
}

void startup_screen()
{
    lv_obj_t *img = lv_img_create(screen_container);
    lv_img_set_src(img, &logo);      
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_img_opa(img, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_clear_flag(img, LV_OBJ_FLAG_GESTURE_BUBBLE);

    ILabel *L1 = new ILabel(screen_container,200,20,0,0,LV_ALIGN_CENTER,&lv_font_montserrat_20,lv_color_hex(0xFFFFFF));
    lv_obj_align(L1->get(),LV_ALIGN_CENTER,0,140);
    L1->set_text((char*)st_config->cihaz_adi);

    const esp_app_desc_t *desc = esp_app_get_description();
    char *mm=(char*)malloc(64);
    strcpy(mm,"Firmware ");
    strcat(mm,desc->version);

    ILabel *L2 = new ILabel(screen_container,200,20,0,0,LV_ALIGN_CENTER,&lv_font_montserrat_14,lv_color_hex(0xFFFFFF));
    lv_obj_align(L2->get(),LV_ALIGN_CENTER,0,160);
    L2->set_text(mm);

    ILabel *L3 = new ILabel(screen_container,200,20,0,0,LV_ALIGN_CENTER,&lv_font_montserrat_14,lv_color_hex(0xFFFFFF));
    lv_obj_align(L3->get(),LV_ALIGN_CENTER,0,185);
    L3->set_text(Lng0->get_text("lang"));
    free(mm);
}

void startup_init(status_t *cnf, exit_callback_t cb, Language *lang)
{
    st_config = cnf;
    callback = cb;
    Lng0 = lang;
    screen_container = lv_obj_create(NULL);
    create_styles();
    
    lv_obj_set_size(screen_container,screen_w,screen_h);
    lv_obj_add_style(screen_container, &style_bg, LV_STATE_DEFAULT);

    startup_screen();
}

void exit(void *arg)
{
    callback(NULL,1);
}

void startup_load(void)
{
    lv_scr_load_anim(screen_container, LV_SCR_LOAD_ANIM_FADE_IN, 0,0, true);
    if (callback!=NULL)
      {
                esp_timer_create_args_t timer_args = {};
                    timer_args.callback = &exit;
                    timer_args.name = "exit_tick";
                esp_timer_handle_t tick_timer = NULL;
                ESP_ERROR_CHECK(esp_timer_create(&timer_args, &tick_timer));
                ESP_ERROR_CHECK(esp_timer_start_once(tick_timer, 2 * 1000000));
      } 
}

void startup_destroy(void)
{
    lv_obj_clean(screen_container);
}