#include "lookuproute.h"


int insert_route(unsigned long  ip4prefix,unsigned int prefixlen,char *ifname,unsigned int ifindex,unsigned long  nexthopaddr)
{
	struct route* route_idx = (struct route*)malloc(sizeof(struct route));

}

int lookup_route(struct in_addr dstaddr,struct nextaddr *nexthopinfo)
{
	
}

int delete_route(struct in_addr dstaddr,unsigned int prefixlen)
{
	
}

