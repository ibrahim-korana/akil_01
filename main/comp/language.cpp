

#include "language.h"

lang_t *Language::parse(char *txt)
{
    lang_t *kk = NULL;
    const char *delp;
    char *inx, *txt0;

    if(strlen(txt)<3) return NULL;
    inx = (char *)calloc(1,50);
    txt0 = (char *)calloc(1,255);

    uint8_t i=1;
    delp = strtok(txt, "&");
    while (delp != NULL)  {
        if (i==1) strcpy(inx,delp);
        if (i==2) strcpy(txt0,delp);
        i++;
      delp = strtok(NULL, "&");
    }
    if (i>1) {
        kk=(lang_t *)calloc(1,sizeof(lang_t));
        kk->inx = (char *)calloc(1,strlen(inx)+1);
        strcpy(kk->inx,inx);
        kk->txt = (char *)calloc(1,strlen(txt0)+1);
        strcpy(kk->txt,txt0);
    }
    free(inx);
    free(txt0);

   return kk; 
}