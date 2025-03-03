#ifndef LAMP_BUTTON_H
#define LAMP_BUTTON_H

#include "lvgl.h"
#include "panel.h"
#include "ilabel.h"
#include "../core.h"

class Lamp_Button: public lv_obj_t {
public:
    Lamp_Button(lv_obj_t* parent,int width, int height,const char *text, uint8_t loc);
    ~Lamp_Button() 
    {
        lv_obj_clean(img);
        delete panel; 
        delete label;
    };
    void set_state(bool dr);
    lv_obj_t *get(void) {return bck;};

private:
    Panel* panel;
    lv_obj_t* img;
    ILabel* label;
    lv_obj_t *bck;
    static void local_callback(lv_event_t * e);
    void set_on(void);
    void set_off(void);
    bool state=false;
    uint8_t index;
    uint8_t local;
    const void *choice_icon(uint8_t inx);

};

#endif // BUTTON_CORE_H