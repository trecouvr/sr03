#ifndef INIOBJ_H_
#define INIOBJ_H_
#include "defobj.h"

// WARNING 0 <= TABN <= 9
#define TABN    3

obj tabObj[TABN];

    
void initObj()
{
    int i=0;
    char a1[] = "ident_ox";
    char a2[] = "description_ox";
    for (i=0; i<TABN; ++i)
    {
        a1[strlen(a1) - 1] = ('0' + i);
        a2[strlen(a2) - 1] = ('0' + i);
        strcpy(tabObj[i].chaine12, a1);
        strcpy(tabObj[i].chaine, a2);
        tabObj[i].ii = (i+1)*10 + 1;
        tabObj[i].jj = (i+1)*10 + 2;
        tabObj[i].dd = ((float)((i+1)*10)) + 0.2345;
    }
}

#endif
