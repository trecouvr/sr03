#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    char chaine12[12];
    char chaine[24];
    int ii;
    int jj;
    int iqt;
    double dd;
    
    
    
} obj;


void printObj(obj * o)
{
    printf("obj('%s', '%s', %d, %d, %lf)", o->chaine12, o->chaine, o->ii, o->jj, o->dd);
}


