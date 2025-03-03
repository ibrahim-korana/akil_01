#ifndef CUR_BUTTON_H
#define CUR_BUTTON_H

#include "lvgl.h"
#include "panel.h"
#include "ilabel.h"
#include "../core.h"


class Cur_Button: public lv_obj_t {
public:
    Cur_Button(lv_obj_t* parent,int width, int height,const char *text, uint8_t loc);
    ~Cur_Button() 
    {
        lv_obj_clean(img1);
        lv_obj_clean(img2);
        delete panel; 
        delete panel1; 
        delete panel2; 
        delete label;
    };
    void set_state(bool dr);
    lv_obj_t *get(void) {return bck;};

private:
    Panel* panel, *panel1, *panel2;
    lv_obj_t* img1, *img2;
    ILabel* label;
    lv_obj_t *bck;
    uint8_t local;
    void set_on(void);
    void set_off(void);
    bool state=false;
    static void up_callback(lv_event_t * e);
    static void down_callback(lv_event_t * e);
};

#endif // BUTTON_CORE_H