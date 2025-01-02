#include "lvgl.h"
#include "stdio.h"
#include "esp_https_ota.h"
#include "esp_timer.h"
#include "ana_ekran.h"

#include "storage.h"
#include "../comp/language.h"

#include "../utils/fooder.h"
#include "../utils/header.h"
#include "../utils/content.h"

static lv_coord_t screen_h;
static lv_coord_t screen_w;
status_t *an_config = NULL;
exit_callback_t an_callback = NULL;
Storage *an_dsk;
Language *Lng;

static lv_obj_t *screen_container;
static lv_style_t style_bg;

LV_IMG_DECLARE(bg);


void an_create_styles(void)
{
    screen_h = lv_obj_get_height(screen_container);
    screen_w = lv_obj_get_width(screen_container);

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


void anaekran_screen()
{
    
   // 
    lv_obj_t *img = lv_img_create(screen_container);
    lv_img_set_src(img, &bg);

    uint8_t header_h = 60, fooder_h = 80;
    uint16_t content_h = screen_h - (header_h + fooder_h);
       
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_style_img_opa(img, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(img, 50, LV_STATE_DEFAULT);
    
    Header *pa = new Header(an_config, screen_container, header_h);
    lv_obj_align(pa->get(),LV_ALIGN_TOP_MID,0,0);

    Content *co = new Content(an_config, screen_container, content_h, an_dsk, Lng);
    lv_obj_align_to(co->get(),pa->get(),LV_ALIGN_OUT_BOTTOM_MID,0,0);

    Fooder *fo = new Fooder(an_config, screen_container, fooder_h);
    lv_obj_align_to(fo->get(),co->get(),LV_ALIGN_OUT_BOTTOM_MID,0,0);
}

void anaekran_init(status_t *cnf, exit_callback_t cb, Storage *dsk, Language *lang)
{
    an_config = cnf;
    an_callback = cb;
    an_dsk = dsk;
    Lng = lang;
    screen_container = lv_obj_create(NULL);
    an_create_styles();

    lv_obj_set_size(screen_container,screen_w,screen_h);
    lv_obj_add_style(screen_container, &style_bg, LV_STATE_DEFAULT);

    anaekran_screen();
}

void an_exit(void *arg)
{
    an_callback(NULL,2);
}

void anaekran_load(void)
{
    lv_scr_load_anim(screen_container, LV_SCR_LOAD_ANIM_FADE_IN, 0,0, true);
}

void anaekran_destroy(void)
{
    lv_obj_clean(screen_container);
}