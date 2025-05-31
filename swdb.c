/* std */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

/* inet */
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

/* local headers */
#include "global.h"



int32_t clientSocket=0;
int32_t servSockD=0;

void
handle_sigint (int32_t sig)
{
	puts("Closing sockets...\n");
	close(clientSocket);
	close(servSockD);
	exit(0);
}


int32_t
main (void)
{
	Server srvData[2]; /* set your length here */
	strcpy(srvData[0].ip,"127.0.0.1");
	srvData[0].port      = 9002;
	strcpy(srvData[0].shortDesc,"test server.");

	strcpy(srvData[1].ip,"127.0.0.1");
	srvData[1].port      = 9003;
	strcpy(srvData[1].shortDesc,"second server.");

	struct sockaddr_in servAddr= {
		.sin_family      = AF_INET,
		.sin_port        = htons(9001),
		.sin_addr.s_addr = INADDR_ANY,
	};

	servSockD = socket(servAddr.sin_family,SOCK_STREAM,0);
	int32_t stat = bind(servSockD,(struct sockaddr*)&servAddr,sizeof(servAddr));
	listen(servSockD,1);

	/* accept cycle */
	int32_t valRead=0;
	int32_t buff=0;

	signal(SIGINT,handle_sigint);
	for (;;) {
		buff         = 0;
		clientSocket = accept(servSockD,0,0);
		valRead      = read(clientSocket,&buff,sizeof(buff));

		/* error || socket was killed */
		if (valRead <= 0) {
			goto skip_send;
		}

		send(clientSocket,&srvData,(sizeof(Server)*ntohl(buff))+1,0);
skip_send:
		close(clientSocket);
	}

	return 0;
}
