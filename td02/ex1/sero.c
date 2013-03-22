#include "defobj.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>



// port internet =10 000 + 45 = 10045 (incrementer entre deux tests (+100))
int main(int argc, char *argv[])
{
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    int clntSock = -1;
    
    if (argc != 2)
    {
        fprintf(stderr, "Usage %s <Server Port>\n", argv[0]);
        exit(1);
    }
    
    int sd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    bzero(&serverAddr,sizeof(serverAddr));
    
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=htonl(INADDR_ANY); //hton(l&s): coherence du format avec internet (Big Endian)
    serverAddr.sin_port=htons(atoi(argv[1]));// s=short et l=long
    
    if(bind(sd,(struct sockaddr *) &serverAddr, sizeof(serverAddr))< 0)
    {
        perror("error bind");
        exit(1);
    }
    
    if (listen(sd,SOMAXCONN)<0)
    {
        perror("error listen");
        exit(1);
    }
    
    while (1)
    {
        printf("Wait client connection...\n");
        int cltSize = sizeof clientAddr;
        if ((clntSock = accept(sd, (struct sockaddr *) &clientAddr, &cltSize)) < 0)
        {
            perror("error accept");
            exit(1);
        }
        
        printf("client %s\n",inet_ntoa(clientAddr.sin_addr));
        pid_t pid=fork();
        
        
        if (pid < 0)
        {
            perror("error fork\n");
        }
        
        if (pid ==0)
        {
            obj o;
            int size;
            printf("Fils\n");
            
            while (1)
            {
                if ((size=recv(clntSock, &o, sizeof(o), 0)) < 0)
                {
                    perror("error rcv");
                    exit(1);            
                
                }
                if (size!=sizeof(o))
                {
                    perror("size");
                    exit(1);
                }
                
                printObj(&o);
                putchar('\n');
                if (o.iqt < 0) break;
            }
            
            exit(0);
            
        }
        else
        {   //cas du pere
            printf("Pere\n");
            int status=-1;
            waitpid(pid, &status, WUNTRACED);
            printf("pid : %d\n", status);
            
            if (status == 0)
            {
                close(clntSock);
                break;
                
            }
        }
    }
    
    printf("Arret serveur\n");
    
    
    return 0;
}
