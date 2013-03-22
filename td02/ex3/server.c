#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>




#include "../ex1/defobj.h"

void childDieHandler(int sig)
{
	int status=42;
	pid_t childPID = wait(&status);
	printf("Process %d finished with status %d\n", childPID, status);
}

int main(int argc, char *argv[])
{
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	int clntSock = -1;
	ssize_t size = -1;
	int read = 0;
	obj o;
	
	if (argc != 2)
	{
		fprintf(stderr, "Usage %s <Server Port>\n", argv[0]);
		exit(1);
	}
	
	(void) signal(SIGCHLD, childDieHandler);
	
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
		
		printf("Accept client %s\n", inet_ntoa(clientAddr.sin_addr));
		pid_t pid=fork();
		
		
		if (pid < 0)
		{
			perror("error fork\n");
		}
		
		if (pid ==0) // child
		{
			printf("Client %s run on process %d\n", inet_ntoa(clientAddr.sin_addr), getpid());
			while (1)
			{
                if (recvObj(clntSock, &o) < 0) {
					perror("recvObj");
					exit(1);
				}
                
                printf("recv from client %s (pid=%d): ", inet_ntoa(clientAddr.sin_addr), getpid());
                printObj(&o);
                putchar('\n');
                
                //printf("o.iqt = %d\n", o.iqt);
                
				sleep(1);
				
                if (o.iqt < 0) break;
                else {
							
					// change object
					strcpy(o.chaine12, "yooo");
					strcpy(o.chaine, "je suis le serveur");
					
					// send it
					int s = sizeof(o);
					send(clntSock,&s,sizeof(s),0);
					send(clntSock,&o,sizeof(o),0);
				}
			}
			
			
			printf("End client %s (pid=%d)\n", inet_ntoa(clientAddr.sin_addr), getpid());
			close(clntSock);
			exit(0);
		}
		else
		{
			// father, do nothing
		}
	}
	
	return 0;
}

