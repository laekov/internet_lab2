#include <stdlib.h>
#include <errno.h>
#include "recvroute.h"


int static_route_get(struct selfroute *selfrt)
{
	static int recvfd = -1;
	struct sockaddr_in serv_addr, cli_addr;
	int clilen, recvlen;
	char buf[1500];
	if (recvfd == -1) {
		if ((recvfd=socket(AF_INET,SOCK_STREAM,0))==-1)	
		{
			printf("recvfd() error\n");
			return -1;
		}	
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(800);
		if (bind(recvfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0)
			error("ERROR on binding");
		listen(recvfd,5);
	}
	clilen = sizeof(cli_addr);
	while (1) {
		int newsockfd;
		newsockfd = accept(recvfd, (struct sockaddr *) &cli_addr, &clilen);
		recvlen = read(newsockfd,buf, sizeof(buf));

		if (recvlen < 0) {
			printf("Error with %lx\n", errno);
		}
		else {
			memcpy(selfrt, buf, sizeof(struct selfroute));
			return 1;
		}
	}
}
