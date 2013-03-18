#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#include "../ex1/defobj.h"

// adrese ip
int main(int argc, char *argv[])
{   
	if (argc != 3)
	{
		fprintf(stderr, "Usage %s <Server name> <Server Port>\n", argv[0]);
		exit(1);
	}
	
	
	struct sockaddr_in serverAddr;
	struct hostent * hid;
	int sd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	char buff[500];
	int to_read = -1;
	int read = 0;
	int size_msg = 0;
	ssize_t size = 0;
	
	hid = gethostbyname(argv[1]);
	if (hid == NULL)
	{
		perror("gethostbyname");
		exit(1);
	}
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(atoi(argv[2]));
	bcopy(hid->h_addr,&(serverAddr.sin_addr.s_addr),hid->h_length);
	
	if (connect(sd,(struct sockaddr *) &serverAddr,sizeof(serverAddr)) < 0)
	{
		perror("error connect");
		exit(1);
	}
	
	puts("Connected to the server");
	
	obj o;
	
	// send message to server
	strcpy(o.chaine12, "coucou");
	strcpy(o.chaine, "ca va ?");
	o.ii = 4;
	o.jj = 2;
	o.iqt = 1;
	o.dd = 7.7;
	
	send(sd,&o,sizeof(o),0);
	
	
	return 0;
}

