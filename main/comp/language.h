#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

    #include <stdio.h>
    #include <stdint.h>
    #include "../core.h"
    #include "storage.h"
    

typedef struct {
    char *inx;
    char *txt;
    void *next;   
} lang_t;

class Language {
    public:
        Language() {};
        ~Language() {};
        void init(const char *inx, Storage *dsk) 
        {
            index = inx;
            disk = dsk;
            char *fn = (char *)calloc(1,25);
            sprintf(fn,"/config/%s.lng",index);
            char *satir = (char *)calloc(1,250);
            if (disk->file_search(fn))
              {
                 FILE *fd = fopen(fn, "r");
                    while (!feof(fd)) 
                      {
                        fgets(satir,250,fd);
                        lang_t *kk = parse(satir);
                        if (kk!=NULL)
                          {
                             kk->next = (void *)lang;
                             lang = kk; 
                          }                       
                      }
                 fclose(fd);
              } 
            free(satir);  
            free(fn);  
        }

        char *get_text(const char *inx) {
          lang_t *tmp = lang;
          while(tmp) {
            if (strcmp(tmp->inx,inx)==0)
              {
                return tmp->txt;
              }
            tmp = (lang_t *)tmp->next;  
          }
          return (char *)inx;
        }

    private:
        lang_t *parse(char *txt);
    protected:  
        const char *index; 
        Storage *disk;
        lang_t *lang=NULL;
};

#endif