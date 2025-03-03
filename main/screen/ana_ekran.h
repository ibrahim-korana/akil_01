#ifndef _ANAEKRAN_H_
#define _ANAEKRAN_H_

     #include "../core.h"
     #include "storage.h"
     #include "../comp/language.h"

     void anaekran_init(status_t *cnf, exit_callback_t cb, Storage *dsk, Language *lng, lamps_t *lamp);
     void anaekran_load(void);
     void anaekran_destroy(void); 
     void anaekran_set_lamp(lamps_t *lamp);

#endif