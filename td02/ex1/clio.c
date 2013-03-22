#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#include "iniobj.h"

// adrese ip
int main(int argc, char *argv[])
{
    initObj();
    
    if (argc != 3)
    {
        fprintf(stderr, "Usage %s <Server name> <Server Port>\n", argv[0]);
        exit(1);
    }
    
    int i=0;
    for (i=0; i<TABN; ++i)
    {
        printObj(&tabObj[i]);
        putchar('\n');
    }
    
    
    struct sockaddr_in serverAddr,clientAddr;
    struct hostent * hid;
    bzero(&serverAddr,sizeof(serverAddr));
    int sd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(atoi(argv[2]));
    
    
    hid = gethostbyname(argv[1]);
    
    bcopy(hid->h_addr,&(serverAddr.sin_addr.s_addr),hid->h_length);
    
    if (connect(sd,(struct sockaddr *) &serverAddr,sizeof(serverAddr))<0)
    {
        perror("error connect");
        exit(1); 
    }
    
    
    for(i=0;i<TABN;i++)
    {
        if (i==TABN-1)
        {
            tabObj[i].iqt=-1;
        }
        
        else
        {
            tabObj[i].iqt=1;  
        }
        
        send(sd,&tabObj[i],sizeof(tabObj[i]),0);
    }
    
    close(sd);
    return 0;
}
