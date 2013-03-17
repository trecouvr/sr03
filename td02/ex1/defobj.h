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

int recvObj(int sock, obj * o)
{
	int to_read = sizeof((*o));
	int read = 0;
	int size=-1;
	
	while (read < to_read)
	{
		if ((size=recv(sock, ((char*)o)+read, to_read-read, 0)) < 0)
		{
			return size;
		}
		read += size;
		//printf("read=%d\n", read);
	}
	
	return 1;
}


