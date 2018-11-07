#include "recvroute.h"

int static_route_get(struct selfroute *selfrt)
{
	int recvfd,recvlen;
	char skbuf[1500];
	if((recvfd=socket(AF_INET,SOCK_STREAM,0))==-1)	
	{
		printf("recvfd() error\n");
		return -1;
	}	
	while(1)
	{
		recvlen=recv(recvfd,skbuf,sizeof(skbuf),0);
		if (recvlen > 0) {
			memcpy(selfrt, skbuf + ETHER_HEADER_LEN, sizeof(struct selfroute));
			return 0;
		}
	}
}
