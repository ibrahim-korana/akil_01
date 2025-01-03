#ifndef SCREEN_SAVER_AYAR_H_
#define SCREEN_SAVER_AYAR_H_

#include "lvgl.h"
#include "../core_ayar.h"
#include "../../../core.h"
#include "../../../comp/language.h"
#include "stdio.h"
#include "panel.h"
#include "ibutton.h"
#include "iswitch.h"
#include "ikeyboard.h"
#include "esp_wifi.h"

class Wifi_Ayarlari : public CoreAyar {
    public:
        Wifi_Ayarlari(lv_obj_t *parent, status_t *stat, Language *lang) {
            _parent = parent;
            Lng = lang;
            status = stat;
            screen_init();
        };
        ~Wifi_Ayarlari() {}
        void unregister(void) {
            if (keyb!=NULL) delete keyb;
            if (sta_netif!=NULL) {
                esp_wifi_stop();
                esp_netif_destroy(sta_netif);
            }
            free(tmp_ssid);
            free(tmp_pass);
            lv_obj_clean(list);
            delete btn0;
            delete Wifi_Sw;
            delete Time_Sw;
            esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &scan_event_handler);
            lv_msg_unsubscribe(subs0);
            lv_msg_unsubscribe(subs1);
            //printf("unreg\n");
        };
        
        
    private:
        void screen_init(void);
        IButton *btn0;
      
        Language *Lng; 
        status_t *status;
        ISwitch *Wifi_Sw;
        ISwitch *Time_Sw;
        IKeyboard *keyb = NULL;
        lv_obj_t * list;
        char *tmp_ssid;
        char *tmp_pass;
        void *subs0;
        void *subs1;
        esp_netif_t *sta_netif = NULL;
        bool ssid_ok = false;
        bool pass_ok = false;
        bool wifi_ok = false;
        bool time_ok = false;
        lv_obj_t  *mbox1;

        esp_err_t wifi_open_start();
        esp_err_t wifi_close_start();
        static void scan_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data);
        static void Btn_Callback(void *arg, const void *inx);
        static void btn_event_handler(lv_event_t * e);
        static void keyb_callback(void * s, lv_msg_t * m);
        static void swchg_callback(void * s, lv_msg_t * m);
        static void box_event_cb(lv_event_t * e);
    protected:        
};

#endif
