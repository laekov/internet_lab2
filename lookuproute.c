#include "lookuproute.h"

// Linked list version

int insert_route(unsigned long  ip4prefix,unsigned int prefixlen,char *ifname,unsigned int ifindex,unsigned long  nexthopaddr)
{
	struct route* route_idx = (struct route*)malloc(sizeof(struct route));
	route_idx->ip4prefix.s_addr = ip4prefix;
	route_idx->prefixlen = prefixlen;
	route_idx->nexthop = (struct nexthop*)malloc(sizeof(struct nexthop));
	route_idx->nexthop->ifname = ifname;
	route_idx->nexthop->ifindex = ifindex;
	route_idx->nexthop->nexthopaddr.s_addr = nexthopaddr;
	route_idx->next = route_table->next;
	route_table->next = route_idx;
	return 0;
}

unsigned int get_mask(int l) {
	return ((1u << l) - 1u);
}

int lookup_route(struct in_addr dstaddr,struct nextaddr *nexthopinfo)
{
	nexthopinfo->prefixl = 0;
	for (struct route* r = route_table->next; r; r = r->next) {
		int mask = get_mask(r->prefixlen);
		if ((dstaddr.s_addr & mask) == (r->ip4prefix.s_addr & mask)) {
			if (r->prefixlen > nexthopinfo->prefixl) {
				nexthopinfo->ifname = r->nexthop->ifname;
				nexthopinfo->ipv4addr = r->nexthop->nexthopaddr;
				nexthopinfo->prefixl = r->prefixlen;
			}
		}
	}
	if (nexthopinfo->prefixl == 0) {
		return -1;
	}
	return 0;
}

int delete_route(struct in_addr dstaddr,unsigned int prefixlen)
{
	struct route* last = route_table;
	for (struct route* r = route_table->next; r; last = r, r = r->next) {
		int mask = get_mask(r->prefixlen);
		if ((dstaddr.s_addr & mask) == (r->ip4prefix.s_addr & mask) && r->prefixlen == prefixlen) {
			last->next = r->next;
			free(r);
			return 0;
		}
	}
	return 404;
}

