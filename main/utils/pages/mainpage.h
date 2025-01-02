
#ifndef _MAINPAGE_H_
#define _MAINPAGE_H_

     #include "../../core.h"
     #include "panel.h"
     #include "ilabel.h"
     #include "sub_content.h"
     #include "../../comp/language.h"

class Main_Page:public SubContent {
    public:
       Main_Page(status_t *stat, lv_obj_t *par, Storage *dsk, Language *lang) {
         Lng = lang;
         init(stat,par, dsk);
       }       
       void icerik(void);
       void unregister(void);
    private:
    protected:
      lv_obj_t *img1;
      Language *Lng;
};

#endif