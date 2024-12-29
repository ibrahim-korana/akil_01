#ifndef _STARTUP_H_
#define _STARTUP_H_

     #include "../core.h"
     #include "../comp/language.h"

     void startup_init(status_t *cnf, exit_callback_t cb, Language *lang);
     void startup_load(void);
     void startup_destroy(void); 

#endif