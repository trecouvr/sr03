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
	
	
	struct sockaddr_in serverAddr,clientAddr;
	struct hostent * hid;
	bzero(&serverAddr,sizeof(serverAddr));
	int sd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	char buff[500];
	int to_read = -1;
	ssize_t size = 0;
	
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(atoi(argv[2]));
	
	
	hid = gethostbyname(argv[1]);
	
	bcopy(hid->h_addr,&(serverAddr.sin_addr.s_addr),hid->h_length);
	
	if (connect(sd,(struct sockaddr *) &serverAddr,sizeof(serverAddr))<0)
	{
		perror("error connect");
		exit(1); 
	}
	
	obj o;
	
	// send message to server
	strcpy(o.chaine12, "coucou");
	strcpy(o.chaine, "ca va ?");
	o.ii = 4;
	o.jj = 2;
	o.iqt = 1;
	o.dd = 7.7;
	
	send(sd,&o,sizeof(o),0);
	
	
	// get response
	
	// get size to read
	if ((size=recv(sd, &to_read, sizeof(to_read), 0)) < 0)
	{
		perror("error rcv");
		exit(1);
	
	}
	if (size!=sizeof(to_read))
	{
		perror("size");
		exit(1);
	}
	
	printf("Je vais lire %d octets\n", to_read);
	
	// get object
	if ((size=recv(sd, &o, to_read, 0)) < 0)
	{
		perror("error rcv");
		exit(1);
	}
	if (size!=to_read)
	{
		perror("size");
		exit(1);
	}
	
	puts("recv from server : ");
	printObj(&o);
	putchar('\n');
	
	
	// send quit message
	strcpy(o.chaine12, "byebye");
	strcpy(o.chaine, "tchuss");
	o.iqt = -1;
	
	send(sd,&o,sizeof(o),0);
	
	
	return 0;
}

