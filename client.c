/* std */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <strings.h>

/* inet */
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

/* local headers */
#include "global.h"


/* global vars */
static char ** argv0;




int32_t
get_servers (long amount)
{
	int32_t sockD,connectStatus,srvLen;

	Server srvData[amount+1];
	struct sockaddr_in servAddr= {
		.sin_family      = AF_INET,
		.sin_port   		 = htons(9001),
		.sin_addr.s_addr = INADDR_ANY,
	};

	sockD         = socket(servAddr.sin_family,SOCK_STREAM,0);
	connectStatus = connect(sockD,(struct sockaddr *)&servAddr,sizeof(servAddr));
	if (-1==connectStatus) {
		perror("main,connect: stat == -1");
		return -1;
	}

	srvLen = htonl(amount);
	send(sockD,&srvLen,sizeof(srvLen),0);
	printf("Size: %ld\n",sizeof(Server)*amount+1);
	recv(sockD,&srvData,sizeof(Server)*amount+1,0);

	int32_t i=0;
	for (;;) {
		if (!(i<amount)) { break; }
		printf("ip: %s\nport: %d\nDescription: %s\n",srvData[i].ip,srvData[i].port,srvData[i].shortDesc);
		i++;
	}

	return 0;
}

int32_t
main (argc,argv)
	int32_t argc; char ** argv;
{
	int32_t status=0;
	long    amount=0;
	argv0 = argv;

	if (argc<2) {
		fprintf(stderr,"No args given.\nSee --help.\n");
		return 1;
	}

	*argv++;
	for (;;) {
		if (!*argv) { break; }
		if (1==argv-argv0) {
			amount = strtol(*argv,0,10);

			if (errno) {
				fprintf(stderr,"Not a number.\nSee --help.\n");
				return 2;
			}
		}

		*argv++;
	}

	status = get_servers(amount);
	return status;
}
